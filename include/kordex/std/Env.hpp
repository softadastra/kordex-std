/**
 *
 *  @file Env.hpp
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

#ifndef KORDEX_STD_ENV_HPP
#define KORDEX_STD_ENV_HPP

#include <string>

#include <kordex/bindings/NativeFunction.hpp>
#include <kordex/bindings/NativeModule.hpp>
#include <kordex/bindings/Value.hpp>

#include <kordex/std/Error.hpp>
#include <kordex/std/Result.hpp>
#include <kordex/std/StdConfig.hpp>
#include <kordex/std/StdOptions.hpp>

namespace kordex::standard
{
  /**
   * @class Env
   * @brief Kordex standard environment module factory.
   *
   * Env creates the native `env` module exposed through kordex::bindings.
   */
  class Env
  {
  public:
    /**
     * @brief Construct env module factory with default config.
     */
    Env();

    /**
     * @brief Construct env module factory with explicit config.
     */
    explicit Env(StdConfig config);

    /**
     * @brief Create env from options.
     */
    [[nodiscard]] static Result<Env> create(
        const StdOptions &options = StdOptions::defaults());

    /**
     * @brief Return the env config.
     */
    [[nodiscard]] const StdConfig &config() const noexcept;

    /**
     * @brief Return true if env module is enabled.
     */
    [[nodiscard]] bool enabled() const noexcept;

    /**
     * @brief Validate env configuration.
     */
    [[nodiscard]] Error validate() const;

    /**
     * @brief Create the native env module.
     */
    [[nodiscard]] Result<kordex::bindings::NativeModule> module() const;

    /**
     * @brief Return the value of an environment variable.
     */
    [[nodiscard]] static kordex::bindings::Result<kordex::bindings::Value> get(
        const kordex::bindings::NativeFunctionArguments &args);

    /**
     * @brief Return true if an environment variable exists.
     */
    [[nodiscard]] static kordex::bindings::Result<kordex::bindings::Value> has(
        const kordex::bindings::NativeFunctionArguments &args);

    /**
     * @brief Set an environment variable.
     */
    [[nodiscard]] static kordex::bindings::Result<kordex::bindings::Value> set(
        const kordex::bindings::NativeFunctionArguments &args);

    /**
     * @brief Remove an environment variable.
     */
    [[nodiscard]] static kordex::bindings::Result<kordex::bindings::Value> unset(
        const kordex::bindings::NativeFunctionArguments &args);

  private:
    [[nodiscard]] static kordex::bindings::NativeFunction make_function(
        ::std::string name,
        ::std::string description,
        ::std::size_t min_args,
        ::std::size_t max_args,
        kordex::bindings::NativeFunctionCallback callback);

    [[nodiscard]] static kordex::bindings::Result<::std::string> string_argument(
        const kordex::bindings::NativeFunctionArguments &args,
        ::std::size_t index,
        const char *name);

    StdConfig config_{};
  };

  /**
   * @brief Create the standard env native module.
   */
  [[nodiscard]] Result<kordex::bindings::NativeModule> create_env_module(
      const StdOptions &options = StdOptions::defaults());

} // namespace kordex::standard

#endif // KORDEX_STD_ENV_HPP
