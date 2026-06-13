/**
 *
 *  @file Error.cpp
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
#include <utility>
#include <kordex/std/Error.hpp>

namespace kordex::standard
{
  Error ok()
  {
    return Error{};
  }

  Error make_std_error(
      StdErrorCode code,
      std::string message)
  {
    if (code == StdErrorCode::None)
    {
      return ok();
    }

    if (message.empty())
    {
      message = std::string(default_message(code));
    }

    return Error(
        to_error_code(code),
        std::move(message));
  }

  vix::error::ErrorCode to_error_code(
      StdErrorCode code) noexcept
  {
    switch (code)
    {
    case StdErrorCode::None:
      return vix::error::ErrorCode::Ok;

    case StdErrorCode::InvalidArgument:
      return vix::error::ErrorCode::InvalidArgument;

    case StdErrorCode::InvalidConfig:
      return vix::error::ErrorCode::ConfigError;

    case StdErrorCode::PermissionDenied:
    case StdErrorCode::ModuleDisabled:
      return vix::error::ErrorCode::PermissionDenied;

    case StdErrorCode::ModuleRegistrationFailed:
    case StdErrorCode::ConsoleError:
    case StdErrorCode::FsError:
    case StdErrorCode::PathError:
    case StdErrorCode::EnvError:
    case StdErrorCode::ProcessError:
    case StdErrorCode::TimerError:
    case StdErrorCode::CryptoError:
    case StdErrorCode::HttpError:
    case StdErrorCode::SoftadastraError:
    case StdErrorCode::InternalError:
      return vix::error::ErrorCode::InternalError;
    }

    return vix::error::ErrorCode::InternalError;
  }

  const char *to_string(
      StdErrorCode code) noexcept
  {
    switch (code)
    {
    case StdErrorCode::None:
      return "none";
    case StdErrorCode::InvalidArgument:
      return "invalid_argument";
    case StdErrorCode::InvalidConfig:
      return "invalid_config";
    case StdErrorCode::PermissionDenied:
      return "permission_denied";
    case StdErrorCode::ModuleDisabled:
      return "module_disabled";
    case StdErrorCode::ModuleRegistrationFailed:
      return "module_registration_failed";
    case StdErrorCode::ConsoleError:
      return "console_error";
    case StdErrorCode::FsError:
      return "fs_error";
    case StdErrorCode::PathError:
      return "path_error";
    case StdErrorCode::EnvError:
      return "env_error";
    case StdErrorCode::ProcessError:
      return "process_error";
    case StdErrorCode::TimerError:
      return "timer_error";
    case StdErrorCode::CryptoError:
      return "crypto_error";
    case StdErrorCode::HttpError:
      return "http_error";
    case StdErrorCode::SoftadastraError:
      return "softadastra_error";
    case StdErrorCode::InternalError:
      return "internal_error";
    }

    return "internal_error";
  }

  std::string_view default_message(
      StdErrorCode code) noexcept
  {
    switch (code)
    {
    case StdErrorCode::None:
      return "no error";
    case StdErrorCode::InvalidArgument:
      return "invalid std argument";
    case StdErrorCode::InvalidConfig:
      return "invalid std configuration";
    case StdErrorCode::PermissionDenied:
      return "std permission denied";
    case StdErrorCode::ModuleDisabled:
      return "std module is disabled";
    case StdErrorCode::ModuleRegistrationFailed:
      return "std module registration failed";
    case StdErrorCode::ConsoleError:
      return "console module error";
    case StdErrorCode::FsError:
      return "filesystem module error";
    case StdErrorCode::PathError:
      return "path module error";
    case StdErrorCode::EnvError:
      return "environment module error";
    case StdErrorCode::ProcessError:
      return "process module error";
    case StdErrorCode::TimerError:
      return "timer module error";
    case StdErrorCode::CryptoError:
      return "crypto module error";
    case StdErrorCode::HttpError:
      return "http module error";
    case StdErrorCode::SoftadastraError:
      return "softadastra module error";
    case StdErrorCode::InternalError:
      return "internal std error";
    }

    return "internal std error";
  }

} // namespace kordex::standard
