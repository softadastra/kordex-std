# Changelog

All notable changes to `kordex::standard` will be documented in this file.

The format follows a simple release-oriented structure.

## [0.1.0] - 2026-01-01

### Added

- Added initial `kordex::standard` module.
- Added public umbrella header:

```cpp
#include <kordex/std/Std.hpp>
```

- Added safe C++ namespace:

```cpp
namespace kordex::standard
```

- Kept public include path under:

```
include/kordex/std/
```

- Added version API:
  - `version()`
  - `version_major()`
  - `version_minor()`
  - `version_patch()`
- Added shared std error model:
  - `StdErrorCode`
  - `make_std_error()`
  - `ok()`
  - `to_error_code()`
  - `default_message()`
- Added result aliases:
  - `Result<T>`
  - `BoolResult`
  - `StringResult`
  - `IntResult`
- Added standard module options:
  - `StdOptions::defaults()`
  - `StdOptions::development()`
  - `StdOptions::production()`
  - `StdOptions::test()`
  - `StdOptions::minimal()`
- Added normalized standard module config:
  - `StdConfig`
  - module enable flags
  - native module permission flags
  - native function permission flags
  - registry name
  - namespace name
  - module path support
  - max module count support
- Added `console` standard module:
  - `log`
  - `info`
  - `warn`
  - `error`
  - `debug`
- Added `fs` standard module:
  - `exists`
  - `isFile`
  - `isDirectory`
  - `readText`
  - `writeText`
  - `remove`
- Added `path` standard module:
  - `normalize`
  - `join`
  - `dirname`
  - `basename`
  - `extension`
  - `isAbsolute`
  - `isRelative`
  - `separator` value
- Added `env` standard module:
  - `get`
  - `has`
  - `set`
  - `unset`
- Added `process` standard module:
  - `cwd`
  - `chdir`
  - `run`
- Added `timer` standard module:
  - `now`
  - `sleep`
  - `unixMs`
- Added `crypto` standard module:
  - `hash`
  - `random`
  - `randomInt`
  - `equals`
  - deterministic FNV-1a hexadecimal helper
- Added `http` standard module:
  - `isSuccess`
  - `isRedirect`
  - `isClientError`
  - `isServerError`
  - `statusText`
  - `buildUrl`
  - `normalizeMethod`
  - `isMethod`
  - `GET` value
  - `POST` value
- Added `ModuleFactory`:
  - create one standard native module by name
  - create all enabled modules
  - support plain names and `kordex:` names
  - canonical module name handling
  - enabled module listing
- Added `StdRegistry`:
  - create and own standard native modules
  - initialize and shutdown lifecycle
  - register one module
  - register all enabled modules
  - install modules into `kordex::bindings::ModuleRegistry`
  - install modules into `kordex::bindings::EngineContext`
  - install modules into `kordex::bindings::Engine`
- Added high-level public helpers:
  - `package_name()`
  - `package_description()`
  - `default_namespace()`
  - `default_registry_name()`
  - `build_enabled_modules()`
  - `create_factory()`
  - `create_registry()`
  - `create_initialized_registry()`
  - `create_module()`
  - `create_modules()`
  - `install()`
- Added examples:
  - `console.cpp`
  - `fs.cpp`
  - `path.cpp`
  - `env.cpp`
  - `process.cpp`
  - `timer.cpp`
  - `registry.cpp`
- Added tests for:
  - version API
  - error model
  - std options
  - std config
  - console module
  - fs module
  - path module
  - env module
  - process module
  - timer module
  - crypto module
  - http module
  - module factory
  - std registry
- Added CMake package support:
  - `kordex::std`
  - standalone build
  - umbrella build support
  - install/export support
  - package config support

### Notes

- The include path uses `kordex/std`, but the C++ namespace is `kordex::standard`.
- `namespace kordex::std` is intentionally avoided because it conflicts with the C++ standard namespace.
- The `process` module is marked unsafe.
- Production options disable `process` and `http` by default.
- The first `crypto` implementation provides utility primitives only.
- Real cryptographic and HTTP backends can be connected later without changing the public module shape.

## Roadmap

### Planned

- Expose modules through real JavaScript `kordex:` imports.
- Connect `std` modules to the bindings engine import system.
- Add richer filesystem APIs.
- Add async timer APIs.
- Connect process helpers to `kordex::runtime::Process`.
- Replace placeholder crypto helpers with stronger backends.
- Connect HTTP helpers to real client/fetch APIs.
- Add permission checks per standard module.
- Add structured diagnostics for module calls.
- Add better runtime bridge integration.
