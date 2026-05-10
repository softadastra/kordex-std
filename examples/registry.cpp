/**
 *
 *  @file registry.cpp
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
  auto registry_result = kordex::standard::create_initialized_registry(
      kordex::standard::StdOptions::minimal());

  if (!registry_result)
  {
    std::cerr << "failed to create std registry: "
              << registry_result.error().message()
              << '\n';
    return 1;
  }

  auto registry = std::move(registry_result.value());

  std::cout << "registry  = " << registry.name() << '\n';
  std::cout << "namespace = " << registry.namespace_name() << '\n';
  std::cout << "modules   = " << registry.module_count() << '\n';

  for (const auto &module : registry.modules())
  {
    std::cout << "- " << module.name()
              << " functions=" << module.function_count()
              << " values=" << module.value_count()
              << '\n';
  }

  kordex::bindings::ModuleRegistry module_registry;

  const auto install_error = registry.install_into(module_registry);
  if (install_error)
  {
    std::cerr << "failed to install std modules: "
              << install_error.message()
              << '\n';
    return 1;
  }

  auto console = module_registry.import_module("console");
  if (!console)
  {
    std::cerr << "failed to import console module: "
              << console.error().message()
              << '\n';
    return 1;
  }

  auto call_result = console.value().call(
      "log",
      {kordex::bindings::Value::string("Hello from installed std registry")});

  if (!call_result)
  {
    std::cerr << "console.log failed: "
              << call_result.error().message()
              << '\n';
    return 1;
  }

  return 0;
}
