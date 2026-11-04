cmake_minimum_required(VERSION 3.16)
project(siaod_pr2_part2 LANGUAGES CXX)

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

add_library(text_search src/text_search.cpp)
target_include_directories(text_search PUBLIC include)
enable_project_warnings(text_search)

add_executable(text_search_app src/main.cpp)
target_link_libraries(text_search_app PRIVATE text_search)
enable_project_warnings(text_search_app)

include(CTest)
if(BUILD_TESTING)
    add_executable(text_search_tests tests/text_search_tests.cpp)
    target_link_libraries(text_search_tests PRIVATE text_search)
    enable_project_warnings(text_search_tests)
    add_test(NAME text_search_tests COMMAND text_search_tests)

    add_test(
        NAME cli_workflow
        COMMAND ${CMAKE_COMMAND}
            "-DAPP=$<TARGET_FILE:text_search_app>"
            "-DTASK1_INPUT=${CMAKE_CURRENT_SOURCE_DIR}/tests/cli_task1_scenario.txt"
            "-DTASK2_INPUT=${CMAKE_CURRENT_SOURCE_DIR}/tests/cli_task2_scenario.txt"
            "-DEOF_INPUT=${CMAKE_CURRENT_SOURCE_DIR}/tests/cli_eof_scenario.txt"
            -P "${CMAKE_CURRENT_SOURCE_DIR}/tests/test_cli.cmake"
    )
endif()
