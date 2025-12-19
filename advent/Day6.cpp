#include "../common/AutoRegister.hpp"

#include <queue>
#include <ranges>
#include <functional>
#include <charconv>

namespace advent {

template <class Int>
requires std::is_integral_v<Int>
inline constexpr auto to_int = std::views::transform([](auto&& r) -> Int {
    auto b = std::ranges::begin(r);
    auto e = std::ranges::end(r);
    auto sv = std::string_view{ b, e };
    Int value{};
    auto [ptr, ec] = std::from_chars(sv.data(), sv.data() + sv.size(), value);
    if (ec != std::errc{} || ptr != sv.data() + sv.size()) {
        throw std::runtime_error("bad int (" + std::string(sv.data(), sv.data() + sv.size()) + ")");
    }
    return value;
});

class Day6 final : public common::BaseDay {
public:
    using BaseDay::BaseDay;
    common::AocResultType run() override {
        // ==== Part 1 ====

        // Process input
        std::vector<std::pair<std::function<uint64_t(uint64_t, uint64_t)>, uint64_t>> accumulators_1;

        // Operations
        auto oper_col_view = std::ranges::views::split(m_input[m_input.size() - 1], ' ')
            | std::ranges::views::filter([](auto && r) { return std::ranges::begin(r) != std::ranges::end(r); } );

        for (auto && oper : oper_col_view) {
            if (oper[0] == '*') {
                accumulators_1.emplace_back(std::multiplies<>{}, 1);
            } else if (oper[0] == '+') {
                accumulators_1.emplace_back(std::plus<>{}, 0);
            }
        }

        // Iterate through all lines, except last
        for (auto line = m_input.begin(); (line + 1) != m_input.end(); ++line) {
            auto int_view =
                std::ranges::views::split(*line, ' ')
                | std::ranges::views::filter([](auto && r) { return std::ranges::begin(r) != std::ranges::end(r); })
                | to_int<uint64_t>();
            for (auto && [idx, value] : std::views::zip(std::views::iota(std::size_t{0}), int_view)) {
                accumulators_1[idx].second = accumulators_1[idx].first(value, accumulators_1[idx].second);
            }
        }

        // Get result
        uint64_t result_1 = 0;
        for (auto & val : accumulators_1 | std::views::values) {
            result_1 += val;
        }

        // ==== Part 2 ====

        // Transpose input
        std::size_t i_max = m_input.size();
        std::size_t j_max = m_input[0].size();

        std::vector<std::string> transposed_input(j_max);

        for (std::size_t j = 0; j < j_max; ++j) {
            for (std::size_t i = 0; i < i_max; ++i) {
                transposed_input[j].push_back(m_input[i][j]);
            }
        }

        uint64_t result_2 = 0;
        std::vector<uint64_t> curr_values;
        char curr_op = '*';

        for (auto & line_raw : transposed_input) {
            auto parsed_vals = trimAndGetOp(line_raw);
            if (!parsed_vals.has_value()) {
                if (curr_op == '*') {
                    result_2 += multiply(curr_values);
                } else {
                    result_2 += add(curr_values);
                }
                curr_values.clear();
            } else {
                auto [value, op] = parsed_vals.value();
                if (op.has_value()) {
                    curr_op = op.value();
                }
                curr_values.push_back(value);
            }
        }

        // Last column
        if (curr_op == '*') {
            result_2 += multiply(curr_values);
        } else {
            result_2 += add(curr_values);
        }

        return std::pair(result_1, result_2);
    }
private:
    static uint64_t multiply(const std::vector<uint64_t>& values) {
        uint64_t result = 1;
        for (const auto val : values) {
            result *= val;
        }
        return result;
    }
    static uint64_t add(const std::vector<uint64_t>& values) {
        uint64_t result = 0;
        for (const auto val : values) {
            result += val;
        }
        return result;
    }
    static std::optional<std::pair<uint64_t, std::optional<char>>> trimAndGetOp(std::string_view input) {
        std::optional<char> op;
        if (input.back() == '*' || input.back() == '+') {
            op = input.back();
            input.remove_suffix(1);
        }
        while (!input.empty() && std::isspace(input.back())) {
            input.remove_suffix(1);
        }
        while (!input.empty() && std::isspace(input.front())) {
            input.remove_prefix(1);
        }
        if (input.empty()) {
            return std::nullopt;
        }
        uint64_t return_value;
        std::from_chars(input.begin(), input.end(), return_value);
        return std::make_pair(return_value, op);
    }
};

// To force registration at startup
namespace {
    [[maybe_unused]]
    common::AutoRegister<Day6, 6> day6_registrar;
}

}