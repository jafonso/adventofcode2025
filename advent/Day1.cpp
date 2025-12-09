#include "../common/AutoRegister.hpp"

namespace advent {

class Day1 : public common::BaseDay {
public:
    common::AocResultType run(std::vector<std::string> input) override {
        return std::pair(1, 1);
    }
};

// To force registration at startup
namespace {
    [[maybe_unused]]
    common::AutoRegister<Day1, 1> day1_registrar;
}

}