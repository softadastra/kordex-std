/**
 *
 *  @file ModuleFactory.hpp
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

#ifndef KORDEX_STD_MODULE_FACTORY_HPP
#define KORDEX_STD_MODULE_FACTORY_HPP

#include <string>
#include <vector>

#include <kordex/bindings/NativeModule.hpp>

#include <kordex/std/Console.hpp>
#include <kordex/std/Crypto.hpp>
#include <kordex/std/Env.hpp>
#include <kordex/std/Error.hpp>
#include <kordex/std/Fs.hpp>
#include <kordex/std/Http.hpp>
#include <kordex/std/Path.hpp>
#include <kordex/std/Process.hpp>
#include <kordex/std/Result.hpp>
#include <kordex/std/StdConfig.hpp>
#include <kordex/std/StdOptions.hpp>
#include <kordex/std/Timer.hpp>

namespace kordex::standard
{
  /**
   * @class ModuleFactory
   * @brief Factory for creating Kordex standard native modules.
   *
   * ModuleFactory centralizes the creation of all standard modules.
   * It respects StdConfig and only creates modules enabled by configuration.
   */
  class ModuleFactory
  {
  public:
    /**
     * @brief Construct factory with default config.
     */
    ModuleFactory();

    /**
     * @brief Construct factory with explicit config.
     */
    explicit ModuleFactory(StdConfig config);

    /**
     * @brief Create factory from options.
     */
    [[nodiscard]] static Result<ModuleFactory> create(
        const StdOptions &options = StdOptions::defaults());

    /**
     * @brief Return the factory config.
     */
    [[nodiscard]] const StdConfig &config() const noexcept;

    /**
     * @brief Return true if the standard layer is enabled.
     */
    [[nodiscard]] bool enabled() const noexcept;

    /**
     * @brief Validate factory configuration.
     */
    [[nodiscard]] Error validate() const;

    /**
     * @brief Return true if a module can be created.
     */
    [[nodiscard]] bool can_create(
        const ::std::string &name) const noexcept;

    /**
     * @brief Create one native module by name.
     *
     * Accepted names:
     * - console / kordex:console
     * - fs / kordex:fs
     * - path / kordex:path
     * - env / kordex:env
     * - process / kordex:process
     * - timer / kordex:timer
     * - crypto / kordex:crypto
     * - http / kordex:http
     */
    [[nodiscard]] Result<kordex::bindings::NativeModule> create_module(
        const ::std::string &name) const;

    /**
     * @brief Create all enabled standard modules.
     */
    [[nodiscard]] Result<::std::vector<kordex::bindings::NativeModule>> create_all() const;

    /**
     * @brief Return enabled module names.
     */
    [[nodiscard]] ::std::vector<::std::string> enabled_module_names() const;

    /**
     * @brief Return canonical module name without the kordex: prefix.
     */
    [[nodiscard]] static ::std::string canonical_name(
        const ::std::string &name);

    /**
     * @brief Return true if the name is a known standard module.
     */
    [[nodiscard]] static bool known_module(
        const ::std::string &name) noexcept;

  private:
    StdConfig config_{};
  };

  /**
   * @brief Create one standard native module by name.
   */
  [[nodiscard]] Result<kordex::bindings::NativeModule> create_std_module(
      const ::std::string &name,
      const StdOptions &options = StdOptions::defaults());

  /**
   * @brief Create all enabled standard native modules.
   */
  [[nodiscard]] Result<::std::vector<kordex::bindings::NativeModule>> create_std_modules(
      const StdOptions &options = StdOptions::defaults());

} // namespace kordex::standard

#endif // KORDEX_STD_MODULE_FACTORY_HPP
