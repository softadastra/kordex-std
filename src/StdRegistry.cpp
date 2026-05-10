/**
 *
 *  @file StdRegistry.cpp
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

#include <utility>
#include <kordex/std/StdRegistry.hpp>

namespace kordex::standard
{
  namespace
  {
    [[nodiscard]] ::std::string canonical_module_name(
        const ::std::string &name)
    {
      return ModuleFactory::canonical_name(name);
    }
  } // namespace

  bool StdRegistryInfo::has_name() const noexcept
  {
    return !name.empty();
  }

  bool StdRegistryInfo::has_namespace_name() const noexcept
  {
    return !namespace_name.empty();
  }

  StdRegistry::StdRegistry()
      : config_(),
        factory_(config_),
        info_()
  {
    info_.name = config_.registry_name;
    info_.namespace_name = config_.namespace_name;
  }

  StdRegistry::StdRegistry(StdConfig config)
      : config_(::std::move(config)),
        factory_(config_),
        info_()
  {
    info_.name = config_.registry_name;
    info_.namespace_name = config_.namespace_name;
  }

  Result<StdRegistry> StdRegistry::create(
      const StdOptions &options)
  {
    auto config = StdConfig::from_options(options);
    if (!config)
    {
      return config.error();
    }

    StdRegistry registry(config.value());

    const auto validation = registry.validate();
    if (validation)
    {
      return validation;
    }

    return registry;
  }

  const StdRegistryInfo &StdRegistry::info() const noexcept
  {
    return info_;
  }

  const ::std::string &StdRegistry::name() const noexcept
  {
    return info_.name;
  }

  const ::std::string &StdRegistry::namespace_name() const noexcept
  {
    return info_.namespace_name;
  }

  const StdConfig &StdRegistry::config() const noexcept
  {
    return config_;
  }

  bool StdRegistry::initialized() const noexcept
  {
    return info_.initialized;
  }

  bool StdRegistry::enabled() const noexcept
  {
    return config_.enabled;
  }

  ::std::size_t StdRegistry::module_count() const noexcept
  {
    return modules_.size();
  }

  Error StdRegistry::validate() const
  {
    const auto config_validation = config_.validate();
    if (config_validation)
    {
      return config_validation;
    }

    if (!info_.has_name())
    {
      return make_std_error(
          StdErrorCode::InvalidConfig,
          "std registry name cannot be empty");
    }

    if (!info_.has_namespace_name())
    {
      return make_std_error(
          StdErrorCode::InvalidConfig,
          "std registry namespace cannot be empty");
    }

    if (!config_.allow_native_modules)
    {
      return make_std_error(
          StdErrorCode::InvalidConfig,
          "std registry requires native modules");
    }

    if (!config_.allow_native_functions)
    {
      return make_std_error(
          StdErrorCode::InvalidConfig,
          "std registry requires native functions");
    }

    return ok();
  }

  Error StdRegistry::initialize()
  {
    if (info_.initialized)
    {
      return ok();
    }

    const auto validation = validate();
    if (validation)
    {
      return validation;
    }

    const auto register_error = register_all();
    if (register_error)
    {
      return register_error;
    }

    info_.initialized = true;
    info_.module_count = modules_.size();

    return ok();
  }

  Error StdRegistry::shutdown()
  {
    clear();
    info_.initialized = false;
    info_.module_count = 0;

    return ok();
  }

  void StdRegistry::clear()
  {
    modules_.clear();
    info_.module_count = 0;
  }

  const ::std::vector<kordex::bindings::NativeModule> &StdRegistry::modules() const noexcept
  {
    return modules_;
  }

  ::std::vector<::std::string> StdRegistry::enabled_module_names() const
  {
    return factory_.enabled_module_names();
  }

  bool StdRegistry::has_module(
      const ::std::string &name) const noexcept
  {
    const auto canonical = canonical_module_name(name);

    for (const auto &module_value : modules_)
    {
      if (module_name_matches(module_value, canonical))
      {
        return true;
      }
    }

    return false;
  }

  Result<kordex::bindings::NativeModule> StdRegistry::module(
      const ::std::string &name) const
  {
    const auto state = require_initialized();
    if (state)
    {
      return state;
    }

    const auto canonical = canonical_module_name(name);

    for (const auto &module_value : modules_)
    {
      if (module_name_matches(module_value, canonical))
      {
        return module_value;
      }
    }

    return make_std_error(
        StdErrorCode::InvalidArgument,
        "std module is not registered: " + name);
  }

  Error StdRegistry::register_module(
      const ::std::string &name)
  {
    const auto validation = validate();
    if (validation)
    {
      return validation;
    }

    const auto canonical = canonical_module_name(name);

    if (has_module(canonical))
    {
      return ok();
    }

    const auto capacity_error = ensure_capacity_for_one();
    if (capacity_error)
    {
      return capacity_error;
    }

    auto module_result = factory_.create_module(canonical);
    if (!module_result)
    {
      return module_result.error();
    }

    modules_.push_back(::std::move(module_result.value()));
    info_.module_count = modules_.size();

    return ok();
  }

  Error StdRegistry::register_all()
  {
    const auto validation = validate();
    if (validation)
    {
      return validation;
    }

    for (const auto &module_name : enabled_module_names())
    {
      const auto register_error = register_module(module_name);
      if (register_error)
      {
        return register_error;
      }
    }

    info_.module_count = modules_.size();

    return ok();
  }

  Error StdRegistry::install_into(
      kordex::bindings::ModuleRegistry &registry) const
  {
    const auto state = require_initialized();
    if (state)
    {
      return state;
    }

    for (const auto &module_value : modules_)
    {
      const auto register_error = registry.register_native_module(module_value);
      if (register_error)
      {
        return make_std_error(
            StdErrorCode::ModuleRegistrationFailed,
            ::std::string(register_error.message()));
      }
    }

    return ok();
  }

  Error StdRegistry::install_into(
      kordex::bindings::EngineContext &context) const
  {
    const auto state = require_initialized();
    if (state)
    {
      return state;
    }

    if (!context.initialized())
    {
      return make_std_error(
          StdErrorCode::InvalidConfig,
          "engine context must be initialized before installing std modules");
    }

    for (const auto &module_value : modules_)
    {
      const auto register_error = context.register_native_module(module_value);
      if (register_error)
      {
        return make_std_error(
            StdErrorCode::ModuleRegistrationFailed,
            ::std::string(register_error.message()));
      }
    }

    return ok();
  }

  Error StdRegistry::install_into(
      kordex::bindings::Engine &engine) const
  {
    const auto state = require_initialized();
    if (state)
    {
      return state;
    }

    if (!engine.initialized())
    {
      return make_std_error(
          StdErrorCode::InvalidConfig,
          "engine must be initialized before installing std modules");
    }

    for (const auto &module_value : modules_)
    {
      const auto register_error = engine.register_native_module(module_value);
      if (register_error)
      {
        return make_std_error(
            StdErrorCode::ModuleRegistrationFailed,
            ::std::string(register_error.message()));
      }
    }

    return ok();
  }

  Error StdRegistry::require_initialized() const
  {
    if (!info_.initialized)
    {
      return make_std_error(
          StdErrorCode::InvalidConfig,
          "std registry is not initialized");
    }

    return ok();
  }

  Error StdRegistry::ensure_capacity_for_one() const
  {
    if (!config_.has_max_modules())
    {
      return ok();
    }

    if (modules_.size() + 1 > config_.max_modules)
    {
      return make_std_error(
          StdErrorCode::InvalidConfig,
          "std registry max_modules limit reached");
    }

    return ok();
  }

  bool StdRegistry::module_name_matches(
      const kordex::bindings::NativeModule &module,
      const ::std::string &name) noexcept
  {
    return module.name() == name ||
           module.name() == canonical_module_name(name);
  }

  Result<StdRegistry> create_std_registry(
      const StdOptions &options)
  {
    auto registry = StdRegistry::create(options);
    if (!registry)
    {
      return registry.error();
    }

    const auto init_error = registry.value().initialize();
    if (init_error)
    {
      return init_error;
    }

    return registry;
  }

  Error install_std_modules(
      kordex::bindings::ModuleRegistry &registry,
      const StdOptions &options)
  {
    auto std_registry = create_std_registry(options);
    if (!std_registry)
    {
      return std_registry.error();
    }

    return std_registry.value().install_into(registry);
  }

  Error install_std_modules(
      kordex::bindings::EngineContext &context,
      const StdOptions &options)
  {
    auto std_registry = create_std_registry(options);
    if (!std_registry)
    {
      return std_registry.error();
    }

    return std_registry.value().install_into(context);
  }

  Error install_std_modules(
      kordex::bindings::Engine &engine,
      const StdOptions &options)
  {
    auto std_registry = create_std_registry(options);
    if (!std_registry)
    {
      return std_registry.error();
    }

    return std_registry.value().install_into(engine);
  }

} // namespace kordex::standard
