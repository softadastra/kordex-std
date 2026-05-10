/**
 *
 *  @file test_timer.cpp
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
#include <kordex/std/Timer.hpp>

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

  [[nodiscard]] bool test_create_timer()
  {
    auto result = kordex::standard::Timer::create(
        kordex::standard::StdOptions::defaults());

    if (!expect_true(
            result_ok(result),
            "timer creation should succeed"))
    {
      return false;
    }

    const auto &timer = result.value();

    return expect_true(
               timer.enabled(),
               "timer should be enabled") &&
           expect_true(
               timer.config().module_enabled("timer"),
               "config should enable timer") &&
           expect_true(
               !timer.validate(),
               "timer validation should succeed");
  }

  [[nodiscard]] bool test_create_timer_module()
  {
    auto result = kordex::standard::create_timer_module();

    if (!expect_true(
            result_ok(result),
            "timer module creation should succeed"))
    {
      return false;
    }

    const auto &module = result.value();

    return expect_true(
               module.valid(),
               "timer module should be valid") &&
           expect_true(
               module.importable(),
               "timer module should be importable") &&
           expect_true(
               module.safe(),
               "timer module should be safe") &&
           expect_true(
               ::std::string_view(module.name()) == "timer",
               "timer module name should match") &&
           expect_true(
               module.has_function("now"),
               "timer module should expose now") &&
           expect_true(
               module.has_function("sleep"),
               "timer module should expose sleep") &&
           expect_true(
               module.has_function("unixMs"),
               "timer module should expose unixMs") &&
           expect_true(
               module.function_count() == 3,
               "timer module should expose three functions") &&
           expect_true(
               module.has_value("name"),
               "timer module should expose name value") &&
           expect_true(
               module.has_value("namespace"),
               "timer module should expose namespace value");
  }

  [[nodiscard]] bool test_timer_module_values()
  {
    auto result = kordex::standard::create_timer_module();

    if (!expect_true(
            result_ok(result),
            "timer module creation should succeed"))
    {
      return false;
    }

    auto name = result.value().value("name");
    auto namespace_name = result.value().value("namespace");

    if (!expect_true(
            binding_result_ok(name) &&
                binding_result_ok(namespace_name),
            "timer module values should exist"))
    {
      return false;
    }

    auto name_text = name.value().as_string();
    auto namespace_text = namespace_name.value().as_string();

    if (!expect_true(
            binding_result_ok(name_text) &&
                binding_result_ok(namespace_text),
            "timer module values should convert to string"))
    {
      return false;
    }

    return expect_true(
               name_text.value() == "timer",
               "timer name value should match") &&
           expect_true(
               namespace_text.value() == "kordex",
               "timer namespace value should match");
  }

  [[nodiscard]] bool test_now()
  {
    auto module_result = kordex::standard::create_timer_module();

    if (!expect_true(
            result_ok(module_result),
            "timer module creation should succeed"))
    {
      return false;
    }

    auto first_result = module_result.value().call("now", {});
    auto second_result = module_result.value().call("now", {});

    if (!expect_true(
            binding_result_ok(first_result) &&
                binding_result_ok(second_result),
            "timer.now calls should succeed"))
    {
      return false;
    }

    auto first = first_result.value().as_number();
    auto second = second_result.value().as_number();

    if (!expect_true(
            binding_result_ok(first) &&
                binding_result_ok(second),
            "timer.now values should convert to number"))
    {
      return false;
    }

    return expect_true(
               first.value() >= 0.0,
               "timer.now first value should be positive") &&
           expect_true(
               second.value() >= first.value(),
               "timer.now should be monotonic");
  }

  [[nodiscard]] bool test_unix_ms()
  {
    auto module_result = kordex::standard::create_timer_module();

    if (!expect_true(
            result_ok(module_result),
            "timer module creation should succeed"))
    {
      return false;
    }

    auto result = module_result.value().call("unixMs", {});

    if (!expect_true(
            binding_result_ok(result),
            "timer.unixMs should succeed"))
    {
      return false;
    }

    auto value = result.value().as_number();

    if (!expect_true(
            binding_result_ok(value),
            "timer.unixMs value should convert to number"))
    {
      return false;
    }

    return expect_true(
        value.value() > 0.0,
        "timer.unixMs should return positive timestamp");
  }

  [[nodiscard]] bool test_sleep_zero()
  {
    auto module_result = kordex::standard::create_timer_module();

    if (!expect_true(
            result_ok(module_result),
            "timer module creation should succeed"))
    {
      return false;
    }

    kordex::bindings::NativeFunctionArguments args{
        kordex::bindings::Value::number(0.0)};

    auto result = module_result.value().call("sleep", args);

    if (!expect_true(
            binding_result_ok(result),
            "timer.sleep should succeed with zero delay"))
    {
      return false;
    }

    return expect_true(
        result.value().is_undefined(),
        "timer.sleep should return undefined");
  }

  [[nodiscard]] bool test_sleep_small_delay()
  {
    auto module_result = kordex::standard::create_timer_module();

    if (!expect_true(
            result_ok(module_result),
            "timer module creation should succeed"))
    {
      return false;
    }

    auto before_result = module_result.value().call("now", {});

    kordex::bindings::NativeFunctionArguments sleep_args{
        kordex::bindings::Value::number(1.0)};

    auto sleep_result = module_result.value().call("sleep", sleep_args);
    auto after_result = module_result.value().call("now", {});

    if (!expect_true(
            binding_result_ok(before_result) &&
                binding_result_ok(sleep_result) &&
                binding_result_ok(after_result),
            "timer sleep flow should succeed"))
    {
      return false;
    }

    auto before = before_result.value().as_number();
    auto after = after_result.value().as_number();

    if (!expect_true(
            binding_result_ok(before) &&
                binding_result_ok(after),
            "timer sleep timestamps should convert"))
    {
      return false;
    }

    return expect_true(
               sleep_result.value().is_undefined(),
               "timer.sleep should return undefined") &&
           expect_true(
               after.value() >= before.value(),
               "timer timestamp after sleep should be greater or equal");
  }

  [[nodiscard]] bool test_invalid_arguments()
  {
    auto module_result = kordex::standard::create_timer_module();

    if (!expect_true(
            result_ok(module_result),
            "timer module creation should succeed"))
    {
      return false;
    }

    auto now_with_arg = module_result.value().call(
        "now",
        {kordex::bindings::Value::number(1.0)});

    auto unix_ms_with_arg = module_result.value().call(
        "unixMs",
        {kordex::bindings::Value::number(1.0)});

    auto sleep_missing_arg = module_result.value().call("sleep", {});

    auto sleep_non_number_arg = module_result.value().call(
        "sleep",
        {kordex::bindings::Value::string("10")});

    auto sleep_negative_arg = module_result.value().call(
        "sleep",
        {kordex::bindings::Value::number(-1.0)});

    return expect_true(
               !now_with_arg,
               "timer.now with argument should fail") &&
           expect_true(
               !unix_ms_with_arg,
               "timer.unixMs with argument should fail") &&
           expect_true(
               !sleep_missing_arg,
               "timer.sleep missing delay should fail") &&
           expect_true(
               !sleep_non_number_arg,
               "timer.sleep non-number delay should fail") &&
           expect_true(
               !sleep_negative_arg,
               "timer.sleep negative delay should fail") &&
           expect_true(
               now_with_arg.error().code() ==
                   kordex::bindings::ErrorCode::InvalidArgument,
               "timer.now with arg should map to invalid argument") &&
           expect_true(
               unix_ms_with_arg.error().code() ==
                   kordex::bindings::ErrorCode::InvalidArgument,
               "timer.unixMs with arg should map to invalid argument") &&
           expect_true(
               sleep_missing_arg.error().code() ==
                   kordex::bindings::ErrorCode::InvalidArgument,
               "missing sleep delay should map to invalid argument") &&
           expect_true(
               sleep_non_number_arg.error().code() ==
                   kordex::bindings::ErrorCode::InvalidArgument,
               "non-number sleep delay should map to invalid argument") &&
           expect_true(
               sleep_negative_arg.error().code() ==
                   kordex::bindings::ErrorCode::InvalidArgument,
               "negative sleep delay should map to invalid argument");
  }

  [[nodiscard]] bool test_timer_disabled()
  {
    auto options = kordex::standard::StdOptions::defaults();
    options.enable_timer = false;

    auto result = kordex::standard::Timer::create(options);

    return expect_true(
               !result,
               "disabled timer should fail creation") &&
           expect_true(
               result.error().code() ==
                   vix::error::ErrorCode::PermissionDenied,
               "disabled timer should map to permission denied");
  }

  [[nodiscard]] bool test_timer_native_functions_disabled()
  {
    auto options = kordex::standard::StdOptions::defaults();
    options.allow_native_functions = false;

    auto result = kordex::standard::Timer::create(options);

    return expect_true(
               !result,
               "timer without native functions should fail") &&
           expect_true(
               result.error().code() ==
                   vix::error::ErrorCode::ConfigError,
               "disabled native functions should map to config error");
  }
} // namespace

int main()
{
  const bool ok =
      test_create_timer() &&
      test_create_timer_module() &&
      test_timer_module_values() &&
      test_now() &&
      test_unix_ms() &&
      test_sleep_zero() &&
      test_sleep_small_delay() &&
      test_invalid_arguments() &&
      test_timer_disabled() &&
      test_timer_native_functions_disabled();

  return ok ? 0 : 1;
}
