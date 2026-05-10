/**
 *
 *  @file Fs.hpp
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

#ifndef KORDEX_STD_FS_HPP
#define KORDEX_STD_FS_HPP

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
   * @class Fs
   * @brief Kordex standard filesystem module factory.
   *
   * Fs creates the native `fs` module exposed through kordex::bindings.
   */
  class Fs
  {
  public:
    /**
     * @brief Construct fs module factory with default config.
     */
    Fs();

    /**
     * @brief Construct fs module factory with explicit config.
     */
    explicit Fs(StdConfig config);

    /**
     * @brief Create fs from options.
     */
    [[nodiscard]] static Result<Fs> create(
        const StdOptions &options = StdOptions::defaults());

    /**
     * @brief Return the fs config.
     */
    [[nodiscard]] const StdConfig &config() const noexcept;

    /**
     * @brief Return true if fs module is enabled.
     */
    [[nodiscard]] bool enabled() const noexcept;

    /**
     * @brief Validate fs configuration.
     */
    [[nodiscard]] Error validate() const;

    /**
     * @brief Create the native fs module.
     */
    [[nodiscard]] Result<kordex::bindings::NativeModule> module() const;

    /**
     * @brief Return true if a path exists.
     */
    [[nodiscard]] static kordex::bindings::Result<kordex::bindings::Value> exists(
        const kordex::bindings::NativeFunctionArguments &args);

    /**
     * @brief Return true if a path is a regular file.
     */
    [[nodiscard]] static kordex::bindings::Result<kordex::bindings::Value> is_file(
        const kordex::bindings::NativeFunctionArguments &args);

    /**
     * @brief Return true if a path is a directory.
     */
    [[nodiscard]] static kordex::bindings::Result<kordex::bindings::Value> is_directory(
        const kordex::bindings::NativeFunctionArguments &args);

    /**
     * @brief Read a text file.
     */
    [[nodiscard]] static kordex::bindings::Result<kordex::bindings::Value> read_text(
        const kordex::bindings::NativeFunctionArguments &args);

    /**
     * @brief Write a text file.
     */
    [[nodiscard]] static kordex::bindings::Result<kordex::bindings::Value> write_text(
        const kordex::bindings::NativeFunctionArguments &args);

    /**
     * @brief Remove a file or empty directory.
     */
    [[nodiscard]] static kordex::bindings::Result<kordex::bindings::Value> remove(
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
   * @brief Create the standard fs native module.
   */
  [[nodiscard]] Result<kordex::bindings::NativeModule> create_fs_module(
      const StdOptions &options = StdOptions::defaults());

} // namespace kordex::standard

#endif // KORDEX_STD_FS_HPP
