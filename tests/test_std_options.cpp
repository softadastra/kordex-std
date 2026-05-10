/**
 *
 *  @file test_std_options.cpp
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
#include <kordex/std/StdOptions.hpp>

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

  [[nodiscard]] bool test_default_options()
  {
    const auto options = kordex::standard::StdOptions::defaults();

    return expect_true(
               options.enabled,
               "default options should be enabled") &&
           expect_true(
               options.safe_by_default,
               "default modules should be safe") &&
           expect_true(
               options.allow_native_modules,
               "default options should allow native modules") &&
           expect_true(
               options.allow_native_functions,
               "default options should allow native functions") &&
           expect_true(
               options.has_registry_name(),
               "default options should have registry name") &&
           expect_true(
               options.has_namespace_name(),
               "default options should have namespace name") &&
           expect_true(
               options.has_enabled_modules(),
               "default options should enable modules") &&
           expect_true(
               ::std::string_view(options.registry_name) == "kordex-std",
               "default registry name should match") &&
           expect_true(
               ::std::string_view(options.namespace_name) == "kordex",
               "default namespace should match");
  }

  [[nodiscard]] bool test_development_options()
  {
    const auto options = kordex::standard::StdOptions::development();

    return expect_true(
               options.enabled,
               "development options should be enabled") &&
           expect_true(
               options.enable_console,
               "development should enable console") &&
           expect_true(
               options.enable_fs,
               "development should enable fs") &&
           expect_true(
               options.enable_process,
               "development should enable process") &&
           expect_true(
               options.enable_http,
               "development should enable http") &&
           expect_true(
               !options.has_max_modules(),
               "development should not limit module count") &&
           expect_true(
               ::std::string_view(options.registry_name) ==
                   "kordex-std-dev",
               "development registry name should match");
  }

  [[nodiscard]] bool test_production_options()
  {
    const auto options = kordex::standard::StdOptions::production();

    return expect_true(
               options.enabled,
               "production options should be enabled") &&
           expect_true(
               options.enable_console,
               "production should enable console") &&
           expect_true(
               options.enable_fs,
               "production should enable fs") &&
           expect_true(
               options.enable_path,
               "production should enable path") &&
           expect_true(
               options.enable_env,
               "production should enable env") &&
           expect_true(
               !options.enable_process,
               "production should disable process by default") &&
           expect_true(
               options.enable_crypto,
               "production should enable crypto") &&
           expect_true(
               !options.enable_http,
               "production should disable http by default");
  }

  [[nodiscard]] bool test_test_options()
  {
    const auto options = kordex::standard::StdOptions::test();

    return expect_true(
               options.enabled,
               "test options should be enabled") &&
           expect_true(
               options.enable_console &&
                   options.enable_fs &&
                   options.enable_path &&
                   options.enable_env &&
                   options.enable_process &&
                   options.enable_timer &&
                   options.enable_crypto &&
                   options.enable_http,
               "test options should enable all modules") &&
           expect_true(
               options.has_max_modules(),
               "test options should have max modules") &&
           expect_true(
               options.max_modules == 16,
               "test max modules should be 16") &&
           expect_true(
               ::std::string_view(options.registry_name) ==
                   "kordex-std-test",
               "test registry name should match");
  }

  [[nodiscard]] bool test_minimal_options()
  {
    const auto options = kordex::standard::StdOptions::minimal();

    return expect_true(
               options.enabled,
               "minimal options should be enabled") &&
           expect_true(
               options.enable_console,
               "minimal should enable console") &&
           expect_true(
               options.enable_path,
               "minimal should enable path") &&
           expect_true(
               !options.enable_fs,
               "minimal should disable fs") &&
           expect_true(
               !options.enable_env,
               "minimal should disable env") &&
           expect_true(
               !options.enable_process,
               "minimal should disable process") &&
           expect_true(
               !options.enable_timer,
               "minimal should disable timer") &&
           expect_true(
               !options.enable_crypto,
               "minimal should disable crypto") &&
           expect_true(
               !options.enable_http,
               "minimal should disable http");
  }

  [[nodiscard]] bool test_option_helpers()
  {
    auto options = kordex::standard::StdOptions::defaults();

    const bool initial_state =
        options.has_registry_name() &&
        options.has_namespace_name() &&
        !options.has_module_paths() &&
        !options.has_max_modules() &&
        options.has_enabled_modules();

    options.module_paths.push_back("modules");
    options.max_modules = 4;

    const bool configured_state =
        options.has_module_paths() &&
        options.has_max_modules();

    options.enabled = false;

    const bool disabled_state =
        !options.has_enabled_modules();

    return expect_true(
               initial_state,
               "initial helper state should match") &&
           expect_true(
               configured_state,
               "configured helper state should match") &&
           expect_true(
               disabled_state,
               "disabled options should report no enabled modules");
  }

  [[nodiscard]] bool test_no_enabled_modules()
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

    return expect_true(
        !options.has_enabled_modules(),
        "options with all modules disabled should report no enabled modules");
  }
} // namespace

int main()
{
  const bool ok =
      test_default_options() &&
      test_development_options() &&
      test_production_options() &&
      test_test_options() &&
      test_minimal_options() &&
      test_option_helpers() &&
      test_no_enabled_modules();

  return ok ? 0 : 1;
}
