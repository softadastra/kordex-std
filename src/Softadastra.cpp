/**
 *
 *  @file Softadastra.cpp
 *  @author Softadastra
 *
 *  Copyright 2026, Softadastra.
 *  All rights reserved.
 *  https://github.com/softadastra/kordex-std
 *
 *  Use of this source code is governed by a MIT license
 *  that can be found in the LICENSE file.
 *
 *  Kordex Std
 *
 */

#include <memory>
#include <mutex>
#include <sstream>
#include <utility>
#include <filesystem>

#include <softadastra/sdk.hpp>
#include <kordex/std/Softadastra.hpp>

namespace kordex::standard
{
  namespace
  {
    struct SoftadastraRuntime
    {
      ::std::mutex mutex;
      ::std::unique_ptr<softadastra::sdk::Client> client{};
    };

    [[nodiscard]] SoftadastraRuntime &runtime()
    {
      static SoftadastraRuntime instance;
      return instance;
    }

    [[nodiscard]] kordex::bindings::Error make_softadastra_binding_error(
        const ::std::string &message)
    {
      return kordex::bindings::make_binding_error(
          kordex::bindings::BindingErrorCode::FunctionCallFailed,
          message);
    }

    [[nodiscard]] kordex::bindings::Error make_softadastra_binding_error(
        const softadastra::sdk::Error &error)
    {
      ::std::string message(error.code_string());

      if (!error.message().empty())
      {
        message += ": ";
        message += error.message();
      }

      if (error.has_context())
      {
        message += " (";
        message += error.context();
        message += ")";
      }

      return make_softadastra_binding_error(message);
    }

    [[nodiscard]] kordex::bindings::Result<softadastra::sdk::Client *> require_client()
    {
      auto &state = runtime();

      if (!state.client || !state.client->is_open())
      {
        return make_softadastra_binding_error(
            "softadastra client is not open");
      }

      return state.client.get();
    }

    [[nodiscard]] ::std::string json_escape(
        const ::std::string &value)
    {
      ::std::ostringstream stream;

      for (char character : value)
      {
        switch (character)
        {
        case '"':
          stream << "\\\"";
          break;
        case '\\':
          stream << "\\\\";
          break;
        case '\b':
          stream << "\\b";
          break;
        case '\f':
          stream << "\\f";
          break;
        case '\n':
          stream << "\\n";
          break;
        case '\r':
          stream << "\\r";
          break;
        case '\t':
          stream << "\\t";
          break;
        default:
          stream << character;
          break;
        }
      }

      return stream.str();
    }

    [[nodiscard]] ::std::string tick_result_json(
        const softadastra::sdk::TickResult &tick)
    {
      ::std::ostringstream stream;

      stream << "{";
      stream << "\"retriedCount\":" << tick.retried_count() << ",";
      stream << "\"prunedCount\":" << tick.pruned_count() << ",";
      stream << "\"batchSize\":" << tick.batch_size() << ",";
      stream << "\"hasWork\":" << (tick.has_work() ? "true" : "false");
      stream << "}";

      return stream.str();
    }

    [[nodiscard]] kordex::bindings::Error ensure_parent_directory(
        const ::std::string &file_path)
    {
      if (file_path.empty())
      {
        return kordex::bindings::ok();
      }

      try
      {
        const ::std::filesystem::path path(file_path);
        const auto parent = path.parent_path();

        if (parent.empty())
        {
          return kordex::bindings::ok();
        }

        ::std::filesystem::create_directories(parent);
        return kordex::bindings::ok();
      }
      catch (const ::std::exception &exception)
      {
        return kordex::bindings::make_binding_error(
            kordex::bindings::BindingErrorCode::FunctionCallFailed,
            ::std::string("failed to create Softadastra data directory: ") +
                exception.what());
      }
    }

    [[nodiscard]] ::std::string sync_state_json(
        const softadastra::sdk::SyncState &state)
    {
      ::std::ostringstream stream;

      stream << "{";
      stream << "\"outboxSize\":" << state.outbox_size() << ",";
      stream << "\"queuedCount\":" << state.queued_count() << ",";
      stream << "\"inFlightCount\":" << state.in_flight_count() << ",";
      stream << "\"acknowledgedCount\":" << state.acknowledged_count() << ",";
      stream << "\"failedCount\":" << state.failed_count() << ",";
      stream << "\"lastSubmittedVersion\":" << state.last_submitted_version() << ",";
      stream << "\"lastAppliedRemoteVersion\":" << state.last_applied_remote_version() << ",";
      stream << "\"totalRetries\":" << state.total_retries() << ",";
      stream << "\"hasQueued\":" << (state.has_queued() ? "true" : "false") << ",";
      stream << "\"hasInFlight\":" << (state.has_in_flight() ? "true" : "false") << ",";
      stream << "\"hasFailed\":" << (state.has_failed() ? "true" : "false") << ",";
      stream << "\"hasWork\":" << (state.has_work() ? "true" : "false");
      stream << "}";

      return stream.str();
    }

    [[nodiscard]] ::std::string node_info_json(
        const softadastra::sdk::NodeInfo &node)
    {
      ::std::ostringstream stream;

      stream << "{";
      stream << "\"nodeId\":\"" << json_escape(node.node_id()) << "\",";
      stream << "\"displayName\":\"" << json_escape(node.display_name()) << "\",";
      stream << "\"hostname\":\"" << json_escape(node.hostname()) << "\",";
      stream << "\"osName\":\"" << json_escape(node.os_name()) << "\",";
      stream << "\"version\":\"" << json_escape(node.version()) << "\",";
      stream << "\"startedAtMs\":" << node.started_at_ms() << ",";
      stream << "\"uptimeMs\":" << node.uptime_ms() << ",";
      stream << "\"capabilities\":[";

      const auto &capabilities = node.capabilities();

      for (::std::size_t index = 0; index < capabilities.size(); ++index)
      {
        if (index > 0)
        {
          stream << ",";
        }

        stream << "\"" << json_escape(capabilities[index]) << "\"";
      }

      stream << "]";
      stream << "}";

      return stream.str();
    }

    [[nodiscard]] softadastra::sdk::ClientOptions options_from_mode(
        const ::std::string &mode,
        const ::std::string &node_id,
        const ::std::string &wal_path)
    {
      if (mode == "memory" ||
          mode == "memory_only" ||
          mode == "memory-only")
      {
        return softadastra::sdk::ClientOptions::memory_only(node_id);
      }

      if (mode == "local")
      {
        return softadastra::sdk::ClientOptions::local(node_id);
      }

      if (mode == "fast")
      {
        return softadastra::sdk::ClientOptions::fast(
            node_id,
            wal_path);
      }

      if (mode == "persistent")
      {
        return softadastra::sdk::ClientOptions::persistent(
            node_id,
            wal_path);
      }

      return softadastra::sdk::ClientOptions::durable(
          node_id,
          wal_path);
    }

  } // namespace

  Softadastra::Softadastra()
      : config_()
  {
  }

  Softadastra::Softadastra(StdConfig config)
      : config_(::std::move(config))
  {
  }

  Result<Softadastra> Softadastra::create(
      const StdOptions &options)
  {
    auto config = StdConfig::from_options(options);
    if (!config)
    {
      return config.error();
    }

    Softadastra softadastra(config.value());

    const auto validation = softadastra.validate();
    if (validation)
    {
      return validation;
    }

    return softadastra;
  }

  const StdConfig &Softadastra::config() const noexcept
  {
    return config_;
  }

  bool Softadastra::enabled() const noexcept
  {
    return config_.module_enabled("softadastra");
  }

  Error Softadastra::validate() const
  {
    const auto config_validation = config_.validate();
    if (config_validation)
    {
      return config_validation;
    }

    if (!enabled())
    {
      return make_std_error(
          StdErrorCode::ModuleDisabled,
          "softadastra module is disabled");
    }

    if (!config_.allow_native_modules)
    {
      return make_std_error(
          StdErrorCode::InvalidConfig,
          "softadastra module requires native modules");
    }

    if (!config_.allow_native_functions)
    {
      return make_std_error(
          StdErrorCode::InvalidConfig,
          "softadastra module requires native functions");
    }

    return ok();
  }

  Result<kordex::bindings::NativeModule> Softadastra::module() const
  {
    const auto validation = validate();
    if (validation)
    {
      return validation;
    }

    kordex::bindings::NativeModuleInfo info;
    info.name = "softadastra";
    info.namespace_name = config_.namespace_name;
    info.description = "Kordex standard Softadastra module";
    info.importable = true;
    info.safe = false;

    auto module = kordex::bindings::NativeModule::create(info);

    const auto name_error = module.set_value(
        "name",
        kordex::bindings::Value::string("softadastra"));

    if (name_error)
    {
      return make_std_error(
          StdErrorCode::ModuleRegistrationFailed,
          ::std::string(name_error.message()));
    }

    const auto namespace_error = module.set_value(
        "namespace",
        kordex::bindings::Value::string(config_.namespace_name));

    if (namespace_error)
    {
      return make_std_error(
          StdErrorCode::ModuleRegistrationFailed,
          ::std::string(namespace_error.message()));
    }

    const auto version_error = module.set_value(
        "version",
        kordex::bindings::Value::string(
            ::std::string(softadastra::sdk::sdk_version())));

    if (version_error)
    {
      return make_std_error(
          StdErrorCode::ModuleRegistrationFailed,
          ::std::string(version_error.message()));
    }

    const auto open_error = module.add_function(
        make_function(
            "open",
            "Open the Softadastra SDK client",
            0,
            3,
            &Softadastra::open));

    if (open_error)
    {
      return make_std_error(
          StdErrorCode::ModuleRegistrationFailed,
          ::std::string(open_error.message()));
    }

    const auto close_error = module.add_function(
        make_function(
            "close",
            "Close the Softadastra SDK client",
            0,
            0,
            &Softadastra::close));

    if (close_error)
    {
      return make_std_error(
          StdErrorCode::ModuleRegistrationFailed,
          ::std::string(close_error.message()));
    }

    const auto is_open_error = module.add_function(
        make_function(
            "isOpen",
            "Return true if the Softadastra SDK client is open",
            0,
            0,
            &Softadastra::is_open));

    if (is_open_error)
    {
      return make_std_error(
          StdErrorCode::ModuleRegistrationFailed,
          ::std::string(is_open_error.message()));
    }

    const auto put_error = module.add_function(
        make_function(
            "put",
            "Store a string value by key",
            2,
            2,
            &Softadastra::put));

    if (put_error)
    {
      return make_std_error(
          StdErrorCode::ModuleRegistrationFailed,
          ::std::string(put_error.message()));
    }

    const auto get_error = module.add_function(
        make_function(
            "get",
            "Read a string value by key",
            1,
            1,
            &Softadastra::get));

    if (get_error)
    {
      return make_std_error(
          StdErrorCode::ModuleRegistrationFailed,
          ::std::string(get_error.message()));
    }

    const auto remove_error = module.add_function(
        make_function(
            "remove",
            "Remove a value by key",
            1,
            1,
            &Softadastra::remove));

    if (remove_error)
    {
      return make_std_error(
          StdErrorCode::ModuleRegistrationFailed,
          ::std::string(remove_error.message()));
    }

    const auto contains_error = module.add_function(
        make_function(
            "contains",
            "Return true if a key exists",
            1,
            1,
            &Softadastra::contains));

    if (contains_error)
    {
      return make_std_error(
          StdErrorCode::ModuleRegistrationFailed,
          ::std::string(contains_error.message()));
    }

    const auto size_error = module.add_function(
        make_function(
            "size",
            "Return local store size",
            0,
            0,
            &Softadastra::size));

    if (size_error)
    {
      return make_std_error(
          StdErrorCode::ModuleRegistrationFailed,
          ::std::string(size_error.message()));
    }

    const auto tick_error = module.add_function(
        make_function(
            "tick",
            "Run one Softadastra sync tick",
            0,
            1,
            &Softadastra::tick));

    if (tick_error)
    {
      return make_std_error(
          StdErrorCode::ModuleRegistrationFailed,
          ::std::string(tick_error.message()));
    }

    const auto sync_state_error = module.add_function(
        make_function(
            "syncState",
            "Return Softadastra sync state as JSON",
            0,
            0,
            &Softadastra::sync_state));

    if (sync_state_error)
    {
      return make_std_error(
          StdErrorCode::ModuleRegistrationFailed,
          ::std::string(sync_state_error.message()));
    }

    const auto node_info_error = module.add_function(
        make_function(
            "nodeInfo",
            "Return local node metadata as JSON",
            0,
            0,
            &Softadastra::node_info));

    if (node_info_error)
    {
      return make_std_error(
          StdErrorCode::ModuleRegistrationFailed,
          ::std::string(node_info_error.message()));
    }

    return module;
  }

  kordex::bindings::Result<kordex::bindings::Value> Softadastra::open(
      const kordex::bindings::NativeFunctionArguments &args)
  {
    ::std::string mode = "durable";
    ::std::string node_id = "kordex-node";
    ::std::string wal_path = ".kordex/data/kordex-store.wal";

    if (args.size() > 0)
    {
      auto value = string_argument(args, 0, "mode");
      if (!value)
      {
        return value.error();
      }

      mode = value.value();
    }

    if (args.size() > 1)
    {
      auto value = string_argument(args, 1, "node");
      if (!value)
      {
        return value.error();
      }

      node_id = value.value();
    }

    if (args.size() > 2)
    {
      auto value = string_argument(args, 2, "wal");
      if (!value)
      {
        return value.error();
      }

      wal_path = value.value();
    }

    auto options = options_from_mode(
        mode,
        node_id,
        wal_path);

    if (mode == "durable" ||
        mode == "persistent" ||
        mode == "fast")
    {
      const auto directory_error = ensure_parent_directory(wal_path);
      if (directory_error)
      {
        return directory_error;
      }
    }

    options.set_metadata(
        "Kordex",
        ::std::string(softadastra::sdk::sdk_version()));

    auto client = ::std::make_unique<softadastra::sdk::Client>(
        ::std::move(options));

    auto result = client->open();
    if (result.is_err())
    {
      return make_softadastra_binding_error(result.error());
    }

    auto &state = runtime();
    ::std::lock_guard<::std::mutex> lock(state.mutex);

    state.client = ::std::move(client);

    return kordex::bindings::Value::boolean(true);
  }

  kordex::bindings::Result<kordex::bindings::Value> Softadastra::close(
      const kordex::bindings::NativeFunctionArguments &args)
  {
    if (!args.empty())
    {
      return kordex::bindings::make_binding_error(
          kordex::bindings::BindingErrorCode::InvalidArgument,
          "softadastra.close expects no arguments");
    }

    auto &state = runtime();
    ::std::lock_guard<::std::mutex> lock(state.mutex);

    if (state.client)
    {
      state.client->close();
      state.client.reset();
    }

    return kordex::bindings::Value::undefined();
  }

  kordex::bindings::Result<kordex::bindings::Value> Softadastra::is_open(
      const kordex::bindings::NativeFunctionArguments &args)
  {
    if (!args.empty())
    {
      return kordex::bindings::make_binding_error(
          kordex::bindings::BindingErrorCode::InvalidArgument,
          "softadastra.isOpen expects no arguments");
    }

    auto &state = runtime();
    ::std::lock_guard<::std::mutex> lock(state.mutex);

    return kordex::bindings::Value::boolean(
        state.client && state.client->is_open());
  }

  kordex::bindings::Result<kordex::bindings::Value> Softadastra::put(
      const kordex::bindings::NativeFunctionArguments &args)
  {
    auto key = string_argument(args, 0, "key");
    if (!key)
    {
      return key.error();
    }

    auto value = string_argument(args, 1, "value");
    if (!value)
    {
      return value.error();
    }

    auto &state = runtime();
    ::std::lock_guard<::std::mutex> lock(state.mutex);

    auto client = require_client();
    if (!client)
    {
      return client.error();
    }

    auto result = client.value()->put(
        key.value(),
        value.value());

    if (result.is_err())
    {
      return make_softadastra_binding_error(result.error());
    }

    return kordex::bindings::Value::undefined();
  }

  kordex::bindings::Result<kordex::bindings::Value> Softadastra::get(
      const kordex::bindings::NativeFunctionArguments &args)
  {
    auto key = string_argument(args, 0, "key");
    if (!key)
    {
      return key.error();
    }

    auto &state = runtime();
    ::std::lock_guard<::std::mutex> lock(state.mutex);

    auto client = require_client();
    if (!client)
    {
      return client.error();
    }

    auto result = client.value()->get(key.value());
    if (result.is_err())
    {
      return make_softadastra_binding_error(result.error());
    }

    return kordex::bindings::Value::string(
        result.value().to_string());
  }

  kordex::bindings::Result<kordex::bindings::Value> Softadastra::remove(
      const kordex::bindings::NativeFunctionArguments &args)
  {
    auto key = string_argument(args, 0, "key");
    if (!key)
    {
      return key.error();
    }

    auto &state = runtime();
    ::std::lock_guard<::std::mutex> lock(state.mutex);

    auto client = require_client();
    if (!client)
    {
      return client.error();
    }

    auto result = client.value()->remove(key.value());
    if (result.is_err())
    {
      return make_softadastra_binding_error(result.error());
    }

    return kordex::bindings::Value::undefined();
  }

  kordex::bindings::Result<kordex::bindings::Value> Softadastra::contains(
      const kordex::bindings::NativeFunctionArguments &args)
  {
    auto key = string_argument(args, 0, "key");
    if (!key)
    {
      return key.error();
    }

    auto &state = runtime();
    ::std::lock_guard<::std::mutex> lock(state.mutex);

    auto client = require_client();
    if (!client)
    {
      return client.error();
    }

    return kordex::bindings::Value::boolean(
        client.value()->contains(key.value()));
  }

  kordex::bindings::Result<kordex::bindings::Value> Softadastra::size(
      const kordex::bindings::NativeFunctionArguments &args)
  {
    if (!args.empty())
    {
      return kordex::bindings::make_binding_error(
          kordex::bindings::BindingErrorCode::InvalidArgument,
          "softadastra.size expects no arguments");
    }

    auto &state = runtime();
    ::std::lock_guard<::std::mutex> lock(state.mutex);

    auto client = require_client();
    if (!client)
    {
      return client.error();
    }

    return kordex::bindings::Value::number(
        static_cast<double>(client.value()->size()));
  }

  kordex::bindings::Result<kordex::bindings::Value> Softadastra::tick(
      const kordex::bindings::NativeFunctionArguments &args)
  {
    const bool prune_completed = bool_argument_or(args, 0, false);

    auto &state = runtime();
    ::std::lock_guard<::std::mutex> lock(state.mutex);

    auto client = require_client();
    if (!client)
    {
      return client.error();
    }

    auto result = client.value()->tick(prune_completed);
    if (result.is_err())
    {
      return make_softadastra_binding_error(result.error());
    }

    return kordex::bindings::Value::string(
        tick_result_json(result.value()));
  }

  kordex::bindings::Result<kordex::bindings::Value> Softadastra::sync_state(
      const kordex::bindings::NativeFunctionArguments &args)
  {
    if (!args.empty())
    {
      return kordex::bindings::make_binding_error(
          kordex::bindings::BindingErrorCode::InvalidArgument,
          "softadastra.syncState expects no arguments");
    }

    auto &state = runtime();
    ::std::lock_guard<::std::mutex> lock(state.mutex);

    auto client = require_client();
    if (!client)
    {
      return client.error();
    }

    auto result = client.value()->sync_state();
    if (result.is_err())
    {
      return make_softadastra_binding_error(result.error());
    }

    return kordex::bindings::Value::string(
        sync_state_json(result.value()));
  }

  kordex::bindings::Result<kordex::bindings::Value> Softadastra::node_info(
      const kordex::bindings::NativeFunctionArguments &args)
  {
    if (!args.empty())
    {
      return kordex::bindings::make_binding_error(
          kordex::bindings::BindingErrorCode::InvalidArgument,
          "softadastra.nodeInfo expects no arguments");
    }

    auto &state = runtime();
    ::std::lock_guard<::std::mutex> lock(state.mutex);

    auto client = require_client();
    if (!client)
    {
      return client.error();
    }

    auto result = client.value()->node_info();
    if (result.is_err())
    {
      return make_softadastra_binding_error(result.error());
    }

    return kordex::bindings::Value::string(
        node_info_json(result.value()));
  }

  kordex::bindings::NativeFunction Softadastra::make_function(
      ::std::string name,
      ::std::string description,
      ::std::size_t min_args,
      ::std::size_t max_args,
      kordex::bindings::NativeFunctionCallback callback)
  {
    kordex::bindings::NativeFunctionInfo info;
    info.name = ::std::move(name);
    info.module_name = "softadastra";
    info.description = ::std::move(description);
    info.min_args = min_args;
    info.max_args = max_args;
    info.callable = true;
    info.safe = false;

    return kordex::bindings::NativeFunction::create(
        info,
        ::std::move(callback));
  }

  kordex::bindings::Result<::std::string> Softadastra::string_argument(
      const kordex::bindings::NativeFunctionArguments &args,
      ::std::size_t index,
      const char *name)
  {
    if (index >= args.size())
    {
      return kordex::bindings::make_binding_error(
          kordex::bindings::BindingErrorCode::InvalidArgument,
          ::std::string(name ? name : "argument") + " argument is missing");
    }

    auto value = args[index].as_string();
    if (!value)
    {
      return kordex::bindings::make_binding_error(
          kordex::bindings::BindingErrorCode::InvalidArgument,
          ::std::string(name ? name : "argument") + " argument must be a string");
    }

    if (value.value().empty())
    {
      return kordex::bindings::make_binding_error(
          kordex::bindings::BindingErrorCode::InvalidArgument,
          ::std::string(name ? name : "argument") + " argument cannot be empty");
    }

    return value.value();
  }

  bool Softadastra::bool_argument_or(
      const kordex::bindings::NativeFunctionArguments &args,
      ::std::size_t index,
      bool fallback)
  {
    if (index >= args.size())
    {
      return fallback;
    }

    auto value = args[index].as_boolean();
    if (!value)
    {
      return fallback;
    }

    return value.value();
  }

  Result<kordex::bindings::NativeModule> create_softadastra_module(
      const StdOptions &options)
  {
    auto softadastra = Softadastra::create(options);
    if (!softadastra)
    {
      return softadastra.error();
    }

    return softadastra.value().module();
  }

} // namespace kordex::standard
