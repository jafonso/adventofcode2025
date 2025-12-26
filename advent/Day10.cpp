#include <charconv>

#include "../common/AutoRegister.hpp"

#include <regex>
#include <set>
#include <vector>
#include <ranges>
#include <bitset>
#include <queue>

namespace advent {

class Day10 final : public common::BaseDay {
public:
    static constexpr int BITSET_SIZE = 16;
    using LightsT = std::bitset<BITSET_SIZE>;
    using ButtonsT = std::vector<std::bitset<BITSET_SIZE>>;
    using JoltagesT = std::vector<int>;
    using ParsedInput = std::tuple<LightsT, ButtonsT, JoltagesT>;
    using BaseDay::BaseDay;
    common::AocResultType run() override {

        // Get input

        std::vector<ParsedInput> input;
        for (auto line : m_input) {
            input.push_back(parseInput(line));
        }

        // ==== Part 1 ====

        uint64_t result_1 = 0;

        for (auto & in : input) {
            auto in_result = findFewestPresses(std::get<0>(in), std::get<1>(in));
            if (!in_result.has_value()) {
                throw std::runtime_error("Unable to find buttons leading to lights");
            }
            result_1 += in_result.value();
        }

        return std::pair(result_1, std::nullopt);
    }
private:
    static std::optional<int> findFewestPresses(const LightsT & lights, const ButtonsT & buttons) {

        std::queue<std::tuple<int, LightsT, std::bitset<BITSET_SIZE>>> bfs_queue;

        if (lights.none()) {
            return 0;
        }

        bfs_queue.emplace(0, lights, std::bitset<BITSET_SIZE>());

        while (!bfs_queue.empty()) {
            auto & [depth, curr_lights, used_buttons] = bfs_queue.front();
            for (std::size_t i = 0; i < buttons.size(); i++) {
                if (!used_buttons[i] && (buttons[i] & curr_lights).any()) {
                    auto next_lights = curr_lights;
                    auto next_used_buttons = used_buttons;
                    next_lights ^= buttons[i];
                    next_used_buttons[i] = true;
                    if (next_lights.none()) {
                        return depth + 1;
                    }
                    bfs_queue.emplace(depth + 1, next_lights, std::move(next_used_buttons));
                }
            }
            bfs_queue.pop();
        }

        return std::nullopt;
    }
    static ParsedInput parseInput(const std::string& line) {
        std::regex input_regex(R"((\[.*\]) (\(.*\)) (\{.*\}))");
        std::smatch match;
        LightsT lights;
        ButtonsT buttons;
        JoltagesT joltages;
        if (std::regex_match(line, match, input_regex)) {
            std::string lights_match = match[1].str();
            std::string buttons_match = match[2].str();
            std::string joltage_match = match[3].str();

            std::size_t idx = 0;
            for (char c : lights_match) {
                if (c == '#') {
                    lights.set(idx, true);
                    idx++;
                } else if (c == '.') {
                    idx++;
                }
            }

            for (auto button_str : buttons_match | std::views::split(' ')) {
                std::string_view button_view((button_str.data()), button_str.size());
                std::bitset<BITSET_SIZE> new_button;
                for (auto number_str : button_view.substr(1, button_view.size() - 2) | std::views::split(',')) {
                    int number;
                    std::from_chars(number_str.data(), number_str.data() + number_str.size(), number);
                    new_button.set(number, true);
                }
                buttons.emplace_back(new_button);
            }

            for (auto joltage_str : joltage_match.substr(1, joltages.size() - 2) | std::views::split(',')) {
                int joltage;
                std::from_chars(joltage_str.data(), joltage_str.data() + joltage_str.size(), joltage);
                joltages.push_back(joltage);
            }

        } else {
            throw std::runtime_error("Regex does not match: " + line);
        }

        return {lights, buttons, joltages};
    }
    std::map<LightsT, std::optional<int>> m_cached;
};

// To force registration at startup
namespace {
    [[maybe_unused]]
    common::AutoRegister<Day10, 10> day10_registrar;
}

}