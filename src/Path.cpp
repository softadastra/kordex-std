/**
 *
 *  @file Path.cpp
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
#include <utility>

#include <kordex/std/Path.hpp>

namespace kordex::standard
{
  namespace
  {
    [[nodiscard]] kordex::bindings::Error make_path_binding_error(
        const ::std::string &message)
    {
      return kordex::bindings::make_binding_error(
          kordex::bindings::BindingErrorCode::FunctionCallFailed,
          message);
    }

    [[nodiscard]] ::std::string generic_string(
        const ::std::filesystem::path &path)
    {
      return path.generic_string();
    }
  } // namespace

  Path::Path()
      : config_()
  {
  }

  Path::Path(StdConfig config)
      : config_(::std::move(config))
  {
  }

  Result<Path> Path::create(
      const StdOptions &options)
  {
    auto config = StdConfig::from_options(options);
    if (!config)
    {
      return config.error();
    }

    Path path(config.value());

    const auto validation = path.validate();
    if (validation)
    {
      return validation;
    }

    return path;
  }

  const StdConfig &Path::config() const noexcept
  {
    return config_;
  }

  bool Path::enabled() const noexcept
  {
    return config_.module_enabled("path");
  }

  Error Path::validate() const
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
          "path module is disabled");
    }

    if (!config_.allow_native_modules)
    {
      return make_std_error(
          StdErrorCode::InvalidConfig,
          "path module requires native modules");
    }

    if (!config_.allow_native_functions)
    {
      return make_std_error(
          StdErrorCode::InvalidConfig,
          "path module requires native functions");
    }

    return ok();
  }

  Result<kordex::bindings::NativeModule> Path::module() const
  {
    const auto validation = validate();
    if (validation)
    {
      return validation;
    }

    kordex::bindings::NativeModuleInfo info;
    info.name = "path";
    info.namespace_name = config_.namespace_name;
    info.description = "Kordex standard path module";
    info.importable = true;
    info.safe = config_.safe_by_default;

    auto module = kordex::bindings::NativeModule::create(info);

    const auto name_error = module.set_value(
        "name",
        kordex::bindings::Value::string("path"));

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

    const auto separator_error = module.set_value(
        "separator",
        kordex::bindings::Value::string(
            ::std::string(1, ::std::filesystem::path::preferred_separator)));

    if (separator_error)
    {
      return make_std_error(
          StdErrorCode::ModuleRegistrationFailed,
          ::std::string(separator_error.message()));
    }

    const auto normalize_error = module.add_function(
        make_function(
            "normalize",
            "Normalize a path lexically",
            1,
            1,
            &Path::normalize));

    if (normalize_error)
    {
      return make_std_error(
          StdErrorCode::ModuleRegistrationFailed,
          ::std::string(normalize_error.message()));
    }

    const auto join_error = module.add_function(
        make_function(
            "join",
            "Join path segments",
            1,
            0,
            &Path::join));

    if (join_error)
    {
      return make_std_error(
          StdErrorCode::ModuleRegistrationFailed,
          ::std::string(join_error.message()));
    }

    const auto dirname_error = module.add_function(
        make_function(
            "dirname",
            "Return the parent directory of a path",
            1,
            1,
            &Path::dirname));

    if (dirname_error)
    {
      return make_std_error(
          StdErrorCode::ModuleRegistrationFailed,
          ::std::string(dirname_error.message()));
    }

    const auto basename_error = module.add_function(
        make_function(
            "basename",
            "Return the filename part of a path",
            1,
            1,
            &Path::basename));

    if (basename_error)
    {
      return make_std_error(
          StdErrorCode::ModuleRegistrationFailed,
          ::std::string(basename_error.message()));
    }

    const auto extension_error = module.add_function(
        make_function(
            "extension",
            "Return the extension part of a path",
            1,
            1,
            &Path::extension));

    if (extension_error)
    {
      return make_std_error(
          StdErrorCode::ModuleRegistrationFailed,
          ::std::string(extension_error.message()));
    }

    const auto is_absolute_error = module.add_function(
        make_function(
            "isAbsolute",
            "Return true if a path is absolute",
            1,
            1,
            &Path::is_absolute));

    if (is_absolute_error)
    {
      return make_std_error(
          StdErrorCode::ModuleRegistrationFailed,
          ::std::string(is_absolute_error.message()));
    }

    const auto is_relative_error = module.add_function(
        make_function(
            "isRelative",
            "Return true if a path is relative",
            1,
            1,
            &Path::is_relative));

    if (is_relative_error)
    {
      return make_std_error(
          StdErrorCode::ModuleRegistrationFailed,
          ::std::string(is_relative_error.message()));
    }

    return module;
  }

  kordex::bindings::Result<kordex::bindings::Value> Path::normalize(
      const kordex::bindings::NativeFunctionArguments &args)
  {
    auto path = string_argument(args, 0, "path");
    if (!path)
    {
      return path.error();
    }

    try
    {
      return kordex::bindings::Value::string(
          generic_string(::std::filesystem::path(path.value()).lexically_normal()));
    }
    catch (const ::std::exception &exception)
    {
      return make_path_binding_error(exception.what());
    }
  }

  kordex::bindings::Result<kordex::bindings::Value> Path::join(
      const kordex::bindings::NativeFunctionArguments &args)
  {
    if (args.empty())
    {
      return kordex::bindings::make_binding_error(
          kordex::bindings::BindingErrorCode::InvalidArgument,
          "path segment argument is missing");
    }

    try
    {
      ::std::filesystem::path result;

      for (::std::size_t index = 0; index < args.size(); ++index)
      {
        auto segment = string_argument(args, index, "path segment");
        if (!segment)
        {
          return segment.error();
        }

        result /= segment.value();
      }

      return kordex::bindings::Value::string(
          generic_string(result.lexically_normal()));
    }
    catch (const ::std::exception &exception)
    {
      return make_path_binding_error(exception.what());
    }
  }

  kordex::bindings::Result<kordex::bindings::Value> Path::dirname(
      const kordex::bindings::NativeFunctionArguments &args)
  {
    auto path = string_argument(args, 0, "path");
    if (!path)
    {
      return path.error();
    }

    try
    {
      return kordex::bindings::Value::string(
          generic_string(::std::filesystem::path(path.value()).parent_path()));
    }
    catch (const ::std::exception &exception)
    {
      return make_path_binding_error(exception.what());
    }
  }

  kordex::bindings::Result<kordex::bindings::Value> Path::basename(
      const kordex::bindings::NativeFunctionArguments &args)
  {
    auto path = string_argument(args, 0, "path");
    if (!path)
    {
      return path.error();
    }

    try
    {
      return kordex::bindings::Value::string(
          generic_string(::std::filesystem::path(path.value()).filename()));
    }
    catch (const ::std::exception &exception)
    {
      return make_path_binding_error(exception.what());
    }
  }

  kordex::bindings::Result<kordex::bindings::Value> Path::extension(
      const kordex::bindings::NativeFunctionArguments &args)
  {
    auto path = string_argument(args, 0, "path");
    if (!path)
    {
      return path.error();
    }

    try
    {
      return kordex::bindings::Value::string(
          generic_string(::std::filesystem::path(path.value()).extension()));
    }
    catch (const ::std::exception &exception)
    {
      return make_path_binding_error(exception.what());
    }
  }

  kordex::bindings::Result<kordex::bindings::Value> Path::is_absolute(
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
          ::std::filesystem::path(path.value()).is_absolute());
    }
    catch (const ::std::exception &exception)
    {
      return make_path_binding_error(exception.what());
    }
  }

  kordex::bindings::Result<kordex::bindings::Value> Path::is_relative(
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
          ::std::filesystem::path(path.value()).is_relative());
    }
    catch (const ::std::exception &exception)
    {
      return make_path_binding_error(exception.what());
    }
  }

  kordex::bindings::NativeFunction Path::make_function(
      ::std::string name,
      ::std::string description,
      ::std::size_t min_args,
      ::std::size_t max_args,
      kordex::bindings::NativeFunctionCallback callback)
  {
    kordex::bindings::NativeFunctionInfo info;
    info.name = ::std::move(name);
    info.module_name = "path";
    info.description = ::std::move(description);
    info.min_args = min_args;
    info.max_args = max_args;
    info.callable = true;
    info.safe = true;

    return kordex::bindings::NativeFunction::create(
        info,
        ::std::move(callback));
  }

  kordex::bindings::Result<::std::string> Path::string_argument(
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

  Result<kordex::bindings::NativeModule> create_path_module(
      const StdOptions &options)
  {
    auto path = Path::create(options);
    if (!path)
    {
      return path.error();
    }

    return path.value().module();
  }

} // namespace kordex::standard
