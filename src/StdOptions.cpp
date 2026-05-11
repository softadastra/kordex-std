/**
 *
 *  @file StdOptions.cpp
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

#include <kordex/std/StdOptions.hpp>

namespace kordex::standard
{
  StdOptions StdOptions::defaults()
  {
    return StdOptions{};
  }

  StdOptions StdOptions::development()
  {
    StdOptions options;
    options.registry_name = "kordex-std-dev";
    options.namespace_name = "kordex";
    options.enabled = true;
    options.safe_by_default = true;

    options.allow_native_modules = true;
    options.allow_native_functions = true;

    options.enable_console = true;
    options.enable_fs = true;
    options.enable_path = true;
    options.enable_env = true;
    options.enable_process = true;
    options.enable_timer = true;
    options.enable_crypto = true;
    options.enable_http = true;

    options.max_modules = 0;

    return options;
  }

  StdOptions StdOptions::production()
  {
    StdOptions options;
    options.registry_name = "kordex-std";
    options.namespace_name = "kordex";
    options.enabled = true;
    options.safe_by_default = true;

    options.allow_native_modules = true;
    options.allow_native_functions = true;

    options.enable_console = true;
    options.enable_fs = true;
    options.enable_path = true;
    options.enable_env = true;
    options.enable_process = false;
    options.enable_timer = true;
    options.enable_crypto = true;
    options.enable_http = false;

    options.max_modules = 0;

    return options;
  }

  StdOptions StdOptions::test()
  {
    StdOptions options;
    options.registry_name = "kordex-std-test";
    options.namespace_name = "kordex";
    options.enabled = true;
    options.safe_by_default = true;

    options.allow_native_modules = true;
    options.allow_native_functions = true;

    options.enable_console = true;
    options.enable_fs = true;
    options.enable_path = true;
    options.enable_env = true;
    options.enable_process = true;
    options.enable_timer = true;
    options.enable_crypto = true;
    options.enable_http = true;

    options.max_modules = 16;

    return options;
  }

  StdOptions StdOptions::minimal()
  {
    StdOptions options;
    options.registry_name = "kordex-std-minimal";
    options.namespace_name = "kordex";
    options.enabled = true;
    options.safe_by_default = true;

    options.allow_native_modules = true;
    options.allow_native_functions = true;

    options.enable_console = true;
    options.enable_fs = false;
    options.enable_path = true;
    options.enable_env = false;
    options.enable_process = false;
    options.enable_timer = false;
    options.enable_crypto = false;
    options.enable_http = false;

    options.max_modules = 0;

    return options;
  }

  bool StdOptions::has_registry_name() const noexcept
  {
    return !registry_name.empty();
  }

  bool StdOptions::has_namespace_name() const noexcept
  {
    return !namespace_name.empty();
  }

  bool StdOptions::has_module_paths() const noexcept
  {
    return !module_paths.empty();
  }

  bool StdOptions::has_max_modules() const noexcept
  {
    return max_modules > 0;
  }

  bool StdOptions::has_enabled_modules() const noexcept
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

} // namespace kordex::standard
