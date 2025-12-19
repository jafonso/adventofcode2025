cmake_minimum_required (VERSION 3.17)

add_executable(advent advent.cpp)

target_sources(advent PRIVATE
        day1.cpp
        day2.cpp
        day3.cpp
        day4.cpp
        day5.cpp
        day6.cpp
        day7.cpp
        day8.cpp
        day9.cpp
        day10.cpp
        day11.cpp
        day12.cpp)

target_link_libraries(advent PRIVATE common_objs)