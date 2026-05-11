/**
 *
 *  @file console.cpp
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

#include <iostream>

#include <kordex/std/Std.hpp>

int main()
{
  auto module_result = kordex::standard::create_module("console");
  if (!module_result)
  {
    std::cerr << "failed to create console module: "
              << module_result.error().message()
              << '\n';
    return 1;
  }

  auto module = std::move(module_result.value());

  std::cout << "module = " << module.name() << '\n';

  kordex::bindings::NativeFunctionArguments args{
      kordex::bindings::Value::string("Hello from"),
      kordex::bindings::Value::string("kordex:console")};

  auto call_result = module.call("log", args);
  if (!call_result)
  {
    std::cerr << "console.log failed: "
              << call_result.error().message()
              << '\n';
    return 1;
  }

  return 0;
}
