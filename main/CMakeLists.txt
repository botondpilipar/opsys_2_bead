cmake_minimum_required(VERSION 3.13)

add_executable(
    Main
    main.c
)

add_custom_command(
    TARGET Main
    COMMAND ${CMAKE_COMMAND} -E copy_directory
    ${CMAKE_SOURCE_DIR}/resources
    ${CMAKE_CURRENT_BINARY_DIR}/resources
)
