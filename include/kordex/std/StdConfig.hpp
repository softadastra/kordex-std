/**
 *
 *  @file StdConfig.hpp
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

#ifndef KORDEX_STD_STD_CONFIG_HPP
#define KORDEX_STD_STD_CONFIG_HPP

#include <cstddef>
#include <string>
#include <vector>

#include <kordex/std/Error.hpp>
#include <kordex/std/Result.hpp>
#include <kordex/std/StdOptions.hpp>

namespace kordex::standard
{
  /**
   * @struct StdConfig
   * @brief Final normalized configuration used by Kordex standard modules.
   */
  struct StdConfig
  {
    /**
     * @brief Registry name.
     */
    ::std::string registry_name{"kordex-std"};

    /**
     * @brief Namespace used by standard modules.
     */
    ::std::string namespace_name{"kordex"};

    /**
     * @brief Extra module search paths.
     */
    ::std::vector<::std::string> module_paths{};

    /**
     * @brief Whether the whole standard library layer is enabled.
     */
    bool enabled{true};

    /**
     * @brief Whether generated native modules are safe by default.
     */
    bool safe_by_default{true};

    /**
     * @brief Whether native modules can be created and registered.
     */
    bool allow_native_modules{true};

    /**
     * @brief Whether native functions can be created and registered.
     */
    bool allow_native_functions{true};

    bool enable_console{true};
    bool enable_fs{true};
    bool enable_path{true};
    bool enable_env{true};
    bool enable_process{true};
    bool enable_timer{true};
    bool enable_crypto{true};
    bool enable_http{true};
    bool enable_softadastra{false};

    /**
     * @brief Maximum number of modules allowed in a registry.
     *
     * Zero means unlimited.
     */
    ::std::size_t max_modules{0};

    /**
     * @brief Build a normalized config from options.
     */
    [[nodiscard]] static Result<StdConfig> from_options(
        const StdOptions &options);

    /**
     * @brief Build a normalized config from environment.
     *
     * Environment support is intentionally minimal for now.
     */
    [[nodiscard]] static Result<StdConfig> from_environment(
        const StdOptions &base = StdOptions::defaults());

    /**
     * @brief Return true if the registry name is configured.
     */
    [[nodiscard]] bool has_registry_name() const noexcept;

    /**
     * @brief Return true if the namespace name is configured.
     */
    [[nodiscard]] bool has_namespace_name() const noexcept;

    /**
     * @brief Return true if module paths are configured.
     */
    [[nodiscard]] bool has_module_paths() const noexcept;

    /**
     * @brief Return true if a maximum module count is configured.
     */
    [[nodiscard]] bool has_max_modules() const noexcept;

    /**
     * @brief Return true if at least one module is enabled.
     */
    [[nodiscard]] bool has_enabled_modules() const noexcept;

    /**
     * @brief Return true if the named standard module is enabled.
     */
    [[nodiscard]] bool module_enabled(
        const ::std::string &name) const noexcept;

    /**
     * @brief Return the number of enabled modules.
     */
    [[nodiscard]] ::std::size_t enabled_module_count() const noexcept;

    /**
     * @brief Validate the final standard module configuration.
     */
    [[nodiscard]] Error validate() const;
  };

} // namespace kordex::standard

#endif // KORDEX_STD_STD_CONFIG_HPP
