#  @file KordexStdOptions.cmake
#  @author Softadastra
#
#  Copyright 2026, Softadastra. All rights reserved.
#  https://github.com/softadastra/kordex-std
#  Use of this source code is governed by a MIT license
#  that can be found in the LICENSE file.
#
# ====================================================================
# Kordex Std - Build Options
# ====================================================================

include_guard(GLOBAL)

set(KORDEX_STD_OPTIONS_INCLUDED ON)

# --------------------------------------------------------------------
# Build options
# --------------------------------------------------------------------
option(KORDEX_STD_BUILD_TESTS "Build Kordex Std tests" OFF)
option(KORDEX_STD_BUILD_EXAMPLES "Build Kordex Std examples" OFF)

# --------------------------------------------------------------------
# Install options
# --------------------------------------------------------------------
option(KORDEX_STD_ENABLE_INSTALL
    "Generate install/export rules for Kordex Std"
    OFF)

# --------------------------------------------------------------------
# Developer options
# --------------------------------------------------------------------
option(KORDEX_STD_ENABLE_WARNINGS "Enable compiler warnings for Kordex Std" ON)
option(KORDEX_STD_ENABLE_SANITIZERS "Enable sanitizers for Kordex Std" OFF)

# --------------------------------------------------------------------
# Standard module options
# --------------------------------------------------------------------
option(KORDEX_STD_ENABLE_CONSOLE "Enable kordex:console standard module" ON)
option(KORDEX_STD_ENABLE_FS "Enable kordex:fs standard module" ON)
option(KORDEX_STD_ENABLE_PATH "Enable kordex:path standard module" ON)
option(KORDEX_STD_ENABLE_ENV "Enable kordex:env standard module" ON)
option(KORDEX_STD_ENABLE_PROCESS "Enable kordex:process standard module" ON)
option(KORDEX_STD_ENABLE_TIMER "Enable kordex:timer standard module" ON)
option(KORDEX_STD_ENABLE_CRYPTO "Enable kordex:crypto standard module" ON)
option(KORDEX_STD_ENABLE_HTTP "Enable kordex:http standard module" ON)

# --------------------------------------------------------------------
# Dependency version policy
# --------------------------------------------------------------------
set(KORDEX_VIX_GIT_TAG
    "main"
    CACHE STRING
    "Git tag or branch used for Vix dependencies")

set(KORDEX_RUNTIME_GIT_TAG
    "main"
    CACHE STRING
    "Git tag or branch used for Kordex Runtime")

set(KORDEX_BINDINGS_GIT_TAG
    "main"
    CACHE STRING
    "Git tag or branch used for Kordex Bindings")

# --------------------------------------------------------------------
# Dependency fetch policy
# --------------------------------------------------------------------
option(KORDEX_STD_FETCH_KORDEX_DEPS "Auto-fetch missing Kordex dependencies" ON)
option(KORDEX_STD_FETCH_VIX_DEPS "Auto-fetch missing Vix dependencies" ON)
option(KORDEX_STD_FETCH_TESTS "Auto-fetch vix::tests if missing" ON)

set(KORDEX_STD_FETCH_BINDINGS
    ${KORDEX_STD_FETCH_KORDEX_DEPS}
    CACHE BOOL
    "Auto-fetch kordex::bindings if missing")

set(KORDEX_STD_FETCH_RUNTIME
    ${KORDEX_STD_FETCH_KORDEX_DEPS}
    CACHE BOOL
    "Auto-fetch kordex::runtime if missing")

set(KORDEX_STD_FETCH_ERROR
    ${KORDEX_STD_FETCH_VIX_DEPS}
    CACHE BOOL
    "Auto-fetch vix::error if missing")

set(KORDEX_STD_FETCH_LOG
    ${KORDEX_STD_FETCH_VIX_DEPS}
    CACHE BOOL
    "Auto-fetch vix::log if missing")

set(KORDEX_STD_FETCH_JSON
    ${KORDEX_STD_FETCH_VIX_DEPS}
    CACHE BOOL
    "Auto-fetch vix::json if missing")

set(KORDEX_STD_FETCH_FS
    ${KORDEX_STD_FETCH_VIX_DEPS}
    CACHE BOOL
    "Auto-fetch vix::fs if missing")

set(KORDEX_STD_FETCH_PATH
    ${KORDEX_STD_FETCH_VIX_DEPS}
    CACHE BOOL
    "Auto-fetch vix::path if missing")

set(KORDEX_STD_FETCH_ENV
    ${KORDEX_STD_FETCH_VIX_DEPS}
    CACHE BOOL
    "Auto-fetch vix::env if missing")

set(KORDEX_STD_FETCH_PROCESS
    ${KORDEX_STD_FETCH_VIX_DEPS}
    CACHE BOOL
    "Auto-fetch vix::process if missing")

set(KORDEX_STD_FETCH_TIME
    ${KORDEX_STD_FETCH_VIX_DEPS}
    CACHE BOOL
    "Auto-fetch vix::time if missing")

set(KORDEX_STD_FETCH_CRYPTO
    ${KORDEX_STD_FETCH_VIX_DEPS}
    CACHE BOOL
    "Auto-fetch vix::crypto if missing")

set(KORDEX_STD_FETCH_CORE
    ${KORDEX_STD_FETCH_VIX_DEPS}
    CACHE BOOL
    "Auto-fetch vix::core if missing")

# --------------------------------------------------------------------
# Umbrella build policy
# --------------------------------------------------------------------
if(DEFINED KORDEX_UMBRELLA_BUILD AND KORDEX_UMBRELLA_BUILD)
  set(KORDEX_STD_FETCH_KORDEX_DEPS OFF CACHE BOOL "Auto-fetch missing Kordex dependencies" FORCE)
  set(KORDEX_STD_FETCH_VIX_DEPS OFF CACHE BOOL "Auto-fetch missing Vix dependencies" FORCE)
  set(KORDEX_STD_FETCH_TESTS OFF CACHE BOOL "Auto-fetch vix::tests if missing" FORCE)

  set(KORDEX_STD_FETCH_BINDINGS OFF CACHE BOOL "Auto-fetch kordex::bindings if missing" FORCE)
  set(KORDEX_STD_FETCH_RUNTIME OFF CACHE BOOL "Auto-fetch kordex::runtime if missing" FORCE)

  set(KORDEX_STD_FETCH_ERROR OFF CACHE BOOL "Auto-fetch vix::error if missing" FORCE)
  set(KORDEX_STD_FETCH_LOG OFF CACHE BOOL "Auto-fetch vix::log if missing" FORCE)
  set(KORDEX_STD_FETCH_JSON OFF CACHE BOOL "Auto-fetch vix::json if missing" FORCE)
  set(KORDEX_STD_FETCH_FS OFF CACHE BOOL "Auto-fetch vix::fs if missing" FORCE)
  set(KORDEX_STD_FETCH_PATH OFF CACHE BOOL "Auto-fetch vix::path if missing" FORCE)
  set(KORDEX_STD_FETCH_ENV OFF CACHE BOOL "Auto-fetch vix::env if missing" FORCE)
  set(KORDEX_STD_FETCH_PROCESS OFF CACHE BOOL "Auto-fetch vix::process if missing" FORCE)
  set(KORDEX_STD_FETCH_TIME OFF CACHE BOOL "Auto-fetch vix::time if missing" FORCE)
  set(KORDEX_STD_FETCH_CRYPTO OFF CACHE BOOL "Auto-fetch vix::crypto if missing" FORCE)
  set(KORDEX_STD_FETCH_CORE OFF CACHE BOOL "Auto-fetch vix::core if missing" FORCE)
endif()
