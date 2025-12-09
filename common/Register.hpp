#pragma once

#include <map>
#include <memory>

namespace advent::common {

class BaseDay;

class Register {
public:
    using DayFactory = std::unique_ptr<BaseDay>(*)();
    static Register & getInstance();
    void registerDay(int day, DayFactory dayHandler);
    [[nodiscard]] std::unique_ptr<BaseDay> getDay(int day) const;
private:
    Register() = default;
    std::map<int, DayFactory> m_dayfactorymap;
};

}