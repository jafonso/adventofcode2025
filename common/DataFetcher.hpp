#ifndef ADVENTOFCODE2025_DATAFETCHER_HPP
#define ADVENTOFCODE2025_DATAFETCHER_HPP
#include <string>
#include <vector>

namespace advent::common {

class DataFetcher {
public:
    explicit DataFetcher(int day);
    virtual ~DataFetcher() = default;
    std::vector<std::string> getData() const;
private:
    static constexpr int YEAR = 2025;
    std::vector<std::string> m_data;
};

}

#endif //ADVENTOFCODE2025_DATAFETCHER_HPP