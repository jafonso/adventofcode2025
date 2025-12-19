#include "../common/AutoRegister.hpp"

#include <queue>

namespace advent {

class Day6 final : public common::BaseDay {
public:
    using BaseDay::BaseDay;
    common::AocResultType run() override {
        return std::pair(std::nullopt, std::nullopt);
    }
};

// To force registration at startup
namespace {
    [[maybe_unused]]
    common::AutoRegister<Day6, 6> day6_registrar;
}

}