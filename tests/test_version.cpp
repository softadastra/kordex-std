/**
 *
 *  @file test_version.cpp
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
#include <kordex/std/Version.hpp>

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

  [[nodiscard]] bool test_version_numbers()
  {
    return expect_true(
               kordex::standard::version_major() == 0,
               "major version should be 0") &&
           expect_true(
               kordex::standard::version_minor() == 1,
               "minor version should be 1") &&
           expect_true(
               kordex::standard::version_patch() == 0,
               "patch version should be 0");
  }

  [[nodiscard]] bool test_version_string()
  {
    return expect_true(
        kordex::standard::version() == ::std::string_view("0.1.0"),
        "version string should be 0.1.0");
  }

  [[nodiscard]] bool test_version_constants()
  {
    return expect_true(
               kordex::standard::KORDEX_STD_VERSION_MAJOR_VALUE == 0,
               "major version constant should be 0") &&
           expect_true(
               kordex::standard::KORDEX_STD_VERSION_MINOR_VALUE == 1,
               "minor version constant should be 1") &&
           expect_true(
               kordex::standard::KORDEX_STD_VERSION_PATCH_VALUE == 0,
               "patch version constant should be 0") &&
           expect_true(
               kordex::standard::KORDEX_STD_VERSION ==
                   ::std::string_view("0.1.0"),
               "version constant should be 0.1.0");
  }
} // namespace

int main()
{
  const bool ok =
      test_version_numbers() &&
      test_version_string() &&
      test_version_constants();

  return ok ? 0 : 1;
}
