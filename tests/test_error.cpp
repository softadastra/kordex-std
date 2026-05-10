/**
 *
 *  @file test_error.cpp
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

#include <string_view>
#include <kordex/std/Error.hpp>

namespace
{
  [[nodiscard]] bool expect_true(
      bool condition,
      const char *message)
  {
    if (!condition)
    {
      (void)message;
      return false;
    }

    return true;
  }

  [[nodiscard]] bool test_ok_error()
  {
    const auto error = kordex::standard::ok();

    return expect_true(
        !error.has_error(),
        "ok error should not contain an error");
  }

  [[nodiscard]] bool test_make_std_error()
  {
    const auto error = kordex::standard::make_std_error(
        kordex::standard::StdErrorCode::InvalidArgument,
        "invalid value");

    return expect_true(
               error.has_error(),
               "std error should contain an error") &&
           expect_true(
               error.code() == vix::error::ErrorCode::InvalidArgument,
               "invalid argument should map to invalid argument") &&
           expect_true(
               ::std::string_view(error.message()) == "invalid value",
               "std error message should match");
  }

  [[nodiscard]] bool test_make_std_error_with_default_message()
  {
    const auto error = kordex::standard::make_std_error(
        kordex::standard::StdErrorCode::FsError);

    return expect_true(
               error.has_error(),
               "std error should contain an error") &&
           expect_true(
               error.code() == vix::error::ErrorCode::InternalError,
               "fs error should map to internal error") &&
           expect_true(
               ::std::string_view(error.message()) ==
                   "filesystem module error",
               "default fs error message should match");
  }

  [[nodiscard]] bool test_make_none_error()
  {
    const auto error = kordex::standard::make_std_error(
        kordex::standard::StdErrorCode::None,
        "ignored");

    return expect_true(
        !error.has_error(),
        "none error should return ok");
  }

  [[nodiscard]] bool test_error_code_mapping()
  {
    return expect_true(
               kordex::standard::to_error_code(
                   kordex::standard::StdErrorCode::None) ==
                   vix::error::ErrorCode::Ok,
               "none should map to ok") &&
           expect_true(
               kordex::standard::to_error_code(
                   kordex::standard::StdErrorCode::InvalidArgument) ==
                   vix::error::ErrorCode::InvalidArgument,
               "invalid argument should map to invalid argument") &&
           expect_true(
               kordex::standard::to_error_code(
                   kordex::standard::StdErrorCode::InvalidConfig) ==
                   vix::error::ErrorCode::ConfigError,
               "invalid config should map to config error") &&
           expect_true(
               kordex::standard::to_error_code(
                   kordex::standard::StdErrorCode::PermissionDenied) ==
                   vix::error::ErrorCode::PermissionDenied,
               "permission denied should map to permission denied") &&
           expect_true(
               kordex::standard::to_error_code(
                   kordex::standard::StdErrorCode::ModuleDisabled) ==
                   vix::error::ErrorCode::PermissionDenied,
               "module disabled should map to permission denied") &&
           expect_true(
               kordex::standard::to_error_code(
                   kordex::standard::StdErrorCode::InternalError) ==
                   vix::error::ErrorCode::InternalError,
               "internal error should map to internal error");
  }

  [[nodiscard]] bool test_error_code_strings()
  {
    return expect_true(
               ::std::string_view(kordex::standard::to_string(
                   kordex::standard::StdErrorCode::None)) == "none",
               "none string should match") &&
           expect_true(
               ::std::string_view(kordex::standard::to_string(
                   kordex::standard::StdErrorCode::InvalidArgument)) ==
                   "invalid_argument",
               "invalid argument string should match") &&
           expect_true(
               ::std::string_view(kordex::standard::to_string(
                   kordex::standard::StdErrorCode::InvalidConfig)) ==
                   "invalid_config",
               "invalid config string should match") &&
           expect_true(
               ::std::string_view(kordex::standard::to_string(
                   kordex::standard::StdErrorCode::PermissionDenied)) ==
                   "permission_denied",
               "permission denied string should match") &&
           expect_true(
               ::std::string_view(kordex::standard::to_string(
                   kordex::standard::StdErrorCode::ModuleDisabled)) ==
                   "module_disabled",
               "module disabled string should match") &&
           expect_true(
               ::std::string_view(kordex::standard::to_string(
                   kordex::standard::StdErrorCode::InternalError)) ==
                   "internal_error",
               "internal error string should match");
  }

  [[nodiscard]] bool test_default_messages()
  {
    return expect_true(
               kordex::standard::default_message(
                   kordex::standard::StdErrorCode::None) == "no error",
               "none default message should match") &&
           expect_true(
               kordex::standard::default_message(
                   kordex::standard::StdErrorCode::InvalidArgument) ==
                   "invalid std argument",
               "invalid argument default message should match") &&
           expect_true(
               kordex::standard::default_message(
                   kordex::standard::StdErrorCode::InvalidConfig) ==
                   "invalid std configuration",
               "invalid config default message should match") &&
           expect_true(
               kordex::standard::default_message(
                   kordex::standard::StdErrorCode::ConsoleError) ==
                   "console module error",
               "console default message should match") &&
           expect_true(
               kordex::standard::default_message(
                   kordex::standard::StdErrorCode::HttpError) ==
                   "http module error",
               "http default message should match");
  }
} // namespace

int main()
{
  const bool ok =
      test_ok_error() &&
      test_make_std_error() &&
      test_make_std_error_with_default_message() &&
      test_make_none_error() &&
      test_error_code_mapping() &&
      test_error_code_strings() &&
      test_default_messages();

  return ok ? 0 : 1;
}
