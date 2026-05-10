/**
 *
 *  @file test_fs.cpp
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
#include <fstream>
#include <string_view>

#include <kordex/std/Fs.hpp>

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

  [[nodiscard]] ::std::filesystem::path temp_file_path()
  {
    return ::std::filesystem::temp_directory_path() /
           "kordex_std_fs_test.txt";
  }

  [[nodiscard]] bool test_create_fs()
  {
    auto result = kordex::standard::Fs::create(
        kordex::standard::StdOptions::defaults());

    if (!expect_true(
            result_ok(result),
            "fs creation should succeed"))
    {
      return false;
    }

    const auto &fs = result.value();

    return expect_true(
               fs.enabled(),
               "fs should be enabled") &&
           expect_true(
               fs.config().module_enabled("fs"),
               "config should enable fs") &&
           expect_true(
               !fs.validate(),
               "fs validation should succeed");
  }

  [[nodiscard]] bool test_create_fs_module()
  {
    auto result = kordex::standard::create_fs_module();

    if (!expect_true(
            result_ok(result),
            "fs module creation should succeed"))
    {
      return false;
    }

    const auto &module = result.value();

    return expect_true(
               module.valid(),
               "fs module should be valid") &&
           expect_true(
               module.importable(),
               "fs module should be importable") &&
           expect_true(
               module.safe(),
               "fs module should be safe") &&
           expect_true(
               ::std::string_view(module.name()) == "fs",
               "fs module name should match") &&
           expect_true(
               module.has_function("exists"),
               "fs module should expose exists") &&
           expect_true(
               module.has_function("isFile"),
               "fs module should expose isFile") &&
           expect_true(
               module.has_function("isDirectory"),
               "fs module should expose isDirectory") &&
           expect_true(
               module.has_function("readText"),
               "fs module should expose readText") &&
           expect_true(
               module.has_function("writeText"),
               "fs module should expose writeText") &&
           expect_true(
               module.has_function("remove"),
               "fs module should expose remove") &&
           expect_true(
               module.function_count() == 6,
               "fs module should expose six functions") &&
           expect_true(
               module.has_value("name"),
               "fs module should expose name value") &&
           expect_true(
               module.has_value("namespace"),
               "fs module should expose namespace value");
  }

  [[nodiscard]] bool test_fs_module_values()
  {
    auto result = kordex::standard::create_fs_module();

    if (!expect_true(
            result_ok(result),
            "fs module creation should succeed"))
    {
      return false;
    }

    auto name = result.value().value("name");
    auto namespace_name = result.value().value("namespace");

    if (!expect_true(
            binding_result_ok(name) &&
                binding_result_ok(namespace_name),
            "fs module values should exist"))
    {
      return false;
    }

    auto name_text = name.value().as_string();
    auto namespace_text = namespace_name.value().as_string();

    if (!expect_true(
            binding_result_ok(name_text) &&
                binding_result_ok(namespace_text),
            "fs module values should convert to string"))
    {
      return false;
    }

    return expect_true(
               name_text.value() == "fs",
               "fs name value should match") &&
           expect_true(
               namespace_text.value() == "kordex",
               "fs namespace value should match");
  }

  [[nodiscard]] bool test_write_and_read_text()
  {
    auto module_result = kordex::standard::create_fs_module();

    if (!expect_true(
            result_ok(module_result),
            "fs module creation should succeed"))
    {
      return false;
    }

    const auto path = temp_file_path();
    (void)::std::filesystem::remove(path);

    kordex::bindings::NativeFunctionArguments write_args{
        kordex::bindings::Value::string(path.string()),
        kordex::bindings::Value::string("hello kordex")};

    auto write_result = module_result.value().call("writeText", write_args);

    if (!expect_true(
            binding_result_ok(write_result),
            "fs.writeText should succeed"))
    {
      return false;
    }

    kordex::bindings::NativeFunctionArguments read_args{
        kordex::bindings::Value::string(path.string())};

    auto read_result = module_result.value().call("readText", read_args);

    if (!expect_true(
            binding_result_ok(read_result),
            "fs.readText should succeed"))
    {
      return false;
    }

    auto text = read_result.value().as_string();

    if (!expect_true(
            binding_result_ok(text),
            "read text should convert to string"))
    {
      return false;
    }

    const bool ok =
        write_result.value().is_undefined() &&
        text.value() == "hello kordex";

    (void)::std::filesystem::remove(path);

    return expect_true(
        ok,
        "written and read text should match");
  }

  [[nodiscard]] bool test_exists_and_is_file()
  {
    auto module_result = kordex::standard::create_fs_module();

    if (!expect_true(
            result_ok(module_result),
            "fs module creation should succeed"))
    {
      return false;
    }

    const auto path = temp_file_path();

    {
      ::std::ofstream file(path);
      file << "content";
    }

    kordex::bindings::NativeFunctionArguments args{
        kordex::bindings::Value::string(path.string())};

    auto exists_result = module_result.value().call("exists", args);
    auto is_file_result = module_result.value().call("isFile", args);
    auto is_directory_result = module_result.value().call("isDirectory", args);

    if (!expect_true(
            binding_result_ok(exists_result) &&
                binding_result_ok(is_file_result) &&
                binding_result_ok(is_directory_result),
            "fs stat calls should succeed"))
    {
      (void)::std::filesystem::remove(path);
      return false;
    }

    auto exists = exists_result.value().as_boolean();
    auto is_file = is_file_result.value().as_boolean();
    auto is_directory = is_directory_result.value().as_boolean();

    if (!expect_true(
            binding_result_ok(exists) &&
                binding_result_ok(is_file) &&
                binding_result_ok(is_directory),
            "fs stat values should convert to boolean"))
    {
      (void)::std::filesystem::remove(path);
      return false;
    }

    const bool ok =
        exists.value() &&
        is_file.value() &&
        !is_directory.value();

    (void)::std::filesystem::remove(path);

    return expect_true(
        ok,
        "existing file should be detected as file");
  }

  [[nodiscard]] bool test_is_directory()
  {
    auto module_result = kordex::standard::create_fs_module();

    if (!expect_true(
            result_ok(module_result),
            "fs module creation should succeed"))
    {
      return false;
    }

    const auto path = ::std::filesystem::temp_directory_path();

    kordex::bindings::NativeFunctionArguments args{
        kordex::bindings::Value::string(path.string())};

    auto exists_result = module_result.value().call("exists", args);
    auto is_directory_result = module_result.value().call("isDirectory", args);

    if (!expect_true(
            binding_result_ok(exists_result) &&
                binding_result_ok(is_directory_result),
            "fs directory calls should succeed"))
    {
      return false;
    }

    auto exists = exists_result.value().as_boolean();
    auto is_directory = is_directory_result.value().as_boolean();

    if (!expect_true(
            binding_result_ok(exists) &&
                binding_result_ok(is_directory),
            "fs directory values should convert to boolean"))
    {
      return false;
    }

    return expect_true(
        exists.value() && is_directory.value(),
        "temp directory should exist and be directory");
  }

  [[nodiscard]] bool test_remove_file()
  {
    auto module_result = kordex::standard::create_fs_module();

    if (!expect_true(
            result_ok(module_result),
            "fs module creation should succeed"))
    {
      return false;
    }

    const auto path = temp_file_path();

    {
      ::std::ofstream file(path);
      file << "remove me";
    }

    kordex::bindings::NativeFunctionArguments args{
        kordex::bindings::Value::string(path.string())};

    auto remove_result = module_result.value().call("remove", args);

    if (!expect_true(
            binding_result_ok(remove_result),
            "fs.remove should succeed"))
    {
      return false;
    }

    auto removed = remove_result.value().as_boolean();

    if (!expect_true(
            binding_result_ok(removed),
            "remove result should convert to boolean"))
    {
      return false;
    }

    return expect_true(
               removed.value(),
               "remove should return true for removed file") &&
           expect_true(
               !::std::filesystem::exists(path),
               "file should no longer exist");
  }

  [[nodiscard]] bool test_missing_file()
  {
    auto module_result = kordex::standard::create_fs_module();

    if (!expect_true(
            result_ok(module_result),
            "fs module creation should succeed"))
    {
      return false;
    }

    const auto path = temp_file_path();
    (void)::std::filesystem::remove(path);

    kordex::bindings::NativeFunctionArguments args{
        kordex::bindings::Value::string(path.string())};

    auto exists_result = module_result.value().call("exists", args);
    auto read_result = module_result.value().call("readText", args);

    if (!expect_true(
            binding_result_ok(exists_result),
            "exists on missing file should succeed"))
    {
      return false;
    }

    auto exists = exists_result.value().as_boolean();

    if (!expect_true(
            binding_result_ok(exists),
            "exists result should convert to boolean"))
    {
      return false;
    }

    return expect_true(
               !exists.value(),
               "missing file should not exist") &&
           expect_true(
               !read_result,
               "readText on missing file should fail");
  }

  [[nodiscard]] bool test_invalid_arguments()
  {
    auto module_result = kordex::standard::create_fs_module();

    if (!expect_true(
            result_ok(module_result),
            "fs module creation should succeed"))
    {
      return false;
    }

    auto missing_arg = module_result.value().call("exists", {});
    auto non_string_arg = module_result.value().call(
        "exists",
        {kordex::bindings::Value::number(42.0)});

    auto empty_string_arg = module_result.value().call(
        "exists",
        {kordex::bindings::Value::string("")});

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
               "empty path should map to invalid argument");
  }

  [[nodiscard]] bool test_fs_disabled()
  {
    auto options = kordex::standard::StdOptions::defaults();
    options.enable_fs = false;

    auto result = kordex::standard::Fs::create(options);

    return expect_true(
               !result,
               "disabled fs should fail creation") &&
           expect_true(
               result.error().code() ==
                   vix::error::ErrorCode::PermissionDenied,
               "disabled fs should map to permission denied");
  }

  [[nodiscard]] bool test_fs_native_functions_disabled()
  {
    auto options = kordex::standard::StdOptions::defaults();
    options.allow_native_functions = false;

    auto result = kordex::standard::Fs::create(options);

    return expect_true(
               !result,
               "fs without native functions should fail") &&
           expect_true(
               result.error().code() ==
                   vix::error::ErrorCode::ConfigError,
               "disabled native functions should map to config error");
  }
} // namespace

int main()
{
  const bool ok =
      test_create_fs() &&
      test_create_fs_module() &&
      test_fs_module_values() &&
      test_write_and_read_text() &&
      test_exists_and_is_file() &&
      test_is_directory() &&
      test_remove_file() &&
      test_missing_file() &&
      test_invalid_arguments() &&
      test_fs_disabled() &&
      test_fs_native_functions_disabled();

  return ok ? 0 : 1;
}
