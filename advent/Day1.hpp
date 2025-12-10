#pragma once

#include "../common/BaseDay.hpp"

namespace advent {

class Day1 : public common::BaseDay {
public:
    common::AocResultType run(std::vector<std::string> input) override;
};

}