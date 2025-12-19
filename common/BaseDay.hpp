#pragma once

#include <optional>
#include <utility>
#include <vector>

namespace advent::common {

using AocResultType = std::pair<std::optional<uint64_t>, std::optional<uint64_t>>;

class BaseDay {
public:
    explicit BaseDay(const std::vector<std::string> & input) : m_input(input) {}
    virtual ~BaseDay() = default;
    virtual AocResultType run() = 0;
    const std::vector<std::string> & m_input;
};

}