cmake_minimum_required (VERSION 3.17)

find_package(PkgConfig REQUIRED)
pkg_check_modules(CURLPP REQUIRED curlpp)

execute_process(
        COMMAND brew --prefix curlpp
        OUTPUT_VARIABLE CURLPP_ROOT
        OUTPUT_STRIP_TRAILING_WHITESPACE
)

message(STATUS "curlpp prefix: ${CURLPP_ROOT}")

add_executable(datafetcher DataFetcher.cpp)

target_include_directories(datafetcher PRIVATE "${CURLPP_ROOT}/include")
target_link_libraries(datafetcher PRIVATE curlpp curl)
target_link_directories(datafetcher PRIVATE "${CURLPP_ROOT}/lib")