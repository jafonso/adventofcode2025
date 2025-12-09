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
            []() -> std::unique_ptr<BaseDay> { return std::make_unique<T>();});
    }
};

}
