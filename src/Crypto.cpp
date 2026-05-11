/**
 *
 *  @file Crypto.cpp
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

#include <cmath>
#include <iomanip>
#include <limits>
#include <random>
#include <sstream>
#include <utility>

#include <kordex/std/Crypto.hpp>

namespace kordex::standard
{
  namespace
  {
    [[nodiscard]] kordex::bindings::Error make_crypto_binding_error(
        const ::std::string &message)
    {
      return kordex::bindings::make_binding_error(
          kordex::bindings::BindingErrorCode::FunctionCallFailed,
          message);
    }

    [[nodiscard]] bool valid_number(double value) noexcept
    {
      return ::std::isfinite(value);
    }

    [[nodiscard]] double random_unit()
    {
      static thread_local ::std::random_device device;
      static thread_local ::std::mt19937_64 engine(device());

      ::std::uniform_real_distribution<double> distribution(
          0.0,
          ::std::nextafter(1.0, 0.0));

      return distribution(engine);
    }

    [[nodiscard]] double random_integer(
        double min_value,
        double max_value)
    {
      const auto min_integer = static_cast<long long>(min_value);
      const auto max_integer = static_cast<long long>(max_value);

      static thread_local ::std::random_device device;
      static thread_local ::std::mt19937_64 engine(device());

      ::std::uniform_int_distribution<long long> distribution(
          min_integer,
          max_integer);

      return static_cast<double>(distribution(engine));
    }
  } // namespace

  Crypto::Crypto()
      : config_()
  {
  }

  Crypto::Crypto(StdConfig config)
      : config_(::std::move(config))
  {
  }

  Result<Crypto> Crypto::create(
      const StdOptions &options)
  {
    auto config = StdConfig::from_options(options);
    if (!config)
    {
      return config.error();
    }

    Crypto crypto(config.value());

    const auto validation = crypto.validate();
    if (validation)
    {
      return validation;
    }

    return crypto;
  }

  const StdConfig &Crypto::config() const noexcept
  {
    return config_;
  }

  bool Crypto::enabled() const noexcept
  {
    return config_.module_enabled("crypto");
  }

  Error Crypto::validate() const
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
          "crypto module is disabled");
    }

    if (!config_.allow_native_modules)
    {
      return make_std_error(
          StdErrorCode::InvalidConfig,
          "crypto module requires native modules");
    }

    if (!config_.allow_native_functions)
    {
      return make_std_error(
          StdErrorCode::InvalidConfig,
          "crypto module requires native functions");
    }

    return ok();
  }

  Result<kordex::bindings::NativeModule> Crypto::module() const
  {
    const auto validation = validate();
    if (validation)
    {
      return validation;
    }

    kordex::bindings::NativeModuleInfo info;
    info.name = "crypto";
    info.namespace_name = config_.namespace_name;
    info.description = "Kordex standard crypto module";
    info.importable = true;
    info.safe = config_.safe_by_default;

    auto module = kordex::bindings::NativeModule::create(info);

    const auto name_error = module.set_value(
        "name",
        kordex::bindings::Value::string("crypto"));

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

    const auto hash_error = module.add_function(
        make_function(
            "hash",
            "Return a deterministic hexadecimal hash",
            1,
            1,
            &Crypto::hash));

    if (hash_error)
    {
      return make_std_error(
          StdErrorCode::ModuleRegistrationFailed,
          ::std::string(hash_error.message()));
    }

    const auto random_error = module.add_function(
        make_function(
            "random",
            "Return a random number in the range [0, 1)",
            0,
            0,
            &Crypto::random));

    if (random_error)
    {
      return make_std_error(
          StdErrorCode::ModuleRegistrationFailed,
          ::std::string(random_error.message()));
    }

    const auto random_int_error = module.add_function(
        make_function(
            "randomInt",
            "Return a random integer between min and max inclusive",
            2,
            2,
            &Crypto::random_int));

    if (random_int_error)
    {
      return make_std_error(
          StdErrorCode::ModuleRegistrationFailed,
          ::std::string(random_int_error.message()));
    }

    const auto equals_error = module.add_function(
        make_function(
            "equals",
            "Return true when two strings are equal",
            2,
            2,
            &Crypto::equals));

    if (equals_error)
    {
      return make_std_error(
          StdErrorCode::ModuleRegistrationFailed,
          ::std::string(equals_error.message()));
    }

    return module;
  }

  kordex::bindings::Result<kordex::bindings::Value> Crypto::hash(
      const kordex::bindings::NativeFunctionArguments &args)
  {
    auto input = string_argument(args, 0, "input");
    if (!input)
    {
      return input.error();
    }

    return kordex::bindings::Value::string(
        fnv1a_hex(input.value()));
  }

  kordex::bindings::Result<kordex::bindings::Value> Crypto::random(
      const kordex::bindings::NativeFunctionArguments &args)
  {
    if (!args.empty())
    {
      return kordex::bindings::make_binding_error(
          kordex::bindings::BindingErrorCode::InvalidArgument,
          "crypto.random expects no arguments");
    }

    try
    {
      return kordex::bindings::Value::number(random_unit());
    }
    catch (const ::std::exception &exception)
    {
      return make_crypto_binding_error(exception.what());
    }
  }

  kordex::bindings::Result<kordex::bindings::Value> Crypto::random_int(
      const kordex::bindings::NativeFunctionArguments &args)
  {
    auto min_value = number_argument(args, 0, "min");
    if (!min_value)
    {
      return min_value.error();
    }

    auto max_value = number_argument(args, 1, "max");
    if (!max_value)
    {
      return max_value.error();
    }

    if (!valid_number(min_value.value()) ||
        !valid_number(max_value.value()))
    {
      return kordex::bindings::make_binding_error(
          kordex::bindings::BindingErrorCode::InvalidArgument,
          "min and max must be finite numbers");
    }

    if (min_value.value() > max_value.value())
    {
      return kordex::bindings::make_binding_error(
          kordex::bindings::BindingErrorCode::InvalidArgument,
          "min must be less than or equal to max");
    }

    if (min_value.value() <
            static_cast<double>((::std::numeric_limits<long long>::min)()) ||
        max_value.value() >
            static_cast<double>((::std::numeric_limits<long long>::max)()))
    {
      return kordex::bindings::make_binding_error(
          kordex::bindings::BindingErrorCode::InvalidArgument,
          "random integer range is out of bounds");
    }

    try
    {
      return kordex::bindings::Value::number(
          random_integer(min_value.value(), max_value.value()));
    }
    catch (const ::std::exception &exception)
    {
      return make_crypto_binding_error(exception.what());
    }
  }

  kordex::bindings::Result<kordex::bindings::Value> Crypto::equals(
      const kordex::bindings::NativeFunctionArguments &args)
  {
    auto left = string_argument(args, 0, "left");
    if (!left)
    {
      return left.error();
    }

    auto right = string_argument(args, 1, "right");
    if (!right)
    {
      return right.error();
    }

    return kordex::bindings::Value::boolean(
        left.value() == right.value());
  }

  ::std::string Crypto::fnv1a_hex(
      const ::std::string &input)
  {
    constexpr unsigned long long offset_basis = 14695981039346656037ull;
    constexpr unsigned long long prime = 1099511628211ull;

    unsigned long long hash_value = offset_basis;

    for (unsigned char character : input)
    {
      hash_value ^= static_cast<unsigned long long>(character);
      hash_value *= prime;
    }

    ::std::ostringstream stream;
    stream << ::std::hex
           << ::std::setfill('0')
           << ::std::setw(16)
           << hash_value;

    return stream.str();
  }

  kordex::bindings::NativeFunction Crypto::make_function(
      ::std::string name,
      ::std::string description,
      ::std::size_t min_args,
      ::std::size_t max_args,
      kordex::bindings::NativeFunctionCallback callback)
  {
    kordex::bindings::NativeFunctionInfo info;
    info.name = ::std::move(name);
    info.module_name = "crypto";
    info.description = ::std::move(description);
    info.min_args = min_args;
    info.max_args = max_args;
    info.callable = true;
    info.safe = true;

    return kordex::bindings::NativeFunction::create(
        info,
        ::std::move(callback));
  }

  kordex::bindings::Result<::std::string> Crypto::string_argument(
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

    return value.value();
  }

  kordex::bindings::Result<double> Crypto::number_argument(
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

    auto value = args[index].as_number();
    if (!value)
    {
      return kordex::bindings::make_binding_error(
          kordex::bindings::BindingErrorCode::InvalidArgument,
          ::std::string(name ? name : "argument") + " argument must be a number");
    }

    return value.value();
  }

  Result<kordex::bindings::NativeModule> create_crypto_module(
      const StdOptions &options)
  {
    auto crypto = Crypto::create(options);
    if (!crypto)
    {
      return crypto.error();
    }

    return crypto.value().module();
  }

} // namespace kordex::standard
