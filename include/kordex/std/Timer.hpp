/**
 *
 *  @file Timer.hpp
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

#ifndef KORDEX_STD_TIMER_HPP
#define KORDEX_STD_TIMER_HPP

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
   * @class Timer
   * @brief Kordex standard timer module factory.
   *
   * Timer creates the native `timer` module exposed through
   * kordex::bindings.
   */
  class Timer
  {
  public:
    /**
     * @brief Construct timer module factory with default config.
     */
    Timer();

    /**
     * @brief Construct timer module factory with explicit config.
     */
    explicit Timer(StdConfig config);

    /**
     * @brief Create timer from options.
     */
    [[nodiscard]] static Result<Timer> create(
        const StdOptions &options = StdOptions::defaults());

    /**
     * @brief Return the timer config.
     */
    [[nodiscard]] const StdConfig &config() const noexcept;

    /**
     * @brief Return true if timer module is enabled.
     */
    [[nodiscard]] bool enabled() const noexcept;

    /**
     * @brief Validate timer configuration.
     */
    [[nodiscard]] Error validate() const;

    /**
     * @brief Create the native timer module.
     */
    [[nodiscard]] Result<kordex::bindings::NativeModule> module() const;

    /**
     * @brief Return current monotonic timestamp in milliseconds.
     */
    [[nodiscard]] static kordex::bindings::Result<kordex::bindings::Value> now(
        const kordex::bindings::NativeFunctionArguments &args);

    /**
     * @brief Sleep synchronously for a number of milliseconds.
     */
    [[nodiscard]] static kordex::bindings::Result<kordex::bindings::Value> sleep(
        const kordex::bindings::NativeFunctionArguments &args);

    /**
     * @brief Return the current UNIX timestamp in milliseconds.
     */
    [[nodiscard]] static kordex::bindings::Result<kordex::bindings::Value> unix_ms(
        const kordex::bindings::NativeFunctionArguments &args);

  private:
    [[nodiscard]] static kordex::bindings::NativeFunction make_function(
        ::std::string name,
        ::std::string description,
        ::std::size_t min_args,
        ::std::size_t max_args,
        kordex::bindings::NativeFunctionCallback callback);

    [[nodiscard]] static kordex::bindings::Result<double> number_argument(
        const kordex::bindings::NativeFunctionArguments &args,
        ::std::size_t index,
        const char *name);

    StdConfig config_{};
  };

  /**
   * @brief Create the standard timer native module.
   */
  [[nodiscard]] Result<kordex::bindings::NativeModule> create_timer_module(
      const StdOptions &options = StdOptions::defaults());

} // namespace kordex::standard

#endif // KORDEX_STD_TIMER_HPP
