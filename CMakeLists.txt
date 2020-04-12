cmake_minimum_required(VERSION 3.17)

project(OperatingSystemsAssignment2 C CXX PYTHON)

add_subdirectory(test)
add_subdirectory(processes)

set (PCH "processes/pch.h")
set(CMAKE_EXPORT_COMPILE_COMMANDS ON)

set(DEBUG_COMPILE_OPTIONS
    PUBLIC
    -W -Wall -Wextra
    -pedantic
    -Wfloat-equal
    -Wshadow
    -Wpointer-arith
    -Wcast-align
    -Wstrict-prototypes
    -Wstrict-overflow=5
    -Wwrite-strings
    -Waggregate-return
    -Wcast-qual
    -Wswitch-default
    -Wswitch-enum
    -Wconversion
    -Wunreachable-code
    -g
)
set(RELEASE_COMPILE_OPTIONS
    PUBLIC
    -O3
)

if(CMAKE_BUILD_TYPE EQUALS "Release")
    set(APPLIED_COMPILE_OPITONS ${RELEASE_COMPILE_OPTIONS})
elseif(CMAKE_BUILD_TYPE EQUALS "Debug")
    set(APPLIED_COMPILE_OPTIONS ${DEBUG_COMPILE_OPTIONS})

target_compile_options(
    PROCESSES
    PUBLIC
    APPLIED_COMPILE_OPITONS
)

target_compile_options(
    Main
    PUBLIC
    APPLIED_COMPILE_OPTIONS
)

