/**
 *
 *  @file timer.cpp
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
  auto module_result = kordex::standard::create_module("timer");
  if (!module_result)
  {
    std::cerr << "failed to create timer module: "
              << module_result.error().message()
              << '\n';
    return 1;
  }

  auto module = std::move(module_result.value());

  auto before_result = module.call("now", {});
  if (!before_result)
  {
    std::cerr << "timer.now failed: "
              << before_result.error().message()
              << '\n';
    return 1;
  }

  auto sleep_result = module.call(
      "sleep",
      {kordex::bindings::Value::number(10.0)});

  if (!sleep_result)
  {
    std::cerr << "timer.sleep failed: "
              << sleep_result.error().message()
              << '\n';
    return 1;
  }

  auto after_result = module.call("now", {});
  if (!after_result)
  {
    std::cerr << "timer.now failed: "
              << after_result.error().message()
              << '\n';
    return 1;
  }

  auto unix_result = module.call("unixMs", {});
  if (!unix_result)
  {
    std::cerr << "timer.unixMs failed: "
              << unix_result.error().message()
              << '\n';
    return 1;
  }

  auto before = before_result.value().as_number();
  auto after = after_result.value().as_number();
  auto unix_ms = unix_result.value().as_number();

  if (!before || !after || !unix_ms)
  {
    std::cerr << "failed to convert timer values\n";
    return 1;
  }

  std::cout << "module   = " << module.name() << '\n';
  std::cout << "before   = " << before.value() << " ms\n";
  std::cout << "after    = " << after.value() << " ms\n";
  std::cout << "elapsed  = " << (after.value() - before.value()) << " ms\n";
  std::cout << "unix ms  = " << unix_ms.value() << '\n';

  return 0;
}
