#pragma once
#include <filesystem>
#include <fstream>
#include <sstream>
#include <stdexcept>
#include <string>

namespace TestUtils {

struct TestCase {
    std::string input;
    std::string expected_output;
};

inline TestCase parse_test_file(const std::filesystem::path& file_path) {
    std::ifstream file(file_path);
    if(!file.is_open()) {
        throw std::runtime_error("Could not open test file: " + file_path.string());
    }

    std::string line;
    std::string current_section;
    TestCase result;

    while(std::getline(file, line)) {
        if(line == "===INPUT===") {
            current_section = "INPUT";
        } else if(line == "===OUTPUT===") {
            current_section = "OUTPUT";
        } else if(line == "===END===") {
            break;
        } else {
            if(current_section == "INPUT") {
                result.input += line + "\n";
            } else if(current_section == "OUTPUT") {
                result.expected_output += line + "\n";
            }
        }
    }

    while(!result.expected_output.empty() &&
          (result.expected_output.back() == '\n' || result.expected_output.back() == '\r')) {
        result.expected_output.pop_back();
    }

    return result;
}

inline bool compare_tokens(const std::string& actual, const std::string& expected) {
    std::stringstream as(actual);
    std::stringstream es(expected);
    std::string atok, etok;
    while(as >> atok) {
        if(!(es >> etok))
            return false;
        if(atok != etok)
            return false;
    }
    return !(es >> etok);
}

} // namespace TestUtils
