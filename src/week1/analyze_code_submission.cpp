/*
Data about submission of a programming contest consists a sequence of lines,
each line has the following information: <UserID> <ProblemID> <TimePoint>
<Status> <Point> in which the user <UserID> submits his/her code to solve the
problem <ProblemID> at time-point <TimePoint>, and gets status <Status> and
point <Point>

    <UserID>: string of length from 3 to 10
    <ProblemID>: string under the format Pxy where x, y are digits 0,1,...,9
(for example P03, P10) <TimePoint>: string representing time-point with the
format HH:MM:SS (for example, 09:45:20 means the time-point 9 hour 45 minutes 20
seconds) <Status>: string with two cases (ERR, OK) <Point>: integer from {0, 1,
2, ..., 10}


A user can submit the code for solving each problem several time. The point that
the user gets for a problem is the maximal point among the submissions for that
problem.

Perform a sequence of queries of following types:

    ?total_number_submissions: return the number of submissions of the contest
    ?number_error_submision: return the number of submissions having status ERR
    ?number_error_submision_of_user <UserID>: return the number of submission
having status ERR of user <UserID> ?total_point_of_user <UserID>: return the
total point of user <UserID> ?number_submission_period <from_time_point>
<to_time_point>: return the number of submissions in the period from
<from_time_point> to <to_time_point> (inclusive)


Input
The input consists of two blocks of data:

    The first block is the operational data, which is a sequence of lines
(number of lines can be up to 100000), each line contains the information of a
submission with above format .The first block is terminated with a line
containing the character # The second block is the query block, which is a
sequence of lines (number of lines can be up to 100000), each line is a query
described above. The second block is terminated with a line containing the
character #


Output
Write in each line, the result of the corresponding query

Example
Input
U001 P01 10:30:20 ERR 0
U001 P01 10:35:20 OK 10
U001 P02 10:40:20 ERR 0
U001 P02 10:55:20 OK 7
U002 P01 10:40:20 ERR 0
U001 P01 11:35:20 OK 8
U002 P02 10:40:20 OK 10
#
?total_number_submissions
?number_error_submision
?number_error_submision_of_user U002
?total_point_of_user U001
?number_submission_period 10:00:00 11:30:45
#


Output
7
3
1
17
6
*/

#include <algorithm>
#include <iostream>
#include <sstream>
#include <string>
#include <unordered_map>
#include <vector>

struct Submission {
    std::string user_id;
    std::string problem_id;
    std::string time_point;
    std::string status;
    int point;
};

struct SubmissionStore {
    void add(const Submission& s) {
        submissions_.push_back(s);
        sorted_ = false;

        if(s.status == "ERR") {
            error_count_by_user_[s.user_id]++;
        }
        if(s.status == "OK") {
            auto& cur = max_points_[s.user_id][s.problem_id];
            cur = std::max(cur, s.point);
        }
    }

    int total() const {
        return static_cast<int>(submissions_.size());
    }

    int totalErrors() const {
        return static_cast<int>(std::count_if(
            submissions_.begin(), submissions_.end(), [](const Submission& s) { return s.status == "ERR"; }));
    }

    int errorsOf(const std::string& user_id) const {
        auto it = error_count_by_user_.find(user_id);
        return it != error_count_by_user_.end() ? it->second : 0;
    }

    int totalPointsOf(const std::string& user_id) const {
        auto it = max_points_.find(user_id);
        if(it == max_points_.end())
            return 0;
        int sum = 0;
        for(const auto& [_, pts] : it->second)
            sum += pts;
        return sum;
    }

    int countInPeriod(const std::string& from, const std::string& to) {
        ensureSorted();
        auto lo = std::lower_bound(submissions_.begin(),
                                   submissions_.end(),
                                   from,
                                   [](const Submission& s, const std::string& t) { return s.time_point < t; });
        auto hi = std::upper_bound(submissions_.begin(),
                                   submissions_.end(),
                                   to,
                                   [](const std::string& t, const Submission& s) { return t < s.time_point; });
        return static_cast<int>(std::distance(lo, hi));
    }

private:
    void ensureSorted() {
        if(sorted_)
            return;
        std::sort(submissions_.begin(), submissions_.end(), [](const Submission& a, const Submission& b) {
            return a.time_point < b.time_point;
        });
        sorted_ = true;
    }

    std::vector<Submission> submissions_;
    bool sorted_ = false;
    std::unordered_map<std::string, int> error_count_by_user_;
    std::unordered_map<std::string, std::unordered_map<std::string, int>> max_points_;
};

int main() {
    SubmissionStore store;

    std::string line;
    while(std::getline(std::cin, line) && line != "#") {
        if(line.empty())
            continue;

        std::istringstream ss(line);
        Submission s;
        ss >> s.user_id >> s.problem_id >> s.time_point >> s.status >> s.point;
        store.add(s);
    }

    while(std::getline(std::cin, line) && line != "#") {
        if(line.empty())
            continue;

        std::istringstream ss(line);
        std::string query;
        ss >> query;

        if(query == "?total_number_submissions") {
            std::cout << store.total() << '\n';
        } else if(query == "?number_error_submision") {
            std::cout << store.totalErrors() << '\n';
        } else if(query == "?number_error_submision_of_user") {
            std::string uid;
            ss >> uid;
            std::cout << store.errorsOf(uid) << '\n';
        } else if(query == "?total_point_of_user") {
            std::string uid;
            ss >> uid;
            std::cout << store.totalPointsOf(uid) << '\n';
        } else if(query == "?number_submission_period") {
            std::string from, to;
            ss >> from >> to;
            std::cout << store.countInPeriod(from, to) << '\n';
        }
    }

    return 0;
}
