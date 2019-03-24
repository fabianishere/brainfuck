cmake_minimum_required(VERSION 3.1)

project(brainiac
    LANGUAGES C
    VERSION 3.0.0
)

include(CTest)

option(WITH_PARSER_BRAINFUCK "Enable brainfuck parser implementation" ON)
option(WITH_VM_INTERPRETER "Enable interpreter" ON)
option(WITH_VM_LIGHTNING "Enable GNU lightning JIT" ON)
option(WITH_CLI "Enable the command line interface" ON)
option(WITH_EDITLINE "Enable GNU readline functionality provided by the editline library" ON)
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

add_library(brainiac
        include/brainiac/brainiac.h
        include/brainiac/ir.h
        include/brainiac/parser.h
        include/brainiac/vm.h
        src/brainiac.h
        src/brainiac.c
        src/parser/parser.c
        src/vm/vm.c)
set_target_properties(brainiac PROPERTIES C_STANDARD 90)
target_include_directories(brainiac
        PUBLIC $<BUILD_INTERFACE:${CMAKE_CURRENT_SOURCE_DIR}/include/> $<INSTALL_INTERFACE:include/>
        PRIVATE src)
install(TARGETS brainiac
        LIBRARY DESTINATION lib
        ARCHIVE DESTINATION lib
        PUBLIC_HEADER DESTINATION include/brainiac)

if(WITH_PARSER_BRAINFUCK)
    add_subdirectory(src/parser/brainfuck)
    target_link_libraries(brainiac PRIVATE brainiac-parser-brainfuck)
    target_compile_definitions(brainiac PRIVATE BRAINIAC_PARSER_BRAINFUCK_ENABLED)
endif()

if(WITH_VM_INTERPRETER)
    add_subdirectory(src/vm/interpreter)
    target_link_libraries(brainiac PRIVATE brainiac-vm-interpreter)
    target_compile_definitions(brainiac PRIVATE BRAINIAC_VM_INTERPRETER_ENABLED)
endif()

if(WITH_VM_LIGHTNING)
    add_subdirectory(src/vm/lightning)
    target_link_libraries(brainiac PRIVATE brainiac-vm-lightning)
    target_compile_definitions(brainiac PRIVATE BRAINIAC_VM_LIGHTNING_ENABLED)
endif()

if(WITH_CLI)
    add_executable(brainiac-cli src/cli.c)
    set_target_properties(brainiac-cli PROPERTIES
            OUTPUT_NAME "brainiac"
            C_STANDARD 90)
    target_link_libraries(brainiac-cli brainiac getopt)
    install(TARGETS brainiac-cli RUNTIME DESTINATION bin)
    install(FILES man/brainfuck.1 DESTINATION "share/man/man1")

    if(WITH_EDITLINE)
        target_compile_definitions(brainiac-cli PRIVATE "-DBRAINIAC_EDITLINE_ENABLED")
        target_link_libraries(brainiac-cli -ledit)
    endif()
endif()

if(INSTALL_EXAMPLES)
    set(EXAMPLES_DIR "share/brainfuck")
    file(MAKE_DIRECTORY ${CMAKE_INSTALL_PREFIX}/${EXAMPLES_DIR})
    install(DIRECTORY examples DESTINATION ${EXAMPLES_DIR})
endif()

if(BUILD_TESTING)
    add_subdirectory(tests)
endif()
