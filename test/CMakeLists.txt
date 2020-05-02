cmake_minimum_required(VERSION 3.17)

enable_language(CXX)
set(CXX_STANDARD 11)
set(CXX_STANDARD_REQUIRED ON)

add_subdirectory(googletest)

add_executable(
    DatabaseManagerTestExec
    database_manager_test.hh
    database_manager_test.cc
)

add_executable(
    DailyTaskManagerTestExec
    DailyTaskManagerTest.cc
)

add_executable(
    InputControlTestExec
    input_control_test.cc
)

add_custom_command(
    TARGET DatabaseManagerTestExec
    COMMAND ${CMAKE_COMMAND} -E copy_directory
    ${CMAKE_SOURCE_DIR}/resources
    ${CMAKE_CURRENT_BINARY_DIR}/resources
)

add_custom_command(
    TARGET DailyTaskManagerTestExec
    COMMAND ${CMAKE_COMMAND} -E copy_directory
    ${CMAKE_SOURCE_DIR}/resources
    ${CMAKE_CURRENT_BINARY_DIR}/resources
)