#pragma once

#include <memory>

#include "BaseDay.hpp"
#include "Register.hpp"

namespace advent::common {

template<typename T>
concept DayType = std::derived_from<T, BaseDay> && std::default_initializable<T>;

template<DayType T, int DAY>
class AutoRegister {
    struct RegisterNode {
        RegisterNode() {
            Register::getInstance().registerDay(
                DAY,
                []() -> std::unique_ptr<BaseDay>
                {
                    return std::make_unique<T>();
                });
        }
    };
    inline static RegisterNode registrar;
};

}
