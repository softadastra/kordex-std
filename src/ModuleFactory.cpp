/**
 *
 *  @file ModuleFactory.cpp
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

#include <utility>
#include <kordex/std/ModuleFactory.hpp>

namespace kordex::standard
{
  namespace
  {
    [[nodiscard]] bool starts_with_kordex_prefix(
        const ::std::string &name) noexcept
    {
      constexpr const char *prefix = "kordex:";
      constexpr ::std::size_t prefix_size = 7;

      return name.size() > prefix_size &&
             name.compare(0, prefix_size, prefix) == 0;
    }

    [[nodiscard]] Result<kordex::bindings::NativeModule> disabled_module_error(
        const ::std::string &name)
    {
      return make_std_error(
          StdErrorCode::ModuleDisabled,
          "std module is disabled: " + name);
    }

    [[nodiscard]] Result<kordex::bindings::NativeModule> unknown_module_error(
        const ::std::string &name)
    {
      return make_std_error(
          StdErrorCode::InvalidArgument,
          "unknown std module: " + name);
    }
  } // namespace

  ModuleFactory::ModuleFactory()
      : config_()
  {
  }

  ModuleFactory::ModuleFactory(StdConfig config)
      : config_(::std::move(config))
  {
  }

  Result<ModuleFactory> ModuleFactory::create(
      const StdOptions &options)
  {
    auto config = StdConfig::from_options(options);
    if (!config)
    {
      return config.error();
    }

    ModuleFactory factory(config.value());

    const auto validation = factory.validate();
    if (validation)
    {
      return validation;
    }

    return factory;
  }

  const StdConfig &ModuleFactory::config() const noexcept
  {
    return config_;
  }

  bool ModuleFactory::enabled() const noexcept
  {
    return config_.enabled;
  }

  Error ModuleFactory::validate() const
  {
    const auto config_validation = config_.validate();
    if (config_validation)
    {
      return config_validation;
    }

    if (!config_.allow_native_modules)
    {
      return make_std_error(
          StdErrorCode::InvalidConfig,
          "module factory requires native modules");
    }

    if (!config_.allow_native_functions)
    {
      return make_std_error(
          StdErrorCode::InvalidConfig,
          "module factory requires native functions");
    }

    return ok();
  }

  bool ModuleFactory::can_create(
      const ::std::string &name) const noexcept
  {
    const auto canonical = canonical_name(name);

    return known_module(canonical) &&
           config_.module_enabled(canonical);
  }

  Result<kordex::bindings::NativeModule> ModuleFactory::create_module(
      const ::std::string &name) const
  {
    const auto validation = validate();
    if (validation)
    {
      return validation;
    }

    const auto canonical = canonical_name(name);

    if (!known_module(canonical))
    {
      return unknown_module_error(name);
    }

    if (!config_.module_enabled(canonical))
    {
      return disabled_module_error(canonical);
    }

    StdOptions options;
    options.registry_name = config_.registry_name;
    options.namespace_name = config_.namespace_name;
    options.module_paths = config_.module_paths;

    options.enabled = config_.enabled;
    options.safe_by_default = config_.safe_by_default;
    options.allow_native_modules = config_.allow_native_modules;
    options.allow_native_functions = config_.allow_native_functions;

    options.enable_console = config_.enable_console;
    options.enable_fs = config_.enable_fs;
    options.enable_path = config_.enable_path;
    options.enable_env = config_.enable_env;
    options.enable_process = config_.enable_process;
    options.enable_timer = config_.enable_timer;
    options.enable_crypto = config_.enable_crypto;
    options.enable_http = config_.enable_http;
    options.enable_softadastra = config_.enable_softadastra;

    options.max_modules = config_.max_modules;

    if (canonical == "console")
    {
      return create_console_module(options);
    }

    if (canonical == "fs")
    {
      return create_fs_module(options);
    }

    if (canonical == "path")
    {
      return create_path_module(options);
    }

    if (canonical == "env")
    {
      return create_env_module(options);
    }

    if (canonical == "process")
    {
      return create_process_module(options);
    }

    if (canonical == "timer")
    {
      return create_timer_module(options);
    }

    if (canonical == "crypto")
    {
      return create_crypto_module(options);
    }

    if (canonical == "http")
    {
      return create_http_module(options);
    }

    if (canonical == "softadastra")
    {
#if KORDEX_STD_ENABLE_SOFTADASTRA
      return create_softadastra_module(options);
#else
      return disabled_module_error(canonical);
#endif
    }
    return unknown_module_error(name);
  }

  Result<::std::vector<kordex::bindings::NativeModule>> ModuleFactory::create_all() const
  {
    const auto validation = validate();
    if (validation)
    {
      return validation;
    }

    ::std::vector<kordex::bindings::NativeModule> modules;
    modules.reserve(config_.enabled_module_count());

    for (const auto &name : enabled_module_names())
    {
      auto module = create_module(name);
      if (!module)
      {
        return module.error();
      }

      modules.push_back(::std::move(module.value()));
    }

    return modules;
  }

  ::std::vector<::std::string> ModuleFactory::enabled_module_names() const
  {
    ::std::vector<::std::string> names;

    if (!config_.enabled)
    {
      return names;
    }

    if (config_.enable_console)
    {
      names.push_back("console");
    }

    if (config_.enable_fs)
    {
      names.push_back("fs");
    }

    if (config_.enable_path)
    {
      names.push_back("path");
    }

    if (config_.enable_env)
    {
      names.push_back("env");
    }

    if (config_.enable_process)
    {
      names.push_back("process");
    }

    if (config_.enable_timer)
    {
      names.push_back("timer");
    }

    if (config_.enable_crypto)
    {
      names.push_back("crypto");
    }

    if (config_.enable_http)
    {
      names.push_back("http");
    }

#if KORDEX_STD_ENABLE_SOFTADASTRA
    if (config_.enable_softadastra)
    {
      names.push_back("softadastra");
    }
#endif

    return names;
  }

  ::std::string ModuleFactory::canonical_name(
      const ::std::string &name)
  {
    if (starts_with_kordex_prefix(name))
    {
      return name.substr(7);
    }

    return name;
  }

  bool ModuleFactory::known_module(
      const ::std::string &name) noexcept
  {
    if (name == "console" ||
        name == "fs" ||
        name == "path" ||
        name == "env" ||
        name == "process" ||
        name == "timer" ||
        name == "crypto" ||
        name == "http" ||
        name == "kordex:console" ||
        name == "kordex:fs" ||
        name == "kordex:path" ||
        name == "kordex:env" ||
        name == "kordex:process" ||
        name == "kordex:timer" ||
        name == "kordex:crypto" ||
        name == "kordex:http")
    {
      return true;
    }

#if KORDEX_STD_ENABLE_SOFTADASTRA
    return name == "softadastra" ||
           name == "kordex:softadastra";
#else
    return false;
#endif
  }
  Result<kordex::bindings::NativeModule> create_std_module(
      const ::std::string &name,
      const StdOptions &options)
  {
    auto factory = ModuleFactory::create(options);
    if (!factory)
    {
      return factory.error();
    }

    return factory.value().create_module(name);
  }

  Result<::std::vector<kordex::bindings::NativeModule>> create_std_modules(
      const StdOptions &options)
  {
    auto factory = ModuleFactory::create(options);
    if (!factory)
    {
      return factory.error();
    }

    return factory.value().create_all();
  }

} // namespace kordex::standard
