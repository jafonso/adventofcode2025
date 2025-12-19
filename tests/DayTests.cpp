#include <gtest/gtest.h>
#include "../common/Register.hpp"
#include "../common/BaseDay.hpp"
#include "../common/DataFetcher.hpp"

namespace advent::tests {

TEST(advent, day_1) {
    const std::vector<std::string> test_data = {
        "L68",
        "L30",
        "R48",
        "L5",
        "R60",
        "L55",
        "L1",
        "L99",
        "R14",
        "L82",
    };
    const auto day1f = common::Register::getInstance().getDayFactory(1);
    auto [result1, result2] = day1f(test_data)->run();
    ASSERT_EQ(result1, 3);
    ASSERT_EQ(result2, 6);
}

TEST(advent, day_2) {
    const std::vector<std::string> test_data = {
        "11-22,95-115,998-1012,1188511880-1188511890,222220-222224,1698522-1698528,446443-446449,38593856-38593862,565653-565659,824824821-824824827,2121212118-2121212124"
    };
    const auto day2f = common::Register::getInstance().getDayFactory(2);
    auto [result1, result2] = day2f(test_data)->run();
    ASSERT_EQ(result1, 1227775554);
    ASSERT_EQ(result2, 4174379265);
}

TEST(advent, day_3) {
    const std::vector<std::string> test_data = {
        "987654321111111",
        "811111111111119",
        "234234234234278",
        "818181911112111",
    };
    const auto day3f = common::Register::getInstance().getDayFactory(3);
    auto [result1, result2] = day3f(test_data)->run();
    ASSERT_EQ(result1, 357);
    ASSERT_EQ(result2, 3121910778619);
}

TEST(advent, day_4) {
    const std::vector<std::string> test_data = {
        "..@@.@@@@.",
        "@@@.@.@.@@",
        "@@@@@.@.@@",
        "@.@@@@..@.",
        "@@.@@@@.@@",
        ".@@@@@@@.@",
        ".@.@.@.@@@",
        "@.@@@.@@@@",
        ".@@@@@@@@.",
        "@.@.@@@.@.",
    };
    const auto day4f = common::Register::getInstance().getDayFactory(4);
    auto [result1, result2] = day4f(test_data)->run();
    ASSERT_EQ(result1, 13);
    ASSERT_EQ(result2, 43);
}

}