cmake_minimum_required(VERSION 3.20 FATAL_ERROR)

set(CMAKE_CXX_STANDARD 26)
set(CMAKE_CXX_STANDARD_REQUIRED ON)

project(CppPlayground)

option(ENABLE_SANITIZERS "Enable Address Sanitizer" OFF)
option(ENABLE_CLANG_TIDY "Enable clang-tidy" ON)

if (CMAKE_BUILD_TYPE STREQUAL "Debug")
    set(ENABLE_SANITIZERS ON)
endif()

if (ENABLE_CLANG_TIDY)
    set(CMAKE_EXPORT_COMPILE_COMMANDS ON)
    find_program(CLANG_TIDY_EXE NAMES clang-tidy-18 REQUIRED)
endif()

# Turning Address Sanitizer on
if (ENABLE_SANITIZERS)
    add_compile_options(-fsanitize=address,undefined -fno-omit-frame-pointer)
    add_link_options(-fsanitize=address,undefined)
endif()

# Adding Catch2 library
add_subdirectory(third-party/Catch2)

# Adding Google Benchmark library
set(BENCHMARK_DOWNLOAD_DEPENDENCIES ON CACHE BOOL "" FORCE)
add_subdirectory(third-party/benchmark)

file(GLOB_RECURSE geometry_helper_sources src/geometryHelper/*.cpp src/geometryHelper/*.h)
add_library(geometry_helper SHARED ${geometry_helper_sources})
if (ENABLE_CLANG_TIDY)
    set_target_properties(geometry_helper PROPERTIES CXX_CLANG_TIDY ${CLANG_TIDY_EXE})
endif()

file(GLOB_RECURSE tests_sources tests/*.cpp tests/*.h)
add_executable(tests ${tests_sources})
target_link_libraries(tests PRIVATE Catch2::Catch2WithMain)
target_link_libraries(tests PRIVATE geometry_helper)
target_link_libraries(tests PRIVATE benchmark::benchmark)

add_executable(playground main.cpp)
target_link_libraries(playground PRIVATE geometry_helper)
if (ENABLE_CLANG_TIDY)
    set_target_properties(playground PROPERTIES CXX_CLANG_TIDY ${CLANG_TIDY_EXE})
endif()