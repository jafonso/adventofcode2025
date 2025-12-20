#include <charconv>

#include "../common/AutoRegister.hpp"

#include <map>
#include <set>
#include <queue>

namespace advent {

using Tile = std::pair<uint64_t, uint64_t>;

class Day9 final : public common::BaseDay {
public:
    using BaseDay::BaseDay;
    common::AocResultType run() override {

        std::vector<Tile> all_tiles;
        for (auto & line : m_input) {
            all_tiles.emplace_back(parseTile(line));
        }

        // ==== Part 1 ====

        uint64_t best_rect_area = 0;

        for (auto it_1 = std::ranges::begin(all_tiles); it_1 != std::ranges::end(all_tiles); ++it_1) {
            for (auto it_2 = std::next(it_1); it_2 != std::ranges::end(all_tiles); ++it_2) {
                uint64_t area = rectArea(*it_1, *it_2);
                if (area > best_rect_area) {
                    best_rect_area = area;
                }
            }
        }

        uint64_t result_1 = best_rect_area;

        // ==== Part 2 ====

        return std::pair(result_1, std::nullopt);
    }
private:
    static uint64_t rectArea(const Tile & tile_1, const Tile & tile_2) {
        const uint64_t x = 1 + (tile_1.first > tile_2.first ? tile_1.first - tile_2.first : tile_2.first - tile_1.first);
        const uint64_t y = 1 + (tile_1.second > tile_2.second ? tile_1.second - tile_2.second : tile_2.second - tile_1.second);
        return x * y;
    }
    static Tile parseTile(const std::string & input) {
        auto pos = input.find(',');
        Tile parsed_value;
        std::from_chars(input.data(), input.data() + pos, parsed_value.first);
        std::from_chars(input.data() + pos + 1, input.data() + input.size(), parsed_value.second);
        return parsed_value;
    }
};

// To force registration at startup
namespace {
    [[maybe_unused]]
    common::AutoRegister<Day9, 9> day9_registrar;
}

}