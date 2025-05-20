#pragma once

#include "process_executor.hpp"
#include "test_result.hpp"
#include <filesystem>
#include <string>
#include <vector>

namespace test {

class TestRunner {
public:
    explicit TestRunner(std::string_view week, std::string_view program);

    int run_all_tests();
    void print_summary() const;
    [[nodiscard]] bool has_failures() const noexcept;

private:
    void run_test(const std::filesystem::path& test_file);
    [[nodiscard]] std::filesystem::path get_test_directory() const;
    [[nodiscard]] std::vector<std::filesystem::path> get_test_files() const;

    std::string week_dir_;
    std::string program_name_;
    std::filesystem::path program_path_;
    ProcessExecutor process_executor_;
    TestResultManager result_manager_;
};

} // namespace test