#include <charconv>

#include "../common/AutoRegister.hpp"

#include <map>
#include <set>
#include <queue>

namespace advent {

struct Tile {
    uint64_t x, y;
    auto operator<=>(const Tile&) const = default;
} ;

struct Line {
    Tile t1;
    Tile t2;
    auto operator<=>(const Line&) const = default;
};

class Day9 final : public common::BaseDay {
public:
    using BaseDay::BaseDay;
    common::AocResultType run() override {

        std::vector<Tile> all_tiles;
        for (auto & line : m_input) {
            all_tiles.emplace_back(parseTile(line));
        }

        // This will be used for part 2
        // We will fill it in part 1
        std::priority_queue<std::pair<uint64_t, std::pair<Tile, Tile>>> squares_by_size;

        // ==== Part 1 ====

        uint64_t best_rect_area_1 = 0;

        for (auto it_1 = std::ranges::begin(all_tiles); it_1 != std::ranges::end(all_tiles); ++it_1) {
            for (auto it_2 = std::next(it_1); it_2 != std::ranges::end(all_tiles); ++it_2) {
                uint64_t area = rectArea(*it_1, *it_2);
                squares_by_size.emplace(area, (it_1->x <= it_2->x) ? std::make_pair(*it_1, *it_2) : std::make_pair(*it_2, *it_1)); // This is for Part 2
                if (area > best_rect_area_1) {
                    best_rect_area_1 = area;
                }
            }
        }

        uint64_t result_1 = best_rect_area_1;

        // ==== Part 2 ====

        uint64_t best_rect_area_2 = 0;
        auto [x_lines_set, y_lines_set] = getXYParallelLines(all_tiles);

        bool found = false;
        while (!found && !squares_by_size.empty()) {

            // Calculate limits of the rectangle
            auto [size, tile_pair] = squares_by_size.top();
            squares_by_size.pop();
            auto & [tile_1, tile_2] = tile_pair;

            auto x_min = tile_1.x < tile_2.x ? tile_1.x : tile_2.x;
            auto x_max = tile_1.x > tile_2.x ? tile_1.x : tile_2.x;
            auto y_min = tile_1.y < tile_2.y ? tile_1.y : tile_2.y;
            auto y_max = tile_1.y > tile_2.y ? tile_1.y : tile_2.y;

            // Get iterator for X and Y lines in between the square limits
            {
                auto x_it_begin = x_lines_set.upper_bound(x_min);
                auto x_it_end = x_lines_set.lower_bound(x_max);
                if (x_it_begin != x_it_end) {
                    // Check all X lines within the square limits
                    for (auto it = x_it_begin; it != x_it_end; ++it) {
                        auto & [tile_min, tile_max] = it->second;
                        // When line is fully inside
                        if (tile_min.y >= y_min && tile_max.y <= y_max) goto bypass;
                        // When line is partially inside
                        if (tile_min.y <= y_min && tile_max.y > y_min) goto bypass;
                        if (tile_min.y < y_max && tile_max.y >= y_max) goto bypass;
                    }
                }
            }
            {
                auto y_it_begin = y_lines_set.upper_bound(y_min);
                auto y_it_end = y_lines_set.lower_bound(y_max);
                if (y_it_begin != y_it_end) {
                    // Check all Y lines within the square limits
                    for (auto it = y_it_begin; it != y_it_end; ++it) {
                        auto & [tile_min, tile_max] = it->second;
                        // When line is fully inside
                        if (tile_min.x >= x_min && tile_max.x <= x_max) goto bypass;
                        // When line is partially inside
                        if (tile_min.x <= x_min && tile_max.x > x_min) goto bypass;
                        if (tile_min.x < x_max && tile_max.x >= x_max) goto bypass;
                    }
                }
            }
            // If no lines exist inside the rectangle, we just need to confirm that the rectangle is not outside the limits
            // If it's inside, any line starting from its centre will cross an odd number of red tiles, not an even number.
            {
                uint64_t lines_crossed = 0;
                uint64_t y_ref = (y_max + y_min) / 2;
                uint64_t x_ref = (x_max + x_min) / 2;
                auto x_ref_begin = x_lines_set.lower_bound(x_ref);
                for (auto it = x_ref_begin; it != x_lines_set.end(); ++it) {
                    auto & [tile_min, tile_max] = it->second;
                    if (tile_min.y <= y_ref && tile_max.y > y_ref) {
                        lines_crossed++;
                    }
                }
                if ((lines_crossed % 2) == 0) goto bypass;
            }
            // Finally we are at the end. The rectangle is inside.
            best_rect_area_2 = size;
            found = true;
            // Go back to loop
            bypass:
        }

        uint64_t result_2 = best_rect_area_2;

        return std::pair(result_1, result_2);
    }
private:
    static std::pair<std::multimap<uint64_t, Line>, std::multimap<uint64_t, Line>> getXYParallelLines(const std::vector<Tile> & tiles) {
        Tile old_tile = tiles.back();
        std::multimap<uint64_t, Line> output_x;
        std::multimap<uint64_t, Line> output_y;
        for (auto & new_tile : tiles) {
            if (old_tile.x == new_tile.x) {
                if (old_tile.y < new_tile.y) {
                    output_x.emplace(old_tile.x, Line{old_tile, new_tile});
                } else {
                    output_x.emplace(old_tile.x, Line{new_tile, old_tile});
                }
            } else if (old_tile.y == new_tile.y) {
                if (old_tile.x < new_tile.x) {
                    output_y.emplace(old_tile.y, Line{old_tile, new_tile});
                } else {
                    output_y.emplace(old_tile.y, Line{new_tile, old_tile});
                }
            } else {
                throw std::runtime_error("Tile pair is not aligned along X or Y");
            }
            old_tile = new_tile;
        }
        return {output_x, output_y};
    }
    static uint64_t rectArea(const Tile & tile_1, const Tile & tile_2) {
        const uint64_t x = 1 + (tile_1.x > tile_2.x ? tile_1.x - tile_2.x : tile_2.x - tile_1.x);
        const uint64_t y = 1 + (tile_1.y > tile_2.y ? tile_1.y - tile_2.y : tile_2.y - tile_1.y);
        return x * y;
    }
    static Tile parseTile(const std::string & input) {
        auto pos = input.find(',');
        Tile parsed_value;
        std::from_chars(input.data(), input.data() + pos, parsed_value.x);
        std::from_chars(input.data() + pos + 1, input.data() + input.size(), parsed_value.y);
        return parsed_value;
    }
};

// To force registration at startup
namespace {
    [[maybe_unused]]
    common::AutoRegister<Day9, 9> day9_registrar;
}

}