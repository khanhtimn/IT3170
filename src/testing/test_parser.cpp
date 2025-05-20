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

    size_t input_start = content.find("INPUT:");
    if (input_start == std::string::npos) {
        return { "", "" };
    }

    size_t output_start = content.find("OUTPUT:", input_start);
    if (output_start == std::string::npos) {
        return { "", "" };
    }

    size_t input_quote_start = content.find('"', input_start);
    if (input_quote_start == std::string::npos) {
        return { "", "" };
    }
    size_t input_quote_end = content.find('"', input_quote_start + 1);
    if (input_quote_end == std::string::npos) {
        return { "", "" };
    }
    std::string input = content.substr(input_quote_start + 1, input_quote_end - input_quote_start - 1);

    size_t output_quote_start = content.find('"', output_start);
    if (output_quote_start == std::string::npos) {
        return { "", "" };
    }
    size_t output_quote_end = content.find('"', output_quote_start + 1);
    if (output_quote_end == std::string::npos) {
        return { "", "" };
    }
    std::string expected = content.substr(output_quote_start + 1, output_quote_end - output_quote_start - 1);

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