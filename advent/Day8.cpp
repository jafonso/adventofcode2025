#include <charconv>

#include "../common/AutoRegister.hpp"

#include <set>
#include <map>
#include <ranges>
#include <queue>

namespace advent {

struct Coords {
    uint64_t x;
    uint64_t y;
    uint64_t z;
    auto operator<=>(const Coords&) const = default;
};

inline std::ostream& operator<<(std::ostream& os, const Coords& c) {
    return os << '(' << c.x << ", " << c.y << ", " << c.z << ')';
}

class UnionFind {
public:
    explicit UnionFind(const std::set<Coords> & coords_set) {
        for (auto & coord : coords_set) {
            m_coords_map[coord] = std::make_tuple(coord, 1, 1);
        }
    }
    Coords findRoot(const Coords & coords) {
        auto & entry = m_coords_map.at(coords);
        if (std::get<0>(entry) == coords) {
            return coords;
        }
        std::get<0>(entry) = findRoot(std::get<0>(entry));
        return std::get<0>(entry);
    }
    void unite(const Coords & coords_1, const Coords & coords_2) {
        const auto root_1 = findRoot(coords_1);
        const auto root_2 = findRoot(coords_2);
        const auto root_1_rank = std::get<1>(m_coords_map.at(root_1));
        const auto root_2_rank = std::get<1>(m_coords_map.at(root_2));
        if (root_1 == root_2) {
            return;
        }
        if (root_1_rank > root_2_rank) {
            std::get<0>(m_coords_map.at(root_2)) = root_1;
            std::get<2>(m_coords_map.at(root_1)) += std::get<2>(m_coords_map.at(root_2));
        } else if (root_1_rank < root_2_rank) {
            std::get<0>(m_coords_map.at(root_1)) = root_2;
            std::get<2>(m_coords_map.at(root_2)) += std::get<2>(m_coords_map.at(root_1));
        } else {
            std::get<0>(m_coords_map.at(root_1)) = root_2;
            std::get<1>(m_coords_map.at(root_2))++;
            std::get<2>(m_coords_map.at(root_2)) += std::get<2>(m_coords_map.at(root_1));
        }
    }
    uint64_t processResult1() {
        std::map<Coords, uint64_t> count_circuits;
        for (auto & parent : m_coords_map | std::views::values | std::views::keys) {
            count_circuits[findRoot(parent)]++;
        }
        uint64_t result = 1;
        std::priority_queue<uint64_t> max_pq;
        for (auto values : count_circuits | std::views::values) {
            max_pq.push(values);
        }
        for (std::size_t i = 0; i < 3; i++) {
            result *= max_pq.top();
            //std::cout << result << std::endl;
            max_pq.pop();
        }
        return result;
    }
    bool isUnified() {
        const auto root_tmp = findRoot(m_coords_map.begin()->first);
        return std::get<2>(m_coords_map.at(root_tmp)) >= m_coords_map.size();
    }
private:
    // Key: Coords
    // Value 1: Parent coords
    // Value 2: Ranking
    // Value 3: Number
    std::map<Coords, std::tuple<Coords, uint64_t, uint64_t>> m_coords_map;
};

class Day8 final : public common::BaseDay {
public:
    using BaseDay::BaseDay;
    common::AocResultType run() override {
        // Check if we expect a different number of connections

        if (m_optional_param.has_value()) {
            m_num_connections = m_optional_param.value();
        }

        // Parse all coordinates

        std::set<Coords> coords_set;
        for (auto & line : m_input) {
            auto coords = parseCoords(line);
            coords_set.insert(coords);
        }

        // Get all pairs of coordinates, calculate their distances and store in a table

        std::priority_queue<
            std::tuple<uint64_t, Coords, Coords>,
            std::vector<std::tuple<uint64_t, Coords, Coords>>,
            std::greater<>> connection_queue;
        for (auto it_a = coords_set.begin(); it_a != coords_set.end(); ++it_a) {
            for (auto it_b = std::next(it_a); it_b != coords_set.end(); ++it_b) {
                connection_queue.emplace(calcDist2(*it_a, *it_b), *it_a, *it_b);
            }
        }

        // Do the connections

        UnionFind coords_union{coords_set};
        for (std::size_t i = 0; i < m_num_connections; i++) {
            auto [dist, coords_a, coords_b] = connection_queue.top();
            connection_queue.pop();
            coords_union.unite(coords_a, coords_b);
            //std::cout << coords_a << " " << coords_b << std::endl;
        }

        uint64_t result_1 = coords_union.processResult1();

        // Continue adding edges to get result 2

        Coords last_coords_a, last_coords_b;
        while (true) {
            auto [dist, coords_a, coords_b] = connection_queue.top();
            connection_queue.pop();
            coords_union.unite(coords_a, coords_b);
            if (coords_union.isUnified()) {
                last_coords_a = coords_a;
                last_coords_b = coords_b;
                break;
            }
        }

        uint64_t result_2 = last_coords_a.x * last_coords_b.x;

        return std::pair(result_1, result_2);
    }
    uint64_t m_num_connections = 1000;
private:

    static uint64_t calcDist2(Coords a, Coords b) {
        return (a.x - b.x) * (a.x - b.x) + (a.y - b.y) * (a.y - b.y) + (a.z - b.z) * (a.z - b.z);
    }
    static Coords parseCoords(const std::string & input) {
        std::size_t sep_1 = input.find(',');
        std::size_t sep_2 = input.find(',', sep_1 + 1);
        Coords coords;
        std::from_chars(input.data(), input.data() + sep_1, coords.x);
        std::from_chars(input.data() + sep_1 + 1, input.data() + sep_2, coords.y);
        std::from_chars(input.data() + sep_2 + 1, input.data() + input.size(), coords.z);
        return coords;
    }
};

// To force registration at startup
namespace {
    [[maybe_unused]]
    common::AutoRegister<Day8, 8> day8_registrar;
}

}