cmake_minimum_required(VERSION 2.6)

project(brainfuck C)
set(brainfuck_VERSION_MAJOR 2)
set(brainfuck_VERSION_MINOR 4)
set(brainfuck_VERSION_PATCH 0)
set(CMAKE_ARCHIVE_OUTPUT_DIRECTORY ${CMAKE_BINARY_DIR}/lib)
set(CMAKE_LIBRARY_OUTPUT_DIRECTORY ${CMAKE_BINARY_DIR}/lib)
set(CMAKE_RUNTIME_OUTPUT_DIRECTORY ${CMAKE_BINARY_DIR}/bin)
set(PROJECT_INCLUDE_DIR ${PROJECT_SOURCE_DIR}/include)

add_definitions("-Wall -g")
add_library(libbrainfuck STATIC src/brainfuck.c)
set_target_properties(libbrainfuck PROPERTIES PREFIX "")
add_executable(brainfuck src/main.c)
target_link_libraries(brainfuck libbrainfuck)

