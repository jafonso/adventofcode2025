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
    const auto day1 = common::Register::getInstance().getDay(1);
    auto [result1, result2] = day1->run(test_data);
    ASSERT_EQ(result1, 3);
    ASSERT_EQ(result2, 6);
}

TEST(advent, day_2) {
    const std::vector<std::string> test_data = {
        "11-22,95-115,998-1012,1188511880-1188511890,222220-222224,1698522-1698528,446443-446449,38593856-38593862,565653-565659,824824821-824824827,2121212118-2121212124"
    };

    //auto data_fetcher = advent::common::DataFetcher(2);
    const auto day2 = common::Register::getInstance().getDay(2);

    //auto data_fetcher = advent::common::DataFetcher(2);
    //auto [result1, result2] = day2->run(data_fetcher.getData());

    auto [result1, result2] = day2->run(test_data);
    ASSERT_EQ(result1, 1227775554);
    ASSERT_EQ(result2, 4174379265);
}

}