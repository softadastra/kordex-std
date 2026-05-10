/**
 *
 *  @file test_module_factory.cpp
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
#include <vector>

#include <kordex/std/ModuleFactory.hpp>

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

  [[nodiscard]] bool contains_name(
      const ::std::vector<::std::string> &names,
      ::std::string_view expected)
  {
    for (const auto &name : names)
    {
      if (name == expected)
      {
        return true;
      }
    }

    return false;
  }

  [[nodiscard]] bool test_create_factory()
  {
    auto result = kordex::standard::ModuleFactory::create(
        kordex::standard::StdOptions::defaults());

    if (!expect_true(
            result_ok(result),
            "module factory creation should succeed"))
    {
      return false;
    }

    const auto &factory = result.value();

    return expect_true(
               factory.enabled(),
               "module factory should be enabled") &&
           expect_true(
               factory.config().enabled_module_count() == 8,
               "default factory should expose eight enabled modules") &&
           expect_true(
               !factory.validate(),
               "module factory validation should succeed");
  }

  [[nodiscard]] bool test_canonical_name()
  {
    return expect_true(
               kordex::standard::ModuleFactory::canonical_name("console") ==
                   "console",
               "plain module name should remain unchanged") &&
           expect_true(
               kordex::standard::ModuleFactory::canonical_name("kordex:console") ==
                   "console",
               "prefixed console name should be canonicalized") &&
           expect_true(
               kordex::standard::ModuleFactory::canonical_name("kordex:fs") ==
                   "fs",
               "prefixed fs name should be canonicalized") &&
           expect_true(
               kordex::standard::ModuleFactory::canonical_name("unknown") ==
                   "unknown",
               "unknown plain name should remain unchanged");
  }

  [[nodiscard]] bool test_known_module()
  {
    return expect_true(
               kordex::standard::ModuleFactory::known_module("console"),
               "console should be known") &&
           expect_true(
               kordex::standard::ModuleFactory::known_module("fs"),
               "fs should be known") &&
           expect_true(
               kordex::standard::ModuleFactory::known_module("path"),
               "path should be known") &&
           expect_true(
               kordex::standard::ModuleFactory::known_module("env"),
               "env should be known") &&
           expect_true(
               kordex::standard::ModuleFactory::known_module("process"),
               "process should be known") &&
           expect_true(
               kordex::standard::ModuleFactory::known_module("timer"),
               "timer should be known") &&
           expect_true(
               kordex::standard::ModuleFactory::known_module("crypto"),
               "crypto should be known") &&
           expect_true(
               kordex::standard::ModuleFactory::known_module("http"),
               "http should be known") &&
           expect_true(
               kordex::standard::ModuleFactory::known_module("kordex:console"),
               "kordex:console should be known") &&
           expect_true(
               kordex::standard::ModuleFactory::known_module("kordex:http"),
               "kordex:http should be known") &&
           expect_true(
               !kordex::standard::ModuleFactory::known_module("unknown"),
               "unknown module should not be known");
  }

  [[nodiscard]] bool test_can_create_default_modules()
  {
    auto result = kordex::standard::ModuleFactory::create(
        kordex::standard::StdOptions::defaults());

    if (!expect_true(
            result_ok(result),
            "module factory creation should succeed"))
    {
      return false;
    }

    const auto &factory = result.value();

    return expect_true(
               factory.can_create("console"),
               "factory should create console") &&
           expect_true(
               factory.can_create("kordex:fs"),
               "factory should create kordex:fs") &&
           expect_true(
               factory.can_create("path"),
               "factory should create path") &&
           expect_true(
               factory.can_create("env"),
               "factory should create env") &&
           expect_true(
               factory.can_create("process"),
               "factory should create process") &&
           expect_true(
               factory.can_create("timer"),
               "factory should create timer") &&
           expect_true(
               factory.can_create("crypto"),
               "factory should create crypto") &&
           expect_true(
               factory.can_create("http"),
               "factory should create http") &&
           expect_true(
               !factory.can_create("unknown"),
               "factory should not create unknown module");
  }

  [[nodiscard]] bool test_enabled_module_names_default()
  {
    auto result = kordex::standard::ModuleFactory::create(
        kordex::standard::StdOptions::defaults());

    if (!expect_true(
            result_ok(result),
            "module factory creation should succeed"))
    {
      return false;
    }

    const auto names = result.value().enabled_module_names();

    return expect_true(
               names.size() == 8,
               "default factory should return eight enabled names") &&
           expect_true(
               contains_name(names, "console"),
               "enabled names should contain console") &&
           expect_true(
               contains_name(names, "fs"),
               "enabled names should contain fs") &&
           expect_true(
               contains_name(names, "path"),
               "enabled names should contain path") &&
           expect_true(
               contains_name(names, "env"),
               "enabled names should contain env") &&
           expect_true(
               contains_name(names, "process"),
               "enabled names should contain process") &&
           expect_true(
               contains_name(names, "timer"),
               "enabled names should contain timer") &&
           expect_true(
               contains_name(names, "crypto"),
               "enabled names should contain crypto") &&
           expect_true(
               contains_name(names, "http"),
               "enabled names should contain http");
  }

  [[nodiscard]] bool test_enabled_module_names_minimal()
  {
    auto result = kordex::standard::ModuleFactory::create(
        kordex::standard::StdOptions::minimal());

    if (!expect_true(
            result_ok(result),
            "minimal module factory creation should succeed"))
    {
      return false;
    }

    const auto names = result.value().enabled_module_names();

    return expect_true(
               names.size() == 2,
               "minimal factory should return two enabled names") &&
           expect_true(
               contains_name(names, "console"),
               "minimal names should contain console") &&
           expect_true(
               contains_name(names, "path"),
               "minimal names should contain path") &&
           expect_true(
               !contains_name(names, "fs"),
               "minimal names should not contain fs") &&
           expect_true(
               !contains_name(names, "http"),
               "minimal names should not contain http");
  }

  [[nodiscard]] bool test_enabled_module_names_production()
  {
    auto result = kordex::standard::ModuleFactory::create(
        kordex::standard::StdOptions::production());

    if (!expect_true(
            result_ok(result),
            "production module factory creation should succeed"))
    {
      return false;
    }

    const auto names = result.value().enabled_module_names();

    return expect_true(
               names.size() == 6,
               "production factory should return six enabled names") &&
           expect_true(
               contains_name(names, "console"),
               "production names should contain console") &&
           expect_true(
               contains_name(names, "fs"),
               "production names should contain fs") &&
           expect_true(
               contains_name(names, "path"),
               "production names should contain path") &&
           expect_true(
               contains_name(names, "env"),
               "production names should contain env") &&
           expect_true(
               contains_name(names, "timer"),
               "production names should contain timer") &&
           expect_true(
               contains_name(names, "crypto"),
               "production names should contain crypto") &&
           expect_true(
               !contains_name(names, "process"),
               "production names should not contain process") &&
           expect_true(
               !contains_name(names, "http"),
               "production names should not contain http");
  }

  [[nodiscard]] bool test_create_console_module()
  {
    auto factory_result = kordex::standard::ModuleFactory::create(
        kordex::standard::StdOptions::defaults());

    if (!expect_true(
            result_ok(factory_result),
            "module factory creation should succeed"))
    {
      return false;
    }

    auto module_result = factory_result.value().create_module("console");

    if (!expect_true(
            result_ok(module_result),
            "factory should create console module"))
    {
      return false;
    }

    const auto &module = module_result.value();

    return expect_true(
               module.valid(),
               "console module should be valid") &&
           expect_true(
               ::std::string_view(module.name()) == "console",
               "console module name should match") &&
           expect_true(
               module.has_function("log"),
               "console module should expose log");
  }

  [[nodiscard]] bool test_create_prefixed_fs_module()
  {
    auto factory_result = kordex::standard::ModuleFactory::create(
        kordex::standard::StdOptions::defaults());

    if (!expect_true(
            result_ok(factory_result),
            "module factory creation should succeed"))
    {
      return false;
    }

    auto module_result = factory_result.value().create_module("kordex:fs");

    if (!expect_true(
            result_ok(module_result),
            "factory should create prefixed fs module"))
    {
      return false;
    }

    const auto &module = module_result.value();

    return expect_true(
               module.valid(),
               "fs module should be valid") &&
           expect_true(
               ::std::string_view(module.name()) == "fs",
               "fs module name should match") &&
           expect_true(
               module.has_function("readText"),
               "fs module should expose readText") &&
           expect_true(
               module.has_function("writeText"),
               "fs module should expose writeText");
  }

  [[nodiscard]] bool test_create_path_module()
  {
    auto module_result = kordex::standard::create_std_module("path");

    if (!expect_true(
            result_ok(module_result),
            "create_std_module should create path"))
    {
      return false;
    }

    const auto &module = module_result.value();

    return expect_true(
               ::std::string_view(module.name()) == "path",
               "path module name should match") &&
           expect_true(
               module.has_function("normalize"),
               "path module should expose normalize") &&
           expect_true(
               module.has_function("join"),
               "path module should expose join");
  }

  [[nodiscard]] bool test_create_all_default_modules()
  {
    auto factory_result = kordex::standard::ModuleFactory::create(
        kordex::standard::StdOptions::defaults());

    if (!expect_true(
            result_ok(factory_result),
            "module factory creation should succeed"))
    {
      return false;
    }

    auto modules_result = factory_result.value().create_all();

    if (!expect_true(
            result_ok(modules_result),
            "factory should create all default modules"))
    {
      return false;
    }

    const auto &modules = modules_result.value();

    return expect_true(
               modules.size() == 8,
               "default create_all should create eight modules") &&
           expect_true(
               ::std::string_view(modules[0].name()) == "console",
               "first default module should be console") &&
           expect_true(
               ::std::string_view(modules[1].name()) == "fs",
               "second default module should be fs") &&
           expect_true(
               ::std::string_view(modules[2].name()) == "path",
               "third default module should be path") &&
           expect_true(
               ::std::string_view(modules[3].name()) == "env",
               "fourth default module should be env") &&
           expect_true(
               ::std::string_view(modules[4].name()) == "process",
               "fifth default module should be process") &&
           expect_true(
               ::std::string_view(modules[5].name()) == "timer",
               "sixth default module should be timer") &&
           expect_true(
               ::std::string_view(modules[6].name()) == "crypto",
               "seventh default module should be crypto") &&
           expect_true(
               ::std::string_view(modules[7].name()) == "http",
               "eighth default module should be http");
  }

  [[nodiscard]] bool test_create_all_minimal_modules()
  {
    auto modules_result = kordex::standard::create_std_modules(
        kordex::standard::StdOptions::minimal());

    if (!expect_true(
            result_ok(modules_result),
            "create_std_modules should create minimal modules"))
    {
      return false;
    }

    const auto &modules = modules_result.value();

    return expect_true(
               modules.size() == 2,
               "minimal create_all should create two modules") &&
           expect_true(
               ::std::string_view(modules[0].name()) == "console",
               "first minimal module should be console") &&
           expect_true(
               ::std::string_view(modules[1].name()) == "path",
               "second minimal module should be path");
  }

  [[nodiscard]] bool test_create_disabled_module()
  {
    auto options = kordex::standard::StdOptions::defaults();
    options.enable_fs = false;

    auto factory_result = kordex::standard::ModuleFactory::create(options);

    if (!expect_true(
            result_ok(factory_result),
            "factory with fs disabled should still be valid"))
    {
      return false;
    }

    auto module_result = factory_result.value().create_module("fs");

    return expect_true(
               !module_result,
               "creating disabled fs module should fail") &&
           expect_true(
               module_result.error().code() ==
                   vix::error::ErrorCode::PermissionDenied,
               "disabled module should map to permission denied") &&
           expect_true(
               !factory_result.value().can_create("fs"),
               "factory should not create disabled fs");
  }

  [[nodiscard]] bool test_create_unknown_module()
  {
    auto factory_result = kordex::standard::ModuleFactory::create(
        kordex::standard::StdOptions::defaults());

    if (!expect_true(
            result_ok(factory_result),
            "module factory creation should succeed"))
    {
      return false;
    }

    auto module_result = factory_result.value().create_module("unknown");

    return expect_true(
               !module_result,
               "creating unknown module should fail") &&
           expect_true(
               module_result.error().code() ==
                   vix::error::ErrorCode::InvalidArgument,
               "unknown module should map to invalid argument");
  }

  [[nodiscard]] bool test_create_process_in_production_fails()
  {
    auto factory_result = kordex::standard::ModuleFactory::create(
        kordex::standard::StdOptions::production());

    if (!expect_true(
            result_ok(factory_result),
            "production factory creation should succeed"))
    {
      return false;
    }

    auto module_result = factory_result.value().create_module("process");

    return expect_true(
               !module_result,
               "creating process in production should fail") &&
           expect_true(
               module_result.error().code() ==
                   vix::error::ErrorCode::PermissionDenied,
               "disabled production process should map to permission denied") &&
           expect_true(
               !factory_result.value().can_create("process"),
               "production factory should not create process");
  }

  [[nodiscard]] bool test_create_http_in_production_fails()
  {
    auto factory_result = kordex::standard::ModuleFactory::create(
        kordex::standard::StdOptions::production());

    if (!expect_true(
            result_ok(factory_result),
            "production factory creation should succeed"))
    {
      return false;
    }

    auto module_result = factory_result.value().create_module("http");

    return expect_true(
               !module_result,
               "creating http in production should fail") &&
           expect_true(
               module_result.error().code() ==
                   vix::error::ErrorCode::PermissionDenied,
               "disabled production http should map to permission denied") &&
           expect_true(
               !factory_result.value().can_create("http"),
               "production factory should not create http");
  }

  [[nodiscard]] bool test_invalid_factory_options()
  {
    auto options = kordex::standard::StdOptions::defaults();
    options.registry_name.clear();

    auto factory_result = kordex::standard::ModuleFactory::create(options);

    return expect_true(
               !factory_result,
               "factory with empty registry name should fail") &&
           expect_true(
               factory_result.error().code() ==
                   vix::error::ErrorCode::ConfigError,
               "invalid factory config should map to config error");
  }

  [[nodiscard]] bool test_factory_max_modules_limit()
  {
    auto options = kordex::standard::StdOptions::defaults();
    options.max_modules = 2;

    auto factory_result = kordex::standard::ModuleFactory::create(options);

    return expect_true(
               !factory_result,
               "factory should fail when enabled modules exceed max_modules") &&
           expect_true(
               factory_result.error().code() ==
                   vix::error::ErrorCode::ConfigError,
               "max module failure should map to config error");
  }

} // namespace

int main()
{
  const bool ok =
      test_create_factory() &&
      test_canonical_name() &&
      test_known_module() &&
      test_can_create_default_modules() &&
      test_enabled_module_names_default() &&
      test_enabled_module_names_minimal() &&
      test_enabled_module_names_production() &&
      test_create_console_module() &&
      test_create_prefixed_fs_module() &&
      test_create_path_module() &&
      test_create_all_default_modules() &&
      test_create_all_minimal_modules() &&
      test_create_disabled_module() &&
      test_create_unknown_module() &&
      test_create_process_in_production_fails() &&
      test_create_http_in_production_fails() &&
      test_invalid_factory_options() &&
      test_factory_max_modules_limit();

  return ok ? 0 : 1;
}
