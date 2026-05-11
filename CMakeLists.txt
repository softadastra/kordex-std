#  @file CMakeLists.txt
#  @author Softadastra
#
#  Copyright 2026, Softadastra. All rights reserved.
#  https://github.com/softadastra/kordex-std
#  Use of this source code is governed by a MIT license
#  that can be found in the LICENSE file.
#
# ====================================================================
# Kordex Std
# ====================================================================

cmake_minimum_required(VERSION 3.20)

project(kordex_std
    VERSION 0.1.0
    DESCRIPTION "Standard native modules for Kordex."
    LANGUAGES CXX)

include(GNUInstallDirs)
include(CMakePackageConfigHelpers)

include("${CMAKE_CURRENT_SOURCE_DIR}/cmake/KordexStdOptions.cmake")

# --------------------------------------------------------------------
# Global settings
# --------------------------------------------------------------------
set(CMAKE_CXX_STANDARD 20)
set(CMAKE_CXX_STANDARD_REQUIRED ON)
set(CMAKE_CXX_EXTENSIONS OFF)
set(CMAKE_POSITION_INDEPENDENT_CODE ON)

# --------------------------------------------------------------------
# Export set selection
# --------------------------------------------------------------------
if(DEFINED KORDEX_UMBRELLA_BUILD AND KORDEX_UMBRELLA_BUILD)
  set(KORDEX_STD_EXPORT_SET KordexTargets)
else()
  set(KORDEX_STD_EXPORT_SET kordex_stdTargets)
endif()

# --------------------------------------------------------------------
# Helpers
# --------------------------------------------------------------------
function(kordex_std_require_target target_name package_name sibling_dir fetch_option repository tag)
  if(TARGET ${target_name})
    return()
  endif()

  if(DEFINED KORDEX_UMBRELLA_BUILD AND KORDEX_UMBRELLA_BUILD)
    message(FATAL_ERROR
        "[std] Umbrella build requires target '${target_name}' to be provided by the root project.")
  endif()

  set(local_kordex_dep
      "${CMAKE_CURRENT_LIST_DIR}/../${sibling_dir}")

  set(local_vix_dep
      "${CMAKE_CURRENT_LIST_DIR}/../../deps/vix/${sibling_dir}")

  if(EXISTS "${local_kordex_dep}/CMakeLists.txt")
    message(STATUS "[std] Adding ${target_name} from Kordex sibling: ${local_kordex_dep}")

    add_subdirectory(
        "${local_kordex_dep}"
        "${CMAKE_CURRENT_BINARY_DIR}/_deps/${sibling_dir}")
  elseif(EXISTS "${local_vix_dep}/CMakeLists.txt")
    message(STATUS "[std] Adding ${target_name} from umbrella Vix dependency: ${local_vix_dep}")

    add_subdirectory(
        "${local_vix_dep}"
        "${CMAKE_CURRENT_BINARY_DIR}/_deps/vix_${sibling_dir}")
  else()
    find_package(${package_name} CONFIG QUIET)
  endif()

  if(TARGET ${target_name})
    return()
  endif()

  if(${fetch_option})
    include(FetchContent)

    message(STATUS "[std] Fetching ${target_name}")

    FetchContent_Declare(
        ${package_name}
        GIT_REPOSITORY ${repository}
        GIT_TAG ${tag}
        GIT_SHALLOW TRUE)

    FetchContent_MakeAvailable(${package_name})
  endif()

  if(NOT TARGET ${target_name})
    message(FATAL_ERROR
        "[std] Required dependency target '${target_name}' was not found.\n"
        "Install package '${package_name}', provide sibling '../${sibling_dir}', "
        "provide umbrella dependency '../../deps/vix/${sibling_dir}', "
        "or enable ${fetch_option}.")
  endif()
endfunction()

# --------------------------------------------------------------------
# Dependencies
# --------------------------------------------------------------------
kordex_std_require_target(
    kordex::bindings
    kordex_bindings
    bindings
    KORDEX_STD_FETCH_BINDINGS
    https://github.com/kordexjs/bindings.git
    ${KORDEX_BINDINGS_GIT_TAG})

kordex_std_require_target(
    kordex::runtime
    kordex_runtime
    runtime
    KORDEX_STD_FETCH_RUNTIME
    https://github.com/kordexjs/runtime.git
    ${KORDEX_RUNTIME_GIT_TAG})

kordex_std_require_target(
    vix::error
    vix_error
    error
    KORDEX_STD_FETCH_ERROR
    https://github.com/vixcpp/error.git
    ${KORDEX_VIX_GIT_TAG})

kordex_std_require_target(
    vix::log
    vix_log
    log
    KORDEX_STD_FETCH_LOG
    https://github.com/vixcpp/log.git
    ${KORDEX_VIX_GIT_TAG})

kordex_std_require_target(
    vix::json
    vix_json
    json
    KORDEX_STD_FETCH_JSON
    https://github.com/vixcpp/json.git
    ${KORDEX_VIX_GIT_TAG})

kordex_std_require_target(
    vix::fs
    vix_fs
    fs
    KORDEX_STD_FETCH_FS
    https://github.com/vixcpp/fs.git
    ${KORDEX_VIX_GIT_TAG})

kordex_std_require_target(
    vix::path
    vix_path
    path
    KORDEX_STD_FETCH_PATH
    https://github.com/vixcpp/path.git
    ${KORDEX_VIX_GIT_TAG})

kordex_std_require_target(
    vix::env
    vix_env
    env
    KORDEX_STD_FETCH_ENV
    https://github.com/vixcpp/env.git
    ${KORDEX_VIX_GIT_TAG})

kordex_std_require_target(
    vix::process
    vix_process
    process
    KORDEX_STD_FETCH_PROCESS
    https://github.com/vixcpp/process.git
    ${KORDEX_VIX_GIT_TAG})

kordex_std_require_target(
    vix::time
    vix_time
    time
    KORDEX_STD_FETCH_TIME
    https://github.com/vixcpp/time.git
    ${KORDEX_VIX_GIT_TAG})

if(KORDEX_STD_ENABLE_CRYPTO)
  kordex_std_require_target(
      vix::crypto
      vix_crypto
      crypto
      KORDEX_STD_FETCH_CRYPTO
      https://github.com/vixcpp/crypto.git
      ${KORDEX_VIX_GIT_TAG})
endif()

if(KORDEX_STD_ENABLE_HTTP)
  kordex_std_require_target(
      vix::core
      vix_core
      core
      KORDEX_STD_FETCH_CORE
      https://github.com/vixcpp/core.git
      ${KORDEX_VIX_GIT_TAG})
endif()

set(KORDEX_STD_DEPENDENCIES
    kordex::bindings
    kordex::runtime
    vix::error
    vix::log
    vix::json
    vix::fs
    vix::path
    vix::env
    vix::process
    vix::time)

if(KORDEX_STD_ENABLE_CRYPTO)
  list(APPEND KORDEX_STD_DEPENDENCIES vix::crypto)
endif()

if(KORDEX_STD_ENABLE_HTTP)
  list(APPEND KORDEX_STD_DEPENDENCIES vix::core)
endif()

# --------------------------------------------------------------------
# Sources
# --------------------------------------------------------------------
set(KORDEX_STD_HEADERS
    include/kordex/std/Version.hpp
    include/kordex/std/Error.hpp
    include/kordex/std/Result.hpp
    include/kordex/std/StdOptions.hpp
    include/kordex/std/StdConfig.hpp
    include/kordex/std/Console.hpp
    include/kordex/std/Fs.hpp
    include/kordex/std/Path.hpp
    include/kordex/std/Env.hpp
    include/kordex/std/Process.hpp
    include/kordex/std/Timer.hpp
    include/kordex/std/Crypto.hpp
    include/kordex/std/Http.hpp
    include/kordex/std/ModuleFactory.hpp
    include/kordex/std/StdRegistry.hpp
    include/kordex/std/Std.hpp)

set(KORDEX_STD_SOURCES
    src/Version.cpp
    src/Error.cpp
    src/StdOptions.cpp
    src/StdConfig.cpp
    src/Console.cpp
    src/Fs.cpp
    src/Path.cpp
    src/Env.cpp
    src/Process.cpp
    src/Timer.cpp
    src/Crypto.cpp
    src/Http.cpp
    src/ModuleFactory.cpp
    src/StdRegistry.cpp
    src/Std.cpp)

# --------------------------------------------------------------------
# Library
# --------------------------------------------------------------------
add_library(kordex_std STATIC
    ${KORDEX_STD_HEADERS}
    ${KORDEX_STD_SOURCES})

add_library(kordex::std ALIAS kordex_std)

target_compile_features(kordex_std
    PUBLIC
      cxx_std_20)

target_include_directories(kordex_std
    PUBLIC
      $<BUILD_INTERFACE:${CMAKE_CURRENT_SOURCE_DIR}/include>
      $<INSTALL_INTERFACE:${CMAKE_INSTALL_INCLUDEDIR}>)

target_link_libraries(kordex_std
    PUBLIC
      ${KORDEX_STD_DEPENDENCIES})

target_compile_definitions(kordex_std
    PUBLIC
      KORDEX_STD_VERSION_STRING="${PROJECT_VERSION}"
      KORDEX_STD_ENABLE_CONSOLE=$<BOOL:${KORDEX_STD_ENABLE_CONSOLE}>
      KORDEX_STD_ENABLE_FS=$<BOOL:${KORDEX_STD_ENABLE_FS}>
      KORDEX_STD_ENABLE_PATH=$<BOOL:${KORDEX_STD_ENABLE_PATH}>
      KORDEX_STD_ENABLE_ENV=$<BOOL:${KORDEX_STD_ENABLE_ENV}>
      KORDEX_STD_ENABLE_PROCESS=$<BOOL:${KORDEX_STD_ENABLE_PROCESS}>
      KORDEX_STD_ENABLE_TIMER=$<BOOL:${KORDEX_STD_ENABLE_TIMER}>
      KORDEX_STD_ENABLE_CRYPTO=$<BOOL:${KORDEX_STD_ENABLE_CRYPTO}>
      KORDEX_STD_ENABLE_HTTP=$<BOOL:${KORDEX_STD_ENABLE_HTTP}>)

if(KORDEX_STD_ENABLE_WARNINGS)
  target_compile_options(kordex_std
      PRIVATE
        $<$<CXX_COMPILER_ID:GNU,Clang,AppleClang>:-Wall -Wextra -Wpedantic>
        $<$<CXX_COMPILER_ID:MSVC>:/W4>)
endif()

if(KORDEX_STD_ENABLE_SANITIZERS)
  if(MSVC)
    message(WARNING "[std] Sanitizers are not enabled for MSVC in this CMake configuration.")
  else()
    target_compile_options(kordex_std
        PRIVATE
          -fno-omit-frame-pointer
          -fsanitize=address,undefined)

    target_link_options(kordex_std
        PRIVATE
          -fsanitize=address,undefined)
  endif()
endif()

set_target_properties(kordex_std PROPERTIES
    OUTPUT_NAME kordex_std
    VERSION ${PROJECT_VERSION}
    SOVERSION ${PROJECT_VERSION_MAJOR}
    EXPORT_NAME std)

# --------------------------------------------------------------------
# Tests dependency
# --------------------------------------------------------------------
if(KORDEX_STD_BUILD_TESTS)
  kordex_std_require_target(
      vix::tests
      vix_tests
      tests
      KORDEX_STD_FETCH_TESTS
      https://github.com/vixcpp/tests.git
      ${KORDEX_VIX_GIT_TAG})
endif()

# --------------------------------------------------------------------
# Tests
# --------------------------------------------------------------------
if(KORDEX_STD_BUILD_TESTS)
  include(CTest)
  enable_testing()

  set(KORDEX_STD_TESTS
      test_version
      test_error
      test_std_options
      test_std_config
      test_console
      test_fs
      test_path
      test_env
      test_process
      test_timer
      test_crypto
      test_http
      test_module_factory
      test_std_registry)

  foreach(test_name IN LISTS KORDEX_STD_TESTS)
    set(test_source "${CMAKE_CURRENT_SOURCE_DIR}/tests/${test_name}.cpp")

    if(EXISTS "${test_source}")
      add_executable(${test_name}
          "${test_source}")

      target_link_libraries(${test_name}
          PRIVATE
            kordex::std
            vix::tests)

      add_test(
          NAME ${test_name}
          COMMAND ${test_name})
    endif()
  endforeach()
endif()

# --------------------------------------------------------------------
# Examples
# --------------------------------------------------------------------
if(KORDEX_STD_BUILD_EXAMPLES)
  set(KORDEX_STD_EXAMPLES
      console
      fs
      path
      env
      process
      timer
      registry)

  foreach(example_name IN LISTS KORDEX_STD_EXAMPLES)
    set(example_source "${CMAKE_CURRENT_SOURCE_DIR}/examples/${example_name}.cpp")

    if(EXISTS "${example_source}")
      add_executable(kordex_std_${example_name}
          "${example_source}")

      target_link_libraries(kordex_std_${example_name}
          PRIVATE
            kordex::std)
    endif()
  endforeach()
endif()

# --------------------------------------------------------------------
# Install
# --------------------------------------------------------------------
if(KORDEX_STD_ENABLE_INSTALL
   AND NOT (DEFINED KORDEX_UMBRELLA_BUILD AND KORDEX_UMBRELLA_BUILD))

  install(
      TARGETS kordex_std
      EXPORT kordex_stdTargets
      ARCHIVE DESTINATION ${CMAKE_INSTALL_LIBDIR}
      LIBRARY DESTINATION ${CMAKE_INSTALL_LIBDIR}
      RUNTIME DESTINATION ${CMAKE_INSTALL_BINDIR}
      INCLUDES DESTINATION ${CMAKE_INSTALL_INCLUDEDIR})

  install(
      DIRECTORY include/
      DESTINATION ${CMAKE_INSTALL_INCLUDEDIR}
      FILES_MATCHING
        PATTERN "*.hpp"
        PATTERN "*.h")

  # --------------------------------------------------------------------
  # Standalone package config/export
  # --------------------------------------------------------------------
  configure_package_config_file(
      "${CMAKE_CURRENT_SOURCE_DIR}/cmake/KordexStdConfig.cmake.in"
      "${CMAKE_CURRENT_BINARY_DIR}/kordex_stdConfig.cmake"
      INSTALL_DESTINATION "${CMAKE_INSTALL_LIBDIR}/cmake/kordex_std")

  write_basic_package_version_file(
      "${CMAKE_CURRENT_BINARY_DIR}/kordex_stdConfigVersion.cmake"
      VERSION ${PROJECT_VERSION}
      COMPATIBILITY SameMajorVersion)

  install(
      FILES
        "${CMAKE_CURRENT_BINARY_DIR}/kordex_stdConfig.cmake"
        "${CMAKE_CURRENT_BINARY_DIR}/kordex_stdConfigVersion.cmake"
      DESTINATION
        "${CMAKE_INSTALL_LIBDIR}/cmake/kordex_std")

  install(
      EXPORT kordex_stdTargets
      FILE kordex_stdTargets.cmake
      NAMESPACE kordex::
      DESTINATION "${CMAKE_INSTALL_LIBDIR}/cmake/kordex_std")

endif()

# --------------------------------------------------------------------
# Summary
# --------------------------------------------------------------------
message(STATUS "")
message(STATUS "Kordex Std")
message(STATUS "  version   : ${PROJECT_VERSION}")
message(STATUS "  target    : kordex::std")
message(STATUS "  export    : ${KORDEX_STD_EXPORT_SET}")
message(STATUS "  compiler  : ${CMAKE_CXX_COMPILER_ID} ${CMAKE_CXX_COMPILER_VERSION}")
message(STATUS "  build     : ${CMAKE_BUILD_TYPE}")
message(STATUS "  tests     : ${KORDEX_STD_BUILD_TESTS}")
message(STATUS "  examples  : ${KORDEX_STD_BUILD_EXAMPLES}")
message(STATUS "  warnings  : ${KORDEX_STD_ENABLE_WARNINGS}")
message(STATUS "  sanitizers: ${KORDEX_STD_ENABLE_SANITIZERS}")
message(STATUS "  install   : ${KORDEX_STD_ENABLE_INSTALL}")
message(STATUS "  console   : ${KORDEX_STD_ENABLE_CONSOLE}")
message(STATUS "  fs        : ${KORDEX_STD_ENABLE_FS}")
message(STATUS "  path      : ${KORDEX_STD_ENABLE_PATH}")
message(STATUS "  env       : ${KORDEX_STD_ENABLE_ENV}")
message(STATUS "  process   : ${KORDEX_STD_ENABLE_PROCESS}")
message(STATUS "  timer     : ${KORDEX_STD_ENABLE_TIMER}")
message(STATUS "  crypto    : ${KORDEX_STD_ENABLE_CRYPTO}")
message(STATUS "  http      : ${KORDEX_STD_ENABLE_HTTP}")
message(STATUS "")
