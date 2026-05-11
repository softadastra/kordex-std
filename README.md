# Kordex Std

Standard native modules for Kordex.

`kordex-std` provides the native standard modules exposed to JavaScript and TypeScript scripts through the Kordex bindings layer.

It defines modules such as:

```txt
kordex:console
kordex:fs
kordex:path
kordex:env
kordex:process
kordex:timer
kordex:crypto
kordex:http
```

These modules are implemented in C++ and exposed to scripts through `kordex-bindings`.

## Role

`kordex-std` is the standard library layer of Kordex.

It is responsible for:

- creating native modules
- creating native functions
- registering standard modules into `ModuleRegistry`
- installing standard modules into `EngineContext`
- installing standard modules into `Engine`
- respecting runtime permissions
- exposing safe utility APIs to JavaScript

The module does not execute JavaScript directly.
Execution is handled by `kordex-bindings`.

## Public headers

```txt
include/kordex/std/
├── Console.hpp
├── Crypto.hpp
├── Env.hpp
├── Error.hpp
├── Fs.hpp
├── Http.hpp
├── ModuleFactory.hpp
├── Path.hpp
├── Process.hpp
├── Result.hpp
├── Std.hpp
├── StdConfig.hpp
├── StdOptions.hpp
├── StdRegistry.hpp
├── Timer.hpp
└── Version.hpp
```

## Features

- Console module
- Filesystem module
- Path module
- Environment module
- Process module
- Timer module
- Crypto utility module
- HTTP utility module
- Standard module factory
- Standard module registry
- Native module installation into bindings
- Permission-aware configuration
- Safe default configuration
- Development, production, test, and minimal presets

## Standard modules

### `kordex:console`

Console output helpers.

Available functions:

- `log`
- `info`
- `warn`
- `error`
- `debug`

Example:

```js
import { log } from "kordex:console";

log("Hello from Kordex");
```

### `kordex:path`

Path manipulation helpers.

Available functions:

- `normalize`
- `join`
- `dirname`
- `basename`
- `extension`
- `is_absolute`
- `is_relative`

Example:

```js
import { join } from "kordex:path";

join("/tmp", "kordex", "app");
```

Expected result:

```txt
/tmp/kordex/app
```

### `kordex:fs`

Filesystem helpers.

Available functions:

- `exists`
- `is_file`
- `is_directory`
- `read_text`
- `write_text`
- `remove`

Example:

```js
import { exists } from "kordex:fs";

exists("/tmp");
```

This module should only be installed when filesystem access is allowed.

### `kordex:env`

Environment variable helpers.

Available functions:

- `get`
- `has`
- `set`
- `unset`

Example:

```js
import { get } from "kordex:env";

get("HOME");
```

This module should only be installed when environment access is allowed.

### `kordex:process`

Process helpers.

Available functions:

- `cwd`
- `chdir`
- `run`

Example:

```js
import { cwd } from "kordex:process";

cwd();
```

This module should only be installed when process access is allowed.

### `kordex:timer`

Timer and time helpers.

Available functions:

- `now`
- `sleep`
- `unix_ms`

Example:

```js
import { unix_ms } from "kordex:timer";

unix_ms();
```

### `kordex:crypto`

Crypto utility helpers.

Available functions:

- `hash`
- `random`
- `random_int`
- `equals`

Example:

```js
import { hash } from "kordex:crypto";

hash("hello");
```

This first version provides deterministic utility primitives.
Strong cryptographic backends can be connected later behind the same API.

### `kordex:http`

HTTP utility helpers.

Available functions:

- `is_success`
- `is_redirect`
- `is_client_error`
- `is_server_error`
- `status_text`
- `build_url`
- `normalize_method`
- `is_method`

Example:

```js
import { status_text } from "kordex:http";

status_text(200);
```

This first version provides safe HTTP helper utilities.
Real network requests can be connected later behind the same module.

## Std options

`StdOptions` is the user-facing configuration for standard modules.

```cpp
kordex::standard::StdOptions options;

options.enabled = true;
options.safe_by_default = true;

options.enable_console = true;
options.enable_fs = true;
options.enable_path = true;
options.enable_env = true;
options.enable_process = false;
options.enable_timer = true;
options.enable_crypto = true;
options.enable_http = false;
```

Factory helpers are available:

```cpp
auto defaults = kordex::standard::StdOptions::defaults();
auto dev = kordex::standard::StdOptions::development();
auto prod = kordex::standard::StdOptions::production();
auto test = kordex::standard::StdOptions::test();
auto minimal = kordex::standard::StdOptions::minimal();
```

## Std config

`StdConfig` is the normalized configuration used internally.

```cpp
auto config_result =
    kordex::standard::StdConfig::from_options(options);

if (!config_result)
{
  return 1;
}

auto config = config_result.value();
```

It validates:

- registry name
- namespace name
- global enable state
- native module permission
- native function permission
- enabled module count
- maximum module count

## Permission model

`kordex-std` is designed to be controlled by runtime permissions.

The CLI maps runtime permissions to standard modules like this:

```txt
RuntimeConfig.allow_fs      -> StdOptions.enable_fs
RuntimeConfig.allow_env     -> StdOptions.enable_env
RuntimeConfig.allow_process -> StdOptions.enable_process
RuntimeConfig.allow_net     -> StdOptions.enable_http
```

Safe utility modules can stay enabled by default:

- `console`
- `path`
- `timer`
- `crypto`

Sensitive modules should be explicit:

- `fs`
- `env`
- `process`
- `http`

Example CLI usage:

```bash
kordex run main.js --allow-fs
```

Without `--allow-fs`, `kordex:fs` should not be installed into the script environment.

## Module factory

`ModuleFactory` creates standard native modules by name.

```cpp
kordex::standard::ModuleFactory factory;

auto module = factory.create_module("path");

if (!module)
{
  return 1;
}
```

Supported names:

- `console`
- `fs`
- `path`
- `env`
- `process`
- `timer`
- `crypto`
- `http`

The factory also accepts names with the `kordex:` prefix:

```cpp
auto module = factory.create_module("kordex:path");
```

Create all enabled modules:

```cpp
auto modules = factory.create_all();
```

## Std registry

`StdRegistry` owns the standard native modules and installs them into the bindings layer.

Typical usage:

```cpp
auto registry_result = kordex::standard::create_initialized_registry();

if (!registry_result)
{
  return 1;
}

auto registry = registry_result.value();
```

The registry can install modules into:

- `kordex::bindings::ModuleRegistry`
- `kordex::bindings::EngineContext`
- `kordex::bindings::Engine`

## Installing std modules

Install into a `ModuleRegistry`:

```cpp
kordex::bindings::ModuleRegistry registry;

auto error = kordex::standard::install(
    registry,
    kordex::standard::StdOptions::production());

if (error)
{
  return 1;
}
```

Install into an `EngineContext`:

```cpp
kordex::bindings::EngineContext context(config);

auto init_error = context.initialize();
if (init_error)
{
  return 1;
}

auto error = kordex::standard::install(
    context,
    kordex::standard::StdOptions::production());
```

Install into an `Engine`:

```cpp
auto engine_result = kordex::bindings::create_engine();

if (!engine_result)
{
  return 1;
}

auto engine = std::move(engine_result.value());

auto init = engine.initialize();
if (!init.succeeded())
{
  return 1;
}

auto error = kordex::standard::install(
    engine,
    kordex::standard::StdOptions::production());
```

## JavaScript imports

Once installed, standard modules can be imported by scripts:

```js
import { join } from "kordex:path";

join("/tmp", "kordex", "std");
```

For sensitive modules:

```js
import { exists } from "kordex:fs";

exists("/tmp");
```

The module must be enabled in `StdOptions`.

## Native module shape

Each standard module is exposed as a `kordex::bindings::NativeModule`.

A native module contains native functions:

```cpp
kordex::bindings::NativeFunction
```

The bindings layer converts them into importable script functions.

The JavaScript bridge calls:

```js
__kordex_call_native(moduleName, functionName, ...args)
```

Then the QuickJS backend calls the C++ native function and converts the result back to JavaScript.

## Value types

Standard module functions return `kordex::bindings::Value`.

Supported value types:

- `undefined`
- `null`
- `boolean`
- `number`
- `string`

Example:

```cpp
return kordex::bindings::Value::string("hello");
```

Boolean:

```cpp
return kordex::bindings::Value::boolean(true);
```

Number:

```cpp
return kordex::bindings::Value::number(42.0);
```

## Error handling

`kordex-std` uses structured errors and `Result<T>`.

```cpp
auto module = kordex::standard::create_fs_module();

if (!module)
{
  auto message = module.error().message();
}
```

Standard errors use `StdErrorCode`:

- `None`
- `InvalidArgument`
- `InvalidConfig`
- `PermissionDenied`
- `ModuleDisabled`
- `ModuleRegistrationFailed`
- `ConsoleError`
- `FsError`
- `PathError`
- `EnvError`
- `ProcessError`
- `TimerError`
- `CryptoError`
- `HttpError`
- `InternalError`

## Console example

```cpp
auto module = kordex::standard::create_console_module();

if (!module)
{
  return 1;
}
```

JavaScript:

```js
import { log } from "kordex:console";

log("Hello");
```

## Path example

```cpp
auto module = kordex::standard::create_path_module();

if (!module)
{
  return 1;
}
```

JavaScript:

```js
import { join, basename } from "kordex:path";

join("/tmp", "app");
basename("/tmp/app/main.js");
```

## Filesystem example

```cpp
auto options = kordex::standard::StdOptions::production();
options.enable_fs = true;

auto module = kordex::standard::create_fs_module(options);
```

JavaScript:

```js
import { read_text } from "kordex:fs";

read_text("README.md");
```

## Environment example

```js
import { has, get } from "kordex:env";

has("HOME");
get("HOME");
```

## Process example

```js
import { cwd } from "kordex:process";

cwd();
```

## Timer example

```js
import { now, unix_ms } from "kordex:timer";

now();
unix_ms();
```

## Crypto example

```js
import { hash, random_int } from "kordex:crypto";

hash("kordex");
random_int(1, 10);
```

## HTTP example

```js
import { is_success, status_text } from "kordex:http";

is_success(200);
status_text(404);
```

## Build from source

From the module directory:

```bash
vix build \
  --preset dev-ninja
```

With tests:

```bash
vix build \
  --preset dev-ninja \
  -- \
  -DKORDEX_STD_BUILD_TESTS=ON

vix tests -- --output-on-failure
```

With examples:

```bash
vix build \
  --preset dev-ninja \
  -- \
  -DKORDEX_STD_BUILD_EXAMPLES=ON
```

## Tests

The std tests should cover:

- `StdOptions`
- `StdConfig`
- `ModuleFactory`
- `StdRegistry`
- module creation
- module installation
- console module
- fs module
- path module
- env module
- process module
- timer module
- crypto module
- http module
- disabled modules
- max module count
- permission-gated module behavior

Run all tests:

```bash
vix tests
```

Run tests with raw CTest failure output:

```bash
vix tests -- --output-on-failure
```

Run one test by name or regex:

```bash
vix tests -R std
```

## Run examples

Run a standard module example:

```bash
vix run examples/std_console.cpp
```

Run another example:

```bash
vix run examples/std_modules.cpp
```

## Integration with CLI

When the user runs:

```bash
kordex run main.js --allow-fs
```

The CLI creates runtime options:

```txt
allow_fs = true
```

Then converts them into std options:

```txt
enable_fs = true
```

Then installs the std modules into the engine before script execution.

Pipeline:

```txt
CLI flags
-> RuntimeOptions
-> RuntimeConfig
-> StdOptions
-> standard::install(engine, options)
-> ModuleRegistry
-> ModuleLoader
-> QuickJS native bridge
-> JavaScript import "kordex:fs"
```

## Current limitations

- `crypto` currently provides deterministic utility primitives, not a full cryptographic backend
- `http` currently provides helper utilities, not real network requests
- object and function value conversion is limited by the current bindings `Value` model
- async JavaScript timers are not fully modeled yet
- module names are exposed exactly as registered by each native module
- permission checks are mostly enforced by module installation policy at the CLI/runtime layer

## Relationship with other Kordex modules

```txt
kordex-runtime
  -> provides runtime permissions and execution foundation

kordex-bindings
  -> exposes native modules to JavaScript

kordex-std
  -> creates the native standard modules

kordex-cli
  -> installs std modules according to CLI flags
```

## Module philosophy

`kordex-std` should stay:

- small
- explicit
- permission-aware
- engine-independent
- easy to test
- safe by default
- compatible with the bindings value model

It should expose only capabilities that the runtime has explicitly allowed.

## License

MIT License.
