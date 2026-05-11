/**
 *
 *  @file Timer.cpp
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

#include <chrono>
#include <cmath>
#include <thread>
#include <utility>

#include <kordex/std/Timer.hpp>

namespace kordex::standard
{
  namespace
  {
    [[nodiscard]] kordex::bindings::Error make_timer_binding_error(
        const ::std::string &message)
    {
      return kordex::bindings::make_binding_error(
          kordex::bindings::BindingErrorCode::FunctionCallFailed,
          message);
    }

    [[nodiscard]] double steady_milliseconds_now() noexcept
    {
      const auto now = ::std::chrono::steady_clock::now();
      const auto ms = ::std::chrono::duration_cast<::std::chrono::milliseconds>(
          now.time_since_epoch());

      return static_cast<double>(ms.count());
    }

    [[nodiscard]] double unix_milliseconds_now() noexcept
    {
      const auto now = ::std::chrono::system_clock::now();
      const auto ms = ::std::chrono::duration_cast<::std::chrono::milliseconds>(
          now.time_since_epoch());

      return static_cast<double>(ms.count());
    }

    [[nodiscard]] bool valid_delay(double value) noexcept
    {
      return ::std::isfinite(value) && value >= 0.0;
    }
  } // namespace

  Timer::Timer()
      : config_()
  {
  }

  Timer::Timer(StdConfig config)
      : config_(::std::move(config))
  {
  }

  Result<Timer> Timer::create(
      const StdOptions &options)
  {
    auto config = StdConfig::from_options(options);
    if (!config)
    {
      return config.error();
    }

    Timer timer(config.value());

    const auto validation = timer.validate();
    if (validation)
    {
      return validation;
    }

    return timer;
  }

  const StdConfig &Timer::config() const noexcept
  {
    return config_;
  }

  bool Timer::enabled() const noexcept
  {
    return config_.module_enabled("timer");
  }

  Error Timer::validate() const
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
          "timer module is disabled");
    }

    if (!config_.allow_native_modules)
    {
      return make_std_error(
          StdErrorCode::InvalidConfig,
          "timer module requires native modules");
    }

    if (!config_.allow_native_functions)
    {
      return make_std_error(
          StdErrorCode::InvalidConfig,
          "timer module requires native functions");
    }

    return ok();
  }

  Result<kordex::bindings::NativeModule> Timer::module() const
  {
    const auto validation = validate();
    if (validation)
    {
      return validation;
    }

    kordex::bindings::NativeModuleInfo info;
    info.name = "timer";
    info.namespace_name = config_.namespace_name;
    info.description = "Kordex standard timer module";
    info.importable = true;
    info.safe = config_.safe_by_default;

    auto module = kordex::bindings::NativeModule::create(info);

    const auto name_error = module.set_value(
        "name",
        kordex::bindings::Value::string("timer"));

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

    const auto now_error = module.add_function(
        make_function(
            "now",
            "Return current monotonic timestamp in milliseconds",
            0,
            0,
            &Timer::now));

    if (now_error)
    {
      return make_std_error(
          StdErrorCode::ModuleRegistrationFailed,
          ::std::string(now_error.message()));
    }

    const auto sleep_error = module.add_function(
        make_function(
            "sleep",
            "Sleep synchronously for a number of milliseconds",
            1,
            1,
            &Timer::sleep));

    if (sleep_error)
    {
      return make_std_error(
          StdErrorCode::ModuleRegistrationFailed,
          ::std::string(sleep_error.message()));
    }

    const auto unix_ms_error = module.add_function(
        make_function(
            "unixMs",
            "Return current UNIX timestamp in milliseconds",
            0,
            0,
            &Timer::unix_ms));

    if (unix_ms_error)
    {
      return make_std_error(
          StdErrorCode::ModuleRegistrationFailed,
          ::std::string(unix_ms_error.message()));
    }

    return module;
  }

  kordex::bindings::Result<kordex::bindings::Value> Timer::now(
      const kordex::bindings::NativeFunctionArguments &args)
  {
    if (!args.empty())
    {
      return kordex::bindings::make_binding_error(
          kordex::bindings::BindingErrorCode::InvalidArgument,
          "timer.now expects no arguments");
    }

    return kordex::bindings::Value::number(steady_milliseconds_now());
  }

  kordex::bindings::Result<kordex::bindings::Value> Timer::sleep(
      const kordex::bindings::NativeFunctionArguments &args)
  {
    auto delay = number_argument(args, 0, "delay");
    if (!delay)
    {
      return delay.error();
    }

    if (!valid_delay(delay.value()))
    {
      return kordex::bindings::make_binding_error(
          kordex::bindings::BindingErrorCode::InvalidArgument,
          "delay must be a finite positive number");
    }

    try
    {
      const auto milliseconds =
          static_cast<::std::chrono::milliseconds::rep>(delay.value());

      ::std::this_thread::sleep_for(
          ::std::chrono::milliseconds(milliseconds));

      return kordex::bindings::Value::undefined();
    }
    catch (const ::std::exception &exception)
    {
      return make_timer_binding_error(exception.what());
    }
  }

  kordex::bindings::Result<kordex::bindings::Value> Timer::unix_ms(
      const kordex::bindings::NativeFunctionArguments &args)
  {
    if (!args.empty())
    {
      return kordex::bindings::make_binding_error(
          kordex::bindings::BindingErrorCode::InvalidArgument,
          "timer.unixMs expects no arguments");
    }

    return kordex::bindings::Value::number(unix_milliseconds_now());
  }

  kordex::bindings::NativeFunction Timer::make_function(
      ::std::string name,
      ::std::string description,
      ::std::size_t min_args,
      ::std::size_t max_args,
      kordex::bindings::NativeFunctionCallback callback)
  {
    kordex::bindings::NativeFunctionInfo info;
    info.name = ::std::move(name);
    info.module_name = "timer";
    info.description = ::std::move(description);
    info.min_args = min_args;
    info.max_args = max_args;
    info.callable = true;
    info.safe = true;

    return kordex::bindings::NativeFunction::create(
        info,
        ::std::move(callback));
  }

  kordex::bindings::Result<double> Timer::number_argument(
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

    auto value = args[index].as_number();
    if (!value)
    {
      return kordex::bindings::make_binding_error(
          kordex::bindings::BindingErrorCode::InvalidArgument,
          ::std::string(name ? name : "argument") + " argument must be a number");
    }

    return value.value();
  }

  Result<kordex::bindings::NativeModule> create_timer_module(
      const StdOptions &options)
  {
    auto timer = Timer::create(options);
    if (!timer)
    {
      return timer.error();
    }

    return timer.value().module();
  }

} // namespace kordex::standard
