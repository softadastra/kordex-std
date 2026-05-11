#  @file examples/CMakeLists.txt
#  @author Softadastra
#
#  Copyright 2026, Softadastra. All rights reserved.
#  https://github.com/softadastra/kordex-std
#  Use of this source code is governed by a MIT license
#  that can be found in the LICENSE file.
#
# ====================================================================
# Kordex Std - Examples
# ====================================================================

include_guard(GLOBAL)

# --------------------------------------------------------------------
# Validation
# --------------------------------------------------------------------
if(NOT TARGET kordex::std)
  message(FATAL_ERROR
      "[std/examples] Required target 'kordex::std' was not found. "
      "Examples must be added from the parent kordex-std CMake project.")
endif()

# --------------------------------------------------------------------
# Helper
# --------------------------------------------------------------------
function(kordex_std_add_example target_name source_file)
  add_executable(${target_name}
      ${source_file})

  target_compile_features(${target_name}
      PRIVATE
        cxx_std_20)

  target_link_libraries(${target_name}
      PRIVATE
        kordex::std)

  if(KORDEX_STD_ENABLE_WARNINGS)
    target_compile_options(${target_name}
        PRIVATE
          $<$<CXX_COMPILER_ID:GNU,Clang,AppleClang>:-Wall -Wextra -Wpedantic>
          $<$<CXX_COMPILER_ID:MSVC>:/W4>)
  endif()
endfunction()

# --------------------------------------------------------------------
# Examples
# --------------------------------------------------------------------
kordex_std_add_example(kordex_std_console_example console.cpp)
kordex_std_add_example(kordex_std_env_example env.cpp)
kordex_std_add_example(kordex_std_fs_example fs.cpp)
kordex_std_add_example(kordex_std_path_example path.cpp)
kordex_std_add_example(kordex_std_process_example process.cpp)
kordex_std_add_example(kordex_std_registry_example registry.cpp)
kordex_std_add_example(kordex_std_timer_example timer.cpp)
