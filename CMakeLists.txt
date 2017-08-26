cmake_minimum_required(VERSION 2.6)

project(brainfuck C)
set(brainfuck_VERSION_MAJOR 2)
set(brainfuck_VERSION_MINOR 7)
set(brainfuck_VERSION_PATCH 0)

option(ENABLE_DEBUG "Enable the debug extension for brainfuck.")
option(BUILD_SHARED_LIB "Build a shared library.")

if(ENABLE_DEBUG)
  add_definitions("-DBRAINFUCK_EXTENSION_DEBUG")
endif()

configure_file (
  "${PROJECT_SOURCE_DIR}/include/brainfuck.h"
  "${PROJECT_SOURCE_DIR}/include/brainfuck.h"
)
include_directories(include/)
add_definitions("-Wall -Wextra -O2 -std=c89")
add_library(libbrainfuck_static STATIC src/brainfuck.c)
set_target_properties(libbrainfuck_static PROPERTIES PREFIX "")
set_target_properties(libbrainfuck_static PROPERTIES OUTPUT_NAME "libbrainfuck")
if(BUILD_SHARED_LIB)
  add_library(libbrainfuck SHARED src/brainfuck.c)
  set_target_properties(libbrainfuck PROPERTIES PREFIX "")
endif()
add_executable(brainfuck src/main.c)
target_link_libraries(brainfuck libbrainfuck_static -ledit)
