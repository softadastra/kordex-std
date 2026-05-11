/**
 *
 *  @file StdConfig.cpp
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

#include <algorithm>
#include <kordex/std/StdConfig.hpp>

namespace kordex::standard
{
  namespace
  {
    [[nodiscard]] bool contains_empty_path(
        const ::std::vector<::std::string> &paths) noexcept
    {
      return ::std::any_of(
          paths.begin(),
          paths.end(),
          [](const ::std::string &path)
          {
            return path.empty();
          });
    }
  } // namespace

  Result<StdConfig> StdConfig::from_options(
      const StdOptions &options)
  {
    StdConfig config;

    config.registry_name = options.registry_name;
    config.namespace_name = options.namespace_name;
    config.module_paths = options.module_paths;

    config.enabled = options.enabled;
    config.safe_by_default = options.safe_by_default;

    config.allow_native_modules = options.allow_native_modules;
    config.allow_native_functions = options.allow_native_functions;

    config.enable_console = options.enable_console;
    config.enable_fs = options.enable_fs;
    config.enable_path = options.enable_path;
    config.enable_env = options.enable_env;
    config.enable_process = options.enable_process;
    config.enable_timer = options.enable_timer;
    config.enable_crypto = options.enable_crypto;
    config.enable_http = options.enable_http;

    config.max_modules = options.max_modules;

    const auto validation = config.validate();
    if (validation)
    {
      return validation;
    }

    return config;
  }

  Result<StdConfig> StdConfig::from_environment(
      const StdOptions &base)
  {
    return from_options(base);
  }

  bool StdConfig::has_registry_name() const noexcept
  {
    return !registry_name.empty();
  }

  bool StdConfig::has_namespace_name() const noexcept
  {
    return !namespace_name.empty();
  }

  bool StdConfig::has_module_paths() const noexcept
  {
    return !module_paths.empty();
  }

  bool StdConfig::has_max_modules() const noexcept
  {
    return max_modules > 0;
  }

  bool StdConfig::has_enabled_modules() const noexcept
  {
    return enabled &&
           (enable_console ||
            enable_fs ||
            enable_path ||
            enable_env ||
            enable_process ||
            enable_timer ||
            enable_crypto ||
            enable_http);
  }

  bool StdConfig::module_enabled(
      const ::std::string &name) const noexcept
  {
    if (!enabled)
    {
      return false;
    }

    if (name == "console" || name == "kordex:console")
    {
      return enable_console;
    }

    if (name == "fs" || name == "kordex:fs")
    {
      return enable_fs;
    }

    if (name == "path" || name == "kordex:path")
    {
      return enable_path;
    }

    if (name == "env" || name == "kordex:env")
    {
      return enable_env;
    }

    if (name == "process" || name == "kordex:process")
    {
      return enable_process;
    }

    if (name == "timer" || name == "kordex:timer")
    {
      return enable_timer;
    }

    if (name == "crypto" || name == "kordex:crypto")
    {
      return enable_crypto;
    }

    if (name == "http" || name == "kordex:http")
    {
      return enable_http;
    }

    return false;
  }

  ::std::size_t StdConfig::enabled_module_count() const noexcept
  {
    if (!enabled)
    {
      return 0;
    }

    ::std::size_t count = 0;

    if (enable_console)
    {
      ++count;
    }

    if (enable_fs)
    {
      ++count;
    }

    if (enable_path)
    {
      ++count;
    }

    if (enable_env)
    {
      ++count;
    }

    if (enable_process)
    {
      ++count;
    }

    if (enable_timer)
    {
      ++count;
    }

    if (enable_crypto)
    {
      ++count;
    }

    if (enable_http)
    {
      ++count;
    }

    return count;
  }

  Error StdConfig::validate() const
  {
    if (!enabled)
    {
      return ok();
    }

    if (registry_name.empty())
    {
      return make_std_error(
          StdErrorCode::InvalidConfig,
          "std registry name cannot be empty");
    }

    if (namespace_name.empty())
    {
      return make_std_error(
          StdErrorCode::InvalidConfig,
          "std namespace name cannot be empty");
    }

    if (!allow_native_modules)
    {
      return make_std_error(
          StdErrorCode::InvalidConfig,
          "std requires native modules to be enabled");
    }

    if (!allow_native_functions)
    {
      return make_std_error(
          StdErrorCode::InvalidConfig,
          "std requires native functions to be enabled");
    }

    if (contains_empty_path(module_paths))
    {
      return make_std_error(
          StdErrorCode::InvalidConfig,
          "std module paths cannot contain empty entries");
    }

    if (!has_enabled_modules())
    {
      return make_std_error(
          StdErrorCode::ModuleDisabled,
          "at least one std module must be enabled");
    }

    if (has_max_modules() &&
        enabled_module_count() > max_modules)
    {
      return make_std_error(
          StdErrorCode::InvalidConfig,
          "enabled std module count exceeds max_modules");
    }

    return ok();
  }

} // namespace kordex::standard
