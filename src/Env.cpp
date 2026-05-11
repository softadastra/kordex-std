/**
 *
 *  @file Env.cpp
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
#include <utility>

#if defined(_WIN32)
#include <stdlib.h>
#endif

#include <kordex/std/Env.hpp>

namespace kordex::standard
{
  namespace
  {
    [[nodiscard]] kordex::bindings::Error make_env_binding_error(
        const ::std::string &message)
    {
      return kordex::bindings::make_binding_error(
          kordex::bindings::BindingErrorCode::FunctionCallFailed,
          message);
    }

    [[nodiscard]] bool set_environment_variable(
        const ::std::string &name,
        const ::std::string &value) noexcept
    {
#if defined(_WIN32)
      return _putenv_s(name.c_str(), value.c_str()) == 0;
#else
      return ::setenv(name.c_str(), value.c_str(), 1) == 0;
#endif
    }

    [[nodiscard]] bool unset_environment_variable(
        const ::std::string &name) noexcept
    {
#if defined(_WIN32)
      return _putenv_s(name.c_str(), "") == 0;
#else
      return ::unsetenv(name.c_str()) == 0;
#endif
    }
  } // namespace

  Env::Env()
      : config_()
  {
  }

  Env::Env(StdConfig config)
      : config_(::std::move(config))
  {
  }

  Result<Env> Env::create(
      const StdOptions &options)
  {
    auto config = StdConfig::from_options(options);
    if (!config)
    {
      return config.error();
    }

    Env env(config.value());

    const auto validation = env.validate();
    if (validation)
    {
      return validation;
    }

    return env;
  }

  const StdConfig &Env::config() const noexcept
  {
    return config_;
  }

  bool Env::enabled() const noexcept
  {
    return config_.module_enabled("env");
  }

  Error Env::validate() const
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
          "env module is disabled");
    }

    if (!config_.allow_native_modules)
    {
      return make_std_error(
          StdErrorCode::InvalidConfig,
          "env module requires native modules");
    }

    if (!config_.allow_native_functions)
    {
      return make_std_error(
          StdErrorCode::InvalidConfig,
          "env module requires native functions");
    }

    return ok();
  }

  Result<kordex::bindings::NativeModule> Env::module() const
  {
    const auto validation = validate();
    if (validation)
    {
      return validation;
    }

    kordex::bindings::NativeModuleInfo info;
    info.name = "env";
    info.namespace_name = config_.namespace_name;
    info.description = "Kordex standard environment module";
    info.importable = true;
    info.safe = config_.safe_by_default;

    auto module = kordex::bindings::NativeModule::create(info);

    const auto name_error = module.set_value(
        "name",
        kordex::bindings::Value::string("env"));

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

    const auto get_error = module.add_function(
        make_function(
            "get",
            "Return an environment variable value",
            1,
            1,
            &Env::get));

    if (get_error)
    {
      return make_std_error(
          StdErrorCode::ModuleRegistrationFailed,
          ::std::string(get_error.message()));
    }

    const auto has_error = module.add_function(
        make_function(
            "has",
            "Return true if an environment variable exists",
            1,
            1,
            &Env::has));

    if (has_error)
    {
      return make_std_error(
          StdErrorCode::ModuleRegistrationFailed,
          ::std::string(has_error.message()));
    }

    const auto set_error = module.add_function(
        make_function(
            "set",
            "Set an environment variable",
            2,
            2,
            &Env::set));

    if (set_error)
    {
      return make_std_error(
          StdErrorCode::ModuleRegistrationFailed,
          ::std::string(set_error.message()));
    }

    const auto unset_error = module.add_function(
        make_function(
            "unset",
            "Unset an environment variable",
            1,
            1,
            &Env::unset));

    if (unset_error)
    {
      return make_std_error(
          StdErrorCode::ModuleRegistrationFailed,
          ::std::string(unset_error.message()));
    }

    return module;
  }

  kordex::bindings::Result<kordex::bindings::Value> Env::get(
      const kordex::bindings::NativeFunctionArguments &args)
  {
    auto name = string_argument(args, 0, "name");
    if (!name)
    {
      return name.error();
    }

    const char *value = ::std::getenv(name.value().c_str());
    if (!value)
    {
      return kordex::bindings::Value::null();
    }

    return kordex::bindings::Value::string(value);
  }

  kordex::bindings::Result<kordex::bindings::Value> Env::has(
      const kordex::bindings::NativeFunctionArguments &args)
  {
    auto name = string_argument(args, 0, "name");
    if (!name)
    {
      return name.error();
    }

    return kordex::bindings::Value::boolean(
        ::std::getenv(name.value().c_str()) != nullptr);
  }

  kordex::bindings::Result<kordex::bindings::Value> Env::set(
      const kordex::bindings::NativeFunctionArguments &args)
  {
    auto name = string_argument(args, 0, "name");
    if (!name)
    {
      return name.error();
    }

    auto value = string_argument(args, 1, "value");
    if (!value)
    {
      return value.error();
    }

    if (!set_environment_variable(name.value(), value.value()))
    {
      return make_env_binding_error(
          "failed to set environment variable: " + name.value());
    }

    return kordex::bindings::Value::undefined();
  }

  kordex::bindings::Result<kordex::bindings::Value> Env::unset(
      const kordex::bindings::NativeFunctionArguments &args)
  {
    auto name = string_argument(args, 0, "name");
    if (!name)
    {
      return name.error();
    }

    if (!unset_environment_variable(name.value()))
    {
      return make_env_binding_error(
          "failed to unset environment variable: " + name.value());
    }

    return kordex::bindings::Value::undefined();
  }

  kordex::bindings::NativeFunction Env::make_function(
      ::std::string name,
      ::std::string description,
      ::std::size_t min_args,
      ::std::size_t max_args,
      kordex::bindings::NativeFunctionCallback callback)
  {
    kordex::bindings::NativeFunctionInfo info;
    info.name = ::std::move(name);
    info.module_name = "env";
    info.description = ::std::move(description);
    info.min_args = min_args;
    info.max_args = max_args;
    info.callable = true;
    info.safe = true;

    return kordex::bindings::NativeFunction::create(
        info,
        ::std::move(callback));
  }

  kordex::bindings::Result<::std::string> Env::string_argument(
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

  Result<kordex::bindings::NativeModule> create_env_module(
      const StdOptions &options)
  {
    auto env = Env::create(options);
    if (!env)
    {
      return env.error();
    }

    return env.value().module();
  }

} // namespace kordex::standard
