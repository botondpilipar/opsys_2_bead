cmake_minimum_required(VERSION 3.17)

enable_language(CXX)
set(CXX_STANDARD 11)
set(CXX_STANDARD_REQUIRED ON)

add_executable(
    DatabaseManagerTestExec
    database_manager_test.h
    database_manager_test.cpp
)

add_executable(
    InputControlTestExec
    input_control_test.cpp
)