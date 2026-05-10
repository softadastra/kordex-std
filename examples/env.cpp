/**
 *
 *  @file env.cpp
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
  auto module_result = kordex::standard::create_module("env");
  if (!module_result)
  {
    std::cerr << "failed to create env module: "
              << module_result.error().message()
              << '\n';
    return 1;
  }

  auto module = std::move(module_result.value());

  constexpr const char *key = "KORDEX_STD_EXAMPLE_VALUE";

  auto set_result = module.call(
      "set",
      {kordex::bindings::Value::string(key),
       kordex::bindings::Value::string("hello-env")});

  if (!set_result)
  {
    std::cerr << "env.set failed: "
              << set_result.error().message()
              << '\n';
    return 1;
  }

  auto has_result = module.call(
      "has",
      {kordex::bindings::Value::string(key)});

  if (!has_result)
  {
    std::cerr << "env.has failed: "
              << has_result.error().message()
              << '\n';
    return 1;
  }

  auto get_result = module.call(
      "get",
      {kordex::bindings::Value::string(key)});

  if (!get_result)
  {
    std::cerr << "env.get failed: "
              << get_result.error().message()
              << '\n';
    return 1;
  }

  auto has_value = has_result.value().as_boolean();
  auto text = get_result.value().as_string();

  if (!has_value || !text)
  {
    std::cerr << "failed to convert env values\n";
    return 1;
  }

  std::cout << "module = " << module.name() << '\n';
  std::cout << "has    = " << (has_value.value() ? "yes" : "no") << '\n';
  std::cout << "value  = " << text.value() << '\n';

  auto unset_result = module.call(
      "unset",
      {kordex::bindings::Value::string(key)});

  if (!unset_result)
  {
    std::cerr << "env.unset failed: "
              << unset_result.error().message()
              << '\n';
    return 1;
  }

  return 0;
}
