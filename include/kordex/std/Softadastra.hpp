/**
 *
 *  @file Softadastra.hpp
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

#ifndef KORDEX_STD_SOFTADASTRA_HPP
#define KORDEX_STD_SOFTADASTRA_HPP

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
   * @class Softadastra
   * @brief Kordex standard Softadastra module factory.
   *
   * Softadastra creates the native `softadastra` module exposed through
   * kordex::bindings.
   *
   * This module provides local-first durable storage and sync primitives
   * powered by the Softadastra C++ SDK.
   */
  class Softadastra
  {
  public:
    /**
     * @brief Construct Softadastra module factory with default config.
     */
    Softadastra();

    /**
     * @brief Construct Softadastra module factory with explicit config.
     */
    explicit Softadastra(StdConfig config);

    /**
     * @brief Create Softadastra from options.
     */
    [[nodiscard]] static Result<Softadastra> create(
        const StdOptions &options = StdOptions::defaults());

    /**
     * @brief Return the Softadastra config.
     */
    [[nodiscard]] const StdConfig &config() const noexcept;

    /**
     * @brief Return true if Softadastra module is enabled.
     */
    [[nodiscard]] bool enabled() const noexcept;

    /**
     * @brief Validate Softadastra configuration.
     */
    [[nodiscard]] Error validate() const;

    /**
     * @brief Create the native Softadastra module.
     */
    [[nodiscard]] Result<kordex::bindings::NativeModule> module() const;

    /**
     * @brief Open the Softadastra SDK client.
     *
     * Arguments:
     * - mode: durable | persistent | fast | local | memory
     * - node id
     * - WAL path
     */
    [[nodiscard]] static kordex::bindings::Result<kordex::bindings::Value> open(
        const kordex::bindings::NativeFunctionArguments &args);

    /**
     * @brief Close the Softadastra SDK client.
     */
    [[nodiscard]] static kordex::bindings::Result<kordex::bindings::Value> close(
        const kordex::bindings::NativeFunctionArguments &args);

    /**
     * @brief Return true if the Softadastra SDK client is open.
     */
    [[nodiscard]] static kordex::bindings::Result<kordex::bindings::Value> is_open(
        const kordex::bindings::NativeFunctionArguments &args);

    /**
     * @brief Store a string value by key.
     */
    [[nodiscard]] static kordex::bindings::Result<kordex::bindings::Value> put(
        const kordex::bindings::NativeFunctionArguments &args);

    /**
     * @brief Read a string value by key.
     */
    [[nodiscard]] static kordex::bindings::Result<kordex::bindings::Value> get(
        const kordex::bindings::NativeFunctionArguments &args);

    /**
     * @brief Remove a value by key.
     */
    [[nodiscard]] static kordex::bindings::Result<kordex::bindings::Value> remove(
        const kordex::bindings::NativeFunctionArguments &args);

    /**
     * @brief Return true if a key exists.
     */
    [[nodiscard]] static kordex::bindings::Result<kordex::bindings::Value> contains(
        const kordex::bindings::NativeFunctionArguments &args);

    /**
     * @brief Return local store size.
     */
    [[nodiscard]] static kordex::bindings::Result<kordex::bindings::Value> size(
        const kordex::bindings::NativeFunctionArguments &args);

    /**
     * @brief Run one Softadastra sync tick.
     */
    [[nodiscard]] static kordex::bindings::Result<kordex::bindings::Value> tick(
        const kordex::bindings::NativeFunctionArguments &args);

    /**
     * @brief Return the Softadastra sync state as JSON string.
     */
    [[nodiscard]] static kordex::bindings::Result<kordex::bindings::Value> sync_state(
        const kordex::bindings::NativeFunctionArguments &args);

    /**
     * @brief Return local node metadata as JSON string.
     */
    [[nodiscard]] static kordex::bindings::Result<kordex::bindings::Value> node_info(
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

    [[nodiscard]] static bool bool_argument_or(
        const kordex::bindings::NativeFunctionArguments &args,
        ::std::size_t index,
        bool fallback);

    StdConfig config_{};
  };

  /**
   * @brief Create the standard Softadastra native module.
   */
  [[nodiscard]] Result<kordex::bindings::NativeModule> create_softadastra_module(
      const StdOptions &options = StdOptions::defaults());

} // namespace kordex::standard

#endif // KORDEX_STD_SOFTADASTRA_HPP
