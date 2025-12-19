cmake_minimum_required (VERSION 3.17)

execute_process(
        COMMAND brew --prefix googletest
        OUTPUT_VARIABLE GTest_ROOT
        OUTPUT_STRIP_TRAILING_WHITESPACE
)

message(STATUS "googletest prefix: ${GTest_ROOT}")

# Make sure CMake sees the GTest config file
list(APPEND CMAKE_PREFIX_PATH "${GTest_ROOT}")

# Use the CMake package, NOT pkg-config
find_package(GTest REQUIRED CONFIG)

# Now define the test executable
add_executable(advent_tests
        DayTests.cpp
        ../advent/Day1.cpp
        ../advent/Day2.cpp
        ../advent/Day3.cpp
        ../advent/Day4.cpp
        ../advent/Day5.cpp
)

# Link against the imported gtest targets
target_link_libraries(advent_tests
        PRIVATE
        common_objs
        GTest::gtest
        GTest::gtest_main
)

# Optionally enable test discovery
include(GoogleTest)
gtest_discover_tests(advent_tests)