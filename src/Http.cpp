/**
 *
 *  @file Http.cpp
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
#include <cctype>
#include <cmath>
#include <utility>

#include <kordex/std/Http.hpp>

namespace kordex::standard
{
  namespace
  {
    [[nodiscard]] bool valid_status_code(double value) noexcept
    {
      return ::std::isfinite(value) &&
             value >= 100.0 &&
             value <= 599.0 &&
             value == static_cast<double>(static_cast<int>(value));
    }

    [[nodiscard]] bool supported_method(
        const ::std::string &method) noexcept
    {
      return method == "GET" ||
             method == "POST" ||
             method == "PUT" ||
             method == "PATCH" ||
             method == "DELETE" ||
             method == "HEAD" ||
             method == "OPTIONS";
    }
  } // namespace

  Http::Http()
      : config_()
  {
  }

  Http::Http(StdConfig config)
      : config_(::std::move(config))
  {
  }

  Result<Http> Http::create(
      const StdOptions &options)
  {
    auto config = StdConfig::from_options(options);
    if (!config)
    {
      return config.error();
    }

    Http http(config.value());

    const auto validation = http.validate();
    if (validation)
    {
      return validation;
    }

    return http;
  }

  const StdConfig &Http::config() const noexcept
  {
    return config_;
  }

  bool Http::enabled() const noexcept
  {
    return config_.module_enabled("http");
  }

  Error Http::validate() const
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
          "http module is disabled");
    }

    if (!config_.allow_native_modules)
    {
      return make_std_error(
          StdErrorCode::InvalidConfig,
          "http module requires native modules");
    }

    if (!config_.allow_native_functions)
    {
      return make_std_error(
          StdErrorCode::InvalidConfig,
          "http module requires native functions");
    }

    return ok();
  }

  Result<kordex::bindings::NativeModule> Http::module() const
  {
    const auto validation = validate();
    if (validation)
    {
      return validation;
    }

    kordex::bindings::NativeModuleInfo info;
    info.name = "http";
    info.namespace_name = config_.namespace_name;
    info.description = "Kordex standard HTTP module";
    info.importable = true;
    info.safe = config_.safe_by_default;

    auto module = kordex::bindings::NativeModule::create(info);

    const auto name_error = module.set_value(
        "name",
        kordex::bindings::Value::string("http"));

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

    const auto get_value_error = module.set_value(
        "GET",
        kordex::bindings::Value::string("GET"));

    if (get_value_error)
    {
      return make_std_error(
          StdErrorCode::ModuleRegistrationFailed,
          ::std::string(get_value_error.message()));
    }

    const auto post_value_error = module.set_value(
        "POST",
        kordex::bindings::Value::string("POST"));

    if (post_value_error)
    {
      return make_std_error(
          StdErrorCode::ModuleRegistrationFailed,
          ::std::string(post_value_error.message()));
    }

    const auto is_success_error = module.add_function(
        make_function(
            "isSuccess",
            "Return true if an HTTP status code is successful",
            1,
            1,
            &Http::is_success));

    if (is_success_error)
    {
      return make_std_error(
          StdErrorCode::ModuleRegistrationFailed,
          ::std::string(is_success_error.message()));
    }

    const auto is_redirect_error = module.add_function(
        make_function(
            "isRedirect",
            "Return true if an HTTP status code is a redirect",
            1,
            1,
            &Http::is_redirect));

    if (is_redirect_error)
    {
      return make_std_error(
          StdErrorCode::ModuleRegistrationFailed,
          ::std::string(is_redirect_error.message()));
    }

    const auto is_client_error_error = module.add_function(
        make_function(
            "isClientError",
            "Return true if an HTTP status code is a client error",
            1,
            1,
            &Http::is_client_error));

    if (is_client_error_error)
    {
      return make_std_error(
          StdErrorCode::ModuleRegistrationFailed,
          ::std::string(is_client_error_error.message()));
    }

    const auto is_server_error_error = module.add_function(
        make_function(
            "isServerError",
            "Return true if an HTTP status code is a server error",
            1,
            1,
            &Http::is_server_error));

    if (is_server_error_error)
    {
      return make_std_error(
          StdErrorCode::ModuleRegistrationFailed,
          ::std::string(is_server_error_error.message()));
    }

    const auto status_text_error = module.add_function(
        make_function(
            "statusText",
            "Return the standard reason phrase for an HTTP status code",
            1,
            1,
            &Http::status_text));

    if (status_text_error)
    {
      return make_std_error(
          StdErrorCode::ModuleRegistrationFailed,
          ::std::string(status_text_error.message()));
    }

    const auto build_url_error = module.add_function(
        make_function(
            "buildUrl",
            "Build a URL from base and path",
            2,
            2,
            &Http::build_url));

    if (build_url_error)
    {
      return make_std_error(
          StdErrorCode::ModuleRegistrationFailed,
          ::std::string(build_url_error.message()));
    }

    const auto normalize_method_error = module.add_function(
        make_function(
            "normalizeMethod",
            "Normalize an HTTP method",
            1,
            1,
            &Http::normalize_method));

    if (normalize_method_error)
    {
      return make_std_error(
          StdErrorCode::ModuleRegistrationFailed,
          ::std::string(normalize_method_error.message()));
    }

    const auto is_method_error = module.add_function(
        make_function(
            "isMethod",
            "Return true if a string is a supported HTTP method",
            1,
            1,
            &Http::is_method));

    if (is_method_error)
    {
      return make_std_error(
          StdErrorCode::ModuleRegistrationFailed,
          ::std::string(is_method_error.message()));
    }

    return module;
  }

  kordex::bindings::Result<kordex::bindings::Value> Http::is_success(
      const kordex::bindings::NativeFunctionArguments &args)
  {
    auto status = number_argument(args, 0, "status");
    if (!status)
    {
      return status.error();
    }

    if (!valid_status_code(status.value()))
    {
      return kordex::bindings::make_binding_error(
          kordex::bindings::BindingErrorCode::InvalidArgument,
          "status must be an integer between 100 and 599");
    }

    const int code = static_cast<int>(status.value());

    return kordex::bindings::Value::boolean(
        code >= 200 && code <= 299);
  }

  kordex::bindings::Result<kordex::bindings::Value> Http::is_redirect(
      const kordex::bindings::NativeFunctionArguments &args)
  {
    auto status = number_argument(args, 0, "status");
    if (!status)
    {
      return status.error();
    }

    if (!valid_status_code(status.value()))
    {
      return kordex::bindings::make_binding_error(
          kordex::bindings::BindingErrorCode::InvalidArgument,
          "status must be an integer between 100 and 599");
    }

    const int code = static_cast<int>(status.value());

    return kordex::bindings::Value::boolean(
        code >= 300 && code <= 399);
  }

  kordex::bindings::Result<kordex::bindings::Value> Http::is_client_error(
      const kordex::bindings::NativeFunctionArguments &args)
  {
    auto status = number_argument(args, 0, "status");
    if (!status)
    {
      return status.error();
    }

    if (!valid_status_code(status.value()))
    {
      return kordex::bindings::make_binding_error(
          kordex::bindings::BindingErrorCode::InvalidArgument,
          "status must be an integer between 100 and 599");
    }

    const int code = static_cast<int>(status.value());

    return kordex::bindings::Value::boolean(
        code >= 400 && code <= 499);
  }

  kordex::bindings::Result<kordex::bindings::Value> Http::is_server_error(
      const kordex::bindings::NativeFunctionArguments &args)
  {
    auto status = number_argument(args, 0, "status");
    if (!status)
    {
      return status.error();
    }

    if (!valid_status_code(status.value()))
    {
      return kordex::bindings::make_binding_error(
          kordex::bindings::BindingErrorCode::InvalidArgument,
          "status must be an integer between 100 and 599");
    }

    const int code = static_cast<int>(status.value());

    return kordex::bindings::Value::boolean(
        code >= 500 && code <= 599);
  }

  kordex::bindings::Result<kordex::bindings::Value> Http::status_text(
      const kordex::bindings::NativeFunctionArguments &args)
  {
    auto status = number_argument(args, 0, "status");
    if (!status)
    {
      return status.error();
    }

    if (!valid_status_code(status.value()))
    {
      return kordex::bindings::make_binding_error(
          kordex::bindings::BindingErrorCode::InvalidArgument,
          "status must be an integer between 100 and 599");
    }

    return kordex::bindings::Value::string(
        reason_phrase(static_cast<int>(status.value())));
  }

  kordex::bindings::Result<kordex::bindings::Value> Http::build_url(
      const kordex::bindings::NativeFunctionArguments &args)
  {
    auto base = string_argument(args, 0, "base");
    if (!base)
    {
      return base.error();
    }

    auto path = string_argument(args, 1, "path");
    if (!path)
    {
      return path.error();
    }

    ::std::string result = base.value();

    while (!result.empty() && result.back() == '/')
    {
      result.pop_back();
    }

    ::std::string suffix = path.value();

    while (!suffix.empty() && suffix.front() == '/')
    {
      suffix.erase(suffix.begin());
    }

    return kordex::bindings::Value::string(
        result + "/" + suffix);
  }

  kordex::bindings::Result<kordex::bindings::Value> Http::normalize_method(
      const kordex::bindings::NativeFunctionArguments &args)
  {
    auto method = string_argument(args, 0, "method");
    if (!method)
    {
      return method.error();
    }

    const auto normalized = uppercase(method.value());

    if (!supported_method(normalized))
    {
      return kordex::bindings::make_binding_error(
          kordex::bindings::BindingErrorCode::InvalidArgument,
          "unsupported HTTP method: " + normalized);
    }

    return kordex::bindings::Value::string(normalized);
  }

  kordex::bindings::Result<kordex::bindings::Value> Http::is_method(
      const kordex::bindings::NativeFunctionArguments &args)
  {
    auto method = string_argument(args, 0, "method");
    if (!method)
    {
      return method.error();
    }

    return kordex::bindings::Value::boolean(
        supported_method(uppercase(method.value())));
  }

  const char *Http::reason_phrase(
      int status_code) noexcept
  {
    switch (status_code)
    {
    case 100:
      return "Continue";
    case 101:
      return "Switching Protocols";
    case 102:
      return "Processing";
    case 200:
      return "OK";
    case 201:
      return "Created";
    case 202:
      return "Accepted";
    case 204:
      return "No Content";
    case 301:
      return "Moved Permanently";
    case 302:
      return "Found";
    case 304:
      return "Not Modified";
    case 400:
      return "Bad Request";
    case 401:
      return "Unauthorized";
    case 403:
      return "Forbidden";
    case 404:
      return "Not Found";
    case 409:
      return "Conflict";
    case 422:
      return "Unprocessable Entity";
    case 429:
      return "Too Many Requests";
    case 500:
      return "Internal Server Error";
    case 501:
      return "Not Implemented";
    case 502:
      return "Bad Gateway";
    case 503:
      return "Service Unavailable";
    case 504:
      return "Gateway Timeout";
    default:
      return "Unknown";
    }
  }

  kordex::bindings::NativeFunction Http::make_function(
      ::std::string name,
      ::std::string description,
      ::std::size_t min_args,
      ::std::size_t max_args,
      kordex::bindings::NativeFunctionCallback callback)
  {
    kordex::bindings::NativeFunctionInfo info;
    info.name = ::std::move(name);
    info.module_name = "http";
    info.description = ::std::move(description);
    info.min_args = min_args;
    info.max_args = max_args;
    info.callable = true;
    info.safe = true;

    return kordex::bindings::NativeFunction::create(
        info,
        ::std::move(callback));
  }

  kordex::bindings::Result<::std::string> Http::string_argument(
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

  kordex::bindings::Result<double> Http::number_argument(
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

  ::std::string Http::uppercase(
      ::std::string value)
  {
    ::std::transform(
        value.begin(),
        value.end(),
        value.begin(),
        [](unsigned char character)
        {
          return static_cast<char>(::std::toupper(character));
        });

    return value;
  }

  Result<kordex::bindings::NativeModule> create_http_module(
      const StdOptions &options)
  {
    auto http = Http::create(options);
    if (!http)
    {
      return http.error();
    }

    return http.value().module();
  }

} // namespace kordex::standard
