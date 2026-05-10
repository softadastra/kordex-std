/**
 *
 *  @file Process.hpp
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

#ifndef KORDEX_STD_PROCESS_HPP
#define KORDEX_STD_PROCESS_HPP

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
   * @class Process
   * @brief Kordex standard process module factory.
   *
   * Process creates the native `process` module exposed through
   * kordex::bindings.
   */
  class Process
  {
  public:
    /**
     * @brief Construct process module factory with default config.
     */
    Process();

    /**
     * @brief Construct process module factory with explicit config.
     */
    explicit Process(StdConfig config);

    /**
     * @brief Create process from options.
     */
    [[nodiscard]] static Result<Process> create(
        const StdOptions &options = StdOptions::defaults());

    /**
     * @brief Return the process config.
     */
    [[nodiscard]] const StdConfig &config() const noexcept;

    /**
     * @brief Return true if process module is enabled.
     */
    [[nodiscard]] bool enabled() const noexcept;

    /**
     * @brief Validate process configuration.
     */
    [[nodiscard]] Error validate() const;

    /**
     * @brief Create the native process module.
     */
    [[nodiscard]] Result<kordex::bindings::NativeModule> module() const;

    /**
     * @brief Return current working directory.
     */
    [[nodiscard]] static kordex::bindings::Result<kordex::bindings::Value> cwd(
        const kordex::bindings::NativeFunctionArguments &args);

    /**
     * @brief Change current working directory.
     */
    [[nodiscard]] static kordex::bindings::Result<kordex::bindings::Value> chdir(
        const kordex::bindings::NativeFunctionArguments &args);

    /**
     * @brief Run a shell command and return its exit code.
     */
    [[nodiscard]] static kordex::bindings::Result<kordex::bindings::Value> run(
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
   * @brief Create the standard process native module.
   */
  [[nodiscard]] Result<kordex::bindings::NativeModule> create_process_module(
      const StdOptions &options = StdOptions::defaults());

} // namespace kordex::standard

#endif // KORDEX_STD_PROCESS_HPP
