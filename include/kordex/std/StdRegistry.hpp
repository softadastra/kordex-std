/**
 *
 *  @file StdRegistry.hpp
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

#ifndef KORDEX_STD_STD_REGISTRY_HPP
#define KORDEX_STD_STD_REGISTRY_HPP

#include <string>
#include <vector>

#include <kordex/bindings/Engine.hpp>
#include <kordex/bindings/EngineContext.hpp>
#include <kordex/bindings/ModuleRegistry.hpp>
#include <kordex/bindings/NativeModule.hpp>

#include <kordex/std/Error.hpp>
#include <kordex/std/ModuleFactory.hpp>
#include <kordex/std/Result.hpp>
#include <kordex/std/StdConfig.hpp>
#include <kordex/std/StdOptions.hpp>

namespace kordex::standard
{
  /**
   * @struct StdRegistryInfo
   * @brief Metadata describing a standard module registry.
   */
  struct StdRegistryInfo
  {
    /**
     * @brief Registry name.
     */
    ::std::string name{"kordex-std"};

    /**
     * @brief Namespace used by registered modules.
     */
    ::std::string namespace_name{"kordex"};

    /**
     * @brief Whether the registry has been initialized.
     */
    bool initialized{false};

    /**
     * @brief Number of registered standard modules.
     */
    ::std::size_t module_count{0};

    /**
     * @brief Return true if registry has a name.
     */
    [[nodiscard]] bool has_name() const noexcept;

    /**
     * @brief Return true if registry has a namespace name.
     */
    [[nodiscard]] bool has_namespace_name() const noexcept;
  };

  /**
   * @class StdRegistry
   * @brief Registry manager for Kordex standard native modules.
   *
   * StdRegistry creates and installs standard modules into:
   * - a bindings ModuleRegistry
   * - a bindings EngineContext
   * - a bindings Engine
   */
  class StdRegistry
  {
  public:
    /**
     * @brief Construct registry with default config.
     */
    StdRegistry();

    /**
     * @brief Construct registry with explicit config.
     */
    explicit StdRegistry(StdConfig config);

    /**
     * @brief Create registry from options.
     */
    [[nodiscard]] static Result<StdRegistry> create(
        const StdOptions &options = StdOptions::defaults());

    /**
     * @brief Return registry metadata.
     */
    [[nodiscard]] const StdRegistryInfo &info() const noexcept;

    /**
     * @brief Return registry name.
     */
    [[nodiscard]] const ::std::string &name() const noexcept;

    /**
     * @brief Return registry namespace name.
     */
    [[nodiscard]] const ::std::string &namespace_name() const noexcept;

    /**
     * @brief Return registry config.
     */
    [[nodiscard]] const StdConfig &config() const noexcept;

    /**
     * @brief Return true if registry is initialized.
     */
    [[nodiscard]] bool initialized() const noexcept;

    /**
     * @brief Return true if std layer is enabled.
     */
    [[nodiscard]] bool enabled() const noexcept;

    /**
     * @brief Return number of registered modules.
     */
    [[nodiscard]] ::std::size_t module_count() const noexcept;

    /**
     * @brief Validate registry configuration.
     */
    [[nodiscard]] Error validate() const;

    /**
     * @brief Initialize the registry by creating enabled modules.
     */
    [[nodiscard]] Error initialize();

    /**
     * @brief Shutdown the registry and clear modules.
     */
    [[nodiscard]] Error shutdown();

    /**
     * @brief Clear registered standard modules.
     */
    void clear();

    /**
     * @brief Return all registered native modules.
     */
    [[nodiscard]] const ::std::vector<kordex::bindings::NativeModule> &modules() const noexcept;

    /**
     * @brief Return enabled module names.
     */
    [[nodiscard]] ::std::vector<::std::string> enabled_module_names() const;

    /**
     * @brief Return true if a module is registered.
     */
    [[nodiscard]] bool has_module(
        const ::std::string &name) const noexcept;

    /**
     * @brief Return a registered module by name.
     */
    [[nodiscard]] Result<kordex::bindings::NativeModule> module(
        const ::std::string &name) const;

    /**
     * @brief Register one enabled standard module by name.
     */
    [[nodiscard]] Error register_module(
        const ::std::string &name);

    /**
     * @brief Register all enabled standard modules.
     */
    [[nodiscard]] Error register_all();

    /**
     * @brief Install registered modules into a bindings ModuleRegistry.
     */
    [[nodiscard]] Error install_into(
        kordex::bindings::ModuleRegistry &registry) const;

    /**
     * @brief Install registered modules into a bindings EngineContext.
     */
    [[nodiscard]] Error install_into(
        kordex::bindings::EngineContext &context) const;

    /**
     * @brief Install registered modules into a bindings Engine.
     */
    [[nodiscard]] Error install_into(
        kordex::bindings::Engine &engine) const;

  private:
    [[nodiscard]] Error require_initialized() const;

    [[nodiscard]] Error ensure_capacity_for_one() const;

    [[nodiscard]] static bool module_name_matches(
        const kordex::bindings::NativeModule &module,
        const ::std::string &name) noexcept;

    StdConfig config_{};
    ModuleFactory factory_{};
    StdRegistryInfo info_{};
    ::std::vector<kordex::bindings::NativeModule> modules_{};
  };

  /**
   * @brief Create and initialize a standard registry.
   */
  [[nodiscard]] Result<StdRegistry> create_std_registry(
      const StdOptions &options = StdOptions::defaults());

  /**
   * @brief Install standard modules into a bindings module registry.
   */
  [[nodiscard]] Error install_std_modules(
      kordex::bindings::ModuleRegistry &registry,
      const StdOptions &options = StdOptions::defaults());

  /**
   * @brief Install standard modules into an engine context.
   */
  [[nodiscard]] Error install_std_modules(
      kordex::bindings::EngineContext &context,
      const StdOptions &options = StdOptions::defaults());

  /**
   * @brief Install standard modules into an engine.
   */
  [[nodiscard]] Error install_std_modules(
      kordex::bindings::Engine &engine,
      const StdOptions &options = StdOptions::defaults());

} // namespace kordex::standard

#endif // KORDEX_STD_STD_REGISTRY_HPP
