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
#include <climits>
#include <iostream>
#include <vector>

struct BalancedCoursesSolver {
    int m, n;
    std::vector<std::vector<bool>> can_teach;
    std::vector<std::vector<bool>> conflicts;
    std::vector<int> assignment;
    std::vector<int> load;
    int best_max_load;
    bool solution_found;

    bool has_conflict(int t, int c) {
        for(int i = 1; i < c; ++i) {
            if(assignment[i] == t && conflicts[c][i]) {
                return true;
            }
        }
        return false;
    }

    void try_assignment(int k) {
        if(k > n) {
            int max_load = *max_element(load.begin() + 1, load.begin() + m + 1);
            if(max_load < best_max_load) {
                best_max_load = max_load;
            }
            solution_found = true;
            return;
        }

        for(int t = 1; t <= m; ++t) {
            if(can_teach[t][k] && !has_conflict(t, k)) {
                assignment[k] = t;
                load[t]++;

                if(load[t] < best_max_load) {
                    try_assignment(k + 1);
                }

                load[t]--;
                assignment[k] = 0;
            }
        }
    }

    int solve(int num_teachers,
              int num_courses,
              const std::vector<std::vector<bool>>& teach_matrix,
              const std::vector<std::vector<bool>>& conflict_matrix) {
        m = num_teachers;
        n = num_courses;
        can_teach = teach_matrix;
        conflicts = conflict_matrix;

        assignment.assign(n + 1, 0);
        load.assign(m + 1, 0);
        best_max_load = INT_MAX;
        solution_found = false;

        try_assignment(1);

        if(solution_found) {
            return best_max_load;
        } else {
            return -1;
        }
    }
};

int balanced_courses_solve_wrapper(int m,
                                   int n,
                                   const std::vector<std::vector<bool>>& teach,
                                   const std::vector<std::vector<bool>>& conflict) {
    BalancedCoursesSolver solver;
    return solver.solve(m, n, teach, conflict);
}

int main() {
    int m, n;
    if(!(std::cin >> m >> n))
        return 0;

    std::vector<std::vector<bool>> can_teach(m + 1, std::vector<bool>(n + 1, false));
    std::vector<std::vector<bool>> conflicts(n + 1, std::vector<bool>(n + 1, false));

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
        conflicts[c1][c2] = conflicts[c2][c1] = true;
    }

    BalancedCoursesSolver solver;
    std::cout << solver.solve(m, n, can_teach, conflicts) << std::endl;

    return 0;
}
