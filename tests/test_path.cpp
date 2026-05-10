/**
 *
 *  @file test_path.cpp
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

#include <kordex/std/Path.hpp>

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

  [[nodiscard]] bool test_create_path()
  {
    auto result = kordex::standard::Path::create(
        kordex::standard::StdOptions::defaults());

    if (!expect_true(
            result_ok(result),
            "path creation should succeed"))
    {
      return false;
    }

    const auto &path = result.value();

    return expect_true(
               path.enabled(),
               "path should be enabled") &&
           expect_true(
               path.config().module_enabled("path"),
               "config should enable path") &&
           expect_true(
               !path.validate(),
               "path validation should succeed");
  }

  [[nodiscard]] bool test_create_path_module()
  {
    auto result = kordex::standard::create_path_module();

    if (!expect_true(
            result_ok(result),
            "path module creation should succeed"))
    {
      return false;
    }

    const auto &module = result.value();

    return expect_true(
               module.valid(),
               "path module should be valid") &&
           expect_true(
               module.importable(),
               "path module should be importable") &&
           expect_true(
               module.safe(),
               "path module should be safe") &&
           expect_true(
               ::std::string_view(module.name()) == "path",
               "path module name should match") &&
           expect_true(
               module.has_function("normalize"),
               "path module should expose normalize") &&
           expect_true(
               module.has_function("join"),
               "path module should expose join") &&
           expect_true(
               module.has_function("dirname"),
               "path module should expose dirname") &&
           expect_true(
               module.has_function("basename"),
               "path module should expose basename") &&
           expect_true(
               module.has_function("extension"),
               "path module should expose extension") &&
           expect_true(
               module.has_function("isAbsolute"),
               "path module should expose isAbsolute") &&
           expect_true(
               module.has_function("isRelative"),
               "path module should expose isRelative") &&
           expect_true(
               module.function_count() == 7,
               "path module should expose seven functions") &&
           expect_true(
               module.has_value("name"),
               "path module should expose name value") &&
           expect_true(
               module.has_value("namespace"),
               "path module should expose namespace value") &&
           expect_true(
               module.has_value("separator"),
               "path module should expose separator value");
  }

  [[nodiscard]] bool test_path_module_values()
  {
    auto result = kordex::standard::create_path_module();

    if (!expect_true(
            result_ok(result),
            "path module creation should succeed"))
    {
      return false;
    }

    auto name = result.value().value("name");
    auto namespace_name = result.value().value("namespace");
    auto separator = result.value().value("separator");

    if (!expect_true(
            binding_result_ok(name) &&
                binding_result_ok(namespace_name) &&
                binding_result_ok(separator),
            "path module values should exist"))
    {
      return false;
    }

    auto name_text = name.value().as_string();
    auto namespace_text = namespace_name.value().as_string();
    auto separator_text = separator.value().as_string();

    if (!expect_true(
            binding_result_ok(name_text) &&
                binding_result_ok(namespace_text) &&
                binding_result_ok(separator_text),
            "path module values should convert to string"))
    {
      return false;
    }

    return expect_true(
               name_text.value() == "path",
               "path name value should match") &&
           expect_true(
               namespace_text.value() == "kordex",
               "path namespace value should match") &&
           expect_true(
               !separator_text.value().empty(),
               "path separator should not be empty");
  }

  [[nodiscard]] bool test_normalize()
  {
    auto module_result = kordex::standard::create_path_module();

    if (!expect_true(
            result_ok(module_result),
            "path module creation should succeed"))
    {
      return false;
    }

    kordex::bindings::NativeFunctionArguments args{
        kordex::bindings::Value::string("app/./src/../main.js")};

    auto result = module_result.value().call("normalize", args);

    if (!expect_true(
            binding_result_ok(result),
            "path.normalize should succeed"))
    {
      return false;
    }

    auto text = result.value().as_string();

    if (!expect_true(
            binding_result_ok(text),
            "normalized path should convert to string"))
    {
      return false;
    }

    return expect_true(
        text.value() == "app/main.js",
        "normalized path should match");
  }

  [[nodiscard]] bool test_join()
  {
    auto module_result = kordex::standard::create_path_module();

    if (!expect_true(
            result_ok(module_result),
            "path module creation should succeed"))
    {
      return false;
    }

    kordex::bindings::NativeFunctionArguments args{
        kordex::bindings::Value::string("app"),
        kordex::bindings::Value::string("src"),
        kordex::bindings::Value::string("main.js")};

    auto result = module_result.value().call("join", args);

    if (!expect_true(
            binding_result_ok(result),
            "path.join should succeed"))
    {
      return false;
    }

    auto text = result.value().as_string();

    if (!expect_true(
            binding_result_ok(text),
            "joined path should convert to string"))
    {
      return false;
    }

    return expect_true(
        text.value() == "app/src/main.js",
        "joined path should match");
  }

  [[nodiscard]] bool test_dirname_basename_extension()
  {
    auto module_result = kordex::standard::create_path_module();

    if (!expect_true(
            result_ok(module_result),
            "path module creation should succeed"))
    {
      return false;
    }

    kordex::bindings::NativeFunctionArguments args{
        kordex::bindings::Value::string("app/src/main.test.js")};

    auto dirname_result = module_result.value().call("dirname", args);
    auto basename_result = module_result.value().call("basename", args);
    auto extension_result = module_result.value().call("extension", args);

    if (!expect_true(
            binding_result_ok(dirname_result) &&
                binding_result_ok(basename_result) &&
                binding_result_ok(extension_result),
            "path dirname basename extension should succeed"))
    {
      return false;
    }

    auto dirname = dirname_result.value().as_string();
    auto basename = basename_result.value().as_string();
    auto extension = extension_result.value().as_string();

    if (!expect_true(
            binding_result_ok(dirname) &&
                binding_result_ok(basename) &&
                binding_result_ok(extension),
            "path results should convert to string"))
    {
      return false;
    }

    return expect_true(
               dirname.value() == "app/src",
               "dirname should match") &&
           expect_true(
               basename.value() == "main.test.js",
               "basename should match") &&
           expect_true(
               extension.value() == ".js",
               "extension should match");
  }

  [[nodiscard]] bool test_absolute_and_relative()
  {
    auto module_result = kordex::standard::create_path_module();

    if (!expect_true(
            result_ok(module_result),
            "path module creation should succeed"))
    {
      return false;
    }

    const auto absolute_path = ::std::filesystem::temp_directory_path();

    kordex::bindings::NativeFunctionArguments absolute_args{
        kordex::bindings::Value::string(absolute_path.string())};

    kordex::bindings::NativeFunctionArguments relative_args{
        kordex::bindings::Value::string("app/main.js")};

    auto absolute_result = module_result.value().call("isAbsolute", absolute_args);
    auto relative_absolute_result = module_result.value().call("isAbsolute", relative_args);
    auto relative_result = module_result.value().call("isRelative", relative_args);

    if (!expect_true(
            binding_result_ok(absolute_result) &&
                binding_result_ok(relative_absolute_result) &&
                binding_result_ok(relative_result),
            "path absolute relative checks should succeed"))
    {
      return false;
    }

    auto is_absolute = absolute_result.value().as_boolean();
    auto relative_is_absolute = relative_absolute_result.value().as_boolean();
    auto is_relative = relative_result.value().as_boolean();

    if (!expect_true(
            binding_result_ok(is_absolute) &&
                binding_result_ok(relative_is_absolute) &&
                binding_result_ok(is_relative),
            "path boolean results should convert"))
    {
      return false;
    }

    return expect_true(
               is_absolute.value(),
               "temp path should be absolute") &&
           expect_true(
               !relative_is_absolute.value(),
               "relative path should not be absolute") &&
           expect_true(
               is_relative.value(),
               "relative path should be relative");
  }

  [[nodiscard]] bool test_invalid_arguments()
  {
    auto module_result = kordex::standard::create_path_module();

    if (!expect_true(
            result_ok(module_result),
            "path module creation should succeed"))
    {
      return false;
    }

    auto missing_arg = module_result.value().call("normalize", {});
    auto non_string_arg = module_result.value().call(
        "normalize",
        {kordex::bindings::Value::number(42.0)});

    auto empty_string_arg = module_result.value().call(
        "normalize",
        {kordex::bindings::Value::string("")});

    auto join_missing_arg = module_result.value().call("join", {});

    return expect_true(
               !missing_arg,
               "missing path argument should fail") &&
           expect_true(
               !non_string_arg,
               "non-string path argument should fail") &&
           expect_true(
               !empty_string_arg,
               "empty path argument should fail") &&
           expect_true(
               !join_missing_arg,
               "join without segments should fail") &&
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
               join_missing_arg.error().code() ==
                   kordex::bindings::ErrorCode::InvalidArgument,
               "missing join segment should map to invalid argument");
  }

  [[nodiscard]] bool test_path_disabled()
  {
    auto options = kordex::standard::StdOptions::defaults();
    options.enable_path = false;

    auto result = kordex::standard::Path::create(options);

    return expect_true(
               !result,
               "disabled path should fail creation") &&
           expect_true(
               result.error().code() ==
                   vix::error::ErrorCode::PermissionDenied,
               "disabled path should map to permission denied");
  }

  [[nodiscard]] bool test_path_native_functions_disabled()
  {
    auto options = kordex::standard::StdOptions::defaults();
    options.allow_native_functions = false;

    auto result = kordex::standard::Path::create(options);

    return expect_true(
               !result,
               "path without native functions should fail") &&
           expect_true(
               result.error().code() ==
                   vix::error::ErrorCode::ConfigError,
               "disabled native functions should map to config error");
  }
} // namespace

int main()
{
  const bool ok =
      test_create_path() &&
      test_create_path_module() &&
      test_path_module_values() &&
      test_normalize() &&
      test_join() &&
      test_dirname_basename_extension() &&
      test_absolute_and_relative() &&
      test_invalid_arguments() &&
      test_path_disabled() &&
      test_path_native_functions_disabled();

  return ok ? 0 : 1;
}
