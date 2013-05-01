cmake_minimum_required(VERSION 2.6)

# Project name
project(brainfuck C)

set (brainfuck_VERSION_MAJOR 0)
set (brainfuck_VERSION_MINOR 1)
set (brainfuck_VERSION_PATCH 3)

add_definitions("-Wall")
add_executable(brainfuck src/brainfuck_shell.c src/brainfuck.c src/brainfuck.h)
