#  @file CMakeLists.txt
#  @author Gaspard Kirira
#
#  Copyright 2026, Gaspard Kirira. All rights reserved.
#  https://github.com/kordexjs/std
#  Use of this source code is governed by a MIT license
#  that can be found in the LICENSE file.
#
# ====================================================================
# Kordex Std
# ====================================================================

cmake_minimum_required(VERSION 3.24)

project(kordex_std
    VERSION 0.1.0
    DESCRIPTION "Standard native modules for Kordex"
    LANGUAGES CXX)

include(GNUInstallDirs)
include(CMakePackageConfigHelpers)
include(FetchContent)

include(cmake/KordexStdOptions.cmake)

# --------------------------------------------------------------------
# Global settings
# --------------------------------------------------------------------
set(CMAKE_CXX_STANDARD 20)
set(CMAKE_CXX_STANDARD_REQUIRED ON)
set(CMAKE_CXX_EXTENSIONS OFF)

# --------------------------------------------------------------------
# Helpers
# --------------------------------------------------------------------
function(kordex_std_fetch_dependency dependency_name repository tag)
  FetchContent_Declare(
      ${dependency_name}
      GIT_REPOSITORY ${repository}
      GIT_TAG ${tag}
      GIT_SHALLOW TRUE)

  FetchContent_MakeAvailable(${dependency_name})
endfunction()

function(kordex_std_require_target target_name package_name dependency_name fetch_option repository tag)
  if(TARGET ${target_name})
    return()
  endif()

  find_package(${package_name} CONFIG QUIET)

  if(TARGET ${target_name})
    return()
  endif()

  if(${fetch_option})
    kordex_std_fetch_dependency(
        ${dependency_name}
        ${repository}
        ${tag})

    if(TARGET ${target_name})
      return()
    endif()
  endif()

  message(FATAL_ERROR
      "Required dependency target '${target_name}' was not found. "
      "Install package '${package_name}' or enable ${fetch_option}.")
endfunction()

# --------------------------------------------------------------------
# Dependencies
# --------------------------------------------------------------------
kordex_std_require_target(
    kordex::bindings
    kordex_bindings
    kordex_bindings
    KORDEX_STD_FETCH_BINDINGS
    https://github.com/kordexjs/bindings.git
    ${KORDEX_BINDINGS_GIT_TAG})

kordex_std_require_target(
    kordex::runtime
    kordex_runtime
    kordex_runtime
    KORDEX_STD_FETCH_RUNTIME
    https://github.com/kordexjs/runtime.git
    ${KORDEX_RUNTIME_GIT_TAG})

kordex_std_require_target(
    vix::error
    vix_error
    vix_error
    KORDEX_STD_FETCH_ERROR
    https://github.com/vixcpp/error.git
    ${KORDEX_VIX_GIT_TAG})

kordex_std_require_target(
    vix::log
    vix_log
    vix_log
    KORDEX_STD_FETCH_LOG
    https://github.com/vixcpp/log.git
    ${KORDEX_VIX_GIT_TAG})

kordex_std_require_target(
    vix::json
    vix_json
    vix_json
    KORDEX_STD_FETCH_JSON
    https://github.com/vixcpp/json.git
    ${KORDEX_VIX_GIT_TAG})

kordex_std_require_target(
    vix::fs
    vix_fs
    vix_fs
    KORDEX_STD_FETCH_FS
    https://github.com/vixcpp/fs.git
    ${KORDEX_VIX_GIT_TAG})

kordex_std_require_target(
    vix::path
    vix_path
    vix_path
    KORDEX_STD_FETCH_PATH
    https://github.com/vixcpp/path.git
    ${KORDEX_VIX_GIT_TAG})

kordex_std_require_target(
    vix::env
    vix_env
    vix_env
    KORDEX_STD_FETCH_ENV
    https://github.com/vixcpp/env.git
    ${KORDEX_VIX_GIT_TAG})

kordex_std_require_target(
    vix::process
    vix_process
    vix_process
    KORDEX_STD_FETCH_PROCESS
    https://github.com/vixcpp/process.git
    ${KORDEX_VIX_GIT_TAG})

kordex_std_require_target(
    vix::time
    vix_time
    vix_time
    KORDEX_STD_FETCH_TIME
    https://github.com/vixcpp/time.git
    ${KORDEX_VIX_GIT_TAG})

if(KORDEX_STD_ENABLE_CRYPTO)
  kordex_std_require_target(
      vix::crypto
      vix_crypto
      vix_crypto
      KORDEX_STD_FETCH_CRYPTO
      https://github.com/vixcpp/crypto.git
      ${KORDEX_VIX_GIT_TAG})
endif()

if(KORDEX_STD_ENABLE_HTTP)
  kordex_std_require_target(
      vix::http
      vix_http
      vix_http
      KORDEX_STD_FETCH_HTTP
      https://github.com/vixcpp/http.git
      ${KORDEX_VIX_GIT_TAG})
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

target_include_directories(kordex_std
    PUBLIC
      $<BUILD_INTERFACE:${CMAKE_CURRENT_SOURCE_DIR}/include>
      $<INSTALL_INTERFACE:${CMAKE_INSTALL_INCLUDEDIR}>)

target_link_libraries(kordex_std
    PUBLIC
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
  target_link_libraries(kordex_std PUBLIC vix::crypto)
endif()

if(KORDEX_STD_ENABLE_HTTP)
  target_link_libraries(kordex_std PUBLIC vix::http)
endif()

target_compile_definitions(kordex_std
    PUBLIC
      KORDEX_STD_VERSION_MAJOR=${PROJECT_VERSION_MAJOR}
      KORDEX_STD_VERSION_MINOR=${PROJECT_VERSION_MINOR}
      KORDEX_STD_VERSION_PATCH=${PROJECT_VERSION_PATCH}

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
        $<$<CXX_COMPILER_ID:GNU,Clang>:-Wall -Wextra -Wpedantic>
        $<$<CXX_COMPILER_ID:MSVC>:/W4>)
endif()

if(KORDEX_STD_ENABLE_SANITIZERS AND NOT MSVC)
  target_compile_options(kordex_std PRIVATE -fsanitize=address,undefined)
  target_link_options(kordex_std PRIVATE -fsanitize=address,undefined)
endif()

# --------------------------------------------------------------------
# Tests
# --------------------------------------------------------------------
if(KORDEX_STD_BUILD_TESTS)
  enable_testing()

  kordex_std_require_target(
      vix::tests
      vix_tests
      vix_tests
      KORDEX_STD_FETCH_TESTS
      https://github.com/vixcpp/tests.git
      ${KORDEX_VIX_GIT_TAG})

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
    add_executable(${test_name}
        tests/${test_name}.cpp)

    target_link_libraries(${test_name}
        PRIVATE
          kordex::std
          vix::tests)

    add_test(
        NAME ${test_name}
        COMMAND ${test_name})
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
    add_executable(kordex_std_${example_name}
        examples/${example_name}.cpp)

    target_link_libraries(kordex_std_${example_name}
        PRIVATE
          kordex::std)
  endforeach()
endif()

# --------------------------------------------------------------------
# Install
# --------------------------------------------------------------------
install(
    TARGETS kordex_std
    EXPORT kordex_stdTargets
    ARCHIVE DESTINATION ${CMAKE_INSTALL_LIBDIR}
    LIBRARY DESTINATION ${CMAKE_INSTALL_LIBDIR}
    RUNTIME DESTINATION ${CMAKE_INSTALL_BINDIR}
    INCLUDES DESTINATION ${CMAKE_INSTALL_INCLUDEDIR})

install(
    DIRECTORY include/
    DESTINATION ${CMAKE_INSTALL_INCLUDEDIR})

install(
    EXPORT kordex_stdTargets
    FILE kordex_stdTargets.cmake
    NAMESPACE kordex::
    DESTINATION ${CMAKE_INSTALL_LIBDIR}/cmake/kordex_std)

configure_package_config_file(
    cmake/KordexStdConfig.cmake.in
    ${CMAKE_CURRENT_BINARY_DIR}/kordex_stdConfig.cmake
    INSTALL_DESTINATION ${CMAKE_INSTALL_LIBDIR}/cmake/kordex_std)

write_basic_package_version_file(
    ${CMAKE_CURRENT_BINARY_DIR}/kordex_stdConfigVersion.cmake
    VERSION ${PROJECT_VERSION}
    COMPATIBILITY SameMajorVersion)

install(
    FILES
      ${CMAKE_CURRENT_BINARY_DIR}/kordex_stdConfig.cmake
      ${CMAKE_CURRENT_BINARY_DIR}/kordex_stdConfigVersion.cmake
    DESTINATION ${CMAKE_INSTALL_LIBDIR}/cmake/kordex_std)
