/**
 *
 *  @file Console.cpp
 *  @author Gaspard Kirira
 *
 *  Copyright 2026, Gaspard Kirira.
 *  All rights reserved.
 *  https://github.com/kordexjs/std
 *
 *  Use of this source code is governed by a MIT license
 *  that can be found in the LICENSE file.
 *
 *  Kordex Std
 *
 */

#include <iostream>
#include <sstream>
#include <utility>

#include <kordex/std/Console.hpp>

namespace kordex::standard
{
  namespace
  {
    [[nodiscard]] ::std::ostream &stream_for_level(
        ConsoleLevel level) noexcept
    {
      switch (level)
      {
      case ConsoleLevel::Error:
      case ConsoleLevel::Warn:
        return ::std::cerr;

      case ConsoleLevel::Log:
      case ConsoleLevel::Info:
      case ConsoleLevel::Debug:
        return ::std::cout;
      }

      return ::std::cout;
    }
  } // namespace

  Console::Console()
      : config_()
  {
  }

  Console::Console(StdConfig config)
      : config_(::std::move(config))
  {
  }

  Result<Console> Console::create(
      const StdOptions &options)
  {
    auto config = StdConfig::from_options(options);
    if (!config)
    {
      return config.error();
    }

    Console console(config.value());

    const auto validation = console.validate();
    if (validation)
    {
      return validation;
    }

    return console;
  }

  const StdConfig &Console::config() const noexcept
  {
    return config_;
  }

  bool Console::enabled() const noexcept
  {
    return config_.module_enabled("console");
  }

  Error Console::validate() const
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
          "console module is disabled");
    }

    if (!config_.allow_native_modules)
    {
      return make_std_error(
          StdErrorCode::InvalidConfig,
          "console module requires native modules");
    }

    if (!config_.allow_native_functions)
    {
      return make_std_error(
          StdErrorCode::InvalidConfig,
          "console module requires native functions");
    }

    return ok();
  }

  Result<kordex::bindings::NativeModule> Console::module() const
  {
    const auto validation = validate();
    if (validation)
    {
      return validation;
    }

    kordex::bindings::NativeModuleInfo info;
    info.name = "console";
    info.namespace_name = config_.namespace_name;
    info.description = "Kordex standard console module";
    info.importable = true;
    info.safe = config_.safe_by_default;

    auto module = kordex::bindings::NativeModule::create(info);

    const auto name_error = module.set_value(
        "name",
        kordex::bindings::Value::string("console"));

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

    const auto log_error = module.add_function(
        make_function(ConsoleLevel::Log, "log"));

    if (log_error)
    {
      return make_std_error(
          StdErrorCode::ModuleRegistrationFailed,
          ::std::string(log_error.message()));
    }

    const auto info_error = module.add_function(
        make_function(ConsoleLevel::Info, "info"));

    if (info_error)
    {
      return make_std_error(
          StdErrorCode::ModuleRegistrationFailed,
          ::std::string(info_error.message()));
    }

    const auto warn_error = module.add_function(
        make_function(ConsoleLevel::Warn, "warn"));

    if (warn_error)
    {
      return make_std_error(
          StdErrorCode::ModuleRegistrationFailed,
          ::std::string(warn_error.message()));
    }

    const auto error_error = module.add_function(
        make_function(ConsoleLevel::Error, "error"));

    if (error_error)
    {
      return make_std_error(
          StdErrorCode::ModuleRegistrationFailed,
          ::std::string(error_error.message()));
    }

    const auto debug_error = module.add_function(
        make_function(ConsoleLevel::Debug, "debug"));

    if (debug_error)
    {
      return make_std_error(
          StdErrorCode::ModuleRegistrationFailed,
          ::std::string(debug_error.message()));
    }

    return module;
  }

  kordex::bindings::Result<kordex::bindings::Value> Console::write(
      ConsoleLevel level,
      const kordex::bindings::NativeFunctionArguments &args)
  {
    auto &output = stream_for_level(level);

    output << '[' << Console::to_string(level) << ']';

    const auto text = format_arguments(args);
    if (!text.empty())
    {
      output << ' ' << text;
    }

    output << '\n';

    return kordex::bindings::Value::undefined();
  }

  ::std::string Console::format_arguments(
      const kordex::bindings::NativeFunctionArguments &args)
  {
    ::std::ostringstream stream;

    for (::std::size_t index = 0; index < args.size(); ++index)
    {
      if (index > 0)
      {
        stream << ' ';
      }

      stream << args[index].display();
    }

    return stream.str();
  }

  const char *Console::to_string(
      ConsoleLevel level) noexcept
  {
    switch (level)
    {
    case ConsoleLevel::Log:
      return "log";
    case ConsoleLevel::Info:
      return "info";
    case ConsoleLevel::Warn:
      return "warn";
    case ConsoleLevel::Error:
      return "error";
    case ConsoleLevel::Debug:
      return "debug";
    }

    return "log";
  }

  kordex::bindings::NativeFunction Console::make_function(
      ConsoleLevel level,
      ::std::string name)
  {
    kordex::bindings::NativeFunctionInfo info;
    info.name = ::std::move(name);
    info.module_name = "console";
    info.description = "Write values to the console";
    info.min_args = 0;
    info.max_args = 0;
    info.callable = true;
    info.safe = true;

    return kordex::bindings::NativeFunction::create(
        info,
        [level](const kordex::bindings::NativeFunctionArguments &args)
            -> kordex::bindings::Result<kordex::bindings::Value>
        {
          return Console::write(level, args);
        });
  }

  Result<kordex::bindings::NativeModule> create_console_module(
      const StdOptions &options)
  {
    auto console = Console::create(options);
    if (!console)
    {
      return console.error();
    }

    return console.value().module();
  }

  const char *to_string(
      ConsoleLevel level) noexcept
  {
    return Console::to_string(level);
  }

} // namespace kordex::standard
