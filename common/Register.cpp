#include "Register.hpp"
#include "BaseDay.hpp"
#include "DefaultDay.hpp"

namespace advent::common {

Register & Register::getInstance() {
    static Register instance;
    return instance;
}

void Register::registerDay(const int day, const DayFactory dayHandler) {
    m_dayfactorymap[day] = dayHandler;
}

std::unique_ptr<BaseDay> Register::getDay(const int day) const {
    try {
        return m_dayfactorymap.at(day)();
    } catch (std::out_of_range&) {
        return std::make_unique<DefaultDay>();
    }
}

}
