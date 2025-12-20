#pragma once

#include <optional>
#include <utility>
#include <vector>

namespace advent::common {

using AocResultType = std::pair<std::optional<uint64_t>, std::optional<uint64_t>>;

class BaseDay {
public:
    explicit BaseDay(
        const std::vector<std::string> & input,
        const std::optional<uint64_t> & optional_param) : m_input(input), m_optional_param(optional_param) {}
    virtual ~BaseDay() = default;
    virtual AocResultType run() = 0;
    const std::vector<std::string> m_input;
    const std::optional<uint64_t> m_optional_param;
};

}