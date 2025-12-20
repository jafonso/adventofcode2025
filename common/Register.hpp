#pragma once

#include <map>
#include <memory>
#include <vector>

namespace advent::common {

class BaseDay;

class Register {
public:
    using DayFactory = std::unique_ptr<BaseDay>(*)(const std::vector<std::string> & input, const std::optional<uint64_t> & optional_param);
    static Register & getInstance();
    void registerDay(int day, DayFactory dayHandler);
    [[nodiscard]] DayFactory getDayFactory(int day) const;
private:
    Register() = default;
    std::map<int, DayFactory> m_day_factory_map;
};

}