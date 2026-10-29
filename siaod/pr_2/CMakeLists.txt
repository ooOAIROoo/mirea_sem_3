cmake_minimum_required(VERSION 3.16)
project(siaod_pr2 LANGUAGES CXX)

set(CMAKE_CXX_STANDARD 17)
set(CMAKE_CXX_STANDARD_REQUIRED ON)
set(CMAKE_CXX_EXTENSIONS OFF)

function(enable_project_warnings target)
    if(MSVC)
        # Исходный код и сообщения интерфейса записаны в UTF-8.
        target_compile_options(${target} PRIVATE /W4 /utf-8)
    else()
        target_compile_options(${target} PRIVATE -Wall -Wextra -Wpedantic)
    endif()
endfunction()

add_library(student_hash_table src/student_hash_table.cpp)
target_include_directories(student_hash_table PUBLIC include)
enable_project_warnings(student_hash_table)

add_executable(pr_2_app src/main.cpp)
target_link_libraries(pr_2_app PRIVATE student_hash_table)
enable_project_warnings(pr_2_app)

include(CTest)
if(BUILD_TESTING)
    add_executable(hash_table_tests tests/hash_table_tests.cpp)
    target_link_libraries(hash_table_tests PRIVATE student_hash_table)
    enable_project_warnings(hash_table_tests)
    add_test(NAME hash_table_tests COMMAND hash_table_tests)

    add_test(
        NAME cli_workflow
        COMMAND ${CMAKE_COMMAND}
            "-DAPP=$<TARGET_FILE:pr_2_app>"
            "-DINPUT=${CMAKE_CURRENT_SOURCE_DIR}/tests/cli_scenario.txt"
            "-DEMPTY_INPUT=${CMAKE_CURRENT_SOURCE_DIR}/tests/cli_empty_scenario.txt"
            "-DCOMPACT_INPUT=${CMAKE_CURRENT_SOURCE_DIR}/tests/cli_compact_scenario.txt"
            "-DEOF_INPUT=${CMAKE_CURRENT_SOURCE_DIR}/tests/cli_eof_scenario.txt"
            -P "${CMAKE_CURRENT_SOURCE_DIR}/tests/test_cli.cmake"
    )
endif()
