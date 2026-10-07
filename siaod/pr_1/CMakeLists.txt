cmake_minimum_required(VERSION 3.16)
project(siaod_pr1 LANGUAGES CXX)

# Для работы достаточно стандартного C++17: сторонних библиотек в C++-коде нет.
set(CMAKE_CXX_STANDARD 17)
set(CMAKE_CXX_STANDARD_REQUIRED ON)
set(CMAKE_CXX_EXTENSIONS OFF)

add_library(siaod_common
    common/src/Record.cpp
    common/src/RecordFile.cpp
    common/src/Search.cpp
    common/src/Benchmark.cpp
)
target_include_directories(siaod_common PUBLIC common/include)

# В GCC 8 и старше std::filesystem дополнительно линкуется с stdc++fs.
if(CMAKE_CXX_COMPILER_ID STREQUAL "GNU" AND CMAKE_CXX_COMPILER_VERSION VERSION_LESS 9)
    target_link_libraries(siaod_common PUBLIC stdc++fs)
endif()

# UTF-8 нужен для строк русского интерфейса, в том числе при сборке в Windows/MSVC.
if(MSVC)
    target_compile_options(siaod_common PRIVATE /W4 /utf-8)
else()
    target_compile_options(siaod_common PRIVATE -Wall -Wextra -Wpedantic)
endif()

add_executable(task_1_app task_1/main.cpp)
add_executable(task_2_app task_2/main.cpp)
add_executable(task_3_app task_3/main.cpp)

foreach(target task_1_app task_2_app task_3_app)
    target_link_libraries(${target} PRIVATE siaod_common)
    if(MSVC)
        target_compile_options(${target} PRIVATE /W4 /utf-8)
    else()
        target_compile_options(${target} PRIVATE -Wall -Wextra -Wpedantic)
    endif()
endforeach()

include(CTest)
if(BUILD_TESTING)
    add_executable(siaod_tests tests/test_search.cpp)
    target_link_libraries(siaod_tests PRIVATE siaod_common)
    if(MSVC)
        target_compile_options(siaod_tests PRIVATE /W4 /utf-8)
    else()
        target_compile_options(siaod_tests PRIVATE -Wall -Wextra -Wpedantic)
    endif()
    add_test(NAME search_and_binary_file_tests COMMAND siaod_tests)
endif()
