#include "process_executor.hpp"
#include "output_formatter.hpp"
#include <atomic>
#include <chrono>
#include <fstream>
#include <functional>
#include <iomanip>
#include <iostream>
#include <random>
#include <sstream>
#include <thread>

#ifdef _WIN32
#include <windows.h>
#else
#include <csignal>
#include <fcntl.h>
#include <sys/wait.h>
#include <unistd.h>
#endif

namespace test {

namespace {
    std::atomic<uint64_t> file_counter{ 0 };

    std::string create_temp_file_name(const std::string& prefix)
    {
        std::stringstream ss;
        ss << prefix << "_" 
           << std::this_thread::get_id() << "_"
           << ++file_counter << ".txt";
        return (std::filesystem::temp_directory_path() / ss.str()).string();
    }
}

ProcessExecutor::ProcessExecutor(std::filesystem::path program_path)
    : program_path_(std::move(program_path))
{
}

std::string ProcessExecutor::execute(std::string_view input)
{
    std::string result;
    std::string input_file = create_temp_input_file(input);
    std::string output_file = create_temp_file_name("test_output");

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

        DWORD waitResult = WaitForSingleObject(piProcInfo.hProcess, 2000);
        if (waitResult == WAIT_TIMEOUT) {
            std::cerr << OutputFormatter::warning("Program timed out after 2000ms and was killed");
            TerminateProcess(piProcInfo.hProcess, 1);
            WaitForSingleObject(piProcInfo.hProcess, INFINITE); // Wait for termination
            return "";
        }
        
        DWORD exitCode;
        GetExitCodeProcess(piProcInfo.hProcess, &exitCode);
        if (exitCode != 0) {
            std::cerr << OutputFormatter::warning("Program exited with status " + std::to_string(exitCode));
        }

        CloseHandle(piProcInfo.hProcess);
        CloseHandle(piProcInfo.hThread);
        CloseHandle(hChildStdoutRd);
#else
        std::string cmd = program_path_.string() + " < " + input_file + " > " + output_file;
        
        pid_t pid = fork();
        if (pid == -1) {
            std::cerr << OutputFormatter::error("Failed to fork process");
            return "";
        } else if (pid == 0) {
            // Child process
            execl("/bin/sh", "sh", "-c", cmd.c_str(), nullptr);
            _exit(127); // Should not reach here
        } else {
            // Parent process
            auto start_time = std::chrono::steady_clock::now();
            int status;
            bool timed_out = false;
            
            while (true) {
                pid_t result_pid = waitpid(pid, &status, WNOHANG);
                if (result_pid == pid) {
                    break; // Process finished
                } else if (result_pid == -1) {
                    std::cerr << OutputFormatter::error("waitpid failed");
                    return "";
                }
                
                auto now = std::chrono::steady_clock::now();
                if (std::chrono::duration_cast<std::chrono::milliseconds>(now - start_time).count() > 2000) {
                    timed_out = true;
                    kill(pid, SIGKILL);
                    waitpid(pid, &status, 0); // Clean up zombie
                    break;
                }
                
                std::this_thread::sleep_for(std::chrono::milliseconds(10));
            }
            
            if (timed_out) {
                std::cerr << OutputFormatter::warning("Program timed out after 2000ms and was killed");
                return "";
            } else if (WIFEXITED(status)) {
                int exit_status = WEXITSTATUS(status);
                if (exit_status != 0) {
                    std::cerr << OutputFormatter::warning("Program exited with status " + std::to_string(exit_status));
                }
            } else if (WIFSIGNALED(status)) {
                std::cerr << OutputFormatter::warning("Program was terminated by signal " + std::to_string(WTERMSIG(status)));
            }
            
            // Read output file
            std::ifstream out(output_file, std::ios::binary);
            if (out) {
                std::stringstream buffer;
                buffer << out.rdbuf();
                result = buffer.str();
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
    std::string input_file = create_temp_file_name("test_input");
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