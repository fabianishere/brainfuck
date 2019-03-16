cmake_minimum_required(VERSION 3.1)

project(brainfuck
    LANGUAGES C
    VERSION 3.0.0
)

include(CTest)

option(ENABLE_PARSER_BRAINFUCK "Enable brainfuck parser implementation" ON)
option(ENABLE_CLI "Enable the command line interface" ON)
option(ENABLE_EDITLINE "Enable GNU readline functionality provided by the editline library" ON)
option(ENABLE_EXTENSION_DEBUG "Enable the debug extension for brainfuck")
option(INSTALL_EXAMPLES "Installs the examples")

if(MSVC)
    # Force to always compile with W4
    if(CMAKE_CXX_FLAGS MATCHES "/W[0-4]")
        string(REGEX REPLACE "/W[0-4]" "/W4" CMAKE_CXX_FLAGS "${CMAKE_CXX_FLAGS}")
    else()
        set(CMAKE_CXX_FLAGS "${CMAKE_CXX_FLAGS} /W4")
    endif()
elseif(CMAKE_CXX_COMPILER_ID STREQUAL "GNU" OR CMAKE_CXX_COMPILER_ID MATCHES "Clang")
    set(CMAKE_CXX_FLAGS "${CMAKE_CXX_FLAGS} -Wall -Wno-long-long -pedantic")
endif()

# getopt shim for windows
add_library(getopt INTERFACE)
if (MSVC)
    target_include_directories(getopt INTERFACE ${CMAKE_CURRENT_SOURCE_DIR}/deps/getopt)
endif()

add_library(brainfuck
        include/brainfuck/brainfuck.h
        include/brainfuck/ir.h
        include/brainfuck/parser.h
        include/brainfuck/vm.h
        src/brainfuck.h
        src/brainfuck.c
        src/parser/parser.c
        src/parser/brainfuck.h
        src/parser/brainfuck.c
        src/vm/vm.c
)
set_target_properties(brainfuck PROPERTIES C_STANDARD 90)
target_include_directories(brainfuck PUBLIC
    $<BUILD_INTERFACE:${CMAKE_CURRENT_SOURCE_DIR}/include/>
    $<INSTALL_INTERFACE:include/>
)
install(TARGETS brainfuck
    LIBRARY DESTINATION lib
    ARCHIVE DESTINATION lib
    PUBLIC_HEADER DESTINATION include/brainfuck
)

if(ENABLE_PARSER_BRAINFUCK)
    target_compile_definitions(brainfuck PUBLIC BRAINFUCK_PARSER_BRAINFUCK_ENABLED)
endif()

if(ENABLE_CLI)
    add_executable(brainfuck-cli src/cli.c)
    set_target_properties(brainfuck-cli PROPERTIES
        OUTPUT_NAME "brainfuck"
        C_STANDARD 90
    )
    target_link_libraries(brainfuck-cli brainfuck getopt)
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

if(BUILD_TESTING)
    add_subdirectory(tests)
endif()
