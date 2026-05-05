cmake_minimum_required(VERSION 3.20 FATAL_ERROR)

set(CMAKE_CXX_STANDARD 20)
set(CMAKE_CXX_STANDARD_REQUIRED ON)

project(MyApp)

file(GLOB_RECURSE sources src/*.cpp src/*.h main.cpp)

add_executable(MyApp ${sources})