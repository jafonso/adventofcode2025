#pragma once

#include <optional>
#include <utility>
#include <vector>

namespace advent::common {

using AocResultType = std::pair<std::optional<int>, std::optional<int>>;

class BaseDay {
public:
    virtual ~BaseDay() = default;
    virtual AocResultType run(std::vector<std::string> input) = 0;
};

}