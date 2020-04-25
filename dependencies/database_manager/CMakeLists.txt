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
    UnregisteredWorkerEntry.h
    UnregisteredWorkerEntry.c
)
