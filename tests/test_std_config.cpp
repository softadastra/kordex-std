/**
 *
 *  @file test_std_config.cpp
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
#include <kordex/std/StdConfig.hpp>

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

  [[nodiscard]] bool test_from_default_options()
  {
    auto result = kordex::standard::StdConfig::from_options(
        kordex::standard::StdOptions::defaults());

    if (!expect_true(
            result_ok(result),
            "default std config should be valid"))
    {
      return false;
    }

    const auto &config = result.value();

    return expect_true(
               config.enabled,
               "default config should be enabled") &&
           expect_true(
               config.safe_by_default,
               "default config should be safe by default") &&
           expect_true(
               config.allow_native_modules,
               "default config should allow native modules") &&
           expect_true(
               config.allow_native_functions,
               "default config should allow native functions") &&
           expect_true(
               ::std::string_view(config.registry_name) == "kordex-std",
               "default config registry name should match") &&
           expect_true(
               ::std::string_view(config.namespace_name) == "kordex",
               "default config namespace should match") &&
           expect_true(
               config.enabled_module_count() == 8,
               "default config should enable eight modules");
  }

  [[nodiscard]] bool test_from_development_options()
  {
    auto result = kordex::standard::StdConfig::from_options(
        kordex::standard::StdOptions::development());

    if (!expect_true(
            result_ok(result),
            "development std config should be valid"))
    {
      return false;
    }

    const auto &config = result.value();

    return expect_true(
               ::std::string_view(config.registry_name) ==
                   "kordex-std-dev",
               "development registry name should match") &&
           expect_true(
               config.enable_process,
               "development should enable process") &&
           expect_true(
               config.enable_http,
               "development should enable http") &&
           expect_true(
               !config.has_max_modules(),
               "development should not limit modules");
  }

  [[nodiscard]] bool test_from_production_options()
  {
    auto result = kordex::standard::StdConfig::from_options(
        kordex::standard::StdOptions::production());

    if (!expect_true(
            result_ok(result),
            "production std config should be valid"))
    {
      return false;
    }

    const auto &config = result.value();

    return expect_true(
               config.enable_console,
               "production should enable console") &&
           expect_true(
               config.enable_fs,
               "production should enable fs") &&
           expect_true(
               config.enable_path,
               "production should enable path") &&
           expect_true(
               config.enable_env,
               "production should enable env") &&
           expect_true(
               !config.enable_process,
               "production should disable process") &&
           expect_true(
               config.enable_timer,
               "production should enable timer") &&
           expect_true(
               config.enable_crypto,
               "production should enable crypto") &&
           expect_true(
               !config.enable_http,
               "production should disable http") &&
           expect_true(
               config.enabled_module_count() == 6,
               "production should enable six modules");
  }

  [[nodiscard]] bool test_from_test_options()
  {
    auto result = kordex::standard::StdConfig::from_options(
        kordex::standard::StdOptions::test());

    if (!expect_true(
            result_ok(result),
            "test std config should be valid"))
    {
      return false;
    }

    const auto &config = result.value();

    return expect_true(
               config.has_max_modules(),
               "test config should have max modules") &&
           expect_true(
               config.max_modules == 16,
               "test max modules should be 16") &&
           expect_true(
               config.enabled_module_count() == 8,
               "test config should enable eight modules");
  }

  [[nodiscard]] bool test_from_minimal_options()
  {
    auto result = kordex::standard::StdConfig::from_options(
        kordex::standard::StdOptions::minimal());

    if (!expect_true(
            result_ok(result),
            "minimal std config should be valid"))
    {
      return false;
    }

    const auto &config = result.value();

    return expect_true(
               config.enable_console,
               "minimal should enable console") &&
           expect_true(
               config.enable_path,
               "minimal should enable path") &&
           expect_true(
               !config.enable_fs,
               "minimal should disable fs") &&
           expect_true(
               !config.enable_env,
               "minimal should disable env") &&
           expect_true(
               !config.enable_process,
               "minimal should disable process") &&
           expect_true(
               config.enabled_module_count() == 2,
               "minimal should enable two modules");
  }

  [[nodiscard]] bool test_from_environment()
  {
    auto result = kordex::standard::StdConfig::from_environment(
        kordex::standard::StdOptions::minimal());

    if (!expect_true(
            result_ok(result),
            "std config from environment should be valid"))
    {
      return false;
    }

    const auto &config = result.value();

    return expect_true(
               ::std::string_view(config.registry_name) ==
                   "kordex-std-minimal",
               "environment config should use base options") &&
           expect_true(
               config.enabled_module_count() == 2,
               "environment config should preserve enabled modules");
  }

  [[nodiscard]] bool test_helpers()
  {
    auto options = kordex::standard::StdOptions::minimal();
    options.module_paths.push_back("modules");
    options.max_modules = 4;

    auto result = kordex::standard::StdConfig::from_options(options);

    if (!expect_true(
            result_ok(result),
            "configured std options should be valid"))
    {
      return false;
    }

    const auto &config = result.value();

    return expect_true(
               config.has_registry_name(),
               "config should have registry name") &&
           expect_true(
               config.has_namespace_name(),
               "config should have namespace name") &&
           expect_true(
               config.has_module_paths(),
               "config should have module paths") &&
           expect_true(
               config.has_max_modules(),
               "config should have max modules") &&
           expect_true(
               config.has_enabled_modules(),
               "config should have enabled modules");
  }

  [[nodiscard]] bool test_module_enabled()
  {
    const auto config_result = kordex::standard::StdConfig::from_options(
        kordex::standard::StdOptions::production());

    if (!expect_true(
            result_ok(config_result),
            "production config should be valid"))
    {
      return false;
    }

    const auto &config = config_result.value();

    return expect_true(
               config.module_enabled("console"),
               "console should be enabled") &&
           expect_true(
               config.module_enabled("kordex:console"),
               "kordex:console should be enabled") &&
           expect_true(
               config.module_enabled("fs"),
               "fs should be enabled") &&
           expect_true(
               config.module_enabled("kordex:path"),
               "kordex:path should be enabled") &&
           expect_true(
               !config.module_enabled("process"),
               "process should be disabled in production") &&
           expect_true(
               !config.module_enabled("kordex:http"),
               "http should be disabled in production") &&
           expect_true(
               !config.module_enabled("unknown"),
               "unknown module should not be enabled");
  }

  [[nodiscard]] bool test_disabled_config_is_valid()
  {
    auto options = kordex::standard::StdOptions::defaults();
    options.enabled = false;
    options.registry_name.clear();
    options.namespace_name.clear();

    auto result = kordex::standard::StdConfig::from_options(options);

    if (!expect_true(
            result_ok(result),
            "disabled config should be valid"))
    {
      return false;
    }

    const auto &config = result.value();

    return expect_true(
               !config.enabled,
               "config should be disabled") &&
           expect_true(
               !config.has_enabled_modules(),
               "disabled config should report no enabled modules") &&
           expect_true(
               config.enabled_module_count() == 0,
               "disabled config should have zero enabled modules") &&
           expect_true(
               !config.module_enabled("console"),
               "disabled config should not enable console");
  }

  [[nodiscard]] bool test_invalid_empty_registry_name()
  {
    auto options = kordex::standard::StdOptions::defaults();
    options.registry_name.clear();

    auto result = kordex::standard::StdConfig::from_options(options);

    return expect_true(
               !result,
               "empty registry name should fail") &&
           expect_true(
               result.error().code() ==
                   vix::error::ErrorCode::ConfigError,
               "empty registry name should map to config error");
  }

  [[nodiscard]] bool test_invalid_empty_namespace_name()
  {
    auto options = kordex::standard::StdOptions::defaults();
    options.namespace_name.clear();

    auto result = kordex::standard::StdConfig::from_options(options);

    return expect_true(
               !result,
               "empty namespace name should fail") &&
           expect_true(
               result.error().code() ==
                   vix::error::ErrorCode::ConfigError,
               "empty namespace name should map to config error");
  }

  [[nodiscard]] bool test_invalid_native_modules_disabled()
  {
    auto options = kordex::standard::StdOptions::defaults();
    options.allow_native_modules = false;

    auto result = kordex::standard::StdConfig::from_options(options);

    return expect_true(
               !result,
               "disabled native modules should fail") &&
           expect_true(
               result.error().code() ==
                   vix::error::ErrorCode::ConfigError,
               "disabled native modules should map to config error");
  }

  [[nodiscard]] bool test_invalid_native_functions_disabled()
  {
    auto options = kordex::standard::StdOptions::defaults();
    options.allow_native_functions = false;

    auto result = kordex::standard::StdConfig::from_options(options);

    return expect_true(
               !result,
               "disabled native functions should fail") &&
           expect_true(
               result.error().code() ==
                   vix::error::ErrorCode::ConfigError,
               "disabled native functions should map to config error");
  }

  [[nodiscard]] bool test_invalid_empty_module_path()
  {
    auto options = kordex::standard::StdOptions::defaults();
    options.module_paths.push_back("");

    auto result = kordex::standard::StdConfig::from_options(options);

    return expect_true(
               !result,
               "empty module path should fail") &&
           expect_true(
               result.error().code() ==
                   vix::error::ErrorCode::ConfigError,
               "empty module path should map to config error");
  }

  [[nodiscard]] bool test_invalid_no_enabled_modules()
  {
    auto options = kordex::standard::StdOptions::defaults();

    options.enable_console = false;
    options.enable_fs = false;
    options.enable_path = false;
    options.enable_env = false;
    options.enable_process = false;
    options.enable_timer = false;
    options.enable_crypto = false;
    options.enable_http = false;

    auto result = kordex::standard::StdConfig::from_options(options);

    return expect_true(
               !result,
               "no enabled modules should fail") &&
           expect_true(
               result.error().code() ==
                   vix::error::ErrorCode::PermissionDenied,
               "no enabled modules should map to permission denied");
  }

  [[nodiscard]] bool test_invalid_max_modules()
  {
    auto options = kordex::standard::StdOptions::defaults();
    options.max_modules = 1;

    auto result = kordex::standard::StdConfig::from_options(options);

    return expect_true(
               !result,
               "enabled modules above max should fail") &&
           expect_true(
               result.error().code() ==
                   vix::error::ErrorCode::ConfigError,
               "max modules failure should map to config error");
  }
} // namespace

int main()
{
  const bool ok =
      test_from_default_options() &&
      test_from_development_options() &&
      test_from_production_options() &&
      test_from_test_options() &&
      test_from_minimal_options() &&
      test_from_environment() &&
      test_helpers() &&
      test_module_enabled() &&
      test_disabled_config_is_valid() &&
      test_invalid_empty_registry_name() &&
      test_invalid_empty_namespace_name() &&
      test_invalid_native_modules_disabled() &&
      test_invalid_native_functions_disabled() &&
      test_invalid_empty_module_path() &&
      test_invalid_no_enabled_modules() &&
      test_invalid_max_modules();

  return ok ? 0 : 1;
}
