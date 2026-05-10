/**
 *
 *  @file test_process.cpp
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

#include <filesystem>
#include <string_view>

#include <kordex/std/Process.hpp>

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

  [[nodiscard]] bool test_create_process()
  {
    auto result = kordex::standard::Process::create(
        kordex::standard::StdOptions::defaults());

    if (!expect_true(
            result_ok(result),
            "process creation should succeed"))
    {
      return false;
    }

    const auto &process = result.value();

    return expect_true(
               process.enabled(),
               "process should be enabled") &&
           expect_true(
               process.config().module_enabled("process"),
               "config should enable process") &&
           expect_true(
               !process.validate(),
               "process validation should succeed");
  }

  [[nodiscard]] bool test_create_process_module()
  {
    auto result = kordex::standard::create_process_module();

    if (!expect_true(
            result_ok(result),
            "process module creation should succeed"))
    {
      return false;
    }

    const auto &module = result.value();

    return expect_true(
               module.valid(),
               "process module should be valid") &&
           expect_true(
               module.importable(),
               "process module should be importable") &&
           expect_true(
               !module.safe(),
               "process module should not be marked safe") &&
           expect_true(
               ::std::string_view(module.name()) == "process",
               "process module name should match") &&
           expect_true(
               module.has_function("cwd"),
               "process module should expose cwd") &&
           expect_true(
               module.has_function("chdir"),
               "process module should expose chdir") &&
           expect_true(
               module.has_function("run"),
               "process module should expose run") &&
           expect_true(
               module.function_count() == 3,
               "process module should expose three functions") &&
           expect_true(
               module.has_value("name"),
               "process module should expose name value") &&
           expect_true(
               module.has_value("namespace"),
               "process module should expose namespace value");
  }

  [[nodiscard]] bool test_process_module_values()
  {
    auto result = kordex::standard::create_process_module();

    if (!expect_true(
            result_ok(result),
            "process module creation should succeed"))
    {
      return false;
    }

    auto name = result.value().value("name");
    auto namespace_name = result.value().value("namespace");

    if (!expect_true(
            binding_result_ok(name) &&
                binding_result_ok(namespace_name),
            "process module values should exist"))
    {
      return false;
    }

    auto name_text = name.value().as_string();
    auto namespace_text = namespace_name.value().as_string();

    if (!expect_true(
            binding_result_ok(name_text) &&
                binding_result_ok(namespace_text),
            "process module values should convert to string"))
    {
      return false;
    }

    return expect_true(
               name_text.value() == "process",
               "process name value should match") &&
           expect_true(
               namespace_text.value() == "kordex",
               "process namespace value should match");
  }

  [[nodiscard]] bool test_cwd()
  {
    auto module_result = kordex::standard::create_process_module();

    if (!expect_true(
            result_ok(module_result),
            "process module creation should succeed"))
    {
      return false;
    }

    auto cwd_result = module_result.value().call("cwd", {});

    if (!expect_true(
            binding_result_ok(cwd_result),
            "process.cwd should succeed"))
    {
      return false;
    }

    auto cwd = cwd_result.value().as_string();

    if (!expect_true(
            binding_result_ok(cwd),
            "process.cwd result should convert to string"))
    {
      return false;
    }

    return expect_true(
        !cwd.value().empty(),
        "process.cwd should return non-empty path");
  }

  [[nodiscard]] bool test_chdir()
  {
    auto module_result = kordex::standard::create_process_module();

    if (!expect_true(
            result_ok(module_result),
            "process module creation should succeed"))
    {
      return false;
    }

    const auto original = ::std::filesystem::current_path();
    const auto target = ::std::filesystem::temp_directory_path();

    kordex::bindings::NativeFunctionArguments args{
        kordex::bindings::Value::string(target.string())};

    auto chdir_result = module_result.value().call("chdir", args);
    auto cwd_result = module_result.value().call("cwd", {});

    ::std::filesystem::current_path(original);

    if (!expect_true(
            binding_result_ok(chdir_result) &&
                binding_result_ok(cwd_result),
            "process.chdir and process.cwd should succeed"))
    {
      return false;
    }

    auto cwd = cwd_result.value().as_string();

    if (!expect_true(
            binding_result_ok(cwd),
            "cwd after chdir should convert to string"))
    {
      return false;
    }

    const auto observed = ::std::filesystem::path(cwd.value()).lexically_normal();
    const auto expected = target.lexically_normal();

    return expect_true(
               chdir_result.value().is_undefined(),
               "process.chdir should return undefined") &&
           expect_true(
               observed == expected,
               "process.chdir should change current directory");
  }

  [[nodiscard]] bool test_run()
  {
    auto module_result = kordex::standard::create_process_module();

    if (!expect_true(
            result_ok(module_result),
            "process module creation should succeed"))
    {
      return false;
    }

#if defined(_WIN32)
    const char *command = "cd";
#else
    const char *command = "true";
#endif

    kordex::bindings::NativeFunctionArguments args{
        kordex::bindings::Value::string(command)};

    auto run_result = module_result.value().call("run", args);

    if (!expect_true(
            binding_result_ok(run_result),
            "process.run should succeed"))
    {
      return false;
    }

    auto exit_code = run_result.value().as_number();

    if (!expect_true(
            binding_result_ok(exit_code),
            "process.run result should convert to number"))
    {
      return false;
    }

    return expect_true(
        exit_code.value() == 0.0,
        "process.run should return zero for a successful command");
  }

  [[nodiscard]] bool test_invalid_arguments()
  {
    auto module_result = kordex::standard::create_process_module();

    if (!expect_true(
            result_ok(module_result),
            "process module creation should succeed"))
    {
      return false;
    }

    auto cwd_with_arg = module_result.value().call(
        "cwd",
        {kordex::bindings::Value::string("unexpected")});

    auto missing_arg = module_result.value().call("chdir", {});

    auto non_string_arg = module_result.value().call(
        "chdir",
        {kordex::bindings::Value::number(42.0)});

    auto empty_string_arg = module_result.value().call(
        "chdir",
        {kordex::bindings::Value::string("")});

    auto run_missing_arg = module_result.value().call("run", {});

    return expect_true(
               !cwd_with_arg,
               "cwd with argument should fail") &&
           expect_true(
               !missing_arg,
               "missing path argument should fail") &&
           expect_true(
               !non_string_arg,
               "non-string path argument should fail") &&
           expect_true(
               !empty_string_arg,
               "empty path argument should fail") &&
           expect_true(
               !run_missing_arg,
               "missing command argument should fail") &&
           expect_true(
               cwd_with_arg.error().code() ==
                   kordex::bindings::ErrorCode::InvalidArgument,
               "cwd with argument should map to invalid argument") &&
           expect_true(
               missing_arg.error().code() ==
                   kordex::bindings::ErrorCode::InvalidArgument,
               "missing path should map to invalid argument") &&
           expect_true(
               non_string_arg.error().code() ==
                   kordex::bindings::ErrorCode::InvalidArgument,
               "non-string path should map to invalid argument") &&
           expect_true(
               empty_string_arg.error().code() ==
                   kordex::bindings::ErrorCode::InvalidArgument,
               "empty path should map to invalid argument") &&
           expect_true(
               run_missing_arg.error().code() ==
                   kordex::bindings::ErrorCode::InvalidArgument,
               "missing command should map to invalid argument");
  }

  [[nodiscard]] bool test_process_disabled()
  {
    auto options = kordex::standard::StdOptions::defaults();
    options.enable_process = false;

    auto result = kordex::standard::Process::create(options);

    return expect_true(
               !result,
               "disabled process should fail creation") &&
           expect_true(
               result.error().code() ==
                   vix::error::ErrorCode::PermissionDenied,
               "disabled process should map to permission denied");
  }

  [[nodiscard]] bool test_process_native_functions_disabled()
  {
    auto options = kordex::standard::StdOptions::defaults();
    options.allow_native_functions = false;

    auto result = kordex::standard::Process::create(options);

    return expect_true(
               !result,
               "process without native functions should fail") &&
           expect_true(
               result.error().code() ==
                   vix::error::ErrorCode::ConfigError,
               "disabled native functions should map to config error");
  }

  [[nodiscard]] bool test_process_disabled_in_production()
  {
    auto result = kordex::standard::Process::create(
        kordex::standard::StdOptions::production());

    return expect_true(
               !result,
               "process should be disabled in production options") &&
           expect_true(
               result.error().code() ==
                   vix::error::ErrorCode::PermissionDenied,
               "production process disabled should map to permission denied");
  }
} // namespace

int main()
{
  const bool ok =
      test_create_process() &&
      test_create_process_module() &&
      test_process_module_values() &&
      test_cwd() &&
      test_chdir() &&
      test_run() &&
      test_invalid_arguments() &&
      test_process_disabled() &&
      test_process_native_functions_disabled() &&
      test_process_disabled_in_production();

  return ok ? 0 : 1;
}
