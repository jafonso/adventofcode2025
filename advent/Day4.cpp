#include "../common/AutoRegister.hpp"

namespace advent {

class Day4 final : public common::BaseDay {
public:
    using BaseDay::BaseDay;
    common::AocResultType run() override {

        // Prepare

        std::vector<std::string> input_with_margin;

        input_with_margin.emplace_back(std::string('.', m_input[0].size() + 2));

        for (auto & line : m_input) {
            input_with_margin.emplace_back("." + line + ".");
        }

        input_with_margin.emplace_back(std::string('.', m_input[0].size() + 2));

        // Part 1

        int result_1 = 0;

        for (std::size_t i = 1; i <= m_input.size(); ++i) {
            for (std::size_t j = 1; j <= m_input.size(); ++j) {
                if (isMovableRoll(input_with_margin, i, j)) {
                    ++result_1;
                }
            }
        }

        // Part 2

        int result_2 = 0;

        for (std::size_t i = 1; i <= m_input.size(); ++i) {
            for (std::size_t j = 1; j <= m_input.size(); ++j) {
                result_2 += tryRemoveRolls(input_with_margin, i, j);
            }
        }

        return std::pair(result_1, result_2);
    }
private:
    int tryRemoveRolls(std::vector<std::string> & input, int i, int j) {
        int removed_rolls = 0;
        if (isMovableRoll(input, i, j)) {
            input[i][j] = '.';
            removed_rolls++;
            // Try to remove all rolls around, recursively
            for (int ii = i - 1; ii <= i + 1; ii++) {
                for (int jj = j - 1; jj <= j + 1; jj++) {
                    removed_rolls += tryRemoveRolls(input, ii, jj);
                }
            }
        }
        return removed_rolls;
    }
    bool isMovableRoll(const std::vector<std::string> & input, int i, int j) {
       return input[i][j] == '@' && countAdjRolls(input, i, j) < 4;
    }
    int countAdjRolls(const std::vector<std::string> & input, int i, int j) {
        int result = 0;
        for (int ii = i - 1; ii <= i + 1; ii++) {
            for (int jj = j - 1; jj <= j + 1; jj++) {
                if (input[ii][jj] == '@') {
                    result += 1;
                }
            }
        }
        result--; // Remove the one in the middle
        return result;
    }
};

// To force registration at startup
namespace {
    [[maybe_unused]]
    common::AutoRegister<Day4, 4> day4_registrar;
}

}