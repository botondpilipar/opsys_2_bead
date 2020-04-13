cmake_minimum_required(VERSION 3.17)

project(OperatingSystemsAssignment2 C CXX)

set(PCH dependencies/pch.h)
set(CMAKE_EXPORT_COMPILE_COMMANDS ON)

add_subdirectory(test)
add_subdirectory(processes)
add_subdirectory(main)
add_subdirectory(dependencies/database_manager)

set(C_TARGETS Processes Main DatabaseManager)
set(APPLIED_COMPILE_OPTIONS)

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


foreach(TARGET ${C_TARGETS})
    target_compile_options(
        ${TARGET}
        PUBLIC ${APPLIED_COMPILE_OPTIONS}
    )
    target_include_directories(
        ${TARGET}
        PUBLIC dependencies
    )
    target_precompile_headers(
        ${TARGET}
        PUBLIC ${PCH}
    )
endforeach(TARGET C_TARGETS)

