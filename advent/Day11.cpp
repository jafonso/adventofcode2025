#include <csignal>

#include "../common/AutoRegister.hpp"

#include <map>
#include <set>
#include <stack>

namespace advent {

class InvalidPath final : public std::exception {};

class Day11 final : public common::BaseDay {
public:
    using EntryLine = std::pair<std::string, std::set<std::string>>;
    using BaseDay::BaseDay;
    common::AocResultType run() override {

        // Parse input

        if (m_optional_param.has_value()) {
            m_select_part = m_optional_param.value();
        }

        for (auto & line : m_input) {
            m_links.emplace(parseLine(line));
        }

        // ==== Part 1 ====

        std::optional<uint64_t> result_1;
        if (m_select_part.value_or(1) == 1) {
            result_1 = countPaths("you", "out");
        }

        // ==== Part 2 ====

        std::optional<uint64_t> result_2;
        if (m_select_part.value_or(2) == 2) {
            // This is a directed acyclic graph (DAG), therefore either "dac" leads to "fft" or "fft" leads to "dac",
            // but not both at the same time. Otherwise, the graph would contain a cycle.
            // We need to validate this assumption.

            uint64_t fft_to_dac = countPaths("fft", "dac");
            uint64_t dac_to_fft = countPaths("dac", "fft");

            if (fft_to_dac && dac_to_fft) {
                throw std::runtime_error("'fft' and 'dac' form a cyclic graph");
            }

            uint64_t begin = 0;
            uint64_t middle = 0;
            uint64_t end = 0;

            // Either (N + 0) or (0 + N) because one of these values is zero
            if (fft_to_dac > 0) {
                begin = countPaths("svr", "fft");
                middle = fft_to_dac;
                end = countPaths("dac", "out");
            } else if (dac_to_fft > 0) {
                begin = countPaths("svr", "dac");
                middle = dac_to_fft;
                end = countPaths("fft", "out");
            } else {
                throw std::runtime_error("No path between 'fft' and 'dac'");
            }

            result_2 = begin * middle * end;
        }

        return std::pair(result_1, result_2);
    }
private:
    uint64_t countPaths(const std::string& from_device, const std::string& to_device) {
        // Check if we are at the end
        if (from_device == to_device) {
            return 1;
        }
        // Use cached result, if possible
        if (const auto it = m_cache.find(std::make_pair(from_device, to_device)); it != m_cache.end()) {
            return it->second;
        }
        // Else, calculate number of paths
        int result = 0;
        for (auto & next_device : m_links[from_device]) {
            result += countPaths(next_device, to_device);
        }
        m_cache.emplace(std::make_pair(from_device, to_device), result);
        return result;
    }
    static EntryLine parseLine(std::string_view line) {
        const auto sep_idx = line.find(':');
        auto key_view = line.substr(0, sep_idx);
        auto values_view = line.substr(sep_idx + 2);
        std::set<std::string> values;
        while (values_view.size()) {
            auto next_space = values_view.find(' ');
            if (next_space == std::string_view::npos) {
                values.emplace(values_view.data(), values_view.data() + values_view.size());
                values_view.remove_prefix(values_view.size());
            } else {
                values.emplace(values_view.data(), values_view.data() + next_space);
                values_view.remove_prefix(next_space + 1);
            }
        }
        return {std::string(key_view), values};
    }
    std::map<std::string, std::set<std::string>> m_links;
    std::map<std::pair<std::string, std::string>, uint64_t> m_cache;
    std::optional<int> m_select_part;
};

// To force registration at startup
namespace {
    [[maybe_unused]]
    common::AutoRegister<Day11, 11> day11_registrar;
}

}
