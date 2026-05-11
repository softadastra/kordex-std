/**
 *
 *  @file test_http.cpp
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

#include <string_view>
#include <kordex/std/Http.hpp>

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

  [[nodiscard]] bool test_create_http()
  {
    auto result = kordex::standard::Http::create(
        kordex::standard::StdOptions::defaults());

    if (!expect_true(
            result_ok(result),
            "http creation should succeed"))
    {
      return false;
    }

    const auto &http = result.value();

    return expect_true(
               http.enabled(),
               "http should be enabled") &&
           expect_true(
               http.config().module_enabled("http"),
               "config should enable http") &&
           expect_true(
               !http.validate(),
               "http validation should succeed");
  }

  [[nodiscard]] bool test_create_http_module()
  {
    auto result = kordex::standard::create_http_module();

    if (!expect_true(
            result_ok(result),
            "http module creation should succeed"))
    {
      return false;
    }

    const auto &module = result.value();

    return expect_true(
               module.valid(),
               "http module should be valid") &&
           expect_true(
               module.importable(),
               "http module should be importable") &&
           expect_true(
               module.safe(),
               "http module should be safe") &&
           expect_true(
               ::std::string_view(module.name()) == "http",
               "http module name should match") &&
           expect_true(
               module.has_function("isSuccess"),
               "http module should expose isSuccess") &&
           expect_true(
               module.has_function("isRedirect"),
               "http module should expose isRedirect") &&
           expect_true(
               module.has_function("isClientError"),
               "http module should expose isClientError") &&
           expect_true(
               module.has_function("isServerError"),
               "http module should expose isServerError") &&
           expect_true(
               module.has_function("statusText"),
               "http module should expose statusText") &&
           expect_true(
               module.has_function("buildUrl"),
               "http module should expose buildUrl") &&
           expect_true(
               module.has_function("normalizeMethod"),
               "http module should expose normalizeMethod") &&
           expect_true(
               module.has_function("isMethod"),
               "http module should expose isMethod") &&
           expect_true(
               module.function_count() == 8,
               "http module should expose eight functions") &&
           expect_true(
               module.has_value("name"),
               "http module should expose name value") &&
           expect_true(
               module.has_value("namespace"),
               "http module should expose namespace value") &&
           expect_true(
               module.has_value("GET"),
               "http module should expose GET value") &&
           expect_true(
               module.has_value("POST"),
               "http module should expose POST value");
  }

  [[nodiscard]] bool test_http_module_values()
  {
    auto result = kordex::standard::create_http_module();

    if (!expect_true(
            result_ok(result),
            "http module creation should succeed"))
    {
      return false;
    }

    auto name = result.value().value("name");
    auto namespace_name = result.value().value("namespace");
    auto get = result.value().value("GET");
    auto post = result.value().value("POST");

    if (!expect_true(
            binding_result_ok(name) &&
                binding_result_ok(namespace_name) &&
                binding_result_ok(get) &&
                binding_result_ok(post),
            "http module values should exist"))
    {
      return false;
    }

    auto name_text = name.value().as_string();
    auto namespace_text = namespace_name.value().as_string();
    auto get_text = get.value().as_string();
    auto post_text = post.value().as_string();

    if (!expect_true(
            binding_result_ok(name_text) &&
                binding_result_ok(namespace_text) &&
                binding_result_ok(get_text) &&
                binding_result_ok(post_text),
            "http module values should convert to string"))
    {
      return false;
    }

    return expect_true(
               name_text.value() == "http",
               "http name value should match") &&
           expect_true(
               namespace_text.value() == "kordex",
               "http namespace value should match") &&
           expect_true(
               get_text.value() == "GET",
               "http GET value should match") &&
           expect_true(
               post_text.value() == "POST",
               "http POST value should match");
  }

  [[nodiscard]] bool test_reason_phrase()
  {
    return expect_true(
               ::std::string_view(kordex::standard::Http::reason_phrase(200)) ==
                   "OK",
               "200 reason phrase should match") &&
           expect_true(
               ::std::string_view(kordex::standard::Http::reason_phrase(201)) ==
                   "Created",
               "201 reason phrase should match") &&
           expect_true(
               ::std::string_view(kordex::standard::Http::reason_phrase(404)) ==
                   "Not Found",
               "404 reason phrase should match") &&
           expect_true(
               ::std::string_view(kordex::standard::Http::reason_phrase(500)) ==
                   "Internal Server Error",
               "500 reason phrase should match") &&
           expect_true(
               ::std::string_view(kordex::standard::Http::reason_phrase(599)) ==
                   "Unknown",
               "unknown reason phrase should match");
  }

  [[nodiscard]] bool test_status_classes()
  {
    auto module_result = kordex::standard::create_http_module();

    if (!expect_true(
            result_ok(module_result),
            "http module creation should succeed"))
    {
      return false;
    }

    auto success_result = module_result.value().call(
        "isSuccess",
        {kordex::bindings::Value::number(200.0)});

    auto redirect_result = module_result.value().call(
        "isRedirect",
        {kordex::bindings::Value::number(302.0)});

    auto client_error_result = module_result.value().call(
        "isClientError",
        {kordex::bindings::Value::number(404.0)});

    auto server_error_result = module_result.value().call(
        "isServerError",
        {kordex::bindings::Value::number(500.0)});

    if (!expect_true(
            binding_result_ok(success_result) &&
                binding_result_ok(redirect_result) &&
                binding_result_ok(client_error_result) &&
                binding_result_ok(server_error_result),
            "http status class calls should succeed"))
    {
      return false;
    }

    auto success = success_result.value().as_boolean();
    auto redirect = redirect_result.value().as_boolean();
    auto client_error = client_error_result.value().as_boolean();
    auto server_error = server_error_result.value().as_boolean();

    if (!expect_true(
            binding_result_ok(success) &&
                binding_result_ok(redirect) &&
                binding_result_ok(client_error) &&
                binding_result_ok(server_error),
            "http status class results should convert to boolean"))
    {
      return false;
    }

    return expect_true(
               success.value(),
               "200 should be success") &&
           expect_true(
               redirect.value(),
               "302 should be redirect") &&
           expect_true(
               client_error.value(),
               "404 should be client error") &&
           expect_true(
               server_error.value(),
               "500 should be server error");
  }

  [[nodiscard]] bool test_status_negative_cases()
  {
    auto module_result = kordex::standard::create_http_module();

    if (!expect_true(
            result_ok(module_result),
            "http module creation should succeed"))
    {
      return false;
    }

    auto success_result = module_result.value().call(
        "isSuccess",
        {kordex::bindings::Value::number(404.0)});

    auto redirect_result = module_result.value().call(
        "isRedirect",
        {kordex::bindings::Value::number(200.0)});

    auto client_error_result = module_result.value().call(
        "isClientError",
        {kordex::bindings::Value::number(500.0)});

    auto server_error_result = module_result.value().call(
        "isServerError",
        {kordex::bindings::Value::number(404.0)});

    if (!expect_true(
            binding_result_ok(success_result) &&
                binding_result_ok(redirect_result) &&
                binding_result_ok(client_error_result) &&
                binding_result_ok(server_error_result),
            "http status negative calls should succeed"))
    {
      return false;
    }

    auto success = success_result.value().as_boolean();
    auto redirect = redirect_result.value().as_boolean();
    auto client_error = client_error_result.value().as_boolean();
    auto server_error = server_error_result.value().as_boolean();

    if (!expect_true(
            binding_result_ok(success) &&
                binding_result_ok(redirect) &&
                binding_result_ok(client_error) &&
                binding_result_ok(server_error),
            "http status negative results should convert to boolean"))
    {
      return false;
    }

    return expect_true(
               !success.value(),
               "404 should not be success") &&
           expect_true(
               !redirect.value(),
               "200 should not be redirect") &&
           expect_true(
               !client_error.value(),
               "500 should not be client error") &&
           expect_true(
               !server_error.value(),
               "404 should not be server error");
  }

  [[nodiscard]] bool test_status_text_function()
  {
    auto module_result = kordex::standard::create_http_module();

    if (!expect_true(
            result_ok(module_result),
            "http module creation should succeed"))
    {
      return false;
    }

    auto ok_result = module_result.value().call(
        "statusText",
        {kordex::bindings::Value::number(200.0)});

    auto missing_result = module_result.value().call(
        "statusText",
        {kordex::bindings::Value::number(404.0)});

    auto unknown_result = module_result.value().call(
        "statusText",
        {kordex::bindings::Value::number(599.0)});

    if (!expect_true(
            binding_result_ok(ok_result) &&
                binding_result_ok(missing_result) &&
                binding_result_ok(unknown_result),
            "http.statusText calls should succeed"))
    {
      return false;
    }

    auto ok = ok_result.value().as_string();
    auto missing = missing_result.value().as_string();
    auto unknown = unknown_result.value().as_string();

    if (!expect_true(
            binding_result_ok(ok) &&
                binding_result_ok(missing) &&
                binding_result_ok(unknown),
            "http.statusText results should convert to string"))
    {
      return false;
    }

    return expect_true(
               ok.value() == "OK",
               "statusText 200 should match") &&
           expect_true(
               missing.value() == "Not Found",
               "statusText 404 should match") &&
           expect_true(
               unknown.value() == "Unknown",
               "statusText 599 should match");
  }

  [[nodiscard]] bool test_build_url()
  {
    auto module_result = kordex::standard::create_http_module();

    if (!expect_true(
            result_ok(module_result),
            "http module creation should succeed"))
    {
      return false;
    }

    auto first_result = module_result.value().call(
        "buildUrl",
        {kordex::bindings::Value::string("https://api.kordex.dev"),
         kordex::bindings::Value::string("v1/users")});

    auto second_result = module_result.value().call(
        "buildUrl",
        {kordex::bindings::Value::string("https://api.kordex.dev/"),
         kordex::bindings::Value::string("/v1/users")});

    if (!expect_true(
            binding_result_ok(first_result) &&
                binding_result_ok(second_result),
            "http.buildUrl calls should succeed"))
    {
      return false;
    }

    auto first = first_result.value().as_string();
    auto second = second_result.value().as_string();

    if (!expect_true(
            binding_result_ok(first) &&
                binding_result_ok(second),
            "http.buildUrl results should convert to string"))
    {
      return false;
    }

    return expect_true(
               first.value() == "https://api.kordex.dev/v1/users",
               "buildUrl should join clean URL") &&
           expect_true(
               second.value() == "https://api.kordex.dev/v1/users",
               "buildUrl should normalize duplicate separators");
  }

  [[nodiscard]] bool test_methods()
  {
    auto module_result = kordex::standard::create_http_module();

    if (!expect_true(
            result_ok(module_result),
            "http module creation should succeed"))
    {
      return false;
    }

    auto normalize_get_result = module_result.value().call(
        "normalizeMethod",
        {kordex::bindings::Value::string("get")});

    auto normalize_post_result = module_result.value().call(
        "normalizeMethod",
        {kordex::bindings::Value::string("Post")});

    auto is_get_result = module_result.value().call(
        "isMethod",
        {kordex::bindings::Value::string("GET")});

    auto is_invalid_result = module_result.value().call(
        "isMethod",
        {kordex::bindings::Value::string("TRACE")});

    if (!expect_true(
            binding_result_ok(normalize_get_result) &&
                binding_result_ok(normalize_post_result) &&
                binding_result_ok(is_get_result) &&
                binding_result_ok(is_invalid_result),
            "http method calls should succeed"))
    {
      return false;
    }

    auto normalized_get = normalize_get_result.value().as_string();
    auto normalized_post = normalize_post_result.value().as_string();
    auto is_get = is_get_result.value().as_boolean();
    auto is_invalid = is_invalid_result.value().as_boolean();

    if (!expect_true(
            binding_result_ok(normalized_get) &&
                binding_result_ok(normalized_post) &&
                binding_result_ok(is_get) &&
                binding_result_ok(is_invalid),
            "http method results should convert"))
    {
      return false;
    }

    return expect_true(
               normalized_get.value() == "GET",
               "normalizeMethod should uppercase get") &&
           expect_true(
               normalized_post.value() == "POST",
               "normalizeMethod should uppercase post") &&
           expect_true(
               is_get.value(),
               "GET should be a supported method") &&
           expect_true(
               !is_invalid.value(),
               "TRACE should not be supported");
  }

  [[nodiscard]] bool test_invalid_status_arguments()
  {
    auto module_result = kordex::standard::create_http_module();

    if (!expect_true(
            result_ok(module_result),
            "http module creation should succeed"))
    {
      return false;
    }

    auto missing_arg = module_result.value().call("isSuccess", {});

    auto non_number_arg = module_result.value().call(
        "isSuccess",
        {kordex::bindings::Value::string("200")});

    auto too_low = module_result.value().call(
        "isSuccess",
        {kordex::bindings::Value::number(99.0)});

    auto too_high = module_result.value().call(
        "isSuccess",
        {kordex::bindings::Value::number(600.0)});

    auto decimal = module_result.value().call(
        "isSuccess",
        {kordex::bindings::Value::number(200.5)});

    return expect_true(
               !missing_arg,
               "missing status should fail") &&
           expect_true(
               !non_number_arg,
               "non-number status should fail") &&
           expect_true(
               !too_low,
               "too low status should fail") &&
           expect_true(
               !too_high,
               "too high status should fail") &&
           expect_true(
               !decimal,
               "decimal status should fail") &&
           expect_true(
               missing_arg.error().code() ==
                   kordex::bindings::ErrorCode::InvalidArgument,
               "missing status should map to invalid argument") &&
           expect_true(
               non_number_arg.error().code() ==
                   kordex::bindings::ErrorCode::InvalidArgument,
               "non-number status should map to invalid argument") &&
           expect_true(
               too_low.error().code() ==
                   kordex::bindings::ErrorCode::InvalidArgument,
               "too low status should map to invalid argument") &&
           expect_true(
               too_high.error().code() ==
                   kordex::bindings::ErrorCode::InvalidArgument,
               "too high status should map to invalid argument") &&
           expect_true(
               decimal.error().code() ==
                   kordex::bindings::ErrorCode::InvalidArgument,
               "decimal status should map to invalid argument");
  }

  [[nodiscard]] bool test_invalid_string_arguments()
  {
    auto module_result = kordex::standard::create_http_module();

    if (!expect_true(
            result_ok(module_result),
            "http module creation should succeed"))
    {
      return false;
    }

    auto build_url_missing = module_result.value().call(
        "buildUrl",
        {kordex::bindings::Value::string("https://api.kordex.dev")});

    auto build_url_non_string = module_result.value().call(
        "buildUrl",
        {kordex::bindings::Value::string("https://api.kordex.dev"),
         kordex::bindings::Value::number(42.0)});

    auto build_url_empty = module_result.value().call(
        "buildUrl",
        {kordex::bindings::Value::string(""),
         kordex::bindings::Value::string("v1")});

    auto normalize_missing = module_result.value().call("normalizeMethod", {});

    auto normalize_invalid = module_result.value().call(
        "normalizeMethod",
        {kordex::bindings::Value::string("TRACE")});

    auto is_method_missing = module_result.value().call("isMethod", {});

    return expect_true(
               !build_url_missing,
               "buildUrl missing path should fail") &&
           expect_true(
               !build_url_non_string,
               "buildUrl non-string path should fail") &&
           expect_true(
               !build_url_empty,
               "buildUrl empty base should fail") &&
           expect_true(
               !normalize_missing,
               "normalizeMethod missing method should fail") &&
           expect_true(
               !normalize_invalid,
               "normalizeMethod unsupported method should fail") &&
           expect_true(
               !is_method_missing,
               "isMethod missing method should fail") &&
           expect_true(
               build_url_missing.error().code() ==
                   kordex::bindings::ErrorCode::InvalidArgument,
               "buildUrl missing path should map to invalid argument") &&
           expect_true(
               build_url_non_string.error().code() ==
                   kordex::bindings::ErrorCode::InvalidArgument,
               "buildUrl non-string path should map to invalid argument") &&
           expect_true(
               build_url_empty.error().code() ==
                   kordex::bindings::ErrorCode::InvalidArgument,
               "buildUrl empty base should map to invalid argument") &&
           expect_true(
               normalize_missing.error().code() ==
                   kordex::bindings::ErrorCode::InvalidArgument,
               "normalizeMethod missing method should map to invalid argument") &&
           expect_true(
               normalize_invalid.error().code() ==
                   kordex::bindings::ErrorCode::InvalidArgument,
               "normalizeMethod unsupported method should map to invalid argument") &&
           expect_true(
               is_method_missing.error().code() ==
                   kordex::bindings::ErrorCode::InvalidArgument,
               "isMethod missing method should map to invalid argument");
  }

  [[nodiscard]] bool test_http_disabled()
  {
    auto options = kordex::standard::StdOptions::defaults();
    options.enable_http = false;

    auto result = kordex::standard::Http::create(options);

    return expect_true(
               !result,
               "disabled http should fail creation") &&
           expect_true(
               result.error().code() ==
                   vix::error::ErrorCode::PermissionDenied,
               "disabled http should map to permission denied");
  }

  [[nodiscard]] bool test_http_native_functions_disabled()
  {
    auto options = kordex::standard::StdOptions::defaults();
    options.allow_native_functions = false;

    auto result = kordex::standard::Http::create(options);

    return expect_true(
               !result,
               "http without native functions should fail") &&
           expect_true(
               result.error().code() ==
                   vix::error::ErrorCode::ConfigError,
               "disabled native functions should map to config error");
  }

  [[nodiscard]] bool test_http_disabled_in_production()
  {
    auto result = kordex::standard::Http::create(
        kordex::standard::StdOptions::production());

    return expect_true(
               !result,
               "http should be disabled in production options") &&
           expect_true(
               result.error().code() ==
                   vix::error::ErrorCode::PermissionDenied,
               "production http disabled should map to permission denied");
  }
} // namespace

int main()
{
  const bool ok =
      test_create_http() &&
      test_create_http_module() &&
      test_http_module_values() &&
      test_reason_phrase() &&
      test_status_classes() &&
      test_status_negative_cases() &&
      test_status_text_function() &&
      test_build_url() &&
      test_methods() &&
      test_invalid_status_arguments() &&
      test_invalid_string_arguments() &&
      test_http_disabled() &&
      test_http_native_functions_disabled() &&
      test_http_disabled_in_production();

  return ok ? 0 : 1;
}
