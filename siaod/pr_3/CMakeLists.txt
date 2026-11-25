cmake_minimum_required(VERSION 3.16)
project(siaod_pr3 LANGUAGES CXX)

# Общая сборка обеих частей практической работы №7.
# Можно собирать каждую часть отдельно из part1/ или part2/.

enable_testing()

add_subdirectory(part1)
add_subdirectory(part2)
