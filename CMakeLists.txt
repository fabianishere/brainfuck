cmake_minimum_required(VERSION 2.6)

project(brainfuck C)
set(brainfuck_VERSION_MAJOR 2)
set(brainfuck_VERSION_MINOR 7)
set(brainfuck_VERSION_PATCH 0)

option(ENABLE_DEBUG "Enable the debug extension for brainfuck.")
option(BUILD_SHARED_LIB "Build a shared library.")
option(ENABLE_EDITLINE "Enable GNU readline functionality provided by the editline library." ON)

configure_file (
  "${PROJECT_SOURCE_DIR}/include/brainfuck.h"
  "${PROJECT_SOURCE_DIR}/include/brainfuck.h"
)
include_directories(include/)
add_definitions("-Wall -Wextra -O0 -g -std=c89")
add_library(libbrainfuck_static STATIC src/brainfuck.c)
set_target_properties(libbrainfuck_static PROPERTIES PREFIX "")
set_target_properties(libbrainfuck_static PROPERTIES OUTPUT_NAME "libbrainfuck")
add_executable(brainfuck src/main.c)

if(ENABLE_DEBUG)
  add_definitions("-DBRAINFUCK_EXTENSION_DEBUG")
endif()

if(BUILD_SHARED_LIB)
  add_library(libbrainfuck SHARED src/brainfuck.c)
  set_target_properties(libbrainfuck PROPERTIES PREFIX "")
endif()

if(ENABLE_EDITLINE)
  add_definitions("-DBRAINFUCK_EDITLINE_LIB")
  target_link_libraries(brainfuck libbrainfuck_static -ledit)
else()
  target_link_libraries(brainfuck libbrainfuck_static)
endif()
