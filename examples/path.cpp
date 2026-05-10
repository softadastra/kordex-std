/**
 *
 *  @file path.cpp
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

#include <iostream>
#include <kordex/std/Std.hpp>

int main()
{
  auto module_result = kordex::standard::create_module("path");
  if (!module_result)
  {
    std::cerr << "failed to create path module: "
              << module_result.error().message()
              << '\n';
    return 1;
  }

  auto module = std::move(module_result.value());

  auto joined_result = module.call(
      "join",
      {kordex::bindings::Value::string("app"),
       kordex::bindings::Value::string("src"),
       kordex::bindings::Value::string("main.js")});

  if (!joined_result)
  {
    std::cerr << "path.join failed: "
              << joined_result.error().message()
              << '\n';
    return 1;
  }

  auto normalized_result = module.call(
      "normalize",
      {kordex::bindings::Value::string("app/./src/../main.js")});

  if (!normalized_result)
  {
    std::cerr << "path.normalize failed: "
              << normalized_result.error().message()
              << '\n';
    return 1;
  }

  auto dirname_result = module.call(
      "dirname",
      {kordex::bindings::Value::string("app/src/main.js")});

  if (!dirname_result)
  {
    std::cerr << "path.dirname failed: "
              << dirname_result.error().message()
              << '\n';
    return 1;
  }

  auto joined = joined_result.value().as_string();
  auto normalized = normalized_result.value().as_string();
  auto dirname = dirname_result.value().as_string();

  if (!joined || !normalized || !dirname)
  {
    std::cerr << "failed to convert path result to string\n";
    return 1;
  }

  std::cout << "module     = " << module.name() << '\n';
  std::cout << "join       = " << joined.value() << '\n';
  std::cout << "normalize  = " << normalized.value() << '\n';
  std::cout << "dirname    = " << dirname.value() << '\n';

  return 0;
}
