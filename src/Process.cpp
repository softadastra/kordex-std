/**
 *
 *  @file Process.cpp
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

#include <cstdlib>
#include <filesystem>
#include <utility>

#include <kordex/std/Process.hpp>

namespace kordex::standard
{
  namespace
  {
    [[nodiscard]] kordex::bindings::Error make_process_binding_error(
        const ::std::string &message)
    {
      return kordex::bindings::make_binding_error(
          kordex::bindings::BindingErrorCode::FunctionCallFailed,
          message);
    }

    [[nodiscard]] ::std::string current_path_string()
    {
      return ::std::filesystem::current_path().generic_string();
    }
  } // namespace

  Process::Process()
      : config_()
  {
  }

  Process::Process(StdConfig config)
      : config_(::std::move(config))
  {
  }

  Result<Process> Process::create(
      const StdOptions &options)
  {
    auto config = StdConfig::from_options(options);
    if (!config)
    {
      return config.error();
    }

    Process process(config.value());

    const auto validation = process.validate();
    if (validation)
    {
      return validation;
    }

    return process;
  }

  const StdConfig &Process::config() const noexcept
  {
    return config_;
  }

  bool Process::enabled() const noexcept
  {
    return config_.module_enabled("process");
  }

  Error Process::validate() const
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
          "process module is disabled");
    }

    if (!config_.allow_native_modules)
    {
      return make_std_error(
          StdErrorCode::InvalidConfig,
          "process module requires native modules");
    }

    if (!config_.allow_native_functions)
    {
      return make_std_error(
          StdErrorCode::InvalidConfig,
          "process module requires native functions");
    }

    return ok();
  }

  Result<kordex::bindings::NativeModule> Process::module() const
  {
    const auto validation = validate();
    if (validation)
    {
      return validation;
    }

    kordex::bindings::NativeModuleInfo info;
    info.name = "process";
    info.namespace_name = config_.namespace_name;
    info.description = "Kordex standard process module";
    info.importable = true;
    info.safe = false;

    auto module = kordex::bindings::NativeModule::create(info);

    const auto name_error = module.set_value(
        "name",
        kordex::bindings::Value::string("process"));

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

    const auto cwd_error = module.add_function(
        make_function(
            "cwd",
            "Return current working directory",
            0,
            0,
            &Process::cwd));

    if (cwd_error)
    {
      return make_std_error(
          StdErrorCode::ModuleRegistrationFailed,
          ::std::string(cwd_error.message()));
    }

    const auto chdir_error = module.add_function(
        make_function(
            "chdir",
            "Change current working directory",
            1,
            1,
            &Process::chdir));

    if (chdir_error)
    {
      return make_std_error(
          StdErrorCode::ModuleRegistrationFailed,
          ::std::string(chdir_error.message()));
    }

    const auto run_error = module.add_function(
        make_function(
            "run",
            "Run a shell command and return its exit code",
            1,
            1,
            &Process::run));

    if (run_error)
    {
      return make_std_error(
          StdErrorCode::ModuleRegistrationFailed,
          ::std::string(run_error.message()));
    }

    return module;
  }

  kordex::bindings::Result<kordex::bindings::Value> Process::cwd(
      const kordex::bindings::NativeFunctionArguments &args)
  {
    if (!args.empty())
    {
      return kordex::bindings::make_binding_error(
          kordex::bindings::BindingErrorCode::InvalidArgument,
          "cwd expects no arguments");
    }

    try
    {
      return kordex::bindings::Value::string(current_path_string());
    }
    catch (const ::std::exception &exception)
    {
      return make_process_binding_error(exception.what());
    }
  }

  kordex::bindings::Result<kordex::bindings::Value> Process::chdir(
      const kordex::bindings::NativeFunctionArguments &args)
  {
    auto path = string_argument(args, 0, "path");
    if (!path)
    {
      return path.error();
    }

    try
    {
      ::std::filesystem::current_path(path.value());
      return kordex::bindings::Value::undefined();
    }
    catch (const ::std::exception &exception)
    {
      return make_process_binding_error(exception.what());
    }
  }

  kordex::bindings::Result<kordex::bindings::Value> Process::run(
      const kordex::bindings::NativeFunctionArguments &args)
  {
    auto command = string_argument(args, 0, "command");
    if (!command)
    {
      return command.error();
    }

    const int exit_code = ::std::system(command.value().c_str());

    return kordex::bindings::Value::number(
        static_cast<double>(exit_code));
  }

  kordex::bindings::NativeFunction Process::make_function(
      ::std::string name,
      ::std::string description,
      ::std::size_t min_args,
      ::std::size_t max_args,
      kordex::bindings::NativeFunctionCallback callback)
  {
    kordex::bindings::NativeFunctionInfo info;
    info.name = ::std::move(name);
    info.module_name = "process";
    info.description = ::std::move(description);
    info.min_args = min_args;
    info.max_args = max_args;
    info.callable = true;
    info.safe = false;

    return kordex::bindings::NativeFunction::create(
        info,
        ::std::move(callback));
  }

  kordex::bindings::Result<::std::string> Process::string_argument(
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

  Result<kordex::bindings::NativeModule> create_process_module(
      const StdOptions &options)
  {
    auto process = Process::create(options);
    if (!process)
    {
      return process.error();
    }

    return process.value().module();
  }

} // namespace kordex::standard
