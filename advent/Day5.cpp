#include "../common/AutoRegister.hpp"

#include <queue>

namespace advent {


enum class EventType : int {
    ADD = 1,
    CHECK = 2,
    REMOVE = 3,
};

class Day5 final : public common::BaseDay {
public:
    using BaseDay::BaseDay;
    common::AocResultType run() override {

        enum class ParsingStep {
            RANGES,
            INGREDIENTS,
        } parsing_step = ParsingStep::RANGES;

        std::priority_queue<
            std::pair<uint64_t, EventType>,
            std::vector<std::pair<uint64_t, EventType>>,
            std::greater<>> event_queue_1, event_queue_2;

        // ==== Part 1 ====

        // Parse and prepare input

        for (auto & line : m_input) {
            if (line.empty()) {
                parsing_step = ParsingStep::INGREDIENTS;
                continue;
            }
            if (parsing_step == ParsingStep::RANGES) {
                const auto split = line.find("-");
                event_queue_1.push(std::make_pair(std::stoll(line.substr(0, split)), EventType::ADD));
                event_queue_1.push(std::make_pair(std::stoll(line.substr(split + 1)), EventType::REMOVE));
            } else {
                event_queue_1.push(std::make_pair(std::stoll(line), EventType::CHECK));
            }
        }

        // Calculate how many ingredients are fresh

        uint64_t currently_fresh_count_1 = 0;
        uint64_t result_1 = 0;

        while (!event_queue_1.empty()) {
            auto [ number , type ] = event_queue_1.top();
            event_queue_1.pop();

            if (type == EventType::CHECK && currently_fresh_count_1) {
                result_1++;
            } else if (type == EventType::REMOVE) {
                currently_fresh_count_1--;
            } else if (type == EventType::ADD) {
                currently_fresh_count_1++;
            }
        }

        // ==== Part 2 ====

        // Refill input

        for (auto & line : m_input) {
            if (line.empty()) {
                // No need to check the ingredients here
                break;
            }
            const auto split = line.find("-");
            event_queue_2.push(std::make_pair(std::stoll(line.substr(0, split)), EventType::ADD));
            event_queue_2.push(std::make_pair(std::stoll(line.substr(split + 1)), EventType::REMOVE));
        }

        // Calculate how many ingredients IDs are fresh

        uint64_t currently_fresh_count_2 = 0;
        uint64_t first_fresh_id = 0; // In each overlapping set of ranges, keep track of the first fresh ID
        uint64_t result_2 = 0;

        while (!event_queue_2.empty()) {
            auto [ number , type ] = event_queue_2.top();
            event_queue_2.pop();

            if (type == EventType::ADD) {
                if (currently_fresh_count_2 == 0) { // Soon to become fresh
                    first_fresh_id = number;
                }
                currently_fresh_count_2++;
            } else if (type == EventType::REMOVE) {
                if (currently_fresh_count_2 == 1) { // Soon to become not fresh
                    result_2 += (number - first_fresh_id + 1);
                }
                currently_fresh_count_2--;
            }
        }

        return std::pair(result_1, result_2);
    }
};

// To force registration at startup
namespace {
    [[maybe_unused]]
    common::AutoRegister<Day5, 5> day5_registrar;
}

}