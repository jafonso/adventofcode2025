#pragma once

#include <string>
#include <vector>

namespace advent::common {

class DayNotReadyError final : public std::exception {};
class DataFetchError final : public std::exception {};

class DataFetcher {
public:
    explicit DataFetcher(int day);
    virtual ~DataFetcher() = default;
    [[nodiscard]] std::vector<std::string> getData() const;
private:
    static constexpr int YEAR = 2025;
    std::vector<std::string> m_data;
};

}