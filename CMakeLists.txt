cmake_minimum_required(VERSION 2.6)

project(brainfuck C)
set(BRAINFUCK_VERSION_MAJOR 3)
set(BRAINFUCK_VERSION_MINOR 0)
set(BRAINFUCK_VERSION_PATCH 1)
	
option(WITH_CLI 
	"Build the command line interface for brainfuck." ON)

add_definitions("-W -Wall -Werror -Wformat-security")
add_definitions("-std=c89 -O2")
add_definitions("-fno-exceptions")

include_directories(include/)
add_library(libbrainfuck STATIC src/brainfuck.c)
set_target_properties(libbrainfuck PROPERTIES PREFIX "")

if(WITH_CLI)
	add_executable(brainfuck src/main.c)
	target_link_libraries(brainfuck libbrainfuck)
endif()
