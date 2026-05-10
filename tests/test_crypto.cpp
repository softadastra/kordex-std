/**
 *
 *  @file test_crypto.cpp
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
#include <kordex/std/Crypto.hpp>

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

  [[nodiscard]] bool test_create_crypto()
  {
    auto result = kordex::standard::Crypto::create(
        kordex::standard::StdOptions::defaults());

    if (!expect_true(
            result_ok(result),
            "crypto creation should succeed"))
    {
      return false;
    }

    const auto &crypto = result.value();

    return expect_true(
               crypto.enabled(),
               "crypto should be enabled") &&
           expect_true(
               crypto.config().module_enabled("crypto"),
               "config should enable crypto") &&
           expect_true(
               !crypto.validate(),
               "crypto validation should succeed");
  }

  [[nodiscard]] bool test_create_crypto_module()
  {
    auto result = kordex::standard::create_crypto_module();

    if (!expect_true(
            result_ok(result),
            "crypto module creation should succeed"))
    {
      return false;
    }

    const auto &module = result.value();

    return expect_true(
               module.valid(),
               "crypto module should be valid") &&
           expect_true(
               module.importable(),
               "crypto module should be importable") &&
           expect_true(
               module.safe(),
               "crypto module should be safe") &&
           expect_true(
               ::std::string_view(module.name()) == "crypto",
               "crypto module name should match") &&
           expect_true(
               module.has_function("hash"),
               "crypto module should expose hash") &&
           expect_true(
               module.has_function("random"),
               "crypto module should expose random") &&
           expect_true(
               module.has_function("randomInt"),
               "crypto module should expose randomInt") &&
           expect_true(
               module.has_function("equals"),
               "crypto module should expose equals") &&
           expect_true(
               module.function_count() == 4,
               "crypto module should expose four functions") &&
           expect_true(
               module.has_value("name"),
               "crypto module should expose name value") &&
           expect_true(
               module.has_value("namespace"),
               "crypto module should expose namespace value");
  }

  [[nodiscard]] bool test_crypto_module_values()
  {
    auto result = kordex::standard::create_crypto_module();

    if (!expect_true(
            result_ok(result),
            "crypto module creation should succeed"))
    {
      return false;
    }

    auto name = result.value().value("name");
    auto namespace_name = result.value().value("namespace");

    if (!expect_true(
            binding_result_ok(name) &&
                binding_result_ok(namespace_name),
            "crypto module values should exist"))
    {
      return false;
    }

    auto name_text = name.value().as_string();
    auto namespace_text = namespace_name.value().as_string();

    if (!expect_true(
            binding_result_ok(name_text) &&
                binding_result_ok(namespace_text),
            "crypto module values should convert to string"))
    {
      return false;
    }

    return expect_true(
               name_text.value() == "crypto",
               "crypto name value should match") &&
           expect_true(
               namespace_text.value() == "kordex",
               "crypto namespace value should match");
  }

  [[nodiscard]] bool test_fnv1a_hex()
  {
    const auto empty = kordex::standard::Crypto::fnv1a_hex("");
    const auto hello = kordex::standard::Crypto::fnv1a_hex("hello");
    const auto hello_again = kordex::standard::Crypto::fnv1a_hex("hello");
    const auto world = kordex::standard::Crypto::fnv1a_hex("world");

    return expect_true(
               empty == "cbf29ce484222325",
               "empty fnv1a hash should match") &&
           expect_true(
               hello == hello_again,
               "same input should produce same hash") &&
           expect_true(
               hello != world,
               "different inputs should produce different hashes") &&
           expect_true(
               hello.size() == 16,
               "hash should be sixteen hex characters");
  }

  [[nodiscard]] bool test_hash_function()
  {
    auto module_result = kordex::standard::create_crypto_module();

    if (!expect_true(
            result_ok(module_result),
            "crypto module creation should succeed"))
    {
      return false;
    }

    kordex::bindings::NativeFunctionArguments args{
        kordex::bindings::Value::string("hello")};

    auto result = module_result.value().call("hash", args);

    if (!expect_true(
            binding_result_ok(result),
            "crypto.hash should succeed"))
    {
      return false;
    }

    auto text = result.value().as_string();

    if (!expect_true(
            binding_result_ok(text),
            "crypto.hash result should convert to string"))
    {
      return false;
    }

    return expect_true(
               text.value() ==
                   kordex::standard::Crypto::fnv1a_hex("hello"),
               "crypto.hash should return deterministic hash") &&
           expect_true(
               text.value().size() == 16,
               "crypto.hash should return sixteen hex characters");
  }

  [[nodiscard]] bool test_random_function()
  {
    auto module_result = kordex::standard::create_crypto_module();

    if (!expect_true(
            result_ok(module_result),
            "crypto module creation should succeed"))
    {
      return false;
    }

    auto result = module_result.value().call("random", {});

    if (!expect_true(
            binding_result_ok(result),
            "crypto.random should succeed"))
    {
      return false;
    }

    auto value = result.value().as_number();

    if (!expect_true(
            binding_result_ok(value),
            "crypto.random result should convert to number"))
    {
      return false;
    }

    return expect_true(
        value.value() >= 0.0 && value.value() < 1.0,
        "crypto.random should return value in [0, 1)");
  }

  [[nodiscard]] bool test_random_int_function()
  {
    auto module_result = kordex::standard::create_crypto_module();

    if (!expect_true(
            result_ok(module_result),
            "crypto module creation should succeed"))
    {
      return false;
    }

    kordex::bindings::NativeFunctionArguments args{
        kordex::bindings::Value::number(1.0),
        kordex::bindings::Value::number(3.0)};

    auto result = module_result.value().call("randomInt", args);

    if (!expect_true(
            binding_result_ok(result),
            "crypto.randomInt should succeed"))
    {
      return false;
    }

    auto value = result.value().as_number();

    if (!expect_true(
            binding_result_ok(value),
            "crypto.randomInt result should convert to number"))
    {
      return false;
    }

    return expect_true(
        value.value() >= 1.0 && value.value() <= 3.0,
        "crypto.randomInt should return value inside range");
  }

  [[nodiscard]] bool test_equals_function()
  {
    auto module_result = kordex::standard::create_crypto_module();

    if (!expect_true(
            result_ok(module_result),
            "crypto module creation should succeed"))
    {
      return false;
    }

    auto same_result = module_result.value().call(
        "equals",
        {kordex::bindings::Value::string("hello"),
         kordex::bindings::Value::string("hello")});

    auto different_result = module_result.value().call(
        "equals",
        {kordex::bindings::Value::string("hello"),
         kordex::bindings::Value::string("world")});

    if (!expect_true(
            binding_result_ok(same_result) &&
                binding_result_ok(different_result),
            "crypto.equals calls should succeed"))
    {
      return false;
    }

    auto same = same_result.value().as_boolean();
    auto different = different_result.value().as_boolean();

    if (!expect_true(
            binding_result_ok(same) &&
                binding_result_ok(different),
            "crypto.equals values should convert to boolean"))
    {
      return false;
    }

    return expect_true(
               same.value(),
               "equal strings should return true") &&
           expect_true(
               !different.value(),
               "different strings should return false");
  }

  [[nodiscard]] bool test_invalid_arguments()
  {
    auto module_result = kordex::standard::create_crypto_module();

    if (!expect_true(
            result_ok(module_result),
            "crypto module creation should succeed"))
    {
      return false;
    }

    auto hash_missing_arg = module_result.value().call("hash", {});

    auto hash_non_string_arg = module_result.value().call(
        "hash",
        {kordex::bindings::Value::number(42.0)});

    auto random_with_arg = module_result.value().call(
        "random",
        {kordex::bindings::Value::number(1.0)});

    auto random_int_missing_arg = module_result.value().call(
        "randomInt",
        {kordex::bindings::Value::number(1.0)});

    auto random_int_invalid_range = module_result.value().call(
        "randomInt",
        {kordex::bindings::Value::number(10.0),
         kordex::bindings::Value::number(1.0)});

    auto equals_missing_arg = module_result.value().call(
        "equals",
        {kordex::bindings::Value::string("hello")});

    return expect_true(
               !hash_missing_arg,
               "hash missing input should fail") &&
           expect_true(
               !hash_non_string_arg,
               "hash non-string input should fail") &&
           expect_true(
               !random_with_arg,
               "random with argument should fail") &&
           expect_true(
               !random_int_missing_arg,
               "randomInt missing max should fail") &&
           expect_true(
               !random_int_invalid_range,
               "randomInt invalid range should fail") &&
           expect_true(
               !equals_missing_arg,
               "equals missing right argument should fail") &&
           expect_true(
               hash_missing_arg.error().code() ==
                   kordex::bindings::ErrorCode::InvalidArgument,
               "hash missing input should map to invalid argument") &&
           expect_true(
               hash_non_string_arg.error().code() ==
                   kordex::bindings::ErrorCode::InvalidArgument,
               "hash non-string input should map to invalid argument") &&
           expect_true(
               random_with_arg.error().code() ==
                   kordex::bindings::ErrorCode::InvalidArgument,
               "random with arg should map to invalid argument") &&
           expect_true(
               random_int_missing_arg.error().code() ==
                   kordex::bindings::ErrorCode::InvalidArgument,
               "randomInt missing max should map to invalid argument") &&
           expect_true(
               random_int_invalid_range.error().code() ==
                   kordex::bindings::ErrorCode::InvalidArgument,
               "randomInt invalid range should map to invalid argument") &&
           expect_true(
               equals_missing_arg.error().code() ==
                   kordex::bindings::ErrorCode::InvalidArgument,
               "equals missing argument should map to invalid argument");
  }

  [[nodiscard]] bool test_crypto_disabled()
  {
    auto options = kordex::standard::StdOptions::defaults();
    options.enable_crypto = false;

    auto result = kordex::standard::Crypto::create(options);

    return expect_true(
               !result,
               "disabled crypto should fail creation") &&
           expect_true(
               result.error().code() ==
                   vix::error::ErrorCode::PermissionDenied,
               "disabled crypto should map to permission denied");
  }

  [[nodiscard]] bool test_crypto_native_functions_disabled()
  {
    auto options = kordex::standard::StdOptions::defaults();
    options.allow_native_functions = false;

    auto result = kordex::standard::Crypto::create(options);

    return expect_true(
               !result,
               "crypto without native functions should fail") &&
           expect_true(
               result.error().code() ==
                   vix::error::ErrorCode::ConfigError,
               "disabled native functions should map to config error");
  }
} // namespace

int main()
{
  const bool ok =
      test_create_crypto() &&
      test_create_crypto_module() &&
      test_crypto_module_values() &&
      test_fnv1a_hex() &&
      test_hash_function() &&
      test_random_function() &&
      test_random_int_function() &&
      test_equals_function() &&
      test_invalid_arguments() &&
      test_crypto_disabled() &&
      test_crypto_native_functions_disabled();

  return ok ? 0 : 1;
}
