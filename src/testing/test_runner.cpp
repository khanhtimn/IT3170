#include "test_runner.hpp"
#include "output_formatter.hpp"
#include "test_parser.hpp"
#include <algorithm>
#include <iostream>

#ifdef _WIN32
#include <windows.h>
#else
#include <fcntl.h>
#include <sys/wait.h>
#include <unistd.h>
#endif

namespace test {

TestRunner::TestRunner(std::string_view week, std::string_view program)
    : week_dir_(week)
    , program_name_(program)
    , program_path_([&]() {
#ifdef _WIN32
        return std::filesystem::path("build") / week_dir_ / (program_name_ + ".exe");
#else
        return std::filesystem::path("build") / week_dir_ / program_name_;
#endif
    }())
    , process_executor_(program_path_)
{
}

std::filesystem::path TestRunner::get_test_directory() const
{
    return std::filesystem::path("tests") / week_dir_ / program_name_;
}

std::vector<std::filesystem::path> TestRunner::get_test_files() const
{
    std::vector<std::filesystem::path> test_files;
    auto test_dir = get_test_directory();

    if (!std::filesystem::exists(test_dir)) {
        std::cerr << OutputFormatter::warning("Test directory not found: " + test_dir.string());
        return test_files;
    }

    for (const auto& entry : std::filesystem::directory_iterator(test_dir)) {
        if (entry.path().extension() == ".txt") {
            test_files.push_back(entry.path());
        }
    }

    std::sort(test_files.begin(), test_files.end());
    return test_files;
}

int TestRunner::run_all_tests()
{
    auto test_dir = get_test_directory();
    if (!std::filesystem::exists(test_dir)) {
        std::cerr << OutputFormatter::warning("Test directory not found: " + test_dir.string());
        return 0;
    }

    auto test_files = get_test_files();
    if (test_files.empty()) {
        std::cerr << OutputFormatter::warning("No test files found in " + test_dir.string());
        return 0;
    }

    for (const auto& test_file : test_files) {
        run_test(test_file);
    }

    return result_manager_.get_failed_count();
}

void TestRunner::run_test(const std::filesystem::path& test_file)
{
    auto [input, expected] = TestParser::parse_test_file(test_file);
    std::string actual = process_executor_.execute(input);

    TestParser::normalize_output(actual);
    TestParser::normalize_output(expected);

    TestResult result;
    result.test_name = test_file.filename().string();
    result.passed = (actual == expected);
    result.expected = expected;
    result.actual = actual;
    result_manager_.add_result(result);

    std::cout << OutputFormatter::test_header(result.test_name);
    std::cout << OutputFormatter::test_result(result);
}

void TestRunner::print_summary() const
{
    result_manager_.print_summary();
}

bool TestRunner::has_failures() const noexcept
{
    return result_manager_.has_failures();
}

} // namespace test