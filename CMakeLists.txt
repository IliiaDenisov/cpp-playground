cmake_minimum_required(VERSION 3.20 FATAL_ERROR)

set(CMAKE_CXX_STANDARD 26)
set(CMAKE_CXX_STANDARD_REQUIRED ON)

project(CppPlayground)

file(GLOB_RECURSE geometryHelper_sources src/geometryHelper/*.cpp src/geometryHelper/*.h)
add_library(geometryHelper ${geometryHelper_sources})

add_executable(playground main.cpp)
target_link_libraries(playground geometryHelper)
