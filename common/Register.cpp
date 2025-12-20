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
    if (m_day_factory_map.contains(day)) {
        std::ostringstream oss;
        oss << "Register::registerDay: Duplicate day " << day;
        throw std::runtime_error(oss.str());
    }
    m_day_factory_map[day] = dayHandler;
}

Register::DayFactory Register::getDayFactory(const int day) const {
    try {
        return m_day_factory_map.at(day);
    } catch (std::out_of_range&) {
        return [](const std::vector<std::string> & input, const std::optional<uint64_t> & optional_param) -> std::unique_ptr<BaseDay>
        {
            return std::make_unique<DefaultDay>(input, optional_param);
        };
    }
}

}
