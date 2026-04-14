/*
The first block of data consists of lines (terminated by a line containing #),
each line (number of lines can be up to 100000) is under the form:
        call <from_number> <to_number> <date> <from_time> <end_time>
which is a call from the phone number <from_number> to a phone number <to_number> on <date>,
and starting at time-point <from_time>, terminating at time-point <end_time>
<from_number> and <to_number> are string of 10 characters (a phone number is correct if it contains only digits 0,1,...,9, otherwise, the phone number is incorrect)
<date> is under the form YYYY-MM-DD (for example 2022-10-21)
<from_time> and <to_time> are under the form hh:mm:ss (for example, 10:07:23)

The second block consists of queries (terminated by a line containing #),
each query in a line (number of lines can be up to 100000) and belongs to one of the following types:
?check_phone_number: print to stdout (in a new line) value 1 if no phone number is incorrect
?number_calls_from <phone_number>: print to stdout (in a new line) the number of times a call is made from <phone_number>
?number_total_calls: print to stdout (in a new line) the total number of calls of the data
?count_time_calls_from <phone_number>: print to stdout (in a new line) the total time duration (in seconds) the calls are made from <phone_number>

Example
Input
call 0912345678 0132465789 2022-07-12 10:30:23 10:32:00
call 0912345678 0945324545 2022-07-13 11:30:10 11:35:11
call 0132465789 0945324545 2022-07-13 11:30:23 11:32:23
call 0945324545 0912345678 2022-07-13 07:30:23 07:48:30
#
?check_phone_number
?number_calls_from 0912345678
?number_total_calls
?count_time_calls_from 0912345678
?count_time_calls_from 0132465789
#

Output
1
2
4
398
120
*/

#include <iomanip>
#include <iostream>
#include <sstream>
#include <string>
#include <unordered_map>
#include <vector>

struct Call {
    std::string from_number;
    std::string to_number;
    std::string date;
    std::string from_time;
    std::string end_time;
    int duration_seconds;
};

class Database {

public:
    Database() {
        std::vector<Call> calls;
        std::string line;

        while(getline(std::cin, line) && line != "#") {
            if(line.empty())
                continue;
            calls.push_back(parse_call(line));
        }
        process(calls);
    }

private:
    bool check_phone(const std::string& phone_number) {
        if(phone_number.length() != 10) {
            return false;
        }
        return phone_number.find_first_not_of("0123456789") == std::string::npos;
    }

    int count_time(const std::string& from_time, const std::string& end_time) {
        std::tm tm1 = { }, tm2 = { };
        std::istringstream ss1(from_time);
        std::istringstream ss2(end_time);

        ss1 >> std::get_time(&tm1, "%H:%M:%S");
        ss2 >> std::get_time(&tm2, "%H:%M:%S");

        int seconds1 = tm1.tm_hour * 3600 + tm1.tm_min * 60 + tm1.tm_sec;
        int seconds2 = tm2.tm_hour * 3600 + tm2.tm_min * 60 + tm2.tm_sec;

        return seconds2 - seconds1;
    }

    Call parse_call(const std::string& line) {
        std::istringstream iss(line);
        std::string cmd, from_number, to_number, date, from_time, end_time;

        iss >> cmd >> from_number >> to_number >> date >> from_time >> end_time;

        int duration = count_time(from_time, end_time);

        return { from_number, to_number, date, from_time, end_time, duration };
    }

    void process(const std::vector<Call>& calls) {
        std::string line;

        bool all_valid_phone_numbers = true;
        int total_calls = calls.size();
        std::unordered_map<std::string, int> calls_from_number;
        std::unordered_map<std::string, int> duration_from_number;

        for(const auto& call : calls) {
            if(!check_phone(call.from_number) || !check_phone(call.to_number)) {
                all_valid_phone_numbers = false;
            }

            calls_from_number[call.from_number]++;
            duration_from_number[call.from_number] += call.duration_seconds;
        }

        while(getline(std::cin, line) && line != "#") {
            if(line.empty())
                continue;

            std::istringstream iss(line);
            std::string queryType;
            iss >> queryType;

            if(queryType == "?check_phone_number") {
                std::cout << (all_valid_phone_numbers ? 1 : 0) << std::endl;
            } else if(queryType == "?number_calls_from") {
                std::string phone_number;
                iss >> phone_number;
                std::cout << calls_from_number[phone_number] << std::endl;
            } else if(queryType == "?number_total_calls") {
                std::cout << total_calls << std::endl;
            } else if(queryType == "?count_time_calls_from") {
                std::string phone_number;
                iss >> phone_number;
                std::cout << duration_from_number[phone_number] << std::endl;
            }
        }
    }
};

int main() {
    Database data{ };
    return 0;
}