/**
 *
 *  @file Http.hpp
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

#ifndef KORDEX_STD_HTTP_HPP
#define KORDEX_STD_HTTP_HPP

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
   * @class Http
   * @brief Kordex standard HTTP module factory.
   *
   * Http creates the native `http` module exposed through
   * kordex::bindings.
   *
   * This first version provides safe HTTP utility helpers.
   * Real network requests can later be connected behind this same module.
   */
  class Http
  {
  public:
    /**
     * @brief Construct http module factory with default config.
     */
    Http();

    /**
     * @brief Construct http module factory with explicit config.
     */
    explicit Http(StdConfig config);

    /**
     * @brief Create http from options.
     */
    [[nodiscard]] static Result<Http> create(
        const StdOptions &options = StdOptions::defaults());

    /**
     * @brief Return the http config.
     */
    [[nodiscard]] const StdConfig &config() const noexcept;

    /**
     * @brief Return true if http module is enabled.
     */
    [[nodiscard]] bool enabled() const noexcept;

    /**
     * @brief Validate http configuration.
     */
    [[nodiscard]] Error validate() const;

    /**
     * @brief Create the native http module.
     */
    [[nodiscard]] Result<kordex::bindings::NativeModule> module() const;

    /**
     * @brief Return true if an HTTP status code is successful.
     */
    [[nodiscard]] static kordex::bindings::Result<kordex::bindings::Value> is_success(
        const kordex::bindings::NativeFunctionArguments &args);

    /**
     * @brief Return true if an HTTP status code is a redirect.
     */
    [[nodiscard]] static kordex::bindings::Result<kordex::bindings::Value> is_redirect(
        const kordex::bindings::NativeFunctionArguments &args);

    /**
     * @brief Return true if an HTTP status code is a client error.
     */
    [[nodiscard]] static kordex::bindings::Result<kordex::bindings::Value> is_client_error(
        const kordex::bindings::NativeFunctionArguments &args);

    /**
     * @brief Return true if an HTTP status code is a server error.
     */
    [[nodiscard]] static kordex::bindings::Result<kordex::bindings::Value> is_server_error(
        const kordex::bindings::NativeFunctionArguments &args);

    /**
     * @brief Return the standard reason phrase for an HTTP status code.
     */
    [[nodiscard]] static kordex::bindings::Result<kordex::bindings::Value> status_text(
        const kordex::bindings::NativeFunctionArguments &args);

    /**
     * @brief Build a URL from base and path.
     */
    [[nodiscard]] static kordex::bindings::Result<kordex::bindings::Value> build_url(
        const kordex::bindings::NativeFunctionArguments &args);

    /**
     * @brief Normalize an HTTP method.
     */
    [[nodiscard]] static kordex::bindings::Result<kordex::bindings::Value> normalize_method(
        const kordex::bindings::NativeFunctionArguments &args);

    /**
     * @brief Return true if a string is a supported HTTP method.
     */
    [[nodiscard]] static kordex::bindings::Result<kordex::bindings::Value> is_method(
        const kordex::bindings::NativeFunctionArguments &args);

    /**
     * @brief Return a reason phrase for a status code.
     */
    [[nodiscard]] static const char *reason_phrase(
        int status_code) noexcept;

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

    [[nodiscard]] static ::std::string uppercase(
        ::std::string value);

    StdConfig config_{};
  };

  /**
   * @brief Create the standard http native module.
   */
  [[nodiscard]] Result<kordex::bindings::NativeModule> create_http_module(
      const StdOptions &options = StdOptions::defaults());

} // namespace kordex::standard

#endif // KORDEX_STD_HTTP_HPP
