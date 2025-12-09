cmake_minimum_required (VERSION 3.17)

# For the DataFetcherExample instantiation

find_package(PkgConfig REQUIRED)
pkg_check_modules(CURLPP REQUIRED curlpp)

execute_process(
        COMMAND brew --prefix curlpp
        OUTPUT_VARIABLE CURLPP_ROOT
        OUTPUT_STRIP_TRAILING_WHITESPACE
)

message(STATUS "curlpp prefix: ${CURLPP_ROOT}")

add_library(common_objs OBJECT
        ${CMAKE_CURRENT_LIST_DIR}/DataFetcher.cpp
        ${CMAKE_CURRENT_LIST_DIR}/DefaultDay.cpp
        ${CMAKE_CURRENT_LIST_DIR}/Register.cpp
)

target_include_directories(common_objs PUBLIC "${CURLPP_ROOT}/include")
target_link_libraries(common_objs PUBLIC curlpp curl)
target_link_directories(common_objs PUBLIC "${CURLPP_ROOT}/lib")

add_executable(datafetcher DataFetcherExample.cpp)
target_link_libraries(datafetcher PUBLIC common_objs)