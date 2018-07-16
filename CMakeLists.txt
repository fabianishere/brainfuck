cmake_minimum_required(VERSION 3.1)

project(brainfuck
    LANGUAGES C
    VERSION 2.7.3
)

option(ENABLE_CLI "Enable the command line interface." ON)
option(ENABLE_EDITLINE "Enable GNU readline functionality provided by the editline library." ON)
option(ENABLE_EXTENSION_DEBUG "Enable the debug extension for brainfuck.")
option(INSTALL_EXAMPLES "Installs the examples.")

add_library(brainfuck include/brainfuck.h src/brainfuck.c)
set_target_properties(brainfuck PROPERTIES
    PUBLIC_HEADER "include/brainfuck.h"
    C_STANDARD 90
)
target_compile_options(brainfuck PRIVATE -Wall -Wextra -O2)
target_include_directories(brainfuck PUBLIC
    $<BUILD_INTERFACE:${CMAKE_CURRENT_SOURCE_DIR}/include/>
    $<INSTALL_INTERFACE:include/brainfuck/>
)
target_compile_definitions(brainfuck PUBLIC
    "-DBRAINFUCK_VERSION_MAJOR=${PROJECT_VERSION_MAJOR}"
    "-DBRAINFUCK_VERSION_MINOR=${PROJECT_VERSION_MINOR}"
    "-DBRAINFUCK_VERSION_PATCH=${PROJECT_VERSION_PATCH}"
)
install(TARGETS brainfuck
    LIBRARY DESTINATION lib
    ARCHIVE DESTINATION lib
    PUBLIC_HEADER DESTINATION include/brainfuck
)

if(ENABLE_CLI)
    add_executable(brainfuck-cli src/main.c)
    set_target_properties(brainfuck-cli PROPERTIES
        OUTPUT_NAME "brainfuck"
        C_STANDARD 90
    )
    target_compile_options(brainfuck-cli PRIVATE -Wall -Wextra -O2)
    target_link_libraries(brainfuck-cli brainfuck)
    install(TARGETS brainfuck-cli RUNTIME DESTINATION bin)
    install(FILES man/brainfuck.1 DESTINATION "share/man/man1")

    if(ENABLE_EDITLINE)
        target_compile_definitions(brainfuck-cli PRIVATE "-DBRAINFUCK_EDITLINE_LIB")
        target_link_libraries(brainfuck-cli -ledit)
    endif()
endif()

if(ENABLE_EXTENSION_DEBUG)
    add_definitions("-DBRAINFUCK_EXTENSION_DEBUG")
endif()

if(INSTALL_EXAMPLES)
    set(EXAMPLES_DIR "share/brainfuck")
    file(MAKE_DIRECTORY ${CMAKE_INSTALL_PREFIX}/${EXAMPLES_DIR})
    install(DIRECTORY examples DESTINATION ${EXAMPLES_DIR})
endif()
