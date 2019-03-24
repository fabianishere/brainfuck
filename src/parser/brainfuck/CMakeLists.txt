cmake_minimum_required(VERSION 3.1)

add_library(brainiac-parser-brainfuck OBJECT
        brainfuck.h
        brainfuck.c)
target_include_directories(brainiac-parser-brainfuck
        PRIVATE ../../../include)

set_target_properties(brainiac-parser-brainfuck PROPERTIES C_STANDARD 90)
# @BUG: See https://gitlab.kitware.com/cmake/cmake/issues/17357
install(TARGETS brainiac-parser-brainfuck EXPORT BrainiacConfig)
