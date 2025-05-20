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
        std::cerr << colors::yellow << "Warning: Test directory not found: " << test_dir << colors::reset << std::endl;
        std::cerr << colors::yellow << "Skipping tests for " << week_dir << "/" << program_name << colors::reset << std::endl;
        return 0;
    }

    std::vector<std::filesystem::path> test_files;
    for (const auto& entry : std::filesystem::directory_iterator(test_dir)) {
        if (entry.path().extension() == ".txt") {
            test_files.push_back(entry.path());
        }
    }

    if (test_files.empty()) {
        std::cerr << colors::yellow << "Warning: No test files found in " << test_dir << colors::reset << std::endl;
        std::cerr << colors::yellow << "Skipping tests for " << week_dir << "/" << program_name << colors::reset << std::endl;
        return 0;
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
    if (!fs::exists(abs_program_path)) {
        std::cerr << colors::red << "Error: Program not found: " << abs_program_path << colors::reset << std::endl;
        return "";
    }

    std::string result;
    std::string temp_dir = fs::temp_directory_path().string();
    std::string input_file = (fs::path(temp_dir) / "test_input.txt").string();
    std::string output_file = (fs::path(temp_dir) / "test_output.txt").string();

    try {
        {
            std::ofstream in(input_file, std::ios::binary);
            if (!in) {
                std::cerr << colors::red << "Error: Failed to create input file" << colors::reset << std::endl;
                return "";
            }
            in << input;
            if (!in) {
                std::cerr << colors::red << "Error: Failed to write input file" << colors::reset << std::endl;
                return "";
            }
        }

#ifdef _WIN32
        SECURITY_ATTRIBUTES saAttr;
        saAttr.nLength = sizeof(SECURITY_ATTRIBUTES);
        saAttr.bInheritHandle = TRUE;
        saAttr.lpSecurityDescriptor = NULL;

        HANDLE hChildStdoutRd, hChildStdoutWr;
        if (!CreatePipe(&hChildStdoutRd, &hChildStdoutWr, &saAttr, 0)) {
            std::cerr << colors::red << "Error: Failed to create pipe" << colors::reset << std::endl;
            return "";
        }

        if (!SetHandleInformation(hChildStdoutRd, HANDLE_FLAG_INHERIT, 0)) {
            std::cerr << colors::red << "Error: Failed to set handle information" << colors::reset << std::endl;
            CloseHandle(hChildStdoutRd);
            CloseHandle(hChildStdoutWr);
            return "";
        }

        PROCESS_INFORMATION piProcInfo;
        STARTUPINFO siStartInfo;
        ZeroMemory(&piProcInfo, sizeof(PROCESS_INFORMATION));
        ZeroMemory(&siStartInfo, sizeof(STARTUPINFO));
        siStartInfo.cb = sizeof(STARTUPINFO);
        siStartInfo.hStdError = hChildStdoutWr;
        siStartInfo.hStdOutput = hChildStdoutWr;
        siStartInfo.hStdInput = CreateFile(input_file.c_str(), GENERIC_READ, 0, &saAttr, OPEN_EXISTING, FILE_ATTRIBUTE_NORMAL, NULL);
        siStartInfo.dwFlags |= STARTF_USESTDHANDLES;

        std::string cmdLine = "\"" + abs_program_path.string() + "\"";
        if (!CreateProcess(NULL, const_cast<LPSTR>(cmdLine.c_str()), NULL, NULL, TRUE, 0, NULL, NULL, &siStartInfo, &piProcInfo)) {
            std::cerr << colors::red << "Error: Failed to create process" << colors::reset << std::endl;
            CloseHandle(hChildStdoutRd);
            CloseHandle(hChildStdoutWr);
            if (siStartInfo.hStdInput != INVALID_HANDLE_VALUE) {
                CloseHandle(siStartInfo.hStdInput);
            }
            return "";
        }

        CloseHandle(hChildStdoutWr);
        if (siStartInfo.hStdInput != INVALID_HANDLE_VALUE) {
            CloseHandle(siStartInfo.hStdInput);
        }

        DWORD dwRead;
        CHAR chBuf[4096];
        bool bSuccess = FALSE;

        while (true) {
            bSuccess = ReadFile(hChildStdoutRd, chBuf, sizeof(chBuf), &dwRead, NULL);
            if (!bSuccess || dwRead == 0)
                break;
            result.append(chBuf, dwRead);
        }

        WaitForSingleObject(piProcInfo.hProcess, INFINITE);
        DWORD exitCode;
        GetExitCodeProcess(piProcInfo.hProcess, &exitCode);
        if (exitCode != 0) {
            std::cerr << colors::yellow << "Warning: Program exited with status " << exitCode << colors::reset << std::endl;
        }

        CloseHandle(piProcInfo.hProcess);
        CloseHandle(piProcInfo.hThread);
        CloseHandle(hChildStdoutRd);
#else
        int input_fd = open(input_file.c_str(), O_RDONLY);
        if (input_fd == -1) {
            std::cerr << colors::red << "Error: Failed to open input file" << colors::reset << std::endl;
            return "";
        }

        int output_fd = open(output_file.c_str(), O_WRONLY | O_CREAT | O_TRUNC, 0644);
        if (output_fd == -1) {
            close(input_fd);
            std::cerr << colors::red << "Error: Failed to create output file" << colors::reset << std::endl;
            return "";
        }

        pid_t pid = fork();
        if (pid == -1) {
            close(input_fd);
            close(output_fd);
            std::cerr << colors::red << "Error: Failed to create process" << colors::reset << std::endl;
            return "";
        }

        if (pid == 0) {
            if (dup2(input_fd, STDIN_FILENO) == -1 || dup2(output_fd, STDOUT_FILENO) == -1) {
                std::cerr << colors::red << "Error: Failed to redirect I/O" << colors::reset << std::endl;
                exit(1);
            }

            close(input_fd);
            close(output_fd);

            execl(abs_program_path.c_str(), abs_program_path.c_str(), nullptr);
            std::cerr << colors::red << "Error: Failed to execute program" << colors::reset << std::endl;
            exit(1);
        } else {
            close(input_fd);
            close(output_fd);

            int status;
            if (waitpid(pid, &status, 0) == -1) {
                std::cerr << colors::red << "Error: Failed to wait for program" << colors::reset << std::endl;
                return "";
            }

            if (WIFEXITED(status) && WEXITSTATUS(status) != 0) {
                std::cerr << colors::yellow << "Warning: Program exited with status " << WEXITSTATUS(status) << colors::reset << std::endl;
            }
        }

        std::ifstream out(output_file, std::ios::binary);
        if (!out) {
            std::cerr << colors::red << "Error: Failed to read output file" << colors::reset << std::endl;
            return "";
        }

        result.clear();
        std::string line;
        while (std::getline(out, line)) {
            if (!result.empty()) {
                result += "\n";
            }
            result += line;
        }

        while (!result.empty() && (result.back() == ' ' || result.back() == '\t')) {
            result.pop_back();
        }
#endif

    } catch (const std::exception& e) {
        std::cerr << colors::red << "Error: " << e.what() << colors::reset << std::endl;
        return "";
    }

    try {
        fs::remove(input_file);
        fs::remove(output_file);
    } catch (const std::exception& e) {
        std::cerr << colors::yellow << "Warning: Failed to clean up temporary files: " << e.what() << colors::reset << std::endl;
    }

    return result;
}

std::pair<std::string, std::string> TestRunner::parse_test_file(const std::string& test_file)
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

    auto normalize_line_endings = [](std::string& str) {
        std::string result;
        result.reserve(str.length());
        for (size_t i = 0; i < str.length(); i++) {
            if (str[i] == '\r' && i + 1 < str.length() && str[i + 1] == '\n') {
                result += '\n';
                i++;
            } else if (str[i] != '\r') {
                result += str[i];
            }
        }
        str = result;
    };

    normalize_line_endings(input);
    normalize_line_endings(expected);

    auto trim = [](std::string& str) {
        str.erase(0, str.find_first_not_of(" \t\n\r"));
        str.erase(str.find_last_not_of(" \t\n\r") + 1);
    };

    trim(input);
    trim(expected);

    return { input, expected };
}

void TestRunner::print_summary() const
{
    int passed = std::count_if(results.begin(), results.end(),
        [](const TestResult& r) { return r.passed; });
    int total = results.size();

    if (total != 0) {
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
}