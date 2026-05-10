/**
 *
 *  @file test_console.cpp
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

#include <string_view>
#include <kordex/std/Console.hpp>

namespace
{
  [[nodiscard]] bool expect_true(
      bool condition,
      const char *message)
  {
    if (!condition)
    {
      (void)message;
      return false;
    }

    return true;
  }

  template <typename T>
  [[nodiscard]] bool result_ok(
      const kordex::standard::Result<T> &result)
  {
    return !(!result);
  }

  template <typename T>
  [[nodiscard]] bool binding_result_ok(
      const kordex::bindings::Result<T> &result)
  {
    return !(!result);
  }

  [[nodiscard]] bool test_console_level_strings()
  {
    return expect_true(
               ::std::string_view(kordex::standard::to_string(
                   kordex::standard::ConsoleLevel::Log)) == "log",
               "log level string should match") &&
           expect_true(
               ::std::string_view(kordex::standard::to_string(
                   kordex::standard::ConsoleLevel::Info)) == "info",
               "info level string should match") &&
           expect_true(
               ::std::string_view(kordex::standard::to_string(
                   kordex::standard::ConsoleLevel::Warn)) == "warn",
               "warn level string should match") &&
           expect_true(
               ::std::string_view(kordex::standard::to_string(
                   kordex::standard::ConsoleLevel::Error)) == "error",
               "error level string should match") &&
           expect_true(
               ::std::string_view(kordex::standard::to_string(
                   kordex::standard::ConsoleLevel::Debug)) == "debug",
               "debug level string should match");
  }

  [[nodiscard]] bool test_create_console()
  {
    auto result = kordex::standard::Console::create(
        kordex::standard::StdOptions::defaults());

    if (!expect_true(
            result_ok(result),
            "console creation should succeed"))
    {
      return false;
    }

    const auto &console = result.value();

    return expect_true(
               console.enabled(),
               "console should be enabled") &&
           expect_true(
               console.config().module_enabled("console"),
               "config should enable console") &&
           expect_true(
               !console.validate(),
               "console validation should succeed");
  }

  [[nodiscard]] bool test_create_console_module()
  {
    auto result = kordex::standard::create_console_module();

    if (!expect_true(
            result_ok(result),
            "console module creation should succeed"))
    {
      return false;
    }

    const auto &module = result.value();

    return expect_true(
               module.valid(),
               "console module should be valid") &&
           expect_true(
               module.importable(),
               "console module should be importable") &&
           expect_true(
               module.safe(),
               "console module should be safe") &&
           expect_true(
               ::std::string_view(module.name()) == "console",
               "console module name should match") &&
           expect_true(
               module.has_function("log"),
               "console module should expose log") &&
           expect_true(
               module.has_function("info"),
               "console module should expose info") &&
           expect_true(
               module.has_function("warn"),
               "console module should expose warn") &&
           expect_true(
               module.has_function("error"),
               "console module should expose error") &&
           expect_true(
               module.has_function("debug"),
               "console module should expose debug") &&
           expect_true(
               module.function_count() == 5,
               "console module should expose five functions") &&
           expect_true(
               module.has_value("name"),
               "console module should expose name value") &&
           expect_true(
               module.has_value("namespace"),
               "console module should expose namespace value");
  }

  [[nodiscard]] bool test_console_module_values()
  {
    auto result = kordex::standard::create_console_module();

    if (!expect_true(
            result_ok(result),
            "console module creation should succeed"))
    {
      return false;
    }

    auto name = result.value().value("name");
    auto namespace_name = result.value().value("namespace");

    if (!expect_true(
            binding_result_ok(name) &&
                binding_result_ok(namespace_name),
            "console module values should exist"))
    {
      return false;
    }

    auto name_text = name.value().as_string();
    auto namespace_text = namespace_name.value().as_string();

    if (!expect_true(
            binding_result_ok(name_text) &&
                binding_result_ok(namespace_text),
            "console module values should convert to string"))
    {
      return false;
    }

    return expect_true(
               name_text.value() == "console",
               "console name value should match") &&
           expect_true(
               namespace_text.value() == "kordex",
               "console namespace value should match");
  }

  [[nodiscard]] bool test_format_arguments()
  {
    kordex::bindings::NativeFunctionArguments args{
        kordex::bindings::Value::string("hello"),
        kordex::bindings::Value::number(42.0),
        kordex::bindings::Value::boolean(true)};

    const auto text = kordex::standard::Console::format_arguments(args);

    return expect_true(
        ::std::string_view(text) == "hello 42 true",
        "formatted console arguments should match");
  }

  [[nodiscard]] bool test_format_empty_arguments()
  {
    const kordex::bindings::NativeFunctionArguments args{};

    const auto text = kordex::standard::Console::format_arguments(args);

    return expect_true(
        text.empty(),
        "empty console arguments should format to empty string");
  }

  [[nodiscard]] bool test_write_returns_undefined()
  {
    kordex::bindings::NativeFunctionArguments args{
        kordex::bindings::Value::string("hello")};

    auto result = kordex::standard::Console::write(
        kordex::standard::ConsoleLevel::Log,
        args);

    if (!expect_true(
            binding_result_ok(result),
            "console write should succeed"))
    {
      return false;
    }

    return expect_true(
        result.value().is_undefined(),
        "console write should return undefined");
  }

  [[nodiscard]] bool test_call_console_log()
  {
    auto module_result = kordex::standard::create_console_module();

    if (!expect_true(
            result_ok(module_result),
            "console module creation should succeed"))
    {
      return false;
    }

    kordex::bindings::NativeFunctionArguments args{
        kordex::bindings::Value::string("hello"),
        kordex::bindings::Value::string("console")};

    auto call_result = module_result.value().call("log", args);

    if (!expect_true(
            binding_result_ok(call_result),
            "console.log call should succeed"))
    {
      return false;
    }

    return expect_true(
        call_result.value().is_undefined(),
        "console.log should return undefined");
  }

  [[nodiscard]] bool test_call_all_console_functions()
  {
    auto module_result = kordex::standard::create_console_module();

    if (!expect_true(
            result_ok(module_result),
            "console module creation should succeed"))
    {
      return false;
    }

    kordex::bindings::NativeFunctionArguments args{
        kordex::bindings::Value::string("message")};

    auto log = module_result.value().call("log", args);
    auto info = module_result.value().call("info", args);
    auto warn = module_result.value().call("warn", args);
    auto error = module_result.value().call("error", args);
    auto debug = module_result.value().call("debug", args);

    return expect_true(
               binding_result_ok(log),
               "console.log should succeed") &&
           expect_true(
               binding_result_ok(info),
               "console.info should succeed") &&
           expect_true(
               binding_result_ok(warn),
               "console.warn should succeed") &&
           expect_true(
               binding_result_ok(error),
               "console.error should succeed") &&
           expect_true(
               binding_result_ok(debug),
               "console.debug should succeed") &&
           expect_true(
               log.value().is_undefined() &&
                   info.value().is_undefined() &&
                   warn.value().is_undefined() &&
                   error.value().is_undefined() &&
                   debug.value().is_undefined(),
               "console functions should return undefined");
  }

  [[nodiscard]] bool test_console_disabled()
  {
    auto options = kordex::standard::StdOptions::defaults();
    options.enable_console = false;

    auto result = kordex::standard::Console::create(options);

    return expect_true(
               !result,
               "disabled console should fail creation") &&
           expect_true(
               result.error().code() ==
                   vix::error::ErrorCode::PermissionDenied,
               "disabled console should map to permission denied");
  }

  [[nodiscard]] bool test_console_native_functions_disabled()
  {
    auto options = kordex::standard::StdOptions::defaults();
    options.allow_native_functions = false;

    auto result = kordex::standard::Console::create(options);

    return expect_true(
               !result,
               "console without native functions should fail") &&
           expect_true(
               result.error().code() ==
                   vix::error::ErrorCode::ConfigError,
               "disabled native functions should map to config error");
  }
} // namespace

int main()
{
  const bool ok =
      test_console_level_strings() &&
      test_create_console() &&
      test_create_console_module() &&
      test_console_module_values() &&
      test_format_arguments() &&
      test_format_empty_arguments() &&
      test_write_returns_undefined() &&
      test_call_console_log() &&
      test_call_all_console_functions() &&
      test_console_disabled() &&
      test_console_native_functions_disabled();

  return ok ? 0 : 1;
}
