#  @file tests/CMakeLists.txt
#  @author Softadastra
#
#  Copyright 2026, Softadastra. All rights reserved.
#  https://github.com/softadastra/kordex-std
#  Use of this source code is governed by a MIT license
#  that can be found in the LICENSE file.
#
# ====================================================================
# Kordex Std - Tests
# ====================================================================

include_guard(GLOBAL)

# --------------------------------------------------------------------
# Validation
# --------------------------------------------------------------------
if(NOT TARGET kordex::std)
  message(FATAL_ERROR
      "[std/tests] Required target 'kordex::std' was not found. "
      "Tests must be added from the parent kordex-std CMake project.")
endif()

# --------------------------------------------------------------------
# Helper
# --------------------------------------------------------------------
function(kordex_std_add_test target_name source_file)
  add_executable(${target_name}
      ${source_file})

  target_compile_features(${target_name}
      PRIVATE
        cxx_std_20)

  target_link_libraries(${target_name}
      PRIVATE
        kordex::std)

  if(TARGET vix::tests)
    target_link_libraries(${target_name}
        PRIVATE
          vix::tests)
  endif()

  if(KORDEX_STD_ENABLE_WARNINGS)
    target_compile_options(${target_name}
        PRIVATE
          $<$<CXX_COMPILER_ID:GNU,Clang,AppleClang>:-Wall -Wextra -Wpedantic>
          $<$<CXX_COMPILER_ID:MSVC>:/W4>)
  endif()

  add_test(
      NAME ${target_name}
      COMMAND ${target_name})
endfunction()

# --------------------------------------------------------------------
# Tests
# --------------------------------------------------------------------
kordex_std_add_test(kordex_std_test_console test_console.cpp)
kordex_std_add_test(kordex_std_test_crypto test_crypto.cpp)
kordex_std_add_test(kordex_std_test_env test_env.cpp)
kordex_std_add_test(kordex_std_test_error test_error.cpp)
kordex_std_add_test(kordex_std_test_fs test_fs.cpp)
kordex_std_add_test(kordex_std_test_http test_http.cpp)
kordex_std_add_test(kordex_std_test_module_factory test_module_factory.cpp)
kordex_std_add_test(kordex_std_test_path test_path.cpp)
kordex_std_add_test(kordex_std_test_process test_process.cpp)
kordex_std_add_test(kordex_std_test_std_config test_std_config.cpp)
kordex_std_add_test(kordex_std_test_std_options test_std_options.cpp)
kordex_std_add_test(kordex_std_test_std_registry test_std_registry.cpp)
kordex_std_add_test(kordex_std_test_timer test_timer.cpp)
kordex_std_add_test(kordex_std_test_version test_version.cpp)
