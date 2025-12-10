#include <gtest/gtest.h>
#include "../advent/Day1.hpp"

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
    auto day1 = Day1();
    auto result = day1.run(test_data);
    ASSERT_EQ(result.first, 3);
    ASSERT_EQ(result.second, 6);
}

}