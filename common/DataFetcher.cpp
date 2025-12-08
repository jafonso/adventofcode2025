#include "DataFetcher.hpp"

#include <sstream>
#include <ranges>

#include <curlpp/cURLpp.hpp>
#include <curlpp/Easy.hpp>
#include <curlpp/Options.hpp>

namespace advent::common {

DataFetcher::DataFetcher(int day) {
    // Define the url
    std::ostringstream oss;
    oss << "https://adventofcode.com/" << YEAR << "/day/" << day << "/input";
    auto url = oss.str();

    auto session_c = std::getenv("AOC_SESSION_ID");
    if (session_c == nullptr) {
        throw std::runtime_error("Environment variable AOC_SESSION_ID not set");
    }

    std::string session_cookie = std::string("session=") + session_c;

    std::string input_raw;

    // Custom write callback
    auto writeToString = [&input_raw](char * ptr, size_t size, size_t nmemb) -> size_t {
        size_t total = size * nmemb;
        input_raw.append(ptr, ptr + total);
        return total;
    };

    try
    {
        // Our request to be sent.
        curlpp::Easy myRequest;

        // Set the URL and session cookie.
        myRequest.setOpt<curlpp::options::Url>(url);
        myRequest.setOpt<curlpp::options::CookieSession>(true);
        myRequest.setOpt<curlpp::options::Cookie>(session_cookie);
        myRequest.setOpt<curlpp::options::WriteFunction>(writeToString);

        // Send request and get a result.
        myRequest.perform();

        // Parse the result
        for (auto part : input_raw | std::views::split('\n')) {
            m_data.emplace_back(std::string_view(part));
        }

    } catch(curlpp::RuntimeError & e) {
        std::cout << e.what() << std::endl;
    } catch(curlpp::LogicError & e) {
        std::cout << e.what() << std::endl;
    }

}

std::vector<std::string> DataFetcher::getData() const {
    return m_data;
}

}