/*
Description
The department D has m teachers T={1,2,...,m} and n courses C={1,2,...,n}.
Input
The input consists of following lines
Line 1: contains two integer m and n (1≤m≤10, 1≤n≤30)
Line i+1: contains an positive integer k and k positive integers indicating the
courses that teacher i can teach (∀i=1,…,m) Line m+2: contains an integer k Line
i+m+2: contains two integer i and j indicating two conflicting courses
(∀i=1,…,k) Output The output contains a unique number which is the maximal load
of the teachers in the solution found and the value -1 if not solution found.
Example
Input
4 12
5 1 3 5 10 12
5 9 3 4 8 12
6 1 2 3 4 9 7
7 1 2 3 5 6 10 11
25
1 2
1 3
1 5
2 4
2 5
2 6
3 5
3 7
3 10
4 6
4 9
5 6
5 7
5 8
6 8
6 9
7 8
7 10
7 11
8 9
8 11
8 12
9 12
10 11
11 12

Output
3
*/

#include <algorithm>
#include <iostream>
#include <limits>
#include <vector>


int m, n;
std::vector<std::vector<bool>> can_teach;
std::vector<std::vector<bool>> conflict;
std::vector<int> course;
std::vector<int> load;
int res = std::numeric_limits<int>::max();

bool check(int t, int c) {
    for(int i = 1; i < c; ++i) {
        if(course[i] == t && conflict[c][i]) {
            return false;
        }
    }
    return true;
}

void TRY(int k) {
    if(k > n) {
        int max_load = *max_element(load.begin() + 1, load.begin() + m + 1);
        if(max_load < res) {
            res = max_load;
        }
        return;
    }

    for(int t = 1; t <= m; ++t) {
        if(can_teach[t][k] && check(t, k)) {
            course[k] = t;
            load[t]++;

            if(load[t] < res) {
                TRY(k + 1);
            }

            load[t]--;
            course[k] = 0;
        }
    }
}

int main() {
    std::cin >> m >> n;

    can_teach.resize(m + 1, std::vector<bool>(n + 1, false));
    conflict.resize(n + 1, std::vector<bool>(n + 1, false));
    course.resize(n + 1, 0);
    load.resize(m + 1, 0);

    for(int i = 1; i <= m; ++i) {
        int k;
        std::cin >> k;
        for(int j = 0; j < k; ++j) {
            int course;
            std::cin >> course;
            can_teach[i][course] = true;
        }
    }

    int num_conflicts;
    std::cin >> num_conflicts;
    for(int i = 0; i < num_conflicts; ++i) {
        int c1, c2;
        std::cin >> c1 >> c2;
        conflict[c1][c2] = conflict[c2][c1] = true;
    }

    TRY(1);

    if(res < std::numeric_limits<int>::max()) {
        std::cout << res << std::endl;
    } else {
        std::cout << -1 << std::endl;
    }

    return 0;
}