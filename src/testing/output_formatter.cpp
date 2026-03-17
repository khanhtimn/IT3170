#include "output_formatter.hpp"
#include <sstream>

namespace test {

std::string OutputFormatter::test_header(const std::string& test_name)
{
    std::stringstream ss;
    ss << green << "[ RUN      ]" << reset << " " << test_name << "\n";
    return ss.str();
}

#include <algorithm>
#include <string_view>
#include <utility>

namespace {
    std::string get_mismatch_context(std::string_view expected, std::string_view actual) {
        auto [exp_it, act_it] = std::mismatch(expected.begin(), expected.end(), actual.begin(), actual.end());
        
        size_t mismatch_pos = std::distance(expected.begin(), exp_it);
        
        // Find line start
        size_t line_start = expected.find_last_of('\n', mismatch_pos);
        line_start = (line_start == std::string::npos) ? 0 : line_start + 1;
        
        // Find line end
        size_t exp_line_end = expected.find('\n', mismatch_pos);
        size_t act_line_end = actual.find('\n', std::distance(actual.begin(), act_it));
        
        std::string exp_line = std::string(expected.substr(line_start, exp_line_end - line_start));
        size_t act_line_start = actual.find_last_of('\n', std::distance(actual.begin(), act_it));
        act_line_start = (act_line_start == std::string::npos) ? 0 : act_line_start + 1;
        std::string act_line = std::string(actual.substr(act_line_start, act_line_end - act_line_start));

        size_t col = mismatch_pos - line_start;
        
        std::stringstream ss;
        ss << "Mismatch at line " << std::count(expected.begin(), expected.begin() + mismatch_pos, '\n') + 1 
           << ", column " << col + 1 << ":\n";
        ss << "  Expected: " << exp_line << "\n";
        ss << "  Actual:   " << act_line << "\n";
        ss << "            " << std::string(col, ' ') << "^\n";
        return ss.str();
    }
}

std::string OutputFormatter::test_result(const TestResult& result)
{
    std::stringstream ss;
    if (result.passed) {
        ss << green << "[       OK ]" << reset << " " << result.test_name << "\n\n";
    } else {
        ss << red << "[  FAILED  ]" << reset << " " << result.test_name << "\n";
        ss << get_mismatch_context(result.expected, result.actual) << "\n";
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