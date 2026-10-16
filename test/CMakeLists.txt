add_executable(
    ring_buffer_test
    ring-buffer.cpp
)

target_link_libraries(
    ring_buffer_test
    PRIVATE usu_log
)
target_include_directories(ring_buffer_test PRIVATE "${CMAKE_SOURCE_DIR}/include")
target_compile_options(ring_buffer_test PRIVATE -Wall -Wextra -Wpedantic)
