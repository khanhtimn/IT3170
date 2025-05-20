#include "output_formatter.hpp"
#include <sstream>

namespace test {

std::string OutputFormatter::test_header(const std::string& test_name)
{
    std::stringstream ss;
    ss << green << "[ RUN      ]" << reset << " " << test_name << "\n";
    return ss.str();
}

std::string OutputFormatter::test_result(const TestResult& result)
{
    std::stringstream ss;
    if (result.passed) {
        ss << green << "[       OK ]" << reset << " " << result.test_name << "\n\n";
    } else {
        ss << red << "[  FAILED  ]" << reset << " " << result.test_name << "\n";
        ss << "Expected: \"" << result.expected << "\"\n";
        ss << "Actual:   \"" << result.actual << "\"\n\n";
    }
    return ss.str();
}

std::string OutputFormatter::test_summary_header(int total)
{
    std::stringstream ss;
    ss << green << "[==========]" << reset << " " << total << " test cases ran.\n";
    return ss.str();
}

std::string OutputFormatter::test_passed(int passed)
{
    std::stringstream ss;
    ss << green << "[  PASSED  ]" << reset << " " << passed << " tests.\n";
    return ss.str();
}

std::string OutputFormatter::test_failed_summary(int failed)
{
    std::stringstream ss;
    ss << red << "[  FAILED  ]" << reset << " " << failed << " tests, listed below:\n";
    return ss.str();
}

std::string OutputFormatter::test_failed(const std::string& test_name)
{
    std::stringstream ss;
    ss << red << "[  FAILED  ]" << reset << " " << test_name << "\n";
    return ss.str();
}

std::string OutputFormatter::warning(const std::string& message)
{
    std::stringstream ss;
    ss << yellow << "Warning: " << message << reset << std::endl;
    return ss.str();
}

std::string OutputFormatter::error(const std::string& message)
{
    std::stringstream ss;
    ss << red << "Error: " << message << reset << std::endl;
    return ss.str();
}

} // namespace test