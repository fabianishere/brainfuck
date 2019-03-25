# Copyright (c) 2019 Fabian Mastenbroek
#
# Permission is hereby granted, free of charge, to any person obtaining a copy
# of this software and associated documentation files (the "Software"), to deal
# in the Software without restriction, including without limitation the rights
# to use, copy, modify, merge, publish, distribute, sublicense, and/or sell
# copies of the Software, and to permit persons to whom the Software is
# furnished to do so, subject to the following conditions:
#
# The above copyright notice and this permission notice shall be included in
# all copies or substantial portions of the Software.
#
# THE SOFTWARE IS PROVIDED "AS IS", WITHOUT WARRANTY OF ANY KIND, EXPRESS OR
# IMPLIED, INCLUDING BUT NOT LIMITED TO THE WARRANTIES OF MERCHANTABILITY,
# FITNESS FOR A PARTICULAR PURPOSE AND NONINFRINGEMENT. IN NO EVENT SHALL THE
# AUTHORS OR COPYRIGHT HOLDERS BE LIABLE FOR ANY CLAIM, DAMAGES OR OTHER
# LIABILITY, WHETHER IN AN ACTION OF CONTRACT, TORT OR OTHERWISE, ARISING FROM,
# OUT OF OR IN CONNECTION WITH THE SOFTWARE OR THE USE OR OTHER DEALINGS IN
# THE SOFTWARE.
cmake_minimum_required(VERSION 3.1..3.14)

project(brainiac
    VERSION 3.0.0
    LANGUAGES C
    DESCRIPTION "An modular language implementation of brainfuck and its derivatives, written in C"
)

set(CMAKE_MODULE_PATH "${PROJECT_SOURCE_DIR}/cmake" ${CMAKE_MODULE_PATH})

## Compile-time options ##
option(WITH_PARSER_BRAINFUCK "Enable brainfuck parser implementation" ON)
option(WITH_VM_INTERPRETER "Enable interpreter" ON)
option(WITH_VM_LIGHTNING "Enable GNU lightning JIT" OFF)
option(WITH_CLI "Enable the command line interface" ON)
option(WITH_EDITLINE "Enable GNU readline functionality provided by the editline library" ON)
option(INSTALL_EXAMPLES "Install examples")
option(BUILD_SHARED_LIBS "Build shared library")

## Target ##
set(BRAINIAC_H
        include/brainiac/brainiac.h
        include/brainiac/ir.h
        include/brainiac/parser.h
        include/brainiac/vm.h)
set(BRAINIAC_C_FLAGS
        $<$<OR:$<C_COMPILER_ID:Clang>,$<C_COMPILER_ID:AppleClang>,$<C_COMPILER_ID:GNU>>:
            -pipe -march=native
            $<$<CONFIG:Debug>:-O0 -Wall -Wextra>>
        $<$<C_COMPILER_ID:MSVC>:
            $<$<CONFIG:Debug>:/Od /Wall /Zi>>)

add_library(brainiac
        ${BRAINIAC_H}
        src/brainiac.h
        src/brainiac.c
        src/parser/parser.c
        src/vm/vm.c)
add_library(brainiac::brainiac ALIAS brainiac)
set_target_properties(brainiac
        PROPERTIES
            C_STANDARD 90
            VERSION "${PROJECT_VERSION}"
            PUBLIC_HEADER "${BRAINIAC_H}")
target_include_directories(brainiac
        PUBLIC
            $<BUILD_INTERFACE:${CMAKE_CURRENT_SOURCE_DIR}/include/>
            $<INSTALL_INTERFACE:include/>
        PRIVATE
            ${CMAKE_CURRENT_SOURCE_DIR}/src)
target_compile_options(brainiac PRIVATE ${BRAINIAC_C_FLAGS})

## Compile-time features ##
if(WITH_PARSER_BRAINFUCK)
    include(src/parser/brainfuck/CMakeLists.txt)
    target_compile_definitions(brainiac PRIVATE BRAINIAC_PARSER_BRAINFUCK_ENABLED)
endif()

if(WITH_VM_INTERPRETER)
    include(src/vm/interpreter/CMakeLists.txt)
    target_compile_definitions(brainiac PRIVATE BRAINIAC_VM_INTERPRETER_ENABLED)
endif()

if(WITH_VM_LIGHTNING)
    include(src/vm/lightning/CMakeLists.txt)
    target_compile_definitions(brainiac PRIVATE BRAINIAC_VM_LIGHTNING_ENABLED)
endif()

if(WITH_CLI)
    find_package(Getopt REQUIRED)
    add_executable(brainiac-cli src/cli.c)
    set_target_properties(brainiac-cli PROPERTIES
            OUTPUT_NAME "brainiac"
            C_STANDARD 90)
    target_compile_options(brainiac-cli PRIVATE ${BRAINIAC_C_FLAGS})
    target_link_libraries(brainiac-cli PRIVATE brainiac getopt)

    if(WITH_EDITLINE)
        find_package(Editline REQUIRED)
        target_compile_definitions(brainiac-cli PRIVATE BRAINIAC_EDITLINE_ENABLED)
        target_link_libraries(brainiac-cli PRIVATE editline)
    endif()
endif()

## Installation ##
include(GNUInstallDirs)

install(TARGETS brainiac
        EXPORT BrainiacConfig
        ARCHIVE DESTINATION ${CMAKE_INSTALL_LIBDIR}
        LIBRARY DESTINATION ${CMAKE_INSTALL_LIBDIR}
        RUNTIME DESTINATION ${CMAKE_INSTALL_BINDIR}
        INCLUDES DESTINATION ${LIBLEGACY_INCLUDE_DIRS}
        PUBLIC_HEADER DESTINATION "${CMAKE_INSTALL_INCLUDEDIR}/${PROJECT_NAME}")
install(EXPORT BrainiacConfig
        NAMESPACE brainiac::
        DESTINATION "${CMAKE_INSTALL_DATADIR}/${PROJECT_NAME}/cmake")
export(EXPORT BrainiacConfig)
export(PACKAGE brainiac)

if(WITH_CLI)
    install(TARGETS brainiac-cli
            RUNTIME DESTINATION ${CMAKE_INSTALL_BINDIR})
    install(FILES man/brainfuck.1 DESTINATION "${CMAKE_INSTALL_MANDIR}/man1")
endif()

if(INSTALL_EXAMPLES)
    install(DIRECTORY examples DESTINATION "${CMAKE_INSTALL_DATADIR}/${PROJECT_NAME}")
endif()

## Testing ##
include(CTest)

if(BUILD_TESTING)
    add_subdirectory(tests)
endif()
