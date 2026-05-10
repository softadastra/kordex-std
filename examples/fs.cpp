/**
 *
 *  @file fs.cpp
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

#include <filesystem>
#include <iostream>

#include <kordex/std/Std.hpp>

int main()
{
  auto module_result = kordex::standard::create_module("fs");
  if (!module_result)
  {
    std::cerr << "failed to create fs module: "
              << module_result.error().message()
              << '\n';
    return 1;
  }

  auto module = std::move(module_result.value());

  const auto file_path =
      std::filesystem::temp_directory_path() / "kordex_std_example.txt";

  kordex::bindings::NativeFunctionArguments write_args{
      kordex::bindings::Value::string(file_path.string()),
      kordex::bindings::Value::string("Hello from kordex:fs")};

  auto write_result = module.call("writeText", write_args);
  if (!write_result)
  {
    std::cerr << "fs.writeText failed: "
              << write_result.error().message()
              << '\n';
    return 1;
  }

  kordex::bindings::NativeFunctionArguments path_args{
      kordex::bindings::Value::string(file_path.string())};

  auto exists_result = module.call("exists", path_args);
  if (!exists_result)
  {
    std::cerr << "fs.exists failed: "
              << exists_result.error().message()
              << '\n';
    return 1;
  }

  auto read_result = module.call("readText", path_args);
  if (!read_result)
  {
    std::cerr << "fs.readText failed: "
              << read_result.error().message()
              << '\n';
    return 1;
  }

  auto exists = exists_result.value().as_boolean();
  if (!exists)
  {
    std::cerr << "failed to read exists value: "
              << exists.error().message()
              << '\n';
    return 1;
  }

  auto text = read_result.value().as_string();
  if (!text)
  {
    std::cerr << "failed to read text value: "
              << text.error().message()
              << '\n';
    return 1;
  }

  std::cout << "module = " << module.name() << '\n';
  std::cout << "exists = " << (exists.value() ? "yes" : "no") << '\n';
  std::cout << "text   = " << text.value() << '\n';

  auto remove_result = module.call("remove", path_args);
  if (!remove_result)
  {
    std::cerr << "fs.remove failed: "
              << remove_result.error().message()
              << '\n';
    return 1;
  }

  return 0;
}
