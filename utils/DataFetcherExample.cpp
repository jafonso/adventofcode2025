#include <sstream>

#include <curlpp/cURLpp.hpp>
#include <curlpp/Easy.hpp>
#include <curlpp/Options.hpp>

using namespace curlpp::options;

int main(int argc, const char ** argv) {

    // Day of the challenge
    if (argc != 2) {
        std::cerr << "Requires argument: <day>" << std::endl;
        exit(1);
    }

    int year = 2025;
    int day = std::stoi(argv[1]);

    // Define the url
    std::ostringstream oss;
    oss << "https://adventofcode.com/" << year << "/day/" << day << "/input";
    auto url = oss.str();

    auto session_c = std::getenv("AOC_SESSION_ID");
    if (session_c == nullptr) {
        std::cerr << "Environment variable AOC_SESSION_ID not set." << std::endl;
    }

    std::string session_cookie = std::string("session=") + session_c;

    try
    {
        // Our request to be sent.
        curlpp::Easy myRequest;

        // Set the URL and session cookie.
        myRequest.setOpt<curlpp::options::Url>(url);
        myRequest.setOpt<curlpp::options::CookieSession>(true);
        myRequest.setOpt<curlpp::options::Cookie>(session_cookie);

        // Send request and get a result.
        myRequest.perform();
    } catch(curlpp::RuntimeError & e) {
        std::cout << e.what() << std::endl;
    } catch(curlpp::LogicError & e) {
        std::cout << e.what() << std::endl;
    }

    return 0;
}