#include "../common/AutoRegister.hpp"

#include <bitset>
#include <span>
#include <array>
#include <charconv>
#include <unordered_set>

namespace advent {

using PresentsT = std::array<std::size_t, 6>;

using ShapeT = std::array<std::bitset<3>, 3>;
struct ShapeTHash {
    std::size_t operator()(const ShapeT & a) const noexcept {
        return std::hash<std::bitset<9>>()(a[0].to_ulong() | (a[1].to_ulong() << 3) | (a[2].to_ulong() << 6));
    }
};

struct RegionSizeT {
    std::size_t width;  // x
    std::size_t length; // y
};

/*struct RegionT {
    static constexpr std::size_t MAX_WIDTH = 50;
    RegionSizeT size;
    std::vector<std::bitset<MAX_WIDTH>> region;
    RegionT(const std::size_t width, const std::size_t length) : size(width, length), region(length) {
        if (width > MAX_WIDTH) {
            throw std::invalid_argument("width > MAX_WIDTH");
        }
    }
    ShapeT get3x3At(const std::size_t x, const std::size_t y) const {
        if (x <= 0 || x >= size.width - 1) {
            throw std::invalid_argument("x out of limits [1, width-1)");
        }
        if (y <= 0 || y >= size.length - 1) {
            throw std::invalid_argument("y out of limits [1, length-1)");
        }
        ShapeT output;
        for (auto ix = x; ix < x + 3; ++ix) {
            output[ix] = (region[ix].to_ullong() >> y) & 0b111;
        }
        return output;
    }
};*/

class Day12 final : public common::BaseDay {
public:
    using BaseDay::BaseDay;
    common::AocResultType run() override {

        // Parsing input

        auto [shapes, regions] = parseInput();
        std::vector<std::vector<ShapeT>> shapes_rotated;
        std::vector<std::size_t> shapes_size;
        for (const auto & shape : shapes) {
            shapes_rotated.push_back(allShapeRotations(shape));
            shapes_size.push_back(shapeSize(shape));
        }

        // Try basic approach first, maybe it will work!

        std::size_t successes = 0;
        std::size_t failures = 0;
        std::size_t unknown = 0;

        for (auto & [region, presents_required] : regions) {
            std::size_t region_size = region.length * region.width;
            std::size_t region_occupied_max = 0;
            std::size_t region_occupied_min = 0;
            for (std::size_t idx = 0; idx < presents_required.size(); ++idx) {
                region_occupied_max += 9 * presents_required[idx];
                region_occupied_min += shapes_size[idx] * presents_required[idx];
            }
            if (region_occupied_min > region_size) {
                failures++;
            } else if (region_occupied_max <= region_size) {
                successes++;
            } else {
                unknown++;
            }
        }

        if (unknown > 0) {
            std::cout << "Successes: " << successes << " Failures: " << failures << " Unknown: " << unknown << std::endl;
            throw std::runtime_error("We actually need a more complex solution");
        }

        // If not, we are lucky! This is not complicated at all.

        return std::pair(successes, 0);
    }
private:
    static std::size_t shapeSize(const ShapeT& shape) {
        std::size_t size = 0;
        for (auto row : shape) {
            size += row.count();
        }
        return size;
    }
    static ShapeT flipY(ShapeT shape) {
        std::swap(shape[0], shape[2]);
        return shape;
    }
    static ShapeT flipX(ShapeT shape) {
        std::swap(shape[0][0], shape[0][2]);
        std::swap(shape[1][0], shape[1][2]);
        std::swap(shape[2][0], shape[2][2]);
        return shape;
    }
    static ShapeT rotateShapeClockwise(const ShapeT& shape) {
        ShapeT new_shape;
        new_shape[0][0] = shape[2][0];
        new_shape[0][1] = shape[1][0];
        new_shape[0][2] = shape[0][0];
        new_shape[1][0] = shape[2][1];
        new_shape[1][1] = shape[1][1];
        new_shape[1][2] = shape[0][1];
        new_shape[2][0] = shape[2][2];
        new_shape[2][1] = shape[1][2];
        new_shape[2][2] = shape[0][2];
        return new_shape;
    }
    static std::vector<ShapeT> allShapeRotations(ShapeT shape) {
        std::unordered_set<ShapeT, ShapeTHash> seen_shapes;
        std::vector<ShapeT> output;
        // Just rotation
        for (std::size_t i = 0; i < 4; ++i) {
            if (!seen_shapes.contains(shape)) {
                seen_shapes.insert(shape);
                output.push_back(shape);
            }
            shape = rotateShapeClockwise(shape);
        }
        // Flip x and rotate
        shape = flipX(shape);
        for (std::size_t i = 0; i < 4; ++i) {
            if (!seen_shapes.contains(shape)) {
                seen_shapes.insert(shape);
                output.push_back(shape);
            }
            shape = rotateShapeClockwise(shape);
        }
        shape = flipX(shape);
        // Flip y and rotate
        shape = flipY(shape);
        for (std::size_t i = 0; i < 4; ++i) {
            if (!seen_shapes.contains(shape)) {
                seen_shapes.insert(shape);
                output.push_back(shape);
            }
            shape = rotateShapeClockwise(shape);
        }
        shape = flipY(shape);
        // Flip x/y and rotate
        shape = flipX(shape);
        shape = flipY(shape);
        for (std::size_t i = 0; i < 4; ++i) {
            if (!seen_shapes.contains(shape)) {
                seen_shapes.insert(shape);
                output.push_back(shape);
            }
            shape = rotateShapeClockwise(shape);
        }
        return output;
    }
    static ShapeT parseShape(const std::span<const std::string> shape_input) {
        ShapeT shape;
        std::size_t idx_row = 0;
        for (auto & row : shape_input) {
            std::size_t idx_col = 0;
            for (auto & col : row) {
                if (col == '#') {
                    shape[idx_row][idx_col] = true;
                }
                idx_col++;
            }
            idx_row++;
        }
        return shape;
    }
    static std::pair<RegionSizeT, PresentsT> parseRegion(const std::string & input) {
        std::string_view input_view = input;
        RegionSizeT region;
        {
            auto region_view = input_view.substr(0, input_view.find(':'));
            auto x_idx = region_view.find('x');
            std::from_chars(region_view.data(), region_view.data() + x_idx, region.width);
            std::from_chars(region_view.data() + x_idx + 1, region_view.data() + region_view.size(), region.length);
        }
        PresentsT presents;
        {
            auto presents_view = input_view.substr(input_view.find(':') + 1);
            std::size_t idx = 0;
            while (presents_view.size()) {
                presents_view.remove_prefix(1);
                if (idx >= 6) {
                    throw std::invalid_argument("More than 6 columns");
                }
                auto val_end = presents_view.find(' ');
                if (val_end == std::string_view::npos) {
                    val_end = presents_view.size();
                }
                std::from_chars(presents_view.data(), presents_view.data() + val_end, presents[idx++]);
                presents_view.remove_prefix(val_end);
            }
        }
        return {region, presents};
    }
    std::pair<std::vector<ShapeT>, std::vector<std::pair<RegionSizeT, PresentsT>>> parseInput() const {
        std::vector<ShapeT> present_shapes(6);
        std::vector<std::pair<RegionSizeT, PresentsT>> regions_presents;
        for (std::size_t i = 0; i < m_input.size(); ++i) {
            if (m_input[i].ends_with(':')) {
                std::size_t shape_idx;
                std::from_chars(m_input[i].data(), m_input[i].data() + m_input[i].find(':'), shape_idx);
                if (shape_idx >= 6) {
                    throw std::invalid_argument("More than 6 shapes");
                }
                auto shape = parseShape(std::span(m_input).subspan(i + 1, 3));
                i += 4;
                present_shapes[shape_idx] = std::move(shape);
            } else if (!m_input[i].empty()) {
                regions_presents.push_back(parseRegion(m_input[i]));
            }
        }
        return {present_shapes, regions_presents};
    }
};

// To force registration at startup
namespace {
    [[maybe_unused]]
    common::AutoRegister<Day12, 12> day12_registrar;
}

}
