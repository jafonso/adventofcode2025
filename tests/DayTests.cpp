#include <gtest/gtest.h>
#include "../common/Register.hpp"
#include "../common/BaseDay.hpp"

namespace advent::tests {

TEST(advent, day_1) {
    std::vector<std::string> test_data = {
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

}