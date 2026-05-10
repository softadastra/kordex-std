/**
 *
 *  @file Std.hpp
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

#ifndef KORDEX_STD_STD_HPP
#define KORDEX_STD_STD_HPP

#include <string_view>
#include <vector>

#include <kordex/bindings/Engine.hpp>
#include <kordex/bindings/EngineContext.hpp>
#include <kordex/bindings/ModuleRegistry.hpp>
#include <kordex/bindings/NativeModule.hpp>

#include <kordex/std/Console.hpp>
#include <kordex/std/Crypto.hpp>
#include <kordex/std/Env.hpp>
#include <kordex/std/Error.hpp>
#include <kordex/std/Fs.hpp>
#include <kordex/std/Http.hpp>
#include <kordex/std/ModuleFactory.hpp>
#include <kordex/std/Path.hpp>
#include <kordex/std/Process.hpp>
#include <kordex/std/Result.hpp>
#include <kordex/std/StdConfig.hpp>
#include <kordex/std/StdOptions.hpp>
#include <kordex/std/StdRegistry.hpp>
#include <kordex/std/Timer.hpp>
#include <kordex/std/Version.hpp>

namespace kordex::standard
{
  /**
   * @brief Return the public Kordex std package name.
   */
  [[nodiscard]] ::std::string_view package_name() noexcept;

  /**
   * @brief Return the public Kordex std package description.
   */
  [[nodiscard]] ::std::string_view package_description() noexcept;

  /**
   * @brief Return the default standard module namespace.
   */
  [[nodiscard]] ::std::string_view default_namespace() noexcept;

  /**
   * @brief Return the default standard registry name.
   */
  [[nodiscard]] ::std::string_view default_registry_name() noexcept;

  /**
   * @brief Return true if console support is enabled in this build.
   */
  [[nodiscard]] bool console_enabled() noexcept;

  /**
   * @brief Return true if filesystem support is enabled in this build.
   */
  [[nodiscard]] bool fs_enabled() noexcept;

  /**
   * @brief Return true if path support is enabled in this build.
   */
  [[nodiscard]] bool path_enabled() noexcept;

  /**
   * @brief Return true if environment support is enabled in this build.
   */
  [[nodiscard]] bool env_enabled() noexcept;

  /**
   * @brief Return true if process support is enabled in this build.
   */
  [[nodiscard]] bool process_enabled() noexcept;

  /**
   * @brief Return true if timer support is enabled in this build.
   */
  [[nodiscard]] bool timer_enabled() noexcept;

  /**
   * @brief Return true if crypto support is enabled in this build.
   */
  [[nodiscard]] bool crypto_enabled() noexcept;

  /**
   * @brief Return true if HTTP support is enabled in this build.
   */
  [[nodiscard]] bool http_enabled() noexcept;

  /**
   * @brief Return the list of standard modules enabled at build time.
   */
  [[nodiscard]] ::std::vector<::std::string> build_enabled_modules();

  /**
   * @brief Create a standard module factory with default options.
   */
  [[nodiscard]] Result<ModuleFactory> create_factory();

  /**
   * @brief Create a standard module factory from options.
   */
  [[nodiscard]] Result<ModuleFactory> create_factory(
      const StdOptions &options);

  /**
   * @brief Create a standard registry with default options.
   */
  [[nodiscard]] Result<StdRegistry> create_registry();

  /**
   * @brief Create a standard registry from options.
   */
  [[nodiscard]] Result<StdRegistry> create_registry(
      const StdOptions &options);

  /**
   * @brief Create and initialize a standard registry with default options.
   */
  [[nodiscard]] Result<StdRegistry> create_initialized_registry();

  /**
   * @brief Create and initialize a standard registry from options.
   */
  [[nodiscard]] Result<StdRegistry> create_initialized_registry(
      const StdOptions &options);

  /**
   * @brief Create one standard module using default options.
   */
  [[nodiscard]] Result<kordex::bindings::NativeModule> create_module(
      const ::std::string &name);

  /**
   * @brief Create one standard module from options.
   */
  [[nodiscard]] Result<kordex::bindings::NativeModule> create_module(
      const ::std::string &name,
      const StdOptions &options);

  /**
   * @brief Create all enabled standard modules using default options.
   */
  [[nodiscard]] Result<::std::vector<kordex::bindings::NativeModule>> create_modules();

  /**
   * @brief Create all enabled standard modules from options.
   */
  [[nodiscard]] Result<::std::vector<kordex::bindings::NativeModule>> create_modules(
      const StdOptions &options);

  /**
   * @brief Install standard modules into a bindings ModuleRegistry.
   */
  [[nodiscard]] Error install(
      kordex::bindings::ModuleRegistry &registry,
      const StdOptions &options = StdOptions::defaults());

  /**
   * @brief Install standard modules into a bindings EngineContext.
   */
  [[nodiscard]] Error install(
      kordex::bindings::EngineContext &context,
      const StdOptions &options = StdOptions::defaults());

  /**
   * @brief Install standard modules into a bindings Engine.
   */
  [[nodiscard]] Error install(
      kordex::bindings::Engine &engine,
      const StdOptions &options = StdOptions::defaults());

} // namespace kordex::standard

#endif // KORDEX_STD_STD_HPP
