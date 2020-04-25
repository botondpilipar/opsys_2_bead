cmake_minimum_required(VERSION 3.17)

project(OperatingSystemsAssignment2 C CXX)

set(PCH dependencies/pch.h)
set(CMAKE_EXPORT_COMPILE_COMMANDS ON)
set(CMAKE_C_STANDARD 11)
set(CMAKE_C_STANDARD_REQUIRED ON)

add_subdirectory(test)
add_subdirectory(processes)
add_subdirectory(main)
add_subdirectory(dependencies)

set(PROJECT_TARGETS 
    Processes 
    Main 
    DatabaseManager
    DatabaseManagerTestExec # C++ google test target
    ConsoleIO
    InputControlTestExec)
set(APPLIED_COMPILE_OPTIONS)
set(PROJECT_INCLUDE_DIRECTORIES 
    dependencies
    dependencies/console_interface
    dependencies/database_manager
    processes
    processes/datasets)

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

if(${CMAKE_BUILD_TYPE} EQUAL Release)
    set(APPLIED_COMPILE_OPITONS ${RELEASE_COMPILE_OPTIONS})
elseif(${CMAKE_BUILD_TYPE} EQUAL Debug)
    set(APPLIED_COMPILE_OPTIONS ${DEBUG_COMPILE_OPTIONS})
else()
    set(APPLIED_COMPILE_OPTIONS "")
endif()


foreach(TARGET ${PROJECT_TARGETS})
    target_compile_options(
        ${TARGET}
        PUBLIC ${APPLIED_COMPILE_OPTIONS}
    )
    target_include_directories(
        ${TARGET}
        PUBLIC ${PROJECT_INCLUDE_DIRECTORIES}
    )
    target_precompile_headers(
        ${TARGET}
        PUBLIC ${PCH}
    )
endforeach(TARGET C_TARGETS)

