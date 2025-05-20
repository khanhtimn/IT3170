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
#include <regex>
#include <sstream>
#include <string>
#include <unordered_map>
#include <vector>

using std::cin;
using std::cout;
using std::endl;
using std::getline;
using std::regex;
using std::string;
using std::unordered_map;
using std::vector;

struct Call {
    string fromNumber;
    string toNumber;
    string date;
    string fromTime;
    string endTime;
    int durationSeconds;
};

bool isValidPhoneNumber(const string& phoneNumber)
{
    if (phoneNumber.length() != 10)
        return false;
    return phoneNumber.find_first_not_of("0123456789") == string::npos;
}

int calculateDuration(const string& fromTime, const string& endTime)
{
    std::tm tm1 = {}, tm2 = {};
    std::istringstream ss1(fromTime);
    std::istringstream ss2(endTime);

    ss1 >> std::get_time(&tm1, "%H:%M:%S");
    ss2 >> std::get_time(&tm2, "%H:%M:%S");

    int seconds1 = tm1.tm_hour * 3600 + tm1.tm_min * 60 + tm1.tm_sec;
    int seconds2 = tm2.tm_hour * 3600 + tm2.tm_min * 60 + tm2.tm_sec;

    return seconds2 - seconds1;
}

Call parseCall(const string& line)
{
    std::istringstream iss(line);
    string cmd, fromNumber, toNumber, date, fromTime, endTime;

    iss >> cmd >> fromNumber >> toNumber >> date >> fromTime >> endTime;

    int duration = calculateDuration(fromTime, endTime);

    return { fromNumber, toNumber, date, fromTime, endTime, duration };
}

vector<Call> readCallData()
{
    vector<Call> calls;
    string line;

    while (getline(cin, line) && line != "#") {
        if (line.empty())
            continue;
        calls.push_back(parseCall(line));
    }

    return calls;
}

void processQueries(const vector<Call>& calls)
{
    string line;

    bool allValidPhoneNumbers = true;
    int totalCalls = calls.size();
    unordered_map<string, int> callsFromNumber;
    unordered_map<string, int> durationFromNumber;

    for (const auto& call : calls) {
        if (!isValidPhoneNumber(call.fromNumber) || !isValidPhoneNumber(call.toNumber)) {
            allValidPhoneNumbers = false;
        }

        callsFromNumber[call.fromNumber]++;
        durationFromNumber[call.fromNumber] += call.durationSeconds;
    }

    while (getline(cin, line) && line != "#") {
        if (line.empty())
            continue;

        std::istringstream iss(line);
        string queryType;
        iss >> queryType;

        if (queryType == "?check_phone_number") {
            cout << (allValidPhoneNumbers ? 1 : 0) << endl;
        } else if (queryType == "?number_calls_from") {
            string phoneNumber;
            iss >> phoneNumber;
            cout << callsFromNumber[phoneNumber] << endl;
        } else if (queryType == "?number_total_calls") {
            cout << totalCalls << endl;
        } else if (queryType == "?count_time_calls_from") {
            string phoneNumber;
            iss >> phoneNumber;
            cout << durationFromNumber[phoneNumber] << endl;
        }
    }
}

int main()
{
    vector<Call> calls = readCallData();

    processQueries(calls);

    return 0;
}
