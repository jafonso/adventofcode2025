#include <iostream>
#include <ostream>
#include <iomanip>

#include "../common/Register.hpp"
#include "../common/DataFetcher.hpp"
#include "../common/BaseDay.hpp"

constexpr int MAX_DAYS_2025 = 12;

int main(int argc, const char ** argv) {

    // Day of the challenge
    std::optional<int> day_filter;
    if (argc == 2) {
        day_filter = std::stoi(argv[1]);
    }

    bool reached_last_day = false;

    for (int day = 1; day <= MAX_DAYS_2025; ++day) {
        if (day_filter.has_value() && day_filter.value() != day) {
            // Skip in case we want to run a specific day
            continue;
        }

        advent::common::AocResultType result;

        if (!reached_last_day) {

            bool day_not_ready = false;

            try {
                auto data_fetcher = advent::common::DataFetcher(day);
                auto day_factory = advent::common::Register::getInstance().getDayFactory(day);
                result = day_factory(data_fetcher.getData())->run();
            } catch (const advent::common::DayNotReadyError&) {
                day_not_ready = true;
            }

            // This intermediary use of 'failed' allows the static analyser not to complain
            if (day_not_ready) {
                reached_last_day = true;
            }
        }

        std::cout << "Day " << std::setfill('0') << std::setw(2) << day << " : ";
        if (reached_last_day) {
            std::cout << "{ day_not_ready, day_not_ready }" << std::endl;
        } else {
            std::cout << "{ ";
            std::cout << (result.first ? std::to_string(result.first.value()) : "unknown");
            std::cout << " , ";
            std::cout << (result.second ? std::to_string(result.second.value()) : "unknown");
            std::cout << " }" << std::endl;
        }
    }

    return 0;
}