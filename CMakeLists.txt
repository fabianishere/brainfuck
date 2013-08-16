cmake_minimum_required(VERSION 2.6)

# Project name
project(brainfuck C)

set (brainfuck_VERSION_MAJOR 2)
set (brainfuck_VERSION_MINOR 2)
set (brainfuck_VERSION_PATCH 0)

# Set directories.
set(CMAKE_RUNTIME_OUTPUT_DIRECTORY ${CMAKE_BINARY_DIR}/bin)

add_definitions("-Wall -g")
add_executable(brainfuck src/main.c src/brainfuck.c src/brainfuck.h)

