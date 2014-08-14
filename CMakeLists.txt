cmake_minimum_required(VERSION 2.6)

project(brainfuck C)
set(brainfuck_VERSION_MAJOR 2)
set(brainfuck_VERSION_MINOR 6)
set(brainfuck_VERSION_PATCH 4)

option(ENABLE_DEBUG "Enable the debug extension for brainfuck." OFF)
if(ENABLE_DEBUG)
	add_definitions("-DBRAINFUCK_EXTENSION_DEBUG")
endif()

add_definitions("-Wall -Wextra -O3")
add_library(libbrainfuck STATIC src/brainfuck.c)
set_target_properties(libbrainfuck PROPERTIES PREFIX "")
add_executable(brainfuck src/main.c)
target_link_libraries(brainfuck libbrainfuck)
