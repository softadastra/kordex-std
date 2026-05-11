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

# ifndef(KORDEX_STD_OPTIONS_INCLUDED)
set(KORDEX_STD_OPTIONS_INCLUDED ON)

# --------------------------------------------------------------------
# Build options
# --------------------------------------------------------------------
option(KORDEX_STD_BUILD_TESTS "Build Kordex std tests" OFF)
option(KORDEX_STD_BUILD_EXAMPLES "Build Kordex std examples" OFF)

# --------------------------------------------------------------------
# Developer options
# --------------------------------------------------------------------
option(KORDEX_STD_ENABLE_WARNINGS "Enable compiler warnings for Kordex std" ON)
option(KORDEX_STD_ENABLE_SANITIZERS "Enable sanitizers for Kordex std" OFF)

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
    "Git tag or branch used for kordex runtime")

set(KORDEX_BINDINGS_GIT_TAG
    "main"
    CACHE STRING
    "Git tag or branch used for kordex bindings")

# --------------------------------------------------------------------
# Dependency fetch options
# --------------------------------------------------------------------
option(KORDEX_STD_FETCH_BINDINGS "Auto-fetch kordex::bindings if missing" ON)
option(KORDEX_STD_FETCH_RUNTIME "Auto-fetch kordex::runtime if missing" ON)

option(KORDEX_STD_FETCH_ERROR "Auto-fetch vix::error if missing" ON)
option(KORDEX_STD_FETCH_LOG "Auto-fetch vix::log if missing" ON)
option(KORDEX_STD_FETCH_JSON "Auto-fetch vix::json if missing" ON)
option(KORDEX_STD_FETCH_FS "Auto-fetch vix::fs if missing" ON)
option(KORDEX_STD_FETCH_PATH "Auto-fetch vix::path if missing" ON)
option(KORDEX_STD_FETCH_ENV "Auto-fetch vix::env if missing" ON)
option(KORDEX_STD_FETCH_PROCESS "Auto-fetch vix::process if missing" ON)
option(KORDEX_STD_FETCH_TIME "Auto-fetch vix::time if missing" ON)
option(KORDEX_STD_FETCH_CRYPTO "Auto-fetch vix::crypto if missing" ON)
option(KORDEX_STD_FETCH_HTTP "Auto-fetch vix::http if missing" ON)
option(KORDEX_STD_FETCH_TESTS "Auto-fetch vix::tests if missing" ON)

# --------------------------------------------------------------------
# Umbrella build policy
# --------------------------------------------------------------------
# When Kordex std is built inside the kordex umbrella repository,
# dependencies should be provided by the umbrella build.
#
# In that mode, std must not fetch dependencies by itself.
# The root project is responsible for add_subdirectory order.
# --------------------------------------------------------------------
if(DEFINED KORDEX_UMBRELLA_BUILD AND KORDEX_UMBRELLA_BUILD)
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
  set(KORDEX_STD_FETCH_HTTP OFF CACHE BOOL "Auto-fetch vix::http if missing" FORCE)
  set(KORDEX_STD_FETCH_TESTS OFF CACHE BOOL "Auto-fetch vix::tests if missing" FORCE)
endif()

# endif()
