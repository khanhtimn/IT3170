#include "test_runner.hpp"
#include <algorithm>
#include <filesystem>
#include <fstream>
#include <iostream>

#ifdef _WIN32
#include <windows.h>
#else
#include <fcntl.h>
#include <sys/wait.h>
#include <unistd.h>

#endif

namespace fs = std::filesystem;

namespace colors {
const char* reset = "\033[0m";
const char* green = "\033[32m";
const char* red = "\033[31m";
const char* yellow = "\033[33m";
const char* blue = "\033[34m";
const char* bold = "\033[1m";
}

TestRunner::TestRunner(const std::string& week, const std::string& program)
    : week_dir(week)
    , program_name(program)
{
#ifdef _WIN32
    program_path = "build/" + week_dir + "/" + program_name + ".exe";
#else
    program_path = "build/" + week_dir + "/" + program_name;
#endif
}

int TestRunner::run_all_tests()
{
    std::filesystem::path test_dir = std::filesystem::path("tests") / week_dir / program_name;

    if (!std::filesystem::exists(test_dir)) {
        std::cerr << colors::red << "Error: Test directory not found: " << test_dir << colors::reset << std::endl;
        return 1;
    }

    std::vector<std::filesystem::path> test_files;
    for (const auto& entry : std::filesystem::directory_iterator(test_dir)) {
        if (entry.path().extension() == ".txt") {
            test_files.push_back(entry.path());
        }
    }

    if (test_files.empty()) {
        std::cerr << colors::yellow << "Warning: No test files found in " << test_dir << colors::reset << std::endl;
        return 1;
    }

    std::sort(test_files.begin(), test_files.end());

    std::cout << colors::green << "[==========]" << colors::reset << " Running " << test_files.size() << " test cases." << std::endl
              << std::endl;

    for (const auto& test_file : test_files) {
        run_test(test_file.string());
    }

    return std::count_if(results.begin(), results.end(), [](const TestResult& r) { return !r.passed; });
}

void TestRunner::run_test(const std::string& test_file)
{
    auto [input, expected] = parse_test_file(test_file);
    std::string actual = run_program(input);

    actual.erase(0, actual.find_first_not_of(" \n\r\t"));
    actual.erase(actual.find_last_not_of(" \n\r\t") + 1);
    expected.erase(0, expected.find_first_not_of(" \n\r\t"));
    expected.erase(expected.find_last_not_of(" \n\r\t") + 1);

    TestResult result;
    result.test_name = fs::path(test_file).filename().string();
    result.passed = (actual == expected);
    result.expected = expected;
    result.actual = actual;
    results.push_back(result);

    std::cout << colors::green << "[ RUN      ]" << colors::reset << " " << result.test_name << "\n";

    if (result.passed) {
        std::cout << colors::green << "[       OK ]" << colors::reset << " " << result.test_name << "\n\n";
    } else {
        std::cout << colors::red << "[  FAILED  ]" << colors::reset << " " << result.test_name << "\n";

        std::cout << "Expected: \"" << result.expected << "\"\n";
        std::cout << "Actual:   \"" << result.actual << "\"\n\n";
    }
}

std::string TestRunner::run_program(const std::string& input)
{
    fs::path abs_program_path = fs::absolute(program_path);
    std::string result;

#ifdef _WIN32
    std::string command = "\"" + abs_program_path.string() + "\" < input.txt";

    std::ofstream input_file("input.txt");
    input_file << input;
    input_file.close();

    FILE* pipe = _popen(command.c_str(), "r");
    if (!pipe)
        return "";

    char buffer[128];
    while (fgets(buffer, sizeof(buffer), pipe)) {
        result += buffer;
    }
    _pclose(pipe);
    std::remove("input.txt");
#else
    // Create temporary files for input and output
    std::string input_file = "input.txt";
    std::string output_file = "output.txt";

    // Write input to file
    std::ofstream in(input_file);
    in << input;
    in.close();

    // Create pipes for communication
    int input_fd = open(input_file.c_str(), O_RDONLY);
    int output_fd = open(output_file.c_str(), O_WRONLY | O_CREAT | O_TRUNC, 0644);

    if (input_fd == -1 || output_fd == -1) {
        if (input_fd != -1)
            close(input_fd);
        if (output_fd != -1)
            close(output_fd);
        return "";
    }

    pid_t pid = fork();
    if (pid == -1) {
        close(input_fd);
        close(output_fd);
        return "";
    }

    if (pid == 0) { // Child process
        // Redirect stdin and stdout
        dup2(input_fd, STDIN_FILENO);
        dup2(output_fd, STDOUT_FILENO);

        // Close file descriptors
        close(input_fd);
        close(output_fd);

        // Execute the program
        execl(abs_program_path.c_str(), abs_program_path.c_str(), nullptr);
        exit(1); // If execl fails
    } else { // Parent process
        // Close file descriptors
        close(input_fd);
        close(output_fd);

        // Wait for child to finish
        int status;
        waitpid(pid, &status, 0);

        // Read output
        std::ifstream out(output_file);
        std::string line;
        while (std::getline(out, line)) {
            result += line + "\n";
        }
        out.close();
    }

    // Clean up temporary files
    std::remove(input_file.c_str());
    std::remove(output_file.c_str());
#endif

    return result;
}

std::pair<std::string, std::string> TestRunner::parse_test_file(const std::string& test_file)
{
    std::ifstream file(test_file);
    if (!file.is_open()) {
        return { "", "" };
    }

    std::string line, input, expected;
    bool reading_input = false;
    bool found_input_quote = false;

    while (std::getline(file, line)) {
        if (line.find("INPUT:") == 0) {
            reading_input = true;
            size_t start = line.find('"');
            if (start != std::string::npos) {
                found_input_quote = true;
                input = line.substr(start + 1);
            }
            continue;
        }
        if (line.find("OUTPUT:") == 0) {
            reading_input = false;
            size_t start = line.find('"');
            if (start != std::string::npos) {
                size_t end = line.find('"', start + 1);
                if (end != std::string::npos) {
                    expected = line.substr(start + 1, end - start - 1);
                }
            }
            continue;
        }

        if (reading_input && found_input_quote) {
            size_t end = line.find('"');
            if (end != std::string::npos) {
                input += "\n" + line.substr(0, end);
                found_input_quote = false;
            } else {
                input += "\n" + line;
            }
        }
    }
    file.close();

    return { input, expected };
}

void TestRunner::print_summary() const
{
    int passed = std::count_if(results.begin(), results.end(),
        [](const TestResult& r) { return r.passed; });
    int total = results.size();

    std::cout << colors::green << "[==========]" << colors::reset << " " << total << " test cases ran.\n";
    std::cout << colors::green << "[  PASSED  ]" << colors::reset << " " << passed << " tests.\n";

    if (passed < total) {
        std::cout << colors::red << "[  FAILED  ]" << colors::reset << " " << (total - passed) << " tests, listed below:\n";
        for (const auto& result : results) {
            if (!result.passed) {
                std::cout << colors::red << "[  FAILED  ]" << colors::reset << " " << result.test_name << "\n";
            }
        }
    }
}