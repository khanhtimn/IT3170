#include "test_parser.hpp"
#include <algorithm>
#include <fstream>

namespace test {

std::pair<std::string, std::string> TestParser::parse_test_file(const std::filesystem::path& test_file)
{
    std::ifstream file(test_file);
    if (!file.is_open()) {
        return { "", "" };
    }

    std::string content((std::istreambuf_iterator<char>(file)),
        std::istreambuf_iterator<char>());
    file.close();

    size_t input_start = content.find("===INPUT===");
    if (input_start == std::string::npos) {
        return { "", "" };
    }
    
    // Find the next line after ===INPUT===
    input_start = content.find('\n', input_start);
    if (input_start == std::string::npos) return { "", "" };
    input_start += 1;

    size_t output_start = content.find("===OUTPUT===", input_start);
    if (output_start == std::string::npos) {
        return { "", "" };
    }

    std::string input = content.substr(input_start, output_start - input_start);

    // Find the next line after ===OUTPUT===
    output_start = content.find('\n', output_start);
    if (output_start == std::string::npos) return { "", "" };
    output_start += 1;

    size_t end_start = content.find("===END===", output_start);
    if (end_start == std::string::npos) {
        return { "", "" };
    }

    std::string expected = content.substr(output_start, end_start - output_start);

    return { input, expected };
}

void TestParser::normalize_output(std::string& output)
{
    std::string normalized;
    normalized.reserve(output.size());
    for (size_t i = 0; i < output.size(); ++i) {
        if (output[i] == '\r' && i + 1 < output.size() && output[i + 1] == '\n') {
            normalized += '\n';
            ++i;
        } else if (output[i] == '\r') {
            normalized += '\n';
        } else {
            normalized += output[i];
        }
    }
    output = std::move(normalized);

    output.erase(0, output.find_first_not_of(" \n\t"));

    output.erase(output.find_last_not_of(" \n\t") + 1);

    if (!output.empty() && output.back() != '\n') {
        output += '\n';
    }
}

} // namespace test