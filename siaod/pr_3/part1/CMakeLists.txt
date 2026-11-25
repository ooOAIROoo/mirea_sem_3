cmake_minimum_required(VERSION 3.16)
project(siaod_pr3_part1 LANGUAGES CXX)

set(CMAKE_CXX_STANDARD 17)
set(CMAKE_CXX_STANDARD_REQUIRED ON)
set(CMAKE_CXX_EXTENSIONS OFF)

if(MSVC)
    add_compile_options(/W4 /utf-8)
else()
    add_compile_options(-Wall -Wextra -Wpedantic)
endif()

add_library(rbtree_lib
    src/rbtree.cpp
)
target_include_directories(rbtree_lib PUBLIC ${CMAKE_CURRENT_SOURCE_DIR}/include)

add_executable(pr3_part1 src/main.cpp)
target_link_libraries(pr3_part1 PRIVATE rbtree_lib)

enable_testing()

add_executable(rbtree_tests tests/rbtree_tests.cpp)
target_link_libraries(rbtree_tests PRIVATE rbtree_lib)
add_test(NAME rbtree_tests COMMAND rbtree_tests)
