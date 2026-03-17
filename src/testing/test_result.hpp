#pragma once

#include <mutex>
#include <string>
#include <vector>

namespace test {

struct TestResult {
    std::string test_name;
    bool passed;
    std::string expected;
    std::string actual;
};

class TestResultManager {
public:
    void add_result(const TestResult& result);
    void print_summary() const;
    [[nodiscard]] bool has_failures() const;
    [[nodiscard]] int get_failed_count() const;

private:
    std::vector<TestResult> results_;
    mutable std::mutex mutex_;
};

} // namespace test