#include <sstream>
#include <ranges>
#include <filesystem>
#include <fstream>

#include <curlpp/cURLpp.hpp>
#include <curlpp/Easy.hpp>
#include <curlpp/Options.hpp>

#include "DataFetcher.hpp"

namespace advent::common {

DataFetcher::DataFetcher(int day) {
    // Check if file is cached
    auto cachedfile = std::filesystem::temp_directory_path() / (std::string("advent-2025-") + std::to_string(day));
    if (std::filesystem::exists(cachedfile) && std::filesystem::is_regular_file(cachedfile)) {
        std::ifstream ifstream(cachedfile);
        std::string line;
        while (std::getline(ifstream, line)) {
            m_data.push_back(line);
        }
        return;
    }

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

        if (input_raw.contains("Please don't repeatedly request this endpoint before it unlocks")) {
            throw DayNotReadyError();
        }

        // Parse the result
        for (auto part : input_raw | std::views::split('\n')) {
            m_data.emplace_back(std::string_view(part));
        }

        // Remove last if empty
        while (m_data[m_data.size() - 1].empty()) {
            m_data.pop_back();
        }

    } catch(curlpp::RuntimeError & e) {
        std::cout << e.what() << std::endl;
        throw DataFetchError();
    } catch(curlpp::LogicError & e) {
        std::cout << e.what() << std::endl;
        throw DataFetchError();
    }

    // Cache the data file
    std::ofstream ofstream(cachedfile);
    for (auto & line : m_data) {
        ofstream << line << std::endl;
    }
}

std::vector<std::string> DataFetcher::getData() const {
    return m_data;
}

}