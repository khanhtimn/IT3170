#pragma once

#include <filesystem>
#include <string>

namespace test {

class ProcessExecutor {
public:
    explicit ProcessExecutor(std::filesystem::path program_path);
    std::string execute(std::string_view input);

private:
    std::filesystem::path program_path_;
    std::string create_temp_input_file(std::string_view input);
    void cleanup_temp_files(const std::string& input_file, const std::string& output_file);
};

} // namespace test