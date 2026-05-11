/**
 *
 *  @file Console.hpp
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

#ifndef KORDEX_STD_CONSOLE_HPP
#define KORDEX_STD_CONSOLE_HPP

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
   * @enum ConsoleLevel
   * @brief Console output level.
   */
  enum class ConsoleLevel
  {
    Log,
    Info,
    Warn,
    Error,
    Debug
  };

  /**
   * @class Console
   * @brief Kordex standard console module factory.
   *
   * Console creates the native `console` module exposed through
   * kordex::bindings.
   */
  class Console
  {
  public:
    /**
     * @brief Construct console module factory with default config.
     */
    Console();

    /**
     * @brief Construct console module factory with explicit config.
     */
    explicit Console(StdConfig config);

    /**
     * @brief Create console from options.
     */
    [[nodiscard]] static Result<Console> create(
        const StdOptions &options = StdOptions::defaults());

    /**
     * @brief Return the console config.
     */
    [[nodiscard]] const StdConfig &config() const noexcept;

    /**
     * @brief Return true if console module is enabled.
     */
    [[nodiscard]] bool enabled() const noexcept;

    /**
     * @brief Validate console configuration.
     */
    [[nodiscard]] Error validate() const;

    /**
     * @brief Create the native console module.
     */
    [[nodiscard]] Result<kordex::bindings::NativeModule> module() const;

    /**
     * @brief Write console arguments using a level.
     */
    [[nodiscard]] static kordex::bindings::Result<kordex::bindings::Value> write(
        ConsoleLevel level,
        const kordex::bindings::NativeFunctionArguments &args);

    /**
     * @brief Format console arguments into one string.
     */
    [[nodiscard]] static ::std::string format_arguments(
        const kordex::bindings::NativeFunctionArguments &args);

    /**
     * @brief Convert console level to stable string.
     */
    [[nodiscard]] static const char *to_string(
        ConsoleLevel level) noexcept;

  private:
    [[nodiscard]] static kordex::bindings::NativeFunction make_function(
        ConsoleLevel level,
        ::std::string name);

    StdConfig config_{};
  };

  /**
   * @brief Create the standard console native module.
   */
  [[nodiscard]] Result<kordex::bindings::NativeModule> create_console_module(
      const StdOptions &options = StdOptions::defaults());

  /**
   * @brief Convert console level to stable string.
   */
  [[nodiscard]] const char *to_string(
      ConsoleLevel level) noexcept;

} // namespace kordex::standard

#endif // KORDEX_STD_CONSOLE_HPP
