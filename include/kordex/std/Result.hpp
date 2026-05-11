/**
 *
 *  @file Result.hpp
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

#ifndef KORDEX_STD_RESULT_HPP
#define KORDEX_STD_RESULT_HPP

#include <string>
#include <vix/error/Result.hpp>
#include <kordex/std/Error.hpp>

namespace kordex::standard
{
  /**
   * @brief Kordex standard module result type.
   */
  template <typename T>
  using Result = vix::error::Result<T>;

  /**
   * @brief Result type for boolean-producing std operations.
   */
  using BoolResult = Result<bool>;

  /**
   * @brief Result type for string-producing std operations.
   */
  using StringResult = Result<::std::string>;

  /**
   * @brief Result type for integer-producing std operations.
   */
  using IntResult = Result<int>;

} // namespace kordex::standard

#endif // KORDEX_STD_RESULT_HPP
