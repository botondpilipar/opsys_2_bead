cmake_minimum_required(VERSION 3.17)

add_library(DatabaseManager 
    database_macros.h
    database.h
    worker_entry.h
    worker_entry.c
    working_days.h
    day_enum_conversion.h
    day_enum_conversion.c
    worker_database_managers.h
    worker_database_managers.c
)

target_compile_options(DatabaseManager 
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
)

target_precompile_headers(DatabaseManager
    PUBLIC
    ${CMAKE_SOURCE_DIR}/etc/pch.h
)