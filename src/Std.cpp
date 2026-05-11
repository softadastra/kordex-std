/**
 *
 *  @file Std.cpp
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

#include <kordex/std/Std.hpp>

namespace kordex::standard
{
  ::std::string_view package_name() noexcept
  {
    return "kordex-std";
  }

  ::std::string_view package_description() noexcept
  {
    return "Standard native modules for Kordex.";
  }

  ::std::string_view default_namespace() noexcept
  {
    return "kordex";
  }

  ::std::string_view default_registry_name() noexcept
  {
    return "kordex-std";
  }

  bool console_enabled() noexcept
  {
#if defined(KORDEX_STD_ENABLE_CONSOLE) && KORDEX_STD_ENABLE_CONSOLE
    return true;
#else
    return false;
#endif
  }

  bool fs_enabled() noexcept
  {
#if defined(KORDEX_STD_ENABLE_FS) && KORDEX_STD_ENABLE_FS
    return true;
#else
    return false;
#endif
  }

  bool path_enabled() noexcept
  {
#if defined(KORDEX_STD_ENABLE_PATH) && KORDEX_STD_ENABLE_PATH
    return true;
#else
    return false;
#endif
  }

  bool env_enabled() noexcept
  {
#if defined(KORDEX_STD_ENABLE_ENV) && KORDEX_STD_ENABLE_ENV
    return true;
#else
    return false;
#endif
  }

  bool process_enabled() noexcept
  {
#if defined(KORDEX_STD_ENABLE_PROCESS) && KORDEX_STD_ENABLE_PROCESS
    return true;
#else
    return false;
#endif
  }

  bool timer_enabled() noexcept
  {
#if defined(KORDEX_STD_ENABLE_TIMER) && KORDEX_STD_ENABLE_TIMER
    return true;
#else
    return false;
#endif
  }

  bool crypto_enabled() noexcept
  {
#if defined(KORDEX_STD_ENABLE_CRYPTO) && KORDEX_STD_ENABLE_CRYPTO
    return true;
#else
    return false;
#endif
  }

  bool http_enabled() noexcept
  {
#if defined(KORDEX_STD_ENABLE_HTTP) && KORDEX_STD_ENABLE_HTTP
    return true;
#else
    return false;
#endif
  }

  ::std::vector<::std::string> build_enabled_modules()
  {
    ::std::vector<::std::string> modules;

    if (console_enabled())
    {
      modules.push_back("console");
    }

    if (fs_enabled())
    {
      modules.push_back("fs");
    }

    if (path_enabled())
    {
      modules.push_back("path");
    }

    if (env_enabled())
    {
      modules.push_back("env");
    }

    if (process_enabled())
    {
      modules.push_back("process");
    }

    if (timer_enabled())
    {
      modules.push_back("timer");
    }

    if (crypto_enabled())
    {
      modules.push_back("crypto");
    }

    if (http_enabled())
    {
      modules.push_back("http");
    }

    return modules;
  }

  Result<ModuleFactory> create_factory()
  {
    return ModuleFactory::create();
  }

  Result<ModuleFactory> create_factory(
      const StdOptions &options)
  {
    return ModuleFactory::create(options);
  }

  Result<StdRegistry> create_registry()
  {
    return StdRegistry::create();
  }

  Result<StdRegistry> create_registry(
      const StdOptions &options)
  {
    return StdRegistry::create(options);
  }

  Result<StdRegistry> create_initialized_registry()
  {
    return create_std_registry();
  }

  Result<StdRegistry> create_initialized_registry(
      const StdOptions &options)
  {
    return create_std_registry(options);
  }

  Result<kordex::bindings::NativeModule> create_module(
      const ::std::string &name)
  {
    return create_std_module(name);
  }

  Result<kordex::bindings::NativeModule> create_module(
      const ::std::string &name,
      const StdOptions &options)
  {
    return create_std_module(name, options);
  }

  Result<::std::vector<kordex::bindings::NativeModule>> create_modules()
  {
    return create_std_modules();
  }

  Result<::std::vector<kordex::bindings::NativeModule>> create_modules(
      const StdOptions &options)
  {
    return create_std_modules(options);
  }

  Error install(
      kordex::bindings::ModuleRegistry &registry,
      const StdOptions &options)
  {
    return install_std_modules(registry, options);
  }

  Error install(
      kordex::bindings::EngineContext &context,
      const StdOptions &options)
  {
    return install_std_modules(context, options);
  }

  Error install(
      kordex::bindings::Engine &engine,
      const StdOptions &options)
  {
    return install_std_modules(engine, options);
  }

} // namespace kordex::standard
