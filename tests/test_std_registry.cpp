/**
 *
 *  @file test_std_registry.cpp
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
#include <vector>
#include <kordex/std/StdRegistry.hpp>

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

  [[nodiscard]] bool test_registry_info_helpers()
  {
    kordex::standard::StdRegistryInfo info;
    info.name = "std";
    info.namespace_name = "kordex";
    info.initialized = true;
    info.module_count = 2;

    return expect_true(
               info.has_name(),
               "registry info should have name") &&
           expect_true(
               info.has_namespace_name(),
               "registry info should have namespace name") &&
           expect_true(
               info.initialized,
               "registry info should be initialized") &&
           expect_true(
               info.module_count == 2,
               "registry info module count should match");
  }

  [[nodiscard]] bool test_create_registry()
  {
    auto result = kordex::standard::StdRegistry::create(
        kordex::standard::StdOptions::defaults());

    if (!expect_true(
            result_ok(result),
            "std registry creation should succeed"))
    {
      return false;
    }

    const auto &registry = result.value();

    return expect_true(
               registry.enabled(),
               "std registry should be enabled") &&
           expect_true(
               !registry.initialized(),
               "std registry should not be initialized by create") &&
           expect_true(
               registry.module_count() == 0,
               "new std registry should contain zero modules") &&
           expect_true(
               ::std::string_view(registry.name()) == "kordex-std",
               "std registry name should match") &&
           expect_true(
               ::std::string_view(registry.namespace_name()) == "kordex",
               "std registry namespace should match") &&
           expect_true(
               !registry.validate(),
               "std registry validation should succeed");
  }

  [[nodiscard]] bool test_initialize_default_registry()
  {
    auto result = kordex::standard::StdRegistry::create(
        kordex::standard::StdOptions::defaults());

    if (!expect_true(
            result_ok(result),
            "std registry creation should succeed"))
    {
      return false;
    }

    auto &registry = result.value();

    const auto init_error = registry.initialize();

    return expect_true(
               !init_error,
               "std registry initialize should succeed") &&
           expect_true(
               registry.initialized(),
               "std registry should be initialized") &&
           expect_true(
               registry.module_count() == 8,
               "default std registry should contain eight modules") &&
           expect_true(
               registry.info().module_count == 8,
               "registry info module count should be updated") &&
           expect_true(
               registry.has_module("console"),
               "registry should contain console") &&
           expect_true(
               registry.has_module("kordex:fs"),
               "registry should contain prefixed fs") &&
           expect_true(
               registry.has_module("path"),
               "registry should contain path") &&
           expect_true(
               registry.has_module("env"),
               "registry should contain env") &&
           expect_true(
               registry.has_module("process"),
               "registry should contain process") &&
           expect_true(
               registry.has_module("timer"),
               "registry should contain timer") &&
           expect_true(
               registry.has_module("crypto"),
               "registry should contain crypto") &&
           expect_true(
               registry.has_module("http"),
               "registry should contain http");
  }

  [[nodiscard]] bool test_initialize_is_idempotent()
  {
    auto result = kordex::standard::StdRegistry::create(
        kordex::standard::StdOptions::defaults());

    if (!expect_true(
            result_ok(result),
            "std registry creation should succeed"))
    {
      return false;
    }

    auto &registry = result.value();

    const auto first_error = registry.initialize();
    const auto first_count = registry.module_count();

    const auto second_error = registry.initialize();
    const auto second_count = registry.module_count();

    return expect_true(
               !first_error,
               "first initialize should succeed") &&
           expect_true(
               !second_error,
               "second initialize should succeed") &&
           expect_true(
               first_count == 8,
               "first initialize should register eight modules") &&
           expect_true(
               second_count == first_count,
               "second initialize should not duplicate modules");
  }

  [[nodiscard]] bool test_shutdown_and_clear()
  {
    auto result = kordex::standard::StdRegistry::create(
        kordex::standard::StdOptions::defaults());

    if (!expect_true(
            result_ok(result),
            "std registry creation should succeed"))
    {
      return false;
    }

    auto &registry = result.value();

    const auto init_error = registry.initialize();
    const bool initialized = registry.initialized();

    const auto shutdown_error = registry.shutdown();

    return expect_true(
               !init_error,
               "registry initialize should succeed") &&
           expect_true(
               initialized,
               "registry should be initialized before shutdown") &&
           expect_true(
               !shutdown_error,
               "registry shutdown should succeed") &&
           expect_true(
               !registry.initialized(),
               "registry should not be initialized after shutdown") &&
           expect_true(
               registry.module_count() == 0,
               "registry should contain zero modules after shutdown") &&
           expect_true(
               registry.modules().empty(),
               "registry module list should be empty after shutdown");
  }

  [[nodiscard]] bool test_enabled_module_names_default()
  {
    auto result = kordex::standard::StdRegistry::create(
        kordex::standard::StdOptions::defaults());

    if (!expect_true(
            result_ok(result),
            "std registry creation should succeed"))
    {
      return false;
    }

    const auto names = result.value().enabled_module_names();

    return expect_true(
               names.size() == 8,
               "default registry should expose eight enabled names") &&
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
    auto result = kordex::standard::StdRegistry::create(
        kordex::standard::StdOptions::minimal());

    if (!expect_true(
            result_ok(result),
            "minimal std registry creation should succeed"))
    {
      return false;
    }

    const auto names = result.value().enabled_module_names();

    return expect_true(
               names.size() == 2,
               "minimal registry should expose two enabled names") &&
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

  [[nodiscard]] bool test_register_single_module()
  {
    auto options = kordex::standard::StdOptions::minimal();
    options.max_modules = 2;

    auto result = kordex::standard::StdRegistry::create(options);

    if (!expect_true(
            result_ok(result),
            "std registry creation should succeed"))
    {
      return false;
    }

    auto &registry = result.value();

    const auto register_error = registry.register_module("console");
    const auto duplicate_error = registry.register_module("kordex:console");

    return expect_true(
               !register_error,
               "registering console should succeed") &&
           expect_true(
               !duplicate_error,
               "registering duplicate console should be idempotent") &&
           expect_true(
               registry.module_count() == 1,
               "duplicate registration should not increase count") &&
           expect_true(
               registry.has_module("console"),
               "registry should contain console") &&
           expect_true(
               registry.has_module("kordex:console"),
               "registry should match prefixed console");
  }

  [[nodiscard]] bool test_register_all_minimal_modules()
  {
    auto result = kordex::standard::StdRegistry::create(
        kordex::standard::StdOptions::minimal());

    if (!expect_true(
            result_ok(result),
            "minimal std registry creation should succeed"))
    {
      return false;
    }

    auto &registry = result.value();

    const auto register_error = registry.register_all();

    return expect_true(
               !register_error,
               "register_all should succeed") &&
           expect_true(
               registry.module_count() == 2,
               "minimal register_all should register two modules") &&
           expect_true(
               registry.has_module("console"),
               "minimal registry should contain console") &&
           expect_true(
               registry.has_module("path"),
               "minimal registry should contain path") &&
           expect_true(
               !registry.has_module("fs"),
               "minimal registry should not contain fs");
  }

  [[nodiscard]] bool test_get_registered_module()
  {
    auto registry_result = kordex::standard::create_std_registry(
        kordex::standard::StdOptions::minimal());

    if (!expect_true(
            result_ok(registry_result),
            "initialized std registry creation should succeed"))
    {
      return false;
    }

    auto console = registry_result.value().module("console");
    auto path = registry_result.value().module("kordex:path");

    if (!expect_true(
            result_ok(console) && result_ok(path),
            "registered modules should be readable"))
    {
      return false;
    }

    return expect_true(
               ::std::string_view(console.value().name()) == "console",
               "registered console module name should match") &&
           expect_true(
               ::std::string_view(path.value().name()) == "path",
               "registered path module name should match") &&
           expect_true(
               console.value().has_function("log"),
               "registered console should expose log") &&
           expect_true(
               path.value().has_function("normalize"),
               "registered path should expose normalize");
  }

  [[nodiscard]] bool test_get_module_requires_initialization()
  {
    auto result = kordex::standard::StdRegistry::create(
        kordex::standard::StdOptions::minimal());

    if (!expect_true(
            result_ok(result),
            "std registry creation should succeed"))
    {
      return false;
    }

    auto module_result = result.value().module("console");

    return expect_true(
               !module_result,
               "reading module before initialization should fail") &&
           expect_true(
               module_result.error().code() ==
                   vix::error::ErrorCode::ConfigError,
               "uninitialized registry should map to config error");
  }

  [[nodiscard]] bool test_get_unknown_registered_module()
  {
    auto registry_result = kordex::standard::create_std_registry(
        kordex::standard::StdOptions::minimal());

    if (!expect_true(
            result_ok(registry_result),
            "initialized std registry creation should succeed"))
    {
      return false;
    }

    auto module_result = registry_result.value().module("fs");

    return expect_true(
               !module_result,
               "reading unregistered fs should fail") &&
           expect_true(
               module_result.error().code() ==
                   vix::error::ErrorCode::InvalidArgument,
               "unregistered module should map to invalid argument");
  }

  [[nodiscard]] bool test_create_std_registry_helper()
  {
    auto result = kordex::standard::create_std_registry(
        kordex::standard::StdOptions::minimal());

    if (!expect_true(
            result_ok(result),
            "create_std_registry should succeed"))
    {
      return false;
    }

    const auto &registry = result.value();

    return expect_true(
               registry.initialized(),
               "create_std_registry should initialize registry") &&
           expect_true(
               registry.module_count() == 2,
               "create_std_registry minimal should register two modules") &&
           expect_true(
               registry.has_module("console"),
               "create_std_registry should register console") &&
           expect_true(
               registry.has_module("path"),
               "create_std_registry should register path");
  }

  [[nodiscard]] bool test_install_into_module_registry()
  {
    auto std_registry_result = kordex::standard::create_std_registry(
        kordex::standard::StdOptions::minimal());

    if (!expect_true(
            result_ok(std_registry_result),
            "std registry creation should succeed"))
    {
      return false;
    }

    kordex::bindings::ModuleRegistry module_registry;

    const auto install_error =
        std_registry_result.value().install_into(module_registry);

    auto imported_console = module_registry.import_module("console");
    auto imported_path = module_registry.import_module("path");

    return expect_true(
               !install_error,
               "installing std modules into module registry should succeed") &&
           expect_true(
               binding_result_ok(imported_console),
               "installed console should be importable") &&
           expect_true(
               binding_result_ok(imported_path),
               "installed path should be importable") &&
           expect_true(
               ::std::string_view(imported_console.value().name()) == "console",
               "imported console name should match") &&
           expect_true(
               ::std::string_view(imported_path.value().name()) == "path",
               "imported path name should match");
  }

  [[nodiscard]] bool test_install_into_module_registry_helper()
  {
    kordex::bindings::ModuleRegistry module_registry;

    const auto install_error = kordex::standard::install_std_modules(
        module_registry,
        kordex::standard::StdOptions::minimal());

    auto imported_console = module_registry.import_module("console");

    return expect_true(
               !install_error,
               "install_std_modules helper should succeed for module registry") &&
           expect_true(
               binding_result_ok(imported_console),
               "installed console should be importable") &&
           expect_true(
               imported_console.value().has_function("log"),
               "installed console should expose log");
  }

  [[nodiscard]] bool test_install_into_engine_context()
  {
    auto std_registry_result = kordex::standard::create_std_registry(
        kordex::standard::StdOptions::minimal());

    if (!expect_true(
            result_ok(std_registry_result),
            "std registry creation should succeed"))
    {
      return false;
    }

    kordex::bindings::EngineContext context;

    const auto context_init = context.initialize();

    const auto install_error =
        std_registry_result.value().install_into(context);

    auto imported_console = context.import_module("console");
    auto imported_path = context.import_module("path");

    return expect_true(
               !context_init,
               "engine context initialization should succeed") &&
           expect_true(
               !install_error,
               "installing std modules into engine context should succeed") &&
           expect_true(
               binding_result_ok(imported_console),
               "context should import console") &&
           expect_true(
               binding_result_ok(imported_path),
               "context should import path") &&
           expect_true(
               ::std::string_view(imported_console.value().name()) == "console",
               "context console name should match") &&
           expect_true(
               ::std::string_view(imported_path.value().name()) == "path",
               "context path name should match");
  }

  [[nodiscard]] bool test_install_into_uninitialized_engine_context()
  {
    auto std_registry_result = kordex::standard::create_std_registry(
        kordex::standard::StdOptions::minimal());

    if (!expect_true(
            result_ok(std_registry_result),
            "std registry creation should succeed"))
    {
      return false;
    }

    kordex::bindings::EngineContext context;

    const auto install_error =
        std_registry_result.value().install_into(context);

    return expect_true(
               install_error.has_error(),
               "installing into uninitialized context should fail") &&
           expect_true(
               install_error.code() == vix::error::ErrorCode::ConfigError,
               "uninitialized context should map to config error");
  }

  [[nodiscard]] bool test_install_into_engine()
  {
    auto std_registry_result = kordex::standard::create_std_registry(
        kordex::standard::StdOptions::minimal());

    if (!expect_true(
            result_ok(std_registry_result),
            "std registry creation should succeed"))
    {
      return false;
    }

    auto engine_result = kordex::bindings::Engine::create(
        kordex::bindings::BindingOptions::defaults());

    if (!expect_true(
            binding_result_ok(engine_result),
            "engine creation should succeed"))
    {
      return false;
    }

    auto engine = ::std::move(engine_result.value());

    auto init_result = engine.initialize();

    const auto install_error =
        std_registry_result.value().install_into(engine);

    auto imported_console = engine.import_module("console");

    (void)engine.shutdown();

    return expect_true(
               init_result.succeeded(),
               "engine initialization should succeed") &&
           expect_true(
               !install_error,
               "installing std modules into engine should succeed") &&
           expect_true(
               binding_result_ok(imported_console),
               "engine should import console") &&
           expect_true(
               imported_console.value().has_function("log"),
               "engine console should expose log");
  }

  [[nodiscard]] bool test_install_into_uninitialized_engine()
  {
    auto std_registry_result = kordex::standard::create_std_registry(
        kordex::standard::StdOptions::minimal());

    if (!expect_true(
            result_ok(std_registry_result),
            "std registry creation should succeed"))
    {
      return false;
    }

    auto engine_result = kordex::bindings::Engine::create(
        kordex::bindings::BindingOptions::defaults());

    if (!expect_true(
            binding_result_ok(engine_result),
            "engine creation should succeed"))
    {
      return false;
    }

    auto engine = ::std::move(engine_result.value());

    const auto install_error =
        std_registry_result.value().install_into(engine);

    return expect_true(
               install_error.has_error(),
               "installing into uninitialized engine should fail") &&
           expect_true(
               install_error.code() == vix::error::ErrorCode::ConfigError,
               "uninitialized engine should map to config error");
  }

  [[nodiscard]] bool test_install_requires_initialized_registry()
  {
    auto registry_result = kordex::standard::StdRegistry::create(
        kordex::standard::StdOptions::minimal());

    if (!expect_true(
            result_ok(registry_result),
            "std registry creation should succeed"))
    {
      return false;
    }

    kordex::bindings::ModuleRegistry module_registry;

    const auto install_error =
        registry_result.value().install_into(module_registry);

    return expect_true(
               install_error.has_error(),
               "installing uninitialized registry should fail") &&
           expect_true(
               install_error.code() == vix::error::ErrorCode::ConfigError,
               "uninitialized registry install should map to config error");
  }

  [[nodiscard]] bool test_register_disabled_module_fails()
  {
    auto options = kordex::standard::StdOptions::minimal();

    auto result = kordex::standard::StdRegistry::create(options);

    if (!expect_true(
            result_ok(result),
            "minimal std registry creation should succeed"))
    {
      return false;
    }

    auto register_error = result.value().register_module("fs");

    return expect_true(
               register_error.has_error(),
               "registering disabled fs should fail") &&
           expect_true(
               register_error.code() ==
                   vix::error::ErrorCode::PermissionDenied,
               "disabled module should map to permission denied");
  }

  [[nodiscard]] bool test_register_unknown_module_fails()
  {
    auto result = kordex::standard::StdRegistry::create(
        kordex::standard::StdOptions::defaults());

    if (!expect_true(
            result_ok(result),
            "std registry creation should succeed"))
    {
      return false;
    }

    auto register_error = result.value().register_module("unknown");

    return expect_true(
               register_error.has_error(),
               "registering unknown module should fail") &&
           expect_true(
               register_error.code() ==
                   vix::error::ErrorCode::InvalidArgument,
               "unknown module should map to invalid argument");
  }

  [[nodiscard]] bool test_registry_max_modules_limit()
  {
    auto options = kordex::standard::StdOptions::minimal();
    options.max_modules = 1;

    auto registry_result = kordex::standard::StdRegistry::create(options);

    return expect_true(
               !registry_result,
               "registry creation should fail when enabled modules exceed max") &&
           expect_true(
               registry_result.error().code() ==
                   vix::error::ErrorCode::ConfigError,
               "max modules failure should map to config error");
  }

  [[nodiscard]] bool test_invalid_registry_options()
  {
    auto options = kordex::standard::StdOptions::defaults();
    options.namespace_name.clear();

    auto registry_result = kordex::standard::StdRegistry::create(options);

    return expect_true(
               !registry_result,
               "registry creation with empty namespace should fail") &&
           expect_true(
               registry_result.error().code() ==
                   vix::error::ErrorCode::ConfigError,
               "invalid registry options should map to config error");
  }

} // namespace

int main()
{
  const bool ok =
      test_registry_info_helpers() &&
      test_create_registry() &&
      test_initialize_default_registry() &&
      test_initialize_is_idempotent() &&
      test_shutdown_and_clear() &&
      test_enabled_module_names_default() &&
      test_enabled_module_names_minimal() &&
      test_register_single_module() &&
      test_register_all_minimal_modules() &&
      test_get_registered_module() &&
      test_get_module_requires_initialization() &&
      test_get_unknown_registered_module() &&
      test_create_std_registry_helper() &&
      test_install_into_module_registry() &&
      test_install_into_module_registry_helper() &&
      test_install_into_engine_context() &&
      test_install_into_uninitialized_engine_context() &&
      test_install_into_engine() &&
      test_install_into_uninitialized_engine() &&
      test_install_requires_initialized_registry() &&
      test_register_disabled_module_fails() &&
      test_register_unknown_module_fails() &&
      test_registry_max_modules_limit() &&
      test_invalid_registry_options();

  return ok ? 0 : 1;
}
