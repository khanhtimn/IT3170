#pragma once
#include <string>
#include <vector>

class TestRunner {
public:
    struct TestResult {
        std::string test_name;
        bool passed;
        std::string expected;
        std::string actual;
    };

    TestRunner(const std::string& week, const std::string& program);
    bool run_all_tests();
    void print_summary() const;

private:
    std::string week_dir;
    std::string program_name;
    std::string program_path;
    std::vector<TestResult> results;

    void run_test(const std::string& test_file);
    std::string run_program(const std::string& input);
    std::pair<std::string, std::string> parse_test_file(const std::string& test_file);
};