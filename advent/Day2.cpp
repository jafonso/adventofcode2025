#include "../common/AutoRegister.hpp"

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

        // Part 1

        uint64_t result_1 = 0;
        std::list<std::pair<uint64_t, uint64_t>> repeated_ranges;

        // Split into valid sub-ranges
        for (auto & [str_begin, str_end] : input_ranges_str_tmp) {

            std::cout << str_begin << "-" << str_end << std::endl;

            uint64_t val_begin_1, val_begin_2;
            if (auto len_begin = str_begin.size(); len_begin % 2 == 0) {
                std::from_chars(str_begin.c_str(), str_begin.c_str() + (len_begin/2), val_begin_1);
                std::from_chars(str_begin.c_str() + (len_begin/2), str_begin.c_str() + len_begin, val_begin_2);
            }

            uint64_t val_end_1, val_end_2;
            if (auto len_end = str_end.size(); len_end % 2 == 0) {
                std::from_chars(str_end.c_str(), str_end.c_str() + (len_end/2), val_end_1);
                std::from_chars(str_end.c_str() + (len_end/2), str_end.c_str() + len_end, val_end_2);
            }

            for (auto len = str_begin.size(); len <= str_end.size(); len++) {
                // Repeated numbers must have even number of digits
                if ((len % 2) == 1) {
                    continue;
                }
                bool is_smallest_len = (len == str_begin.size());
                bool is_largest_len = (len == str_end.size());

                if (is_smallest_len && is_largest_len) {
                    repeated_ranges.emplace_back(
                        val_begin_1 >= val_begin_2 ? val_begin_1 : val_begin_1 + 1,
                        val_end_1 <= val_end_2 ? val_end_1 + 1 : val_end_1
                        );
                } else if (is_smallest_len) {
                    repeated_ranges.emplace_back(
                        val_begin_1 >= val_begin_2 ? val_begin_1 : val_begin_1 + 1,
                        pow10(len/2)
                        );
                } else if (is_largest_len) {
                    repeated_ranges.emplace_back(
                        pow10(len/2 - 1),
                        val_end_1 <= val_end_2 ? val_end_1 + 1 : val_end_1
                        );
                } else {
                    repeated_ranges.emplace_back(
                        pow10(len/2 - 1),
                        pow10(len/2)
                        );
                }

                std::cout << "[" << repeated_ranges.back().first << ", " << repeated_ranges.back().second << ")" << std::endl;

                uint64_t res_tmp = sumRange(repeated_ranges.back().first, repeated_ranges.back().second);
                result_1 += res_tmp;
                std::cout << result_1 << " (+" << res_tmp << ")" << std::endl;
            }

        }

        // Part 1



        //std::num_di

        //for (auto & [a, b] : input_ranges_str_tmp) {
        //    std::cout << a << "-" << b << std::endl;
        //}



        return std::pair(result_1, 0);
    }
private:
    static constexpr uint64_t pow10(uint64_t n) {
        uint64_t r = 1;
        while (n--) r *= 10;
        return r;
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
};

// To force registration at startup
namespace {
    [[maybe_unused]]
    common::AutoRegister<Day2, 2> day2_registrar;
}

}