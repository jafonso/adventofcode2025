#include "Register.hpp"
#include "BaseDay.hpp"
#include "DefaultDay.hpp"

#include <sstream>

namespace advent::common {

Register & Register::getInstance() {
    static Register instance;
    return instance;
}

void Register::registerDay(const int day, const DayFactory dayHandler) {
    if (m_dayfactorymap.contains(day)) {
        std::ostringstream oss;
        oss << "Register::registerDay: Duplicate day " << day;
        throw std::runtime_error(oss.str());
    }
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
