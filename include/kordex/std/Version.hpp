/**
 *
 *  @file Version.hpp
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

#ifndef KORDEX_STD_VERSION_HPP
#define KORDEX_STD_VERSION_HPP

#include <string_view>

namespace kordex::standard
{
  inline constexpr int KORDEX_STD_VERSION_MAJOR_VALUE = 0;
  inline constexpr int KORDEX_STD_VERSION_MINOR_VALUE = 1;
  inline constexpr int KORDEX_STD_VERSION_PATCH_VALUE = 0;

  inline constexpr ::std::string_view KORDEX_STD_VERSION = "0.1.0";

  [[nodiscard]] constexpr int version_major() noexcept
  {
    return KORDEX_STD_VERSION_MAJOR_VALUE;
  }

  [[nodiscard]] constexpr int version_minor() noexcept
  {
    return KORDEX_STD_VERSION_MINOR_VALUE;
  }

  [[nodiscard]] constexpr int version_patch() noexcept
  {
    return KORDEX_STD_VERSION_PATCH_VALUE;
  }

  [[nodiscard]] constexpr ::std::string_view version() noexcept
  {
    return KORDEX_STD_VERSION;
  }

} // namespace kordex::standard

#endif // KORDEX_STD_VERSION_HPP
