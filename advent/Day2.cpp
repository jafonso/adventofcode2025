#include "../common/AutoRegister.hpp"

#include <set>
#include <list>
#include <ranges>
#include <string>
#include <charconv>

namespace advent {

template <unsigned N>
inline constexpr uint64_t pow10_v = [] {
    uint64_t r = 1;
    for (unsigned i = 0; i < N; ++i)
        r *= 10;
    return r;
}();

template <std::size_t... I>
constexpr auto make_pow10_table(std::index_sequence<I...>) {
    return std::array<uint64_t, sizeof...(I)>{ pow10_v<I>... };
}

class Day2 : public common::BaseDay
{
public:
    common::AocResultType run(const std::vector<std::string> & input) override {

        // Parsing input

        std::string_view input_view = input[0];
        std::list<std::pair<std::string, std::string>> input_ranges_str_tmp;
        std::list<std::pair<uint64_t, uint64_t>> input_ranges;

        // Parse the input into `input_ranges_str` and `input_ranges`

        while (!input_view.empty()) {
            auto item_end_pos = input_view.find(',');
            auto pair_str = input_view.substr(0, item_end_pos);

            auto separator_pos = pair_str.find('-');
            uint64_t pair_begin, pair_end;
            std::from_chars(pair_str.begin(), pair_str.begin() + separator_pos, pair_begin);
            std::from_chars(pair_str.begin() + separator_pos + 1, pair_str.end(), pair_end);
            pair_end++; // Range in the form [begin ; end), where end is not included

            input_ranges.emplace_back(pair_begin, pair_end);
            input_ranges_str_tmp.emplace_back(
                std::string(pair_str.begin(), pair_str.begin() + separator_pos),
                std::string(pair_str.begin() + separator_pos + 1, pair_str.end())
            );

            if (item_end_pos == std::string_view::npos) {
                input_view.remove_prefix(input_view.size());
            } else {
                input_view.remove_prefix(item_end_pos + 1);
            }
        }

        // Split into same length segments

        std::list<std::pair<std::string, std::string>> input_ranges_str;

        for (auto & [str_begin, str_end] : input_ranges_str_tmp) {
            for (auto len = str_begin.size(); len <= str_end.size(); len++) {

                bool is_smallest_len = (len == str_begin.size());
                bool is_largest_len = (len == str_end.size());

                if (is_smallest_len && is_largest_len) {
                    input_ranges_str.emplace_back(
                        str_begin,
                        std::to_string(std::stoull(str_end) + 1)
                        );
                } else if (is_smallest_len) {
                    input_ranges_str.emplace_back(
                        str_begin,
                        std::to_string(pow10(len))
                        );
                } else if (is_largest_len) {
                    input_ranges_str.emplace_back(
                        std::to_string(pow10(len-1)),
                        std::to_string(std::stoull(str_end) + 1)
                        );
                } else {
                    input_ranges_str.emplace_back(
                        std::to_string(pow10(len-1)),
                        std::to_string(pow10(len))
                        );
                }
            }
        }

        // Part 1

        uint64_t result_1 = 0;
        std::set<uint64_t> used_values_1;

        for (auto & [str_begin, str_end] : input_ranges_str) {
            result_1 += sumRange(str_begin, str_end, 2, used_values_1);
        }

        // Part 2

        uint64_t result_2 = 0;
        std::set<uint64_t> used_values_2;

        for (auto & [str_begin, str_end] : input_ranges_str) {
            for (auto num_parts = str_begin.size(); num_parts >= 2; num_parts--) {
                result_2 += sumRange(str_begin, str_end, num_parts, used_values_2);
            }
        }

        return std::pair(result_1, result_2);
    }
private:
    static constexpr uint64_t pow10(uint64_t n) {
        std::array<uint64_t, 10> POW_10 = {
            1L,
            10L,
            100L,
            1000L,
            10000L,
            100000L,
            1000000L,
            10000000L,
            100000000L,
            1000000000L
        };
        return POW_10[n];
    }
    static constexpr uint64_t valLen(uint64_t n) {
        uint64_t r = 0;
        while (n) {
            r++;
            n /= 10;
        }
        return r;
    }
    static uint64_t sumRange(uint64_t begin, uint64_t end) {
        auto len = valLen(begin);
        auto tmp = (end - begin) * (begin + end - 1) / 2;
        tmp = tmp * pow10(len) + tmp;
        return tmp;
    }

    static uint64_t sumRange(const std::string & str_begin, const std::string & str_end, uint64_t num_parts, std::set<uint64_t> & used_values) {

        auto len = str_begin.size();
        // Ignore if we cannot split in 'num_parts'
        if (len % num_parts != 0) {
            return 0;
        }

        uint64_t num_begin_prefix, num_end_prefix;
        const auto len_prefix = len / num_parts;

        {
            std::from_chars(str_begin.c_str(), str_begin.c_str() + len_prefix, num_begin_prefix);
            uint64_t tmp_begin = calculateNumber(num_begin_prefix, len_prefix, num_parts);
            if (tmp_begin < std::stoull(str_begin)) {
                num_begin_prefix++;
            }
        }

        {
            if (str_begin.size() == str_end.size()) {
                std::from_chars(str_end.c_str(), str_end.c_str() + len_prefix, num_end_prefix);
                uint64_t tmp_end = calculateNumber(num_end_prefix, len_prefix, num_parts);
                if (tmp_end < std::stoull(str_end)) {
                    num_end_prefix++;
                }
            } else {
                std::from_chars(str_end.c_str(), str_end.c_str() + len_prefix + 1, num_end_prefix);
            }
        }

        uint64_t sum = 0;

        for (uint64_t val_prefix = num_begin_prefix; val_prefix < num_end_prefix; val_prefix++) {
            uint64_t value = calculateNumber(val_prefix, len_prefix, num_parts);
            if (used_values.contains(value)) {
                // Ignore if value has already been counted
                continue;
            }
            used_values.insert(value);
            sum += value;
        }

        return sum;
    }

    static uint64_t calculateNumber(uint64_t prefix, std::size_t len_prefix, std::size_t num_parts) {
        uint64_t value = prefix;
        for (std::size_t i = 1; i < num_parts; i++) {
            value = value * pow10(len_prefix) + prefix;
        }
        return value;
    }
};

// To force registration at startup
namespace {
    [[maybe_unused]]
    common::AutoRegister<Day2, 2> day2_registrar;
}

}