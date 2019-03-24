cmake_minimum_required(VERSION 3.1)

add_library(brainiac-vm-lightning OBJECT
        lightning.h
        lightning.c)
target_include_directories(brainiac-vm-lightning
        PRIVATE ../../../include)

set_target_properties(brainiac-vm-lightning PROPERTIES C_STANDARD 90)
target_link_libraries(brainiac-vm-lightning PUBLIC -llightning)
# @BUG: See https://gitlab.kitware.com/cmake/cmake/issues/17357
install(TARGETS brainiac-vm-lightning EXPORT BrainiacConfig)
