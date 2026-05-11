/**
 *
 *  @file Path.hpp
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

#ifndef KORDEX_STD_PATH_HPP
#define KORDEX_STD_PATH_HPP

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
   * @class Path
   * @brief Kordex standard path module factory.
   *
   * Path creates the native `path` module exposed through kordex::bindings.
   */
  class Path
  {
  public:
    /**
     * @brief Construct path module factory with default config.
     */
    Path();

    /**
     * @brief Construct path module factory with explicit config.
     */
    explicit Path(StdConfig config);

    /**
     * @brief Create path from options.
     */
    [[nodiscard]] static Result<Path> create(
        const StdOptions &options = StdOptions::defaults());

    /**
     * @brief Return the path config.
     */
    [[nodiscard]] const StdConfig &config() const noexcept;

    /**
     * @brief Return true if path module is enabled.
     */
    [[nodiscard]] bool enabled() const noexcept;

    /**
     * @brief Validate path configuration.
     */
    [[nodiscard]] Error validate() const;

    /**
     * @brief Create the native path module.
     */
    [[nodiscard]] Result<kordex::bindings::NativeModule> module() const;

    /**
     * @brief Normalize a path lexically.
     */
    [[nodiscard]] static kordex::bindings::Result<kordex::bindings::Value> normalize(
        const kordex::bindings::NativeFunctionArguments &args);

    /**
     * @brief Join path segments.
     */
    [[nodiscard]] static kordex::bindings::Result<kordex::bindings::Value> join(
        const kordex::bindings::NativeFunctionArguments &args);

    /**
     * @brief Return the parent directory of a path.
     */
    [[nodiscard]] static kordex::bindings::Result<kordex::bindings::Value> dirname(
        const kordex::bindings::NativeFunctionArguments &args);

    /**
     * @brief Return the filename part of a path.
     */
    [[nodiscard]] static kordex::bindings::Result<kordex::bindings::Value> basename(
        const kordex::bindings::NativeFunctionArguments &args);

    /**
     * @brief Return the extension part of a path.
     */
    [[nodiscard]] static kordex::bindings::Result<kordex::bindings::Value> extension(
        const kordex::bindings::NativeFunctionArguments &args);

    /**
     * @brief Return true if a path is absolute.
     */
    [[nodiscard]] static kordex::bindings::Result<kordex::bindings::Value> is_absolute(
        const kordex::bindings::NativeFunctionArguments &args);

    /**
     * @brief Return true if a path is relative.
     */
    [[nodiscard]] static kordex::bindings::Result<kordex::bindings::Value> is_relative(
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
   * @brief Create the standard path native module.
   */
  [[nodiscard]] Result<kordex::bindings::NativeModule> create_path_module(
      const StdOptions &options = StdOptions::defaults());

} // namespace kordex::standard

#endif // KORDEX_STD_PATH_HPP
