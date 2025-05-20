#include "process_executor.hpp"
#include "output_formatter.hpp"
#include <fstream>
#include <iostream>

#ifdef _WIN32
#include <windows.h>
#else
#include <fcntl.h>
#include <sys/wait.h>
#include <unistd.h>
#endif

namespace test {

ProcessExecutor::ProcessExecutor(std::filesystem::path program_path)
    : program_path_(std::move(program_path))
{
}

std::string ProcessExecutor::execute(std::string_view input)
{
    std::string result;
    std::string input_file = create_temp_input_file(input);
    std::string output_file = (std::filesystem::path(std::filesystem::temp_directory_path()) / "test_output.txt").string();

    try {
#ifdef _WIN32
        SECURITY_ATTRIBUTES saAttr;
        saAttr.nLength = sizeof(SECURITY_ATTRIBUTES);
        saAttr.bInheritHandle = TRUE;
        saAttr.lpSecurityDescriptor = NULL;

        HANDLE hChildStdoutRd, hChildStdoutWr;
        if (!CreatePipe(&hChildStdoutRd, &hChildStdoutWr, &saAttr, 0)) {
            std::cerr << OutputFormatter::error("Failed to create pipe");
            return "";
        }

        if (!SetHandleInformation(hChildStdoutRd, HANDLE_FLAG_INHERIT, 0)) {
            std::cerr << OutputFormatter::error("Failed to set handle information");
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

        std::string cmdLine = "\"" + program_path_.string() + "\"";
        if (!CreateProcess(NULL, const_cast<LPSTR>(cmdLine.c_str()), NULL, NULL, TRUE, 0, NULL, NULL, &siStartInfo, &piProcInfo)) {
            std::cerr << OutputFormatter::error("Failed to create process");
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
            std::cerr << OutputFormatter::warning("Program exited with status " + std::to_string(exitCode));
        }

        CloseHandle(piProcInfo.hProcess);
        CloseHandle(piProcInfo.hThread);
        CloseHandle(hChildStdoutRd);
#else
        std::string cmd = program_path_.string() + " < " + input_file;
        FILE* pipe = popen(cmd.c_str(), "r");
        if (!pipe) {
            std::cerr << OutputFormatter::error("Failed to execute program");
            return "";
        }

        char buffer[4096];
        while (fgets(buffer, sizeof(buffer), pipe) != nullptr) {
            result.append(buffer);
        }

        int status = pclose(pipe);
        if (status == -1) {
            std::cerr << OutputFormatter::error("Failed to close process");
            return "";
        }

        if (WIFEXITED(status)) {
            int exit_status = WEXITSTATUS(status);
            if (exit_status != 0) {
                std::cerr << OutputFormatter::warning("Program exited with status " + std::to_string(exit_status));
            }
        }
#endif

        cleanup_temp_files(input_file, output_file);

    } catch (const std::exception& e) {
        std::cerr << OutputFormatter::error(e.what());
        return "";
    }

    return result;
}

std::string ProcessExecutor::create_temp_input_file(std::string_view input)
{
    std::string input_file = (std::filesystem::path(std::filesystem::temp_directory_path()) / "test_input.txt").string();
    std::ofstream in(input_file, std::ios::binary);
    if (!in) {
        throw std::runtime_error("Failed to create input file");
    }
    in << input;
    if (!in) {
        throw std::runtime_error("Failed to write input file");
    }
    return input_file;
}

void ProcessExecutor::cleanup_temp_files(const std::string& input_file, const std::string& output_file)
{
    try {
        std::filesystem::remove(input_file);
        std::filesystem::remove(output_file);
    } catch (const std::exception& e) {
        std::cerr << OutputFormatter::warning("Failed to clean up temporary files: " + std::string(e.what()));
    }
}

} // namespace test