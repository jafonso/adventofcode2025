#include "../common/AutoRegister.hpp"
#include "Day1.hpp"

namespace advent {

common::AocResultType Day1::run(std::vector<std::string> input) {

    // Part 1 & 2

    int result1 = 0;
    int result2 = 0;
    int currentposition = 50;
    for (auto & line : input) {
        const char lr = line[0];
        const int value = std::stoi(line.substr(1));
        if (lr == 'L') {
            auto temp_position = currentposition - value;
            result2 += (-temp_position / 100);
            if (temp_position < 0 && currentposition > 0) {
                result2++;
            }
            currentposition = ((temp_position % 100) + 100) % 100;
        } else {
            auto temp_position = currentposition + value;
            result2 += (temp_position / 100);
            currentposition = temp_position % 100;
        }
        if (currentposition == 0) {
            result1++;
        }
    }

    return std::pair(result1, result2);
}

// To force registration at startup
namespace {
    [[maybe_unused]]
    common::AutoRegister<Day1, 1> day1_registrar;
}

}