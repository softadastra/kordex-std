/**
 *
 *  @file process.cpp
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
  auto module_result = kordex::standard::create_module("process");
  if (!module_result)
  {
    std::cerr << "failed to create process module: "
              << module_result.error().message()
              << '\n';
    return 1;
  }

  auto module = std::move(module_result.value());

  auto cwd_result = module.call("cwd", {});
  if (!cwd_result)
  {
    std::cerr << "process.cwd failed: "
              << cwd_result.error().message()
              << '\n';
    return 1;
  }

  auto cwd = cwd_result.value().as_string();
  if (!cwd)
  {
    std::cerr << "failed to convert cwd value: "
              << cwd.error().message()
              << '\n';
    return 1;
  }

#if defined(_WIN32)
  const char *command = "cd";
#else
  const char *command = "true";
#endif

  auto run_result = module.call(
      "run",
      {kordex::bindings::Value::string(command)});

  if (!run_result)
  {
    std::cerr << "process.run failed: "
              << run_result.error().message()
              << '\n';
    return 1;
  }

  auto exit_code = run_result.value().as_number();
  if (!exit_code)
  {
    std::cerr << "failed to convert exit code: "
              << exit_code.error().message()
              << '\n';
    return 1;
  }

  std::cout << "module    = " << module.name() << '\n';
  std::cout << "cwd       = " << cwd.value() << '\n';
  std::cout << "command   = " << command << '\n';
  std::cout << "exit code = " << exit_code.value() << '\n';

  return 0;
}
