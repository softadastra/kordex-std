/**
 *
 *  @file StdOptions.hpp
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

#ifndef KORDEX_STD_STD_OPTIONS_HPP
#define KORDEX_STD_STD_OPTIONS_HPP

#include <cstddef>
#include <string>
#include <vector>

namespace kordex::standard
{
  /**
   * @struct StdOptions
   * @brief User-facing options for Kordex standard modules.
   */
  struct StdOptions
  {
    /**
     * @brief Optional registry name.
     */
    ::std::string registry_name{"kordex-std"};

    /**
     * @brief Optional module namespace.
     *
     * Example:
     * - kordex
     * - kordex.std
     */
    ::std::string namespace_name{"kordex"};

    /**
     * @brief Extra module search paths used by higher-level integrations.
     */
    ::std::vector<::std::string> module_paths{};

    /**
     * @brief Whether standard modules are globally enabled.
     */
    bool enabled{true};

    /**
     * @brief Whether modules are considered safe by default.
     */
    bool safe_by_default{true};

    /**
     * @brief Whether native module registration is allowed.
     */
    bool allow_native_modules{true};

    /**
     * @brief Whether native function registration is allowed.
     */
    bool allow_native_functions{true};

    /**
     * @brief Whether console module is enabled.
     */
    bool enable_console{true};

    /**
     * @brief Whether filesystem module is enabled.
     */
    bool enable_fs{true};

    /**
     * @brief Whether path module is enabled.
     */
    bool enable_path{true};

    /**
     * @brief Whether environment module is enabled.
     */
    bool enable_env{true};

    /**
     * @brief Whether process module is enabled.
     */
    bool enable_process{true};

    /**
     * @brief Whether timer module is enabled.
     */
    bool enable_timer{true};

    /**
     * @brief Whether crypto module is enabled.
     */
    bool enable_crypto{true};

    /**
     * @brief Whether HTTP module is enabled.
     */
    bool enable_http{true};

    /**
     * @brief Optional maximum module count.
     *
     * Zero means unlimited.
     */
    ::std::size_t max_modules{0};

    /**
     * @brief Return default std options.
     */
    [[nodiscard]] static StdOptions defaults();

    /**
     * @brief Return development std options.
     */
    [[nodiscard]] static StdOptions development();

    /**
     * @brief Return production std options.
     */
    [[nodiscard]] static StdOptions production();

    /**
     * @brief Return test std options.
     */
    [[nodiscard]] static StdOptions test();

    /**
     * @brief Return minimal std options.
     */
    [[nodiscard]] static StdOptions minimal();

    /**
     * @brief Return true if a registry name is configured.
     */
    [[nodiscard]] bool has_registry_name() const noexcept;

    /**
     * @brief Return true if a namespace name is configured.
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
     * @brief Return true if at least one standard module is enabled.
     */
    [[nodiscard]] bool has_enabled_modules() const noexcept;
  };

} // namespace kordex::standard

#endif // KORDEX_STD_STD_OPTIONS_HPP
