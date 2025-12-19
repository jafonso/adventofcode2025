#include <algorithm>

#include "../common/AutoRegister.hpp"

#include <ranges>
#include <set>

namespace advent {

class Day7 final : public common::BaseDay {
public:
    using BaseDay::BaseDay;
    common::AocResultType run() override {

        // ==== Part 1 ====

        uint64_t result_1 = 0;
        {
            std::set<uint64_t> current_beams;
            bool is_first = true;

            for (auto & line : m_input) {
                if (is_first) {
                    current_beams.insert(line.find('S'));
                    is_first = false;
                } else {
                    std::size_t pos = line.find('^');
                    std::set<uint64_t> beams_spiters;
                    while (pos != std::string::npos) {
                        beams_spiters.insert(pos);
                        pos = line.find('^', pos + 1);
                    }
                    std::set<uint64_t> actual_beam_splits;
                    std::ranges::set_intersection(current_beams, beams_spiters, std::inserter(actual_beam_splits, actual_beam_splits.end()));
                    result_1 += actual_beam_splits.size();
                    for (auto beam : actual_beam_splits) {
                        current_beams.erase(beam);
                        current_beams.insert(beam - 1);
                        current_beams.insert(beam + 1);
                    }
                }
            }
        }

        // ==== Part 2 ====

        uint64_t result_2 = 0;
        {
            std::map<uint64_t, uint64_t> current_beams_counts;
            bool is_first = true;

            for (auto & line : m_input) {
                if (is_first) {
                    current_beams_counts[line.find('S')]++;
                    is_first = false;
                } else {
                    std::size_t pos = line.find('^');
                    std::set<uint64_t> beams_spiters;
                    while (pos != std::string::npos) {
                        beams_spiters.insert(pos);
                        pos = line.find('^', pos + 1);
                    }
                    std::set<uint64_t> actual_beam_splits;
                    for (auto current_beam : current_beams_counts | std::views::keys) {
                        if (beams_spiters.contains(current_beam)) {
                            actual_beam_splits.insert(current_beam);
                        }
                    }
                    for (auto beam : actual_beam_splits) {
                        const auto beam_count = current_beams_counts[beam];
                        current_beams_counts.erase(beam);
                        current_beams_counts[beam - 1] += beam_count;
                        current_beams_counts[beam + 1] += beam_count;
                    }
                }
            }

            for (auto beam_count : current_beams_counts | std::views::values) {
                result_2 += beam_count;
            }
        }

        return std::pair(result_1, result_2);
    }
};

// To force registration at startup
namespace {
    [[maybe_unused]]
    common::AutoRegister<Day7, 7> day7_registrar;
}

}