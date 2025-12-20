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
    const auto day_f = common::Register::getInstance().getDayFactory(1);
    auto [result1, result2] = day_f(test_data, std::nullopt)->run();
    ASSERT_EQ(result1, 3);
    ASSERT_EQ(result2, 6);
}

TEST(advent, day_2) {
    const std::vector<std::string> test_data = {
        "11-22,95-115,998-1012,1188511880-1188511890,222220-222224,1698522-1698528,446443-446449,38593856-38593862,565653-565659,824824821-824824827,2121212118-2121212124"
    };
    const auto day_f = common::Register::getInstance().getDayFactory(2);
    auto [result1, result2] = day_f(test_data, std::nullopt)->run();
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
    const auto day_f = common::Register::getInstance().getDayFactory(3);
    auto [result1, result2] = day_f(test_data, std::nullopt)->run();
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
    const auto day_f = common::Register::getInstance().getDayFactory(4);
    auto [result1, result2] = day_f(test_data, std::nullopt)->run();
    ASSERT_EQ(result1, 13);
    ASSERT_EQ(result2, 43);
}

TEST(advent, day_5) {
    const std::vector<std::string> test_data = {
        "3-5",
        "10-14",
        "16-20",
        "12-18",
        "",
        "1",
        "5",
        "8",
        "11",
        "17",
        "32",
    };
    const auto day_f = common::Register::getInstance().getDayFactory(5);
    auto [result1, result2] = day_f(test_data, std::nullopt)->run();
    ASSERT_EQ(result1, 3);
    ASSERT_EQ(result2, 14);
}

TEST(advent, day_6) {
    const std::vector<std::string> test_data = {
        "123 328  51 64 ",
        " 45 64  387 23 ",
        "  6 98  215 314",
        "*   +   *   +  ",
    };
    const auto day_f = common::Register::getInstance().getDayFactory(6);
    auto [result1, result2] = day_f(test_data, std::nullopt)->run();
    ASSERT_EQ(result1, 4277556);
    ASSERT_EQ(result2, 3263827);
}

TEST(advent, day_7) {
    const std::vector<std::string> test_data = {
        ".......S.......",
        ".......|.......",
        "......|^|......",
        "......|.|......",
        ".....|^|^|.....",
        ".....|.|.|.....",
        "....|^|^|^|....",
        "....|.|.|.|....",
        "...|^|^|||^|...",
        "...|.|.|||.|...",
        "..|^|^|||^|^|..",
        "..|.|.|||.|.|..",
        ".|^|||^||.||^|.",
        ".|.|||.||.||.|.",
        "|^|^|^|^|^|||^|",
        "|.|.|.|.|.|||.|",
    };
    const auto day_f = common::Register::getInstance().getDayFactory(7);
    auto [result1, result2] = day_f(test_data, std::nullopt)->run();
    ASSERT_EQ(result1, 21);
    ASSERT_EQ(result2, 40);
}

TEST(advent, day_8) {
    const std::vector<std::string> test_data = {
        "162,817,812",
        "57,618,57",
        "906,360,560",
        "592,479,940",
        "352,342,300",
        "466,668,158",
        "542,29,236",
        "431,825,988",
        "739,650,466",
        "52,470,668",
        "216,146,977",
        "819,987,18",
        "117,168,530",
        "805,96,715",
        "346,949,466",
        "970,615,88",
        "941,993,340",
        "862,61,35",
        "984,92,344",
        "425,690,689",
    };
    const auto day_f = common::Register::getInstance().getDayFactory(8);
    auto [result1, result2] = day_f(test_data, 10)->run();
    ASSERT_EQ(result1, 40);
    ASSERT_EQ(result2, 25272);
}

TEST(advent, day_9) {
    const std::vector<std::string> test_data = {
        "7,1",
        "11,1",
        "11,7",
        "9,7",
        "9,5",
        "2,5",
        "2,3",
        "7,3",
    };
    const auto day_f = common::Register::getInstance().getDayFactory(9);
    auto [result1, result2] = day_f(test_data, std::nullopt)->run();
    ASSERT_EQ(result1, 50);
    ASSERT_EQ(result2, 24);
}

TEST(advent, day_10) {
    const std::vector<std::string> test_data = {
        "",
        "",
    };
    const auto day_f = common::Register::getInstance().getDayFactory(10);
    auto [result1, result2] = day_f(test_data, std::nullopt)->run();
    ASSERT_EQ(result1, std::nullopt);
    ASSERT_EQ(result2, std::nullopt);
}

TEST(advent, day_11) {
    const std::vector<std::string> test_data = {
        "",
        "",
    };
    const auto day_f = common::Register::getInstance().getDayFactory(11);
    auto [result1, result2] = day_f(test_data, std::nullopt)->run();
    ASSERT_EQ(result1, std::nullopt);
    ASSERT_EQ(result2, std::nullopt);
}

TEST(advent, day_12) {
    const std::vector<std::string> test_data = {
        "",
        "",
    };
    const auto day_f = common::Register::getInstance().getDayFactory(12);
    auto [result1, result2] = day_f(test_data, std::nullopt)->run();
    ASSERT_EQ(result1, std::nullopt);
    ASSERT_EQ(result2, std::nullopt);
}

}