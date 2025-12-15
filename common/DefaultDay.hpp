#pragma once

#include "AutoRegister.hpp"
#include "BaseDay.hpp"

namespace advent::common {

class DefaultDay final : public BaseDay {
public:
    AocResultType run(const std::vector<std::string> & input) override;
};

}