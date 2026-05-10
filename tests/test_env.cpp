/**
 *
 *  @file test_env.cpp
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

#include <cstdlib>
#include <string_view>

#include <kordex/std/Env.hpp>

namespace
{
  inline constexpr const char *TEST_ENV_KEY = "KORDEX_STD_ENV_TEST_VALUE";

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

  void cleanup_env()
  {
#if defined(_WIN32)
    (void)_putenv_s(TEST_ENV_KEY, "");
#else
    (void)::unsetenv(TEST_ENV_KEY);
#endif
  }

  [[nodiscard]] bool test_create_env()
  {
    auto result = kordex::standard::Env::create(
        kordex::standard::StdOptions::defaults());

    if (!expect_true(
            result_ok(result),
            "env creation should succeed"))
    {
      return false;
    }

    const auto &env = result.value();

    return expect_true(
               env.enabled(),
               "env should be enabled") &&
           expect_true(
               env.config().module_enabled("env"),
               "config should enable env") &&
           expect_true(
               !env.validate(),
               "env validation should succeed");
  }

  [[nodiscard]] bool test_create_env_module()
  {
    auto result = kordex::standard::create_env_module();

    if (!expect_true(
            result_ok(result),
            "env module creation should succeed"))
    {
      return false;
    }

    const auto &module = result.value();

    return expect_true(
               module.valid(),
               "env module should be valid") &&
           expect_true(
               module.importable(),
               "env module should be importable") &&
           expect_true(
               module.safe(),
               "env module should be safe") &&
           expect_true(
               ::std::string_view(module.name()) == "env",
               "env module name should match") &&
           expect_true(
               module.has_function("get"),
               "env module should expose get") &&
           expect_true(
               module.has_function("has"),
               "env module should expose has") &&
           expect_true(
               module.has_function("set"),
               "env module should expose set") &&
           expect_true(
               module.has_function("unset"),
               "env module should expose unset") &&
           expect_true(
               module.function_count() == 4,
               "env module should expose four functions") &&
           expect_true(
               module.has_value("name"),
               "env module should expose name value") &&
           expect_true(
               module.has_value("namespace"),
               "env module should expose namespace value");
  }

  [[nodiscard]] bool test_env_module_values()
  {
    auto result = kordex::standard::create_env_module();

    if (!expect_true(
            result_ok(result),
            "env module creation should succeed"))
    {
      return false;
    }

    auto name = result.value().value("name");
    auto namespace_name = result.value().value("namespace");

    if (!expect_true(
            binding_result_ok(name) &&
                binding_result_ok(namespace_name),
            "env module values should exist"))
    {
      return false;
    }

    auto name_text = name.value().as_string();
    auto namespace_text = namespace_name.value().as_string();

    if (!expect_true(
            binding_result_ok(name_text) &&
                binding_result_ok(namespace_text),
            "env module values should convert to string"))
    {
      return false;
    }

    return expect_true(
               name_text.value() == "env",
               "env name value should match") &&
           expect_true(
               namespace_text.value() == "kordex",
               "env namespace value should match");
  }

  [[nodiscard]] bool test_set_get_has_unset()
  {
    cleanup_env();

    auto module_result = kordex::standard::create_env_module();

    if (!expect_true(
            result_ok(module_result),
            "env module creation should succeed"))
    {
      return false;
    }

    kordex::bindings::NativeFunctionArguments set_args{
        kordex::bindings::Value::string(TEST_ENV_KEY),
        kordex::bindings::Value::string("hello-kordex")};

    auto set_result = module_result.value().call("set", set_args);

    if (!expect_true(
            binding_result_ok(set_result),
            "env.set should succeed"))
    {
      cleanup_env();
      return false;
    }

    kordex::bindings::NativeFunctionArguments key_args{
        kordex::bindings::Value::string(TEST_ENV_KEY)};

    auto has_result = module_result.value().call("has", key_args);
    auto get_result = module_result.value().call("get", key_args);

    if (!expect_true(
            binding_result_ok(has_result) &&
                binding_result_ok(get_result),
            "env.has and env.get should succeed"))
    {
      cleanup_env();
      return false;
    }

    auto has_value = has_result.value().as_boolean();
    auto get_value = get_result.value().as_string();

    if (!expect_true(
            binding_result_ok(has_value) &&
                binding_result_ok(get_value),
            "env values should convert"))
    {
      cleanup_env();
      return false;
    }

    auto unset_result = module_result.value().call("unset", key_args);
    auto has_after_unset_result = module_result.value().call("has", key_args);
    auto get_after_unset_result = module_result.value().call("get", key_args);

    if (!expect_true(
            binding_result_ok(unset_result) &&
                binding_result_ok(has_after_unset_result) &&
                binding_result_ok(get_after_unset_result),
            "env unset and post-unset reads should succeed"))
    {
      cleanup_env();
      return false;
    }

    auto has_after_unset = has_after_unset_result.value().as_boolean();

    if (!expect_true(
            binding_result_ok(has_after_unset),
            "env post-unset has should convert"))
    {
      cleanup_env();
      return false;
    }

    const bool ok =
        set_result.value().is_undefined() &&
        has_value.value() &&
        get_value.value() == "hello-kordex" &&
        unset_result.value().is_undefined() &&
        !has_after_unset.value() &&
        get_after_unset_result.value().is_null();

    cleanup_env();

    return expect_true(
        ok,
        "env set get has unset should behave correctly");
  }

  [[nodiscard]] bool test_get_missing_returns_null()
  {
    cleanup_env();

    auto module_result = kordex::standard::create_env_module();

    if (!expect_true(
            result_ok(module_result),
            "env module creation should succeed"))
    {
      return false;
    }

    kordex::bindings::NativeFunctionArguments args{
        kordex::bindings::Value::string(TEST_ENV_KEY)};

    auto result = module_result.value().call("get", args);

    if (!expect_true(
            binding_result_ok(result),
            "env.get missing key should succeed"))
    {
      return false;
    }

    return expect_true(
        result.value().is_null(),
        "env.get missing key should return null");
  }

  [[nodiscard]] bool test_has_missing_returns_false()
  {
    cleanup_env();

    auto module_result = kordex::standard::create_env_module();

    if (!expect_true(
            result_ok(module_result),
            "env module creation should succeed"))
    {
      return false;
    }

    kordex::bindings::NativeFunctionArguments args{
        kordex::bindings::Value::string(TEST_ENV_KEY)};

    auto result = module_result.value().call("has", args);

    if (!expect_true(
            binding_result_ok(result),
            "env.has missing key should succeed"))
    {
      return false;
    }

    auto value = result.value().as_boolean();

    if (!expect_true(
            binding_result_ok(value),
            "env.has result should convert to boolean"))
    {
      return false;
    }

    return expect_true(
        !value.value(),
        "env.has missing key should return false");
  }

  [[nodiscard]] bool test_invalid_arguments()
  {
    auto module_result = kordex::standard::create_env_module();

    if (!expect_true(
            result_ok(module_result),
            "env module creation should succeed"))
    {
      return false;
    }

    auto missing_arg = module_result.value().call("get", {});
    auto non_string_arg = module_result.value().call(
        "get",
        {kordex::bindings::Value::number(42.0)});

    auto empty_string_arg = module_result.value().call(
        "get",
        {kordex::bindings::Value::string("")});

    auto set_missing_value = module_result.value().call(
        "set",
        {kordex::bindings::Value::string(TEST_ENV_KEY)});

    return expect_true(
               !missing_arg,
               "missing env name should fail") &&
           expect_true(
               !non_string_arg,
               "non-string env name should fail") &&
           expect_true(
               !empty_string_arg,
               "empty env name should fail") &&
           expect_true(
               !set_missing_value,
               "missing env value should fail") &&
           expect_true(
               missing_arg.error().code() ==
                   kordex::bindings::ErrorCode::InvalidArgument,
               "missing env name should map to invalid argument") &&
           expect_true(
               non_string_arg.error().code() ==
                   kordex::bindings::ErrorCode::InvalidArgument,
               "non-string env name should map to invalid argument") &&
           expect_true(
               empty_string_arg.error().code() ==
                   kordex::bindings::ErrorCode::InvalidArgument,
               "empty env name should map to invalid argument") &&
           expect_true(
               set_missing_value.error().code() ==
                   kordex::bindings::ErrorCode::InvalidArgument,
               "missing env value should map to invalid argument");
  }

  [[nodiscard]] bool test_env_disabled()
  {
    auto options = kordex::standard::StdOptions::defaults();
    options.enable_env = false;

    auto result = kordex::standard::Env::create(options);

    return expect_true(
               !result,
               "disabled env should fail creation") &&
           expect_true(
               result.error().code() ==
                   vix::error::ErrorCode::PermissionDenied,
               "disabled env should map to permission denied");
  }

  [[nodiscard]] bool test_env_native_functions_disabled()
  {
    auto options = kordex::standard::StdOptions::defaults();
    options.allow_native_functions = false;

    auto result = kordex::standard::Env::create(options);

    return expect_true(
               !result,
               "env without native functions should fail") &&
           expect_true(
               result.error().code() ==
                   vix::error::ErrorCode::ConfigError,
               "disabled native functions should map to config error");
  }
} // namespace

int main()
{
  const bool ok =
      test_create_env() &&
      test_create_env_module() &&
      test_env_module_values() &&
      test_set_get_has_unset() &&
      test_get_missing_returns_null() &&
      test_has_missing_returns_false() &&
      test_invalid_arguments() &&
      test_env_disabled() &&
      test_env_native_functions_disabled();

  cleanup_env();

  return ok ? 0 : 1;
}
