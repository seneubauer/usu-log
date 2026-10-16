cmake_minimum_required(VERSION 4.2)
project(UsuLog)

set(CMAKE_CXX_STANDARD 23)
set(CMAKE_CXX_STANDARD_REQUIRED ON)
set(CMAKE_EXPORT_COMPILE_COMMANDS ON)

add_library(
    usu_log SHARED
    src/ring_buffer.cpp
)

target_include_directories(usu_log PRIVATE "${CMAKE_SOURCE_DIR}/include")
target_compile_options(usu_log PRIVATE -Wall -Wextra -Wpedantic)
add_subdirectory(test)
