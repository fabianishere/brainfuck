cmake_minimum_required(VERSION 3.8)

project(brainfuck
        LANGUAGES C
        VERSION 2.7.3
        DESCRIPTION "Brainfuck interpreter written in C")

set(CMAKE_MODULE_PATH "${PROJECT_SOURCE_DIR}/cmake" ${CMAKE_MODULE_PATH})

## Compile-time options ##
option(ENABLE_CLI "Enable the command line interface" ON)
option(ENABLE_EDITLINE "Enable GNU readline functionality provided by the editline library" ON)
option(ENABLE_EXTENSION_DEBUG "Enable the debug extension for brainfuck")
option(INSTALL_EXAMPLES "Install the examples")

## Target ##
set(BRAINFUCK_H include/brainfuck.h)
set(BRAINFUCK_C_FLAGS
        $<$<OR:$<C_COMPILER_ID:Clang>,$<C_COMPILER_ID:AppleClang>,$<C_COMPILER_ID:GNU>>:
            -Wall -Wextra>
        $<$<C_COMPILER_ID:MSVC>:/Wall /Zi>)


add_library(brainfuck ${BRAINFUCK_H} src/brainfuck.c)
target_include_directories(brainfuck PUBLIC
    $<BUILD_INTERFACE:${CMAKE_CURRENT_SOURCE_DIR}/include/>
    $<INSTALL_INTERFACE:include/>
)
target_compile_definitions(brainfuck PUBLIC
    "-DBRAINFUCK_VERSION_MAJOR=${PROJECT_VERSION_MAJOR}"
    "-DBRAINFUCK_VERSION_MINOR=${PROJECT_VERSION_MINOR}"
    "-DBRAINFUCK_VERSION_PATCH=${PROJECT_VERSION_PATCH}"
)
target_compile_features(brainfuck PRIVATE c_std_90)
target_compile_options(brainfuck PRIVATE ${BRAINFUCK_C_FLAGS})

## Compile-time features ##
if(ENABLE_EXTENSION_DEBUG)
    target_compile_definitions(brainfuck PRIVATE "-DBRAINFUCK_EXTENSION_DEBUG")
endif()

if(ENABLE_CLI)
    find_package(getopt REQUIRED)
    add_executable(brainfuck-cli src/main.c)
    set_target_properties(brainfuck-cli PROPERTIES
        OUTPUT_NAME "brainfuck"
    )
    target_link_libraries(brainfuck-cli PRIVATE brainfuck getopt)
    target_compile_options(brainfuck-cli PRIVATE "${BRAINFUCK_C_FLAGS}")

    if(ENABLE_EDITLINE)
        find_package(editline REQUIRED)
        target_compile_definitions(brainfuck-cli PRIVATE BRAINFUCK_EDITLINE_LIB)
        target_link_libraries(brainfuck-cli PRIVATE editline)
    endif()
endif()

## Installation ##
include(GNUInstallDirs)

install(TARGETS brainfuck
        EXPORT brainfuck-config)
install(EXPORT brainfuck-config
        NAMESPACE brainfuck::
        DESTINATION "${CMAKE_INSTALL_DATADIR}/${PROJECT_NAME}/cmake")

if (ENABLE_CLI)
    install(TARGETS brainfuck-cli)
    install(FILES man/brainfuck.1 DESTINATION "share/man/man1")
endif()

if(INSTALL_EXAMPLES)
    set(EXAMPLES_DIR "share/brainfuck")
    file(MAKE_DIRECTORY ${CMAKE_INSTALL_PREFIX}/${EXAMPLES_DIR})
    install(DIRECTORY examples DESTINATION ${EXAMPLES_DIR})
endif()

## Testing ##
include(CTest)
if(BUILD_TESTING)
    add_subdirectory(tests)
endif()
