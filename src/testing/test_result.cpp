#include "test_result.hpp"
#include "output_formatter.hpp"
#include <algorithm>
#include <iostream>

namespace test {

void TestResultManager::add_result(const TestResult& result)
{
    results_.push_back(result);
}

void TestResultManager::print_summary() const
{
    int passed = std::count_if(results_.begin(), results_.end(),
        [](const TestResult& r) { return r.passed; });
    int total = results_.size();

    if (total != 0) {
        std::cout << OutputFormatter::test_summary_header(total);
        std::cout << OutputFormatter::test_passed(passed);

        if (passed < total) {
            std::cout << OutputFormatter::test_failed_summary(total - passed);
            for (const auto& result : results_) {
                if (!result.passed) {
                    std::cout << OutputFormatter::test_failed(result.test_name);
                }
            }
        }
    }
}

bool TestResultManager::has_failures() const noexcept
{
    return std::any_of(results_.begin(), results_.end(),
        [](const TestResult& r) { return !r.passed; });
}

int TestResultManager::get_failed_count() const noexcept
{
    return std::count_if(results_.begin(), results_.end(),
        [](const TestResult& r) { return !r.passed; });
}

} // namespace test