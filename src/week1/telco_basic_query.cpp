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
    std::string fromNumber;
    std::string toNumber;
    std::string date;
    std::string fromTime;
    std::string endTime;
    int durationSeconds;
};

static bool isValidPhoneNumber(const std::string& number) {
    return number.length() == 10 && number.find_first_not_of("0123456789") == std::string::npos;
}

static int calculateDuration(const std::string& from, const std::string& to) {
    std::tm t1 = {}, t2 = {};
    std::istringstream s1(from), s2(to);
    s1 >> std::get_time(&t1, "%H:%M:%S");
    s2 >> std::get_time(&t2, "%H:%M:%S");
    return (t2.tm_hour * 3600 + t2.tm_min * 60 + t2.tm_sec) - (t1.tm_hour * 3600 + t1.tm_min * 60 + t1.tm_sec);
}

struct CallStore {
    void add(const Call& c) {
        calls_.push_back(c);

        if(!isValidPhoneNumber(c.fromNumber) || !isValidPhoneNumber(c.toNumber)) {
            allValid_ = false;
        }

        callCount_[c.fromNumber]++;
        duration_[c.fromNumber] += c.durationSeconds;
    }

    int total() const {
        return static_cast<int>(calls_.size());
    }

    bool allValid() const {
        return allValid_;
    }

    int callsFrom(const std::string& number) const {
        auto it = callCount_.find(number);
        return it != callCount_.end() ? it->second : 0;
    }

    int durationFrom(const std::string& number) const {
        auto it = duration_.find(number);
        return it != duration_.end() ? it->second : 0;
    }

private:
    std::vector<Call> calls_;
    bool allValid_ = true;
    std::unordered_map<std::string, int> callCount_;
    std::unordered_map<std::string, int> duration_;
};

int main() {
    CallStore store;

    std::string line;
    while(std::getline(std::cin, line) && line != "#") {
        if(line.empty())
            continue;

        std::istringstream ss(line);
        std::string cmd;
        Call c;
        ss >> cmd >> c.fromNumber >> c.toNumber >> c.date >> c.fromTime >> c.endTime;
        c.durationSeconds = calculateDuration(c.fromTime, c.endTime);
        store.add(c);
    }

    while(std::getline(std::cin, line) && line != "#") {
        if(line.empty())
            continue;

        std::istringstream ss(line);
        std::string query;
        ss >> query;

        if(query == "?check_phone_number") {
            std::cout << (store.allValid() ? 1 : 0) << '\n';
        } else if(query == "?number_calls_from") {
            std::string num;
            ss >> num;
            std::cout << store.callsFrom(num) << '\n';
        } else if(query == "?number_total_calls") {
            std::cout << store.total() << '\n';
        } else if(query == "?count_time_calls_from") {
            std::string num;
            ss >> num;
            std::cout << store.durationFrom(num) << '\n';
        }
    }

    return 0;
}
