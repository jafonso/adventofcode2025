#include <charconv>

#include "../common/AutoRegister.hpp"

#include <regex>
#include <map>
#include <vector>
#include <ranges>
#include <bitset>
#include <queue>

namespace advent {

class Day10 final : public common::BaseDay {
public:
    static constexpr uint64_t LARGE_VALUE = std::numeric_limits<int32_t>::max(); // Value large enough, but far from overflow of 64 bits
    static constexpr int BITSET_SIZE = 16;
    using LightsT = std::bitset<BITSET_SIZE>;
    using ButtonsT = std::vector<LightsT>;
    using JoltagesT = std::vector<int64_t>;
    using ParsedInput = std::tuple<LightsT, ButtonsT, JoltagesT>;
    using BitsetMap = std::map<uint64_t, std::map<JoltagesT, int64_t>>;
    using BaseDay::BaseDay;
    common::AocResultType run() override {

        // Get input

        std::vector<ParsedInput> input;
        std::vector<BitsetMap> bitset_maps;

        for (auto line : m_input) {
            auto parsed_input = parseInput(line);
            bitset_maps.push_back(calculateBitsetMap(std::get<2>(parsed_input).size(), std::get<1>(parsed_input)));
            input.push_back(std::move(parsed_input));
        }

        // ==== Part 1 ====

        uint64_t result_1 = 0;
        for (std::size_t i = 0; i < input.size(); i++) {
            result_1 += findFewestPresses(bitset_maps[i], std::get<0>(input[i]));
        }

        // ==== Part 2 ====

        uint64_t result_2 = 0;
        for (std::size_t i = 0; i < input.size(); i++) {
            std::map<JoltagesT, int> cache;
            result_2 += findFewestPressesComplex(bitset_maps[i], std::get<2>(input[i]), cache);
        }

        return std::pair(result_1, result_2);
    }
private:
    static BitsetMap calculateBitsetMap(const std::size_t size, const ButtonsT & buttons) {
        BitsetMap entries;
        std::size_t max_entries = 1 << buttons.size();
        for (std::size_t entry = 0; entry < max_entries; ++entry) {
            int count = 0;
            LightsT lights;
            JoltagesT joltages(size, 0);
            for (std::size_t i = 0; i < buttons.size(); ++i) {
                bool use = entry & (1 << i);
                if (use) {
                    count++;
                    lights ^= buttons[i];
                    for (std::size_t b = 0; b < size; ++b) {
                        if (buttons[i][b]) {
                            joltages[b]++;
                        }
                    }
                }
            }
            auto & j_to_value_map = entries[lights.to_ullong()];
            if (!j_to_value_map.contains(joltages) || j_to_value_map.at(joltages) > count) {
                j_to_value_map[std::move(joltages)] = count;
            }
        }
        return entries;
    }
    static bool joltageIsZero(const JoltagesT & joltages) {
        for (auto value : joltages) {
            if (value != 0) {
                return false;
            }
        }
        return true;
    }
    static int64_t findFewestPressesComplex(const BitsetMap & bitset_map, JoltagesT joltages, std::map<JoltagesT, int> & cache) {

        // Check if cached
        if (auto it = cache.find(joltages); it != cache.end()) {
            return it->second;
        }
        // Nothing to do
        if (joltageIsZero(joltages)) {
            cache[joltages] = 0;
            return 0;
        }
        // Otherwise, calculate all possibilities
        LightsT partial_lights;
        std::size_t idx = 0;
        for (const auto & value : joltages) {
            if (value % 2 == 1) {
                partial_lights[idx] = true;
            }
            idx++;
        }
        auto j_to_value_it = bitset_map.find(partial_lights.to_ullong());
        if (j_to_value_it == bitset_map.end()) {
            return LARGE_VALUE;
        }
        auto & j_to_value_map = j_to_value_it->second;
        int64_t result = LARGE_VALUE;

        for (auto & [jol, count] : j_to_value_map) {
            JoltagesT new_joltages(joltages.size(), 0);
            for (std::size_t i = 0; i < joltages.size(); ++i) {
                new_joltages[i] = joltages[i] - jol[i];
                if (new_joltages[i] < 0) {
                    goto end_jolt_loop; // Do not bother if any value is negative
                }
                new_joltages[i] /= 2;
            }
            if (const int64_t sub_result = count + (2 * findFewestPressesComplex(bitset_map, new_joltages, cache)); sub_result < result) {
                result = sub_result;
            }
            end_jolt_loop:
        }

        cache[joltages] = result;
        return result;
    }
    static int64_t findFewestPresses(const BitsetMap & bitset_map, LightsT lights) {

        int64_t result = LARGE_VALUE;
        for (const auto & count : bitset_map.at(lights.to_ullong()) | std::views::values) {
            if (result > count) {
                result = count;
            }
        }

        return result;
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