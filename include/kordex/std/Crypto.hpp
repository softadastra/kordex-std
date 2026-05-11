/**
 *
 *  @file Crypto.hpp
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

#ifndef KORDEX_STD_CRYPTO_HPP
#define KORDEX_STD_CRYPTO_HPP

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
   * @class Crypto
   * @brief Kordex standard crypto module factory.
   *
   * Crypto creates the native `crypto` module exposed through
   * kordex::bindings.
   *
   * This first version provides deterministic utility primitives.
   * Strong cryptographic backends can later be connected behind the same API.
   */
  class Crypto
  {
  public:
    /**
     * @brief Construct crypto module factory with default config.
     */
    Crypto();

    /**
     * @brief Construct crypto module factory with explicit config.
     */
    explicit Crypto(StdConfig config);

    /**
     * @brief Create crypto from options.
     */
    [[nodiscard]] static Result<Crypto> create(
        const StdOptions &options = StdOptions::defaults());

    /**
     * @brief Return the crypto config.
     */
    [[nodiscard]] const StdConfig &config() const noexcept;

    /**
     * @brief Return true if crypto module is enabled.
     */
    [[nodiscard]] bool enabled() const noexcept;

    /**
     * @brief Validate crypto configuration.
     */
    [[nodiscard]] Error validate() const;

    /**
     * @brief Create the native crypto module.
     */
    [[nodiscard]] Result<kordex::bindings::NativeModule> module() const;

    /**
     * @brief Return a deterministic hexadecimal hash for a string.
     */
    [[nodiscard]] static kordex::bindings::Result<kordex::bindings::Value> hash(
        const kordex::bindings::NativeFunctionArguments &args);

    /**
     * @brief Return a random number in the range [0, 1).
     */
    [[nodiscard]] static kordex::bindings::Result<kordex::bindings::Value> random(
        const kordex::bindings::NativeFunctionArguments &args);

    /**
     * @brief Return a random integer between min and max inclusive.
     */
    [[nodiscard]] static kordex::bindings::Result<kordex::bindings::Value> random_int(
        const kordex::bindings::NativeFunctionArguments &args);

    /**
     * @brief Return true when two strings are equal.
     */
    [[nodiscard]] static kordex::bindings::Result<kordex::bindings::Value> equals(
        const kordex::bindings::NativeFunctionArguments &args);

    /**
     * @brief Compute a stable FNV-1a hash in hexadecimal form.
     */
    [[nodiscard]] static ::std::string fnv1a_hex(
        const ::std::string &input);

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

    [[nodiscard]] static kordex::bindings::Result<double> number_argument(
        const kordex::bindings::NativeFunctionArguments &args,
        ::std::size_t index,
        const char *name);

    StdConfig config_{};
  };

  /**
   * @brief Create the standard crypto native module.
   */
  [[nodiscard]] Result<kordex::bindings::NativeModule> create_crypto_module(
      const StdOptions &options = StdOptions::defaults());

} // namespace kordex::standard

#endif // KORDEX_STD_CRYPTO_HPP
