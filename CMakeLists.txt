cmake_minimum_required(VERSION 3.20 FATAL_ERROR)

set(CMAKE_CXX_STANDARD 26)
set(CMAKE_CXX_STANDARD_REQUIRED ON)

project(CppPlayground)

option(ENABLE_SANITIZERS "Enable Address Sanitizer" OFF)

if (CMAKE_BUILD_TYPE STREQUAL "Debug")
    set(ENABLE_SANITIZERS ON)
endif()

if (ENABLE_SANITIZERS)
    add_compile_options(-fsanitize=address,undefined -fno-omit-frame-pointer)
    add_link_options(-fsanitize=address,undefined)
endif()

include(FetchContent)

FetchContent_Declare(
    Catch2
    GIT_REPOSITORY https://github.com/catchorg/Catch2.git
    GIT_TAG v3.5.4
)

FetchContent_MakeAvailable(Catch2)

file(GLOB_RECURSE geometry_helper_sources src/geometryHelper/*.cpp src/geometryHelper/*.h)
add_library(geometry_helper ${geometry_helper_sources})

file(GLOB_RECURSE tests_sources tests/*.cpp tests/*.h)
add_executable(tests ${tests_sources})
target_link_libraries(tests PRIVATE Catch2::Catch2WithMain)
target_link_libraries(tests PRIVATE geometry_helper)

add_executable(playground main.cpp)
target_link_libraries(playground PRIVATE geometry_helper)