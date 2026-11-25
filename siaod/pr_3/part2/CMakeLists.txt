cmake_minimum_required(VERSION 3.16)
project(siaod_pr3_part2 LANGUAGES CXX)

set(CMAKE_CXX_STANDARD 17)
set(CMAKE_CXX_STANDARD_REQUIRED ON)
set(CMAKE_CXX_EXTENSIONS OFF)

if(MSVC)
    add_compile_options(/W4 /utf-8)
else()
    add_compile_options(-Wall -Wextra -Wpedantic)
endif()

add_library(graph_lib
    src/graph.cpp
)
target_include_directories(graph_lib PUBLIC ${CMAKE_CURRENT_SOURCE_DIR}/include)

add_executable(pr3_part2 src/main.cpp)
target_link_libraries(pr3_part2 PRIVATE graph_lib)

enable_testing()

add_executable(graph_tests tests/graph_tests.cpp)
target_link_libraries(graph_tests PRIVATE graph_lib)
add_test(NAME graph_tests COMMAND graph_tests)
