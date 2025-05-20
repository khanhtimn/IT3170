#pragma once

#include "test_result.hpp"
#include <string>

namespace test {

class OutputFormatter {
public:
    static std::string test_header(const std::string& test_name);
    static std::string test_result(const TestResult& result);
    static std::string test_summary_header(int total);
    static std::string test_passed(int passed);
    static std::string test_failed_summary(int failed);
    static std::string test_failed(const std::string& test_name);
    static std::string warning(const std::string& message);
    static std::string error(const std::string& message);

private:
    static constexpr const char* reset = "\033[0m";
    static constexpr const char* green = "\033[32m";
    static constexpr const char* red = "\033[31m";
    static constexpr const char* yellow = "\033[33m";
};

} // namespace test