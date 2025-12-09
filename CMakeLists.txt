cmake_minimum_required (VERSION 3.17)

project(adventofcode2025)

set(CMAKE_CXX_STANDARD 23)
set(CMAKE_CXX_STANDARD_REQUIRED ON)
set(CMAKE_CXX_EXTENSIONS OFF)

set(CMAKE_CXX_FLAGS "-fPIC -pedantic -Wall -Wextra -Werror -Wno-unused-parameter -fstack-protector-all")

add_subdirectory(common)
add_subdirectory(advent)