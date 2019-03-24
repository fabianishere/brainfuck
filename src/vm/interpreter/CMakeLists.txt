cmake_minimum_required(VERSION 3.1)

add_library(brainiac-vm-interpreter OBJECT
        interpreter.h
        interpreter.c)
target_include_directories(brainiac-vm-interpreter
        PRIVATE ../../../include)

set_target_properties(brainiac-vm-interpreter PROPERTIES C_STANDARD 90)
# @BUG: See https://gitlab.kitware.com/cmake/cmake/issues/17357
install(TARGETS brainiac-vm-interpreter EXPORT BrainiacConfig)
