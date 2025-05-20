#pragma once

#include <filesystem>
#include <string>
#include <utility>

namespace test {

class TestParser {
public:
    static std::pair<std::string, std::string> parse_test_file(const std::filesystem::path& test_file);
    static void normalize_output(std::string& output);
};

} // namespace test