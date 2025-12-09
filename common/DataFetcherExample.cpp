#include <sstream>

#include <curlpp/cURLpp.hpp>
#include <curlpp/Options.hpp>

#include "DataFetcher.hpp"

using namespace curlpp::options;

int main(int argc, const char ** argv) {

    // Day of the challenge
    if (argc != 2) {
        std::cerr << "Requires argument: <day>" << std::endl;
        exit(1);
    }

    int day = std::stoi(argv[1]);
    auto data_fetcher = advent::common::DataFetcher(day);
    auto input_data = data_fetcher.getData();

    std::cout << "Number of lines: " << std::endl << input_data.size() << std::endl;
    std::cout << "Input: " << std::endl;
    for (auto & line : input_data) {
        std::cout << line << std::endl;
    }

    return 0;
}