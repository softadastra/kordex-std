/**
 *
 *  @file Fs.cpp
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
#include <fstream>
#include <sstream>
#include <utility>

#include <kordex/std/Fs.hpp>

namespace kordex::standard
{
  namespace
  {
    [[nodiscard]] kordex::bindings::Error make_fs_binding_error(
        const ::std::string &message)
    {
      return kordex::bindings::make_binding_error(
          kordex::bindings::BindingErrorCode::FunctionCallFailed,
          message);
    }
  } // namespace

  Fs::Fs()
      : config_()
  {
  }

  Fs::Fs(StdConfig config)
      : config_(::std::move(config))
  {
  }

  Result<Fs> Fs::create(
      const StdOptions &options)
  {
    auto config = StdConfig::from_options(options);
    if (!config)
    {
      return config.error();
    }

    Fs fs(config.value());

    const auto validation = fs.validate();
    if (validation)
    {
      return validation;
    }

    return fs;
  }

  const StdConfig &Fs::config() const noexcept
  {
    return config_;
  }

  bool Fs::enabled() const noexcept
  {
    return config_.module_enabled("fs");
  }

  Error Fs::validate() const
  {
    const auto config_validation = config_.validate();
    if (config_validation)
    {
      return config_validation;
    }

    if (!enabled())
    {
      return make_std_error(
          StdErrorCode::ModuleDisabled,
          "fs module is disabled");
    }

    if (!config_.allow_native_modules)
    {
      return make_std_error(
          StdErrorCode::InvalidConfig,
          "fs module requires native modules");
    }

    if (!config_.allow_native_functions)
    {
      return make_std_error(
          StdErrorCode::InvalidConfig,
          "fs module requires native functions");
    }

    return ok();
  }

  Result<kordex::bindings::NativeModule> Fs::module() const
  {
    const auto validation = validate();
    if (validation)
    {
      return validation;
    }

    kordex::bindings::NativeModuleInfo info;
    info.name = "fs";
    info.namespace_name = config_.namespace_name;
    info.description = "Kordex standard filesystem module";
    info.importable = true;
    info.safe = config_.safe_by_default;

    auto module = kordex::bindings::NativeModule::create(info);

    const auto name_error = module.set_value(
        "name",
        kordex::bindings::Value::string("fs"));

    if (name_error)
    {
      return make_std_error(
          StdErrorCode::ModuleRegistrationFailed,
          ::std::string(name_error.message()));
    }

    const auto namespace_error = module.set_value(
        "namespace",
        kordex::bindings::Value::string(config_.namespace_name));

    if (namespace_error)
    {
      return make_std_error(
          StdErrorCode::ModuleRegistrationFailed,
          ::std::string(namespace_error.message()));
    }

    const auto exists_error = module.add_function(
        make_function(
            "exists",
            "Return true if a path exists",
            1,
            1,
            &Fs::exists));

    if (exists_error)
    {
      return make_std_error(
          StdErrorCode::ModuleRegistrationFailed,
          ::std::string(exists_error.message()));
    }

    const auto is_file_error = module.add_function(
        make_function(
            "isFile",
            "Return true if a path is a file",
            1,
            1,
            &Fs::is_file));

    if (is_file_error)
    {
      return make_std_error(
          StdErrorCode::ModuleRegistrationFailed,
          ::std::string(is_file_error.message()));
    }

    const auto is_directory_error = module.add_function(
        make_function(
            "isDirectory",
            "Return true if a path is a directory",
            1,
            1,
            &Fs::is_directory));

    if (is_directory_error)
    {
      return make_std_error(
          StdErrorCode::ModuleRegistrationFailed,
          ::std::string(is_directory_error.message()));
    }

    const auto read_text_error = module.add_function(
        make_function(
            "readText",
            "Read a text file",
            1,
            1,
            &Fs::read_text));

    if (read_text_error)
    {
      return make_std_error(
          StdErrorCode::ModuleRegistrationFailed,
          ::std::string(read_text_error.message()));
    }

    const auto write_text_error = module.add_function(
        make_function(
            "writeText",
            "Write a text file",
            2,
            2,
            &Fs::write_text));

    if (write_text_error)
    {
      return make_std_error(
          StdErrorCode::ModuleRegistrationFailed,
          ::std::string(write_text_error.message()));
    }

    const auto remove_error = module.add_function(
        make_function(
            "remove",
            "Remove a file or empty directory",
            1,
            1,
            &Fs::remove));

    if (remove_error)
    {
      return make_std_error(
          StdErrorCode::ModuleRegistrationFailed,
          ::std::string(remove_error.message()));
    }

    return module;
  }

  kordex::bindings::Result<kordex::bindings::Value> Fs::exists(
      const kordex::bindings::NativeFunctionArguments &args)
  {
    auto path = string_argument(args, 0, "path");
    if (!path)
    {
      return path.error();
    }

    try
    {
      return kordex::bindings::Value::boolean(
          ::std::filesystem::exists(path.value()));
    }
    catch (const ::std::exception &exception)
    {
      return make_fs_binding_error(exception.what());
    }
  }

  kordex::bindings::Result<kordex::bindings::Value> Fs::is_file(
      const kordex::bindings::NativeFunctionArguments &args)
  {
    auto path = string_argument(args, 0, "path");
    if (!path)
    {
      return path.error();
    }

    try
    {
      return kordex::bindings::Value::boolean(
          ::std::filesystem::is_regular_file(path.value()));
    }
    catch (const ::std::exception &exception)
    {
      return make_fs_binding_error(exception.what());
    }
  }

  kordex::bindings::Result<kordex::bindings::Value> Fs::is_directory(
      const kordex::bindings::NativeFunctionArguments &args)
  {
    auto path = string_argument(args, 0, "path");
    if (!path)
    {
      return path.error();
    }

    try
    {
      return kordex::bindings::Value::boolean(
          ::std::filesystem::is_directory(path.value()));
    }
    catch (const ::std::exception &exception)
    {
      return make_fs_binding_error(exception.what());
    }
  }

  kordex::bindings::Result<kordex::bindings::Value> Fs::read_text(
      const kordex::bindings::NativeFunctionArguments &args)
  {
    auto path = string_argument(args, 0, "path");
    if (!path)
    {
      return path.error();
    }

    ::std::ifstream file(path.value(), ::std::ios::in | ::std::ios::binary);
    if (!file)
    {
      return make_fs_binding_error(
          "failed to open file for reading: " + path.value());
    }

    ::std::ostringstream buffer;
    buffer << file.rdbuf();

    if (file.bad())
    {
      return make_fs_binding_error(
          "failed to read file: " + path.value());
    }

    return kordex::bindings::Value::string(buffer.str());
  }

  kordex::bindings::Result<kordex::bindings::Value> Fs::write_text(
      const kordex::bindings::NativeFunctionArguments &args)
  {
    auto path = string_argument(args, 0, "path");
    if (!path)
    {
      return path.error();
    }

    auto content = string_argument(args, 1, "content");
    if (!content)
    {
      return content.error();
    }

    ::std::ofstream file(path.value(), ::std::ios::out | ::std::ios::binary);
    if (!file)
    {
      return make_fs_binding_error(
          "failed to open file for writing: " + path.value());
    }

    file << content.value();

    if (!file)
    {
      return make_fs_binding_error(
          "failed to write file: " + path.value());
    }

    return kordex::bindings::Value::undefined();
  }

  kordex::bindings::Result<kordex::bindings::Value> Fs::remove(
      const kordex::bindings::NativeFunctionArguments &args)
  {
    auto path = string_argument(args, 0, "path");
    if (!path)
    {
      return path.error();
    }

    try
    {
      return kordex::bindings::Value::boolean(
          ::std::filesystem::remove(path.value()));
    }
    catch (const ::std::exception &exception)
    {
      return make_fs_binding_error(exception.what());
    }
  }

  kordex::bindings::NativeFunction Fs::make_function(
      ::std::string name,
      ::std::string description,
      ::std::size_t min_args,
      ::std::size_t max_args,
      kordex::bindings::NativeFunctionCallback callback)
  {
    kordex::bindings::NativeFunctionInfo info;
    info.name = ::std::move(name);
    info.module_name = "fs";
    info.description = ::std::move(description);
    info.min_args = min_args;
    info.max_args = max_args;
    info.callable = true;
    info.safe = true;

    return kordex::bindings::NativeFunction::create(
        info,
        ::std::move(callback));
  }

  kordex::bindings::Result<::std::string> Fs::string_argument(
      const kordex::bindings::NativeFunctionArguments &args,
      ::std::size_t index,
      const char *name)
  {
    if (index >= args.size())
    {
      return kordex::bindings::make_binding_error(
          kordex::bindings::BindingErrorCode::InvalidArgument,
          ::std::string(name ? name : "argument") + " argument is missing");
    }

    auto value = args[index].as_string();
    if (!value)
    {
      return kordex::bindings::make_binding_error(
          kordex::bindings::BindingErrorCode::InvalidArgument,
          ::std::string(name ? name : "argument") + " argument must be a string");
    }

    if (value.value().empty())
    {
      return kordex::bindings::make_binding_error(
          kordex::bindings::BindingErrorCode::InvalidArgument,
          ::std::string(name ? name : "argument") + " argument cannot be empty");
    }

    return value.value();
  }

  Result<kordex::bindings::NativeModule> create_fs_module(
      const StdOptions &options)
  {
    auto fs = Fs::create(options);
    if (!fs)
    {
      return fs.error();
    }

    return fs.value().module();
  }

} // namespace kordex::standard
