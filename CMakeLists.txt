cmake_minimum_required(VERSION 2.6)

project(brainfuck C)
set(BRAINFUCK_VERSION_MAJOR 3)
set(BRAINFUCK_VERSION_MINOR 0)
set(BRAINFUCK_VERSION_PATCH 0)
	
option(WITH_CLI 
	"Build the command line interface for brainfuck." ON)
option(WITH_DOCUMENTATION 
	"Use Doxygen to create API documentation of this library." OFF)

add_definitions("-W -Werror -Wall -Wextra -Wformat-security")
add_definitions("-std=c89 -g -O0")
add_definitions("-fno-exceptions")

include_directories(include/)
add_library(libbrainfuck STATIC src/brainfuck.c)
set_target_properties(libbrainfuck PROPERTIES PREFIX "")

if(WITH_CLI)
	add_executable(brainfuck src/main.c)
	target_link_libraries(brainfuck libbrainfuck)
endif()

if(WITH_DOCUMENTATION)
	FIND_PACKAGE(Doxygen)
	if (NOT DOXYGEN_FOUND)
		message(FATAL_ERROR
		"Doxygen is needed to build the documentation. Please install it correctly")
	endif()
	add_custom_target (docs ALL
		COMMAND ${DOXYGEN_EXECUTABLE} ${PROJECT_SOURCE_DIR}/Doxyfile
		SOURCES ${PROJECT_SOURCE_DIR}/Doxyfile
		WORKING_DIRECTORY ${PROJECT_SOURCE_DIR}
	)
endif()
