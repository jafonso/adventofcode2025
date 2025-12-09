cmake_minimum_required (VERSION 3.17)

add_executable(advent advent.cpp)

target_link_libraries(advent PRIVATE common_objs)