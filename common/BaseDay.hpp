#pragma once

#include <optional>
#include <utility>
#include <vector>

namespace advent::common {

using AocResultType = std::pair<std::optional<uint64_t>, std::optional<uint64_t>>;

class BaseDay {
public:
    virtual ~BaseDay() = default;
    virtual AocResultType run(const std::vector<std::string> & input) = 0;
};

}