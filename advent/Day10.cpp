#include <charconv>

#include "../common/AutoRegister.hpp"

#include <regex>
#include <set>
#include <vector>
#include <ranges>
#include <queue>

namespace advent {

class Day10 final : public common::BaseDay {
public:
    using LightsT = std::vector<bool>;
    using ButtonsT = std::set<std::set<int>>;
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

        /*int result_1 = 0;

        int idx = 0;
        for (auto & [target_lights, buttons, joltages] : input ) {
            std::cout << idx++ << std::endl;
            auto line_result = recursiveTry(target_lights, std::vector<bool>(target_lights.size(), false), buttons);
            if (!line_result.has_value()) {
                throw std::runtime_error("No result fount for input line");
            }
            result_1 += line_result.value();
        }*/

        return std::pair(std::nullopt, std::nullopt);
    }
private:
     std::optional<int> recursiveTry(const LightsT & ref_lights, const LightsT & cur_lights, const ButtonsT & buttons) {
        if (auto it = m_cached.find(std::make_tuple(ref_lights, cur_lights, buttons)); it != m_cached.end()) {
            return it->second;
        }
        if (ref_lights == cur_lights) {
            return 0;
        }
        //std::cout << buttons.size() << std::endl;
        for (auto & button : buttons) {
            auto next_lights = cur_lights;
            auto remaining_buttons = buttons;
            remaining_buttons.erase(button);
            for (auto button_nr : button) {
                next_lights[button_nr] = !next_lights[button_nr];
            }
            auto result = recursiveTry(ref_lights, next_lights, remaining_buttons);
            if (result.has_value()) {
                m_cached[std::make_tuple(ref_lights, cur_lights, buttons)] = result.value() + 1;
                return result.value() + 1;
            }
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

            for (char c : lights_match) {
                if (c == '.') {
                    lights.emplace_back(false);
                } else if (c == '#') {
                    lights.emplace_back(true);
                }
            }

            for (auto button_str : buttons_match | std::views::split(' ')) {
                std::string_view button_view((button_str.data()), button_str.size());
                std::set<int> new_button;
                for (auto number_str : button_view.substr(1, button_view.size() - 2) | std::views::split(',')) {
                    int number;
                    std::from_chars(number_str.data(), number_str.data() + number_str.size(), number);
                    new_button.insert(number);
                }
                buttons.insert(new_button);
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
    std::map<std::tuple<LightsT, LightsT, ButtonsT>, std::optional<int>> m_cached;
};

// To force registration at startup
namespace {
    [[maybe_unused]]
    common::AutoRegister<Day10, 10> day10_registrar;
}

}