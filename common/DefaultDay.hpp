#pragma once

#include "AutoRegister.hpp"
#include "BaseDay.hpp"

namespace advent::common {

class DefaultDay final : public BaseDay {
public:
    using BaseDay::BaseDay;
    AocResultType run() override;
};

}