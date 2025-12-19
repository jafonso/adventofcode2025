#include "../common/AutoRegister.hpp"

namespace advent {

class Day3 : public common::BaseDay
{
public:
    common::AocResultType run(const std::vector<std::string> & input) override {

        // Part 1

        uint64_t result_1 = 0;
        for (auto & line : input) {
            result_1 += largestJoltage(line, 2);
        }

        // Part 2

        uint64_t result_2 = 0;
        for (auto & line : input) {
            result_2 += largestJoltage(line, 12);
        }

        return std::pair(result_1, result_2);
    }
private:
    static int convValue(const char c) {
        return c - '0';
    }
    static uint64_t largestJoltage(const std::string & input, const std::size_t len) {

        uint64_t result = 0;

        std::size_t curr_largest_index = 0;
        uint64_t curr_largest_value = 0;

        for (std::size_t d = len; d > 0; --d) {
            for (std::size_t i = curr_largest_index; i < input.size() - d + 1; ++i) {
                uint64_t value = convValue(input[i]);
                if (value > curr_largest_value) {
                    curr_largest_index = i;
                    curr_largest_value = value;
                }
            }
            result = result * 10 + curr_largest_value;

            // Prepare for the next digit
            curr_largest_index++;
            curr_largest_value = 0;
        }

        return result;
    }
};

// To force registration at startup
namespace {
    [[maybe_unused]]
    common::AutoRegister<Day3, 3> day3_registrar;
}

}