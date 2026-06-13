# Kordex Std

Standard native modules for Kordex.

`kordex-std` provides the native standard modules exposed to JavaScript and TypeScript programs through the Kordex bindings layer.

It gives Kordex scripts access to small, explicit, permission-aware APIs such as console output, path utilities, filesystem helpers, environment variables, timers, crypto helpers, process helpers, and HTTP utility functions.

## Role

`kordex-std` is the standard library layer of Kordex.

It is responsible for:

- creating native modules
- creating native functions
- registering standard modules into the bindings module registry
- installing standard modules into an engine context
- respecting runtime permissions
- exposing safe utility APIs to JavaScript and TypeScript

`kordex-std` does not execute JavaScript directly.

Script execution is handled by `kordex-bindings`.

## Standard modules

Kordex Std provides these modules:

```txt
kordex:console
kordex:path
kordex:fs
kordex:env
kordex:process
kordex:timer
kordex:crypto
kordex:http
```

Safe utility modules can be enabled by default:

```txt
kordex:console
kordex:path
kordex:timer
kordex:crypto
```

Sensitive modules should be controlled by runtime permissions:

```txt
kordex:fs
kordex:env
kordex:process
kordex:http
```

## JavaScript naming

Kordex Std uses **camelCase** for JavaScript APIs.

Examples:

```js
isAbsolute();
isRelative();
isFile();
isDirectory();
readText();
writeText();
unixMs();
randomInt();
statusText();
buildUrl();
```

C++ internals may use snake_case, but JavaScript users should use camelCase.

## Quick test

Create a file:

```bash
touch main.js
nvim main.js
```

Add:

```js
console.log("Hello from Kordex");
```

Run:

```bash
kordex run main.js
```

Expected output:

```txt
Hello from Kordex
Ran main.js
```

## `kordex:console`

Console output helpers.

Available exports:

```txt
name
namespace
log
info
warn
error
debug
```

Example:

```js
import { log, info, warn, error, debug } from "kordex:console";

log("log works");
info("info works");
warn("warn works");
error("error works");
debug("debug works");
```

Run:

```bash
kordex run main.js
```

Expected output:

```txt
[log] log works
[info] info works
[warn] warn works
[error] error works
[debug] debug works
Ran main.js
```

The global `console` object is also available:

```js
console.log("global console works");
console.error("global console error works");
```

Expected output:

```txt
global console works
global console error works
Ran main.js
```

## `kordex:path`

Path manipulation helpers.

Available exports:

```txt
name
namespace
separator
normalize
join
dirname
basename
extension
isAbsolute
isRelative
```

Example:

```js
import {
  normalize,
  join,
  dirname,
  basename,
  extension,
  isAbsolute,
  isRelative,
  separator,
} from "kordex:path";

console.log(normalize("/tmp/kordex/../kordex/app"));
console.log(join("/tmp", "kordex", "app"));
console.log(dirname("/tmp/kordex/app/main.js"));
console.log(basename("/tmp/kordex/app/main.js"));
console.log(extension("/tmp/kordex/app/main.js"));
console.log(isAbsolute("/tmp/kordex"));
console.log(isRelative("./main.js"));
console.log(separator);
```

Run:

```bash
kordex run main.js
```

Expected output on Linux/macOS:

```txt
/tmp/kordex/app
/tmp/kordex/app
/tmp/kordex/app
main.js
.js
true
true
/
Ran main.js
```

## `kordex:fs`

Filesystem helpers.

Available exports:

```txt
name
namespace
exists
isFile
isDirectory
readText
writeText
remove
```

This module should be enabled only when filesystem access is allowed.

Example:

```js
import {
  exists,
  isFile,
  isDirectory,
  writeText,
  readText,
  remove,
} from "kordex:fs";

const file = "/tmp/kordex-fs-test.txt";

console.log(exists("/tmp"));
console.log(isDirectory("/tmp"));

writeText(file, "Hello from kordex:fs");

console.log(exists(file));
console.log(isFile(file));
console.log(readText(file));

console.log(remove(file));
console.log(exists(file));
```

Run with filesystem permission:

```bash
kordex run main.js --allow-fs
```

Expected output:

```txt
true
true
true
true
Hello from kordex:fs
true
false
Ran main.js
```

Without filesystem permission, importing `kordex:fs` should fail or be blocked by the runtime permission policy.

## `kordex:env`

Environment variable helpers.

Available exports:

```txt
name
namespace
get
has
set
unset
```

This module should be enabled only when environment access is allowed.

Example:

```js
import { get, has, set, unset } from "kordex:env";

console.log(has("HOME"));
console.log(get("HOME") !== null);

set("KORDEX_TEST_ENV", "works");

console.log(has("KORDEX_TEST_ENV"));
console.log(get("KORDEX_TEST_ENV"));

unset("KORDEX_TEST_ENV");

console.log(has("KORDEX_TEST_ENV"));
```

Run with environment permission:

```bash
kordex run main.js --allow-env
```

Expected output:

```txt
true
true
true
works
false
Ran main.js
```

## `kordex:process`

Process helpers.

Available exports:

```txt
name
namespace
cwd
chdir
run
```

This module should be enabled only when process access is allowed.

Example:

```js
import { cwd, chdir, run } from "kordex:process";

console.log(cwd());

chdir("/tmp");

console.log(cwd());

const code = run("true");
console.log(code);
```

Run with process permission:

```bash
kordex run main.js --allow-process
```

Expected output shape:

```txt
/path/where/you-started
/tmp
0
Ran main.js
```

## `kordex:timer`

Timer and time helpers.

Available exports:

```txt
name
namespace
now
sleep
unixMs
```

Example:

```js
import { now, sleep, unixMs } from "kordex:timer";

const before = now();

sleep(10);

const after = now();

console.log(after >= before);
console.log(unixMs() > 0);
```

Run:

```bash
kordex run main.js
```

Expected output:

```txt
true
true
Ran main.js
```

## `kordex:crypto`

Crypto utility helpers.

Available exports:

```txt
name
namespace
hash
random
randomInt
equals
```

This first version provides deterministic utility primitives and random helpers.

Example:

```js
import { hash, random, randomInt, equals } from "kordex:crypto";

console.log(hash("kordex"));
console.log(random() >= 0);
console.log(random() < 1);
console.log(randomInt(1, 10) >= 1);
console.log(equals("hello", "hello"));
console.log(equals("hello", "world"));
```

Run:

```bash
kordex run main.js
```

Expected output shape:

```txt
<hex hash>
true
true
true
true
false
Ran main.js
```

## `kordex:http`

HTTP utility helpers.

Available exports:

```txt
name
namespace
GET
POST
isSuccess
isRedirect
isClientError
isServerError
statusText
buildUrl
normalizeMethod
isMethod
```

This module provides safe HTTP helper utilities.

It does not perform network requests yet.

Example:

```js
import {
  GET,
  POST,
  isSuccess,
  isRedirect,
  isClientError,
  isServerError,
  statusText,
  buildUrl,
  normalizeMethod,
  isMethod,
} from "kordex:http";

console.log(GET);
console.log(POST);

console.log(isSuccess(200));
console.log(isRedirect(302));
console.log(isClientError(404));
console.log(isServerError(500));

console.log(statusText(200));
console.log(statusText(404));

console.log(buildUrl("https://example.com/", "/api/users"));
console.log(normalizeMethod("post"));
console.log(isMethod("PATCH"));
console.log(isMethod("CUSTOM"));
```

Run with network or HTTP permission when the runtime gates HTTP helpers behind `--allow-net`:

```bash
kordex run main.js --allow-net
```

Expected output:

```txt
GET
POST
true
true
true
true
OK
Not Found
https://example.com/api/users
POST
true
false
Ran main.js
```

## Checking exports

You can inspect what a standard module exports.

Example:

```js
import * as path from "kordex:path";

console.log(Object.keys(path).sort().join(", "));
```

Run:

```bash
kordex run main.js
```

Expected output:

```txt
basename, dirname, extension, isAbsolute, isRelative, join, name, namespace, normalize, separator
Ran main.js
```

## Permission model

Kordex Std is designed to be controlled by runtime permissions.

The CLI maps runtime permissions to standard modules like this:

```txt
RuntimeConfig.allow_fs      -> StdOptions.enable_fs
RuntimeConfig.allow_env     -> StdOptions.enable_env
RuntimeConfig.allow_process -> StdOptions.enable_process
RuntimeConfig.allow_net     -> StdOptions.enable_http
```

Safe modules can stay enabled by default:

```txt
console
path
timer
crypto
```

Sensitive modules should require explicit permission:

```txt
fs
env
process
http
```

Examples:

```bash
kordex run main.js --allow-fs
kordex run main.js --allow-env
kordex run main.js --allow-process
kordex run main.js --allow-net
```

## Native module shape

Each standard module is exposed as a `kordex::bindings::NativeModule`.

A native module can contain:

- constant values
- native functions

The bindings layer converts native modules into importable JavaScript modules.

When JavaScript calls a native function, the generated module bridge calls:

```js
__kordex_call_native(moduleName, functionName, ...args);
```

Then the QuickJS backend calls the C++ native function and converts the result back into a JavaScript value.

## Supported value types

Standard module functions return `kordex::bindings::Value`.

Supported primitive value types:

```txt
undefined
null
boolean
number
string
```

Examples in C++:

```cpp
return kordex::bindings::Value::undefined();
return kordex::bindings::Value::null();
return kordex::bindings::Value::boolean(true);
return kordex::bindings::Value::number(42.0);
return kordex::bindings::Value::string("hello");
```

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

## C++ usage

Create a module factory:

```cpp
#include <kordex/std/Std.hpp>

int main()
{
  auto factory_result = kordex::standard::create_factory();

  if (!factory_result)
  {
    return 1;
  }

  auto factory = factory_result.value();

  auto module_result = factory.create_module("path");

  if (!module_result)
  {
    return 1;
  }

  return 0;
}
```

Create all enabled modules:

```cpp
#include <kordex/std/Std.hpp>

int main()
{
  auto modules_result = kordex::standard::create_modules();

  if (!modules_result)
  {
    return 1;
  }

  auto modules = modules_result.value();

  return modules.empty() ? 1 : 0;
}
```

Install standard modules into an engine:

```cpp
#include <kordex/bindings/Bindings.hpp>
#include <kordex/std/Std.hpp>

int main()
{
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

  if (error)
  {
    return 1;
  }

  return 0;
}
```

## Std options

`StdOptions` controls which modules are enabled.

```cpp
kordex::standard::StdOptions options;

options.enabled = true;
options.safe_by_default = true;

options.enable_console = true;
options.enable_path = true;
options.enable_timer = true;
options.enable_crypto = true;

options.enable_fs = false;
options.enable_env = false;
options.enable_process = false;
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

## Error handling

`kordex-std` uses structured errors and `Result<T>`.

Example:

```cpp
auto module = kordex::standard::create_path_module();

if (!module)
{
  auto message = module.error().message();
  return 1;
}
```

Standard errors use `StdErrorCode`:

```txt
None
InvalidArgument
InvalidConfig
PermissionDenied
ModuleDisabled
ModuleRegistrationFailed
ConsoleError
FsError
PathError
EnvError
ProcessError
TimerError
CryptoError
HttpError
InternalError
```

## Build from source

From the Kordex repository root:

```bash
vix build --preset dev-ninja --build-target all -v -- \
  -DKORDEX_ENABLE_QUICKJS=ON \
  -DKORDEX_ENABLE_NATIVE_ENGINE=OFF \
  -DKORDEX_BUILD_APP=ON \
  -DKORDEX_ENABLE_INSTALL=ON
```

Install the CLI:

```bash
sudo cmake --install build-ninja --prefix /usr/local
hash -r
```

Check:

```bash
kordex version
kordex help
```

## Run tests

Run all tests:

```bash
vix tests
```

Run tests with raw failure output:

```bash
vix tests -- --output-on-failure
```

Run tests by regex:

```bash
vix tests -R std
```

## Test checklist

Use this checklist when validating Kordex Std from JavaScript:

```txt
global console
kordex:console
kordex:path
kordex:fs with --allow-fs
kordex:env with --allow-env
kordex:process with --allow-process
kordex:timer
kordex:crypto
kordex:http with --allow-net
```

## Current limitations

- `kordex:http` provides helper utilities, not real network requests yet.
- `kordex:crypto` currently provides utility primitives and random helpers, not a complete cryptography backend.
- JavaScript async timers are not fully modeled yet.
- Object and function value conversion is limited by the current bindings value model.
- Permission checks are mostly enforced by module installation policy at the CLI/runtime layer.

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

## Design philosophy

Kordex Std should stay:

```txt
small
explicit
permission-aware
engine-independent
easy to test
safe by default
compatible with the bindings value model
```

The standard modules should expose only capabilities that the runtime has explicitly allowed.

## License

MIT License.
