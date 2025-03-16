#include <chrono>
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
