#include "../common/AutoRegister.hpp"

namespace advent {

class Day1 final : public common::BaseDay {
public:
    using BaseDay::BaseDay;
    common::AocResultType run() override {

        // Part 1 & 2

        int result1 = 0;
        int result2 = 0;
        int currentposition = 50;
        for (auto & line : m_input) {
            const char lr = line[0];
            const int value = std::stoi(line.substr(1));
            if (value == 0) continue;
            if (lr == 'L') {
                auto temp_position = currentposition - value;
                if (temp_position <= 0) {
                    if (currentposition != 0) {
                        result2 += 1; // For crossing zero first, except if already there
                    }
                    result2 += (-temp_position / 100); // Other times it crossed zero
                }
                currentposition = ((temp_position % 100) + 100) % 100;
            } else {
                auto temp_position = currentposition + value;
                if (temp_position >= 100) {
                    result2 += (temp_position / 100); // Other times it crossed zero
                }
                currentposition = temp_position % 100;
            }
            if (currentposition == 0) {
                result1++;
            }
        }

        return std::pair(result1, result2);
    }
};

// To force registration at startup
namespace {
    [[maybe_unused]]
    common::AutoRegister<Day1, 1> day1_registrar;
}

}