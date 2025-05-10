cmake_minimum_required(VERSION 3.13)

file(GLOB_RECURSE src_keymaps *.c)
add_library(core ${src_keymaps})
target_include_directories(core PUBLIC ${CMAKE_CURRENT_SOURCE_DIR})
