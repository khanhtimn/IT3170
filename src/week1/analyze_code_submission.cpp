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

class Submits {
public:
    Submits() {
        std::string line;

        // parse first part
        while(std::getline(std::cin, line) && line != "#") {
            std::stringstream ss(line);
            Submission submission;
            ss >> submission.user_id >> submission.problem_id >> submission.time_point >> submission.status >>
                submission.point;
            submissions.push_back(submission);
            if(submission.status == "ERR") {
                user_error_count[submission.user_id]++;
            }
            if(submission.status == "OK") {
                user_problem_max_points[submission.user_id][submission.problem_id] =
                    std::max(user_problem_max_points[submission.user_id][submission.problem_id], submission.point);
            }
        }
        sort(submissions.begin(), submissions.end(), [](const Submission& a, const Submission& b) {
            return a.time_point < b.time_point;
        });

        // process
        while(std::getline(std::cin, line) && line != "#") {
            std::stringstream ss(line);
            std::string query;
            ss >> query;
            if(query == "?total_number_submissions") {
                std::cout << total_number_submissions() << std::endl;
            } else if(query == "?number_error_submision") {
                std::cout << number_error_submision() << std::endl;
            } else if(query == "?number_error_submision_of_user") {
                std::string user_id;
                ss >> user_id;
                std::cout << number_error_submision_of_user(user_id) << std::endl;
            } else if(query == "?total_point_of_user") {
                std::string user_id;
                ss >> user_id;
                std::cout << total_point_of_user(user_id) << std::endl;
            } else if(query == "?number_submission_period") {
                std::string from_time, to_time;
                ss >> from_time >> to_time;
                std::cout << number_submission_period(from_time, to_time) << std::endl;
            }
        }
    }

private:
    std::vector<Submission> submissions;
    std::unordered_map<std::string, int> user_error_count;
    std::unordered_map<std::string, std::unordered_map<std::string, int>> user_problem_max_points;

    int total_number_submissions() {
        return submissions.size();
    }

    int number_error_submision() {
        return count_if(submissions.begin(), submissions.end(), [](const Submission& s) { return s.status == "ERR"; });
    }

    int number_error_submision_of_user(const std::string& user_id) {
        return user_error_count[user_id];
    }

    int total_point_of_user(const std::string& user_id) {
        int total_points = 0;
        for(const auto& problem_points : user_problem_max_points[user_id]) {
            total_points += problem_points.second;
        }
        return total_points;
    }

    int number_submission_period(const std::string& from_time, const std::string& to_time) {
        auto lower = lower_bound(submissions.begin(),
                                 submissions.end(),
                                 from_time,
                                 [](const Submission& s, const std::string& time) { return s.time_point < time; });

        auto upper = upper_bound(submissions.begin(),
                                 submissions.end(),
                                 to_time,
                                 [](const std::string& time, const Submission& s) { return time < s.time_point; });

        return distance(lower, upper);
    }
};

int main() {
    Submits s{ };
    return 0;
}
