/**
 *
 *  @file Error.hpp
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

#ifndef KORDEX_STD_ERROR_HPP
#define KORDEX_STD_ERROR_HPP

#include <string>
#include <string_view>

#include <vix/error/Error.hpp>

namespace kordex::standard
{
  /**
   * @enum StdErrorCode
   * @brief Error codes used by the Kordex standard modules.
   */
  enum class StdErrorCode
  {
    None,
    InvalidArgument,
    InvalidConfig,
    PermissionDenied,
    ModuleDisabled,
    ModuleRegistrationFailed,
    ConsoleError,
    FsError,
    PathError,
    EnvError,
    ProcessError,
    TimerError,
    CryptoError,
    HttpError,
    InternalError
  };

  /**
   * @brief Shared error type used by Kordex std.
   */
  using Error = vix::error::Error;

  /**
   * @brief Return a successful std error value.
   */
  [[nodiscard]] Error ok();

  /**
   * @brief Create a Kordex std error.
   */
  [[nodiscard]] Error make_std_error(
      StdErrorCode code,
      std::string message = {});

  /**
   * @brief Convert StdErrorCode to the shared Vix error code.
   */
  [[nodiscard]] vix::error::ErrorCode to_error_code(
      StdErrorCode code) noexcept;

  /**
   * @brief Convert StdErrorCode to a stable string.
   */
  [[nodiscard]] const char *to_string(
      StdErrorCode code) noexcept;

  /**
   * @brief Return the default message for a std error code.
   */
  [[nodiscard]] std::string_view default_message(
      StdErrorCode code) noexcept;

} // namespace kordex::standard

#endif // KORDEX_STD_ERROR_HPP
