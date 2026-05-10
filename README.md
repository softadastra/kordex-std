# Kordex Std

Standard native modules for Kordex.

`kordex::standard` provides the built-in standard modules used by Kordex applications. It exposes native modules such as `console`, `fs`, `path`, `env`, `process`, `timer`, `crypto`, and `http` through the `kordex::bindings` layer.

The module is designed to be installed into:

- a `kordex::bindings::ModuleRegistry`
- a `kordex::bindings::EngineContext`
- a `kordex::bindings::Engine`

## Purpose

Kordex Std is the standard library layer of Kordex.

It gives scripts access to safe, structured native modules while keeping the implementation in C++.

The dependency direction is:

```txt
kordex::std
  -> kordex::bindings
  -> kordex::runtime
  -> vix modules
```

Runtime and bindings must not depend on std.

## Namespace

The folder path is:

```
include/kordex/std/
```

But the C++ namespace is:

```cpp
namespace kordex::standard
```

Do not use `namespace kordex::std`, because it conflicts with C++ `std`.

## Modules

Kordex Std currently provides:

| Module | Description |
|---|---|
| `console` | console logging helpers |
| `fs` | filesystem helpers |
| `path` | path manipulation helpers |
| `env` | environment variable helpers |
| `process` | process helpers |
| `timer` | timing helpers |
| `crypto` | utility crypto-like helpers |
| `http` | HTTP utility helpers |

## Public include

Use the umbrella header:

```cpp
#include <kordex/std/Std.hpp>
```

This includes the full public Kordex Std API.

## Quick example

```cpp
#include <iostream>

#include <kordex/std/Std.hpp>

int main()
{
  auto module_result = kordex::standard::create_module("console");
  if (!module_result)
  {
    std::cerr << module_result.error().message() << '\n';
    return 1;
  }

  auto module = std::move(module_result.value());

  auto call_result = module.call(
      "log",
      {kordex::bindings::Value::string("Hello from Kordex Std")});

  if (!call_result)
  {
    std::cerr << call_result.error().message() << '\n';
    return 1;
  }

  return 0;
}
```

## Main concepts

### StdOptions

`StdOptions` is the user-facing configuration object.

It controls which modules are enabled.

```cpp
auto options = kordex::standard::StdOptions::minimal();
```

Available presets:

```cpp
kordex::standard::StdOptions::defaults();
kordex::standard::StdOptions::development();
kordex::standard::StdOptions::production();
kordex::standard::StdOptions::test();
kordex::standard::StdOptions::minimal();
```

### StdConfig

`StdConfig` is the normalized internal configuration.

```cpp
auto config = kordex::standard::StdConfig::from_options(
    kordex::standard::StdOptions::defaults());
```

It validates:

- registry name
- namespace name
- enabled modules
- native module support
- native function support
- max module count
- module paths

### ModuleFactory

`ModuleFactory` creates standard modules by name.

```cpp
auto factory_result = kordex::standard::ModuleFactory::create();

auto console = factory_result.value().create_module("console");
auto fs = factory_result.value().create_module("kordex:fs");
```

Accepted names:

```
console / kordex:console
fs      / kordex:fs
path    / kordex:path
env     / kordex:env
process / kordex:process
timer   / kordex:timer
crypto  / kordex:crypto
http    / kordex:http
```

### StdRegistry

`StdRegistry` creates, owns, and installs standard modules.

```cpp
auto registry_result = kordex::standard::create_initialized_registry(
    kordex::standard::StdOptions::minimal());

auto registry = std::move(registry_result.value());
```

It can install modules into a bindings registry:

```cpp
kordex::bindings::ModuleRegistry module_registry;

auto error = registry.install_into(module_registry);
if (error)
{
  return 1;
}
```

It can also install modules into an engine context or engine:

```cpp
kordex::bindings::EngineContext context;
context.initialize();

auto error = kordex::standard::install(context);
```

## Console module

Creates `console`.

Functions:

- `log(...)`
- `info(...)`
- `warn(...)`
- `error(...)`
- `debug(...)`

Example:

```cpp
auto module = kordex::standard::create_module("console").value();

module.call(
    "log",
    {kordex::bindings::Value::string("Hello")});
```

## Fs module

Creates `fs`.

Functions:

- `exists(path)`
- `isFile(path)`
- `isDirectory(path)`
- `readText(path)`
- `writeText(path, content)`
- `remove(path)`

Example:

```cpp
auto module = kordex::standard::create_module("fs").value();

module.call(
    "writeText",
    {kordex::bindings::Value::string("hello.txt"),
     kordex::bindings::Value::string("Hello")});
```

## Path module

Creates `path`.

Functions:

- `normalize(path)`
- `join(...segments)`
- `dirname(path)`
- `basename(path)`
- `extension(path)`
- `isAbsolute(path)`
- `isRelative(path)`

Values:

- `name`
- `namespace`
- `separator`

Example:

```cpp
auto module = kordex::standard::create_module("path").value();

auto result = module.call(
    "join",
    {kordex::bindings::Value::string("app"),
     kordex::bindings::Value::string("src"),
     kordex::bindings::Value::string("main.js")});
```

## Env module

Creates `env`.

Functions:

- `get(name)`
- `has(name)`
- `set(name, value)`
- `unset(name)`

Example:

```cpp
auto module = kordex::standard::create_module("env").value();

module.call(
    "set",
    {kordex::bindings::Value::string("KORDEX_ENV"),
     kordex::bindings::Value::string("development")});
```

## Process module

Creates `process`.

Functions:

- `cwd()`
- `chdir(path)`
- `run(command)`

`process` is marked unsafe because it can change process state and run commands.

Production options disable it by default.

Example:

```cpp
auto module = kordex::standard::create_module("process").value();

auto cwd = module.call("cwd", {});
```

## Timer module

Creates `timer`.

Functions:

- `now()`
- `sleep(milliseconds)`
- `unixMs()`

Example:

```cpp
auto module = kordex::standard::create_module("timer").value();

module.call(
    "sleep",
    {kordex::bindings::Value::number(10.0)});
```

## Crypto module

Creates `crypto`.

Functions:

- `hash(input)`
- `random()`
- `randomInt(min, max)`
- `equals(left, right)`

The first version uses deterministic utility primitives. Real cryptographic backends can be connected later behind the same API.

Example:

```cpp
auto module = kordex::standard::create_module("crypto").value();

auto hash = module.call(
    "hash",
    {kordex::bindings::Value::string("hello")});
```

## Http module

Creates `http`.

Functions:

- `isSuccess(status)`
- `isRedirect(status)`
- `isClientError(status)`
- `isServerError(status)`
- `statusText(status)`
- `buildUrl(base, path)`
- `normalizeMethod(method)`
- `isMethod(method)`

Values:

- `GET`
- `POST`

Production options disable `http` by default.

Example:

```cpp
auto module = kordex::standard::create_module("http").value();

auto ok = module.call(
    "isSuccess",
    {kordex::bindings::Value::number(200.0)});
```

## Build

From the module directory:

```bash
vix build --build-target all -v
```

Or with CMake:

```bash
cmake -S . -B build-ninja -G Ninja \
  -DCMAKE_BUILD_TYPE=Debug \
  -DKORDEX_STD_BUILD_TESTS=ON \
  -DKORDEX_STD_BUILD_EXAMPLES=ON

cmake --build build-ninja
```

## Build options

```
KORDEX_STD_BUILD_TESTS=OFF
KORDEX_STD_BUILD_EXAMPLES=OFF

KORDEX_STD_ENABLE_WARNINGS=ON
KORDEX_STD_ENABLE_SANITIZERS=OFF

KORDEX_STD_ENABLE_CONSOLE=ON
KORDEX_STD_ENABLE_FS=ON
KORDEX_STD_ENABLE_PATH=ON
KORDEX_STD_ENABLE_ENV=ON
KORDEX_STD_ENABLE_PROCESS=ON
KORDEX_STD_ENABLE_TIMER=ON
KORDEX_STD_ENABLE_CRYPTO=ON
KORDEX_STD_ENABLE_HTTP=ON

KORDEX_STD_FETCH_BINDINGS=ON
KORDEX_STD_FETCH_RUNTIME=ON

KORDEX_STD_FETCH_ERROR=ON
KORDEX_STD_FETCH_LOG=ON
KORDEX_STD_FETCH_JSON=ON
KORDEX_STD_FETCH_FS=ON
KORDEX_STD_FETCH_PATH=ON
KORDEX_STD_FETCH_ENV=ON
KORDEX_STD_FETCH_PROCESS=ON
KORDEX_STD_FETCH_TIME=ON
KORDEX_STD_FETCH_CRYPTO=ON
KORDEX_STD_FETCH_HTTP=ON
KORDEX_STD_FETCH_TESTS=ON

KORDEX_VIX_GIT_TAG=main
KORDEX_RUNTIME_GIT_TAG=main
KORDEX_BINDINGS_GIT_TAG=main
```

## Tests

Enable and run tests:

```bash
cmake -S . -B build-ninja -G Ninja \
  -DKORDEX_STD_BUILD_TESTS=ON

cmake --build build-ninja

ctest --test-dir build-ninja --output-on-failure
```

Test files:

```
tests/
├── test_version.cpp
├── test_error.cpp
├── test_std_options.cpp
├── test_std_config.cpp
├── test_console.cpp
├── test_fs.cpp
├── test_path.cpp
├── test_env.cpp
├── test_process.cpp
├── test_timer.cpp
├── test_crypto.cpp
├── test_http.cpp
├── test_module_factory.cpp
└── test_std_registry.cpp
```

## Examples

Build examples:

```bash
cmake -S . -B build-ninja -G Ninja \
  -DKORDEX_STD_BUILD_EXAMPLES=ON

cmake --build build-ninja
```

Examples:

```
examples/
├── console.cpp
├── fs.cpp
├── path.cpp
├── env.cpp
├── process.cpp
├── timer.cpp
└── registry.cpp
```

## Project structure

```
modules/std/
├── README.md
├── CHANGELOG.md
├── LICENSE
├── CMakeLists.txt
├── vix.json
├── .gitignore
├── cmake/
│   ├── KordexStdConfig.cmake.in
│   └── KordexStdOptions.cmake
├── include/kordex/std/
│   ├── Version.hpp
│   ├── Error.hpp
│   ├── Result.hpp
│   ├── StdOptions.hpp
│   ├── StdConfig.hpp
│   ├── Console.hpp
│   ├── Fs.hpp
│   ├── Path.hpp
│   ├── Env.hpp
│   ├── Process.hpp
│   ├── Timer.hpp
│   ├── Crypto.hpp
│   ├── Http.hpp
│   ├── ModuleFactory.hpp
│   ├── StdRegistry.hpp
│   └── Std.hpp
├── src/
│   ├── Version.cpp
│   ├── Error.cpp
│   ├── StdOptions.cpp
│   ├── StdConfig.cpp
│   ├── Console.cpp
│   ├── Fs.cpp
│   ├── Path.cpp
│   ├── Env.cpp
│   ├── Process.cpp
│   ├── Timer.cpp
│   ├── Crypto.cpp
│   ├── Http.cpp
│   ├── ModuleFactory.cpp
│   ├── StdRegistry.cpp
│   └── Std.cpp
├── tests/
└── examples/
```

## Design rules

Kordex Std owns standard native modules.

Kordex Bindings owns the script-facing binding layer.

Kordex Runtime owns runtime execution behavior.

- `std` may depend on `bindings`
- `bindings` may depend on `runtime`
- `runtime` must not depend on `std`

This keeps the architecture modular and avoids circular dependencies.

## Roadmap

Planned next steps:

- connect modules to real JavaScript imports
- expose std modules through `kordex:` import specifiers
- add richer fs operations
- add async timer APIs
- connect process helpers to `kordex::runtime::Process`
- replace placeholder crypto helpers with stronger backends
- connect HTTP helpers to real fetch/client APIs
- add permissions per module
- improve diagnostics and structured errors

## License

MIT License.
