cmake_minimum_required(VERSION 3.20 FATAL_ERROR)

set(CMAKE_CXX_STANDARD 23)
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

# adding sanitizers
if (ENABLE_SANITIZERS)
    target_compile_options(geometry_helper PRIVATE -fsanitize=address,undefined -fno-omit-frame-pointer)
    target_link_options(geometry_helper PRIVATE -fsanitize=address,undefined)
endif()


# Unit testing geometry functions
file(GLOB_RECURSE geometryHelper_unit_tests_sources test/geometryHelper/unit-tests/*.cpp test/geometryHelper/unit-tests/*.h)
add_executable(geometryHelper_unit_tests ${geometryHelper_unit_tests_sources})
target_link_libraries(geometryHelper_unit_tests PRIVATE geometry_helper)
target_link_libraries(geometryHelper_unit_tests PRIVATE Catch2::Catch2WithMain)

# Benchmarking geometry functions
file(GLOB_RECURSE geometryHelper_benchmark_sources test/geometryHelper/benchmarks/*.cpp test/geometryHelper/benchmarks/*.h)
add_executable(geometryHelper_benchmarks ${geometryHelper_benchmark_sources})
target_link_libraries(geometryHelper_benchmarks PRIVATE geometry_helper)
target_link_libraries(geometryHelper_benchmarks PRIVATE benchmark::benchmark)

add_executable(playground main.cpp)