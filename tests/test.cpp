#include "test_runner.hpp"
#include <iostream>

int main(int argc, char* argv[])
{
    if (argc != 3) {
        std::cerr << "Usage: " << argv[0] << " <week> <program>" << std::endl;
        return 1;
    }

    std::string week = argv[1];
    std::string program = argv[2];

    TestRunner runner(week, program);
    bool tests_run = runner.run_all_tests();

    if (tests_run) {
        runner.print_summary();
    }

    return 0;
}
