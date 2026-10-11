cmake_minimum_required(VERSION 4.2)
project(UsuLog)

set(CMAKE_CXX_STANDARD 23)
set(CMAKE_CXX_STANDARD_REQUIRED ON)

add_library(
    usu_log STATIC
    src/ring_buffer.cpp
)

target_include_directories(usu_log PRIVATE include)

add_subdirectory(test)
