#pragma once

#include <iostream>
#include <memory>
#include <ostream>

#include "BaseDay.hpp"
#include "Register.hpp"

namespace advent::common {

template<typename T, int DAY>
class AutoRegister {
public:
    AutoRegister() {
        Register::getInstance().registerDay(
            DAY,
            [](const std::vector<std::string> & input, const std::optional<uint64_t> & optional_param) -> std::unique_ptr<BaseDay>
            {
                return std::make_unique<T>(input, optional_param);
            });
    }
};

}
