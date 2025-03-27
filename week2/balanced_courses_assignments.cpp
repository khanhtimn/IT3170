#include <algorithm>
#include <climits>
#include <iostream>
#include <vector>

using std::vector;

int m, n;
vector<vector<bool>> can_teach;
vector<vector<bool>> conflicts;
vector<int> assignment;
vector<int> load;
int best_max_load = INT_MAX;
bool solution_found = false;

bool has_conflict(int t, int c)
{
    for (int i = 1; i < c; ++i) {
        if (assignment[i] == t && conflicts[c][i]) {
            return true;
        }
    }
    return false;
}

void try_assignment(int k)
{
    if (k > n) {
        int max_load = *max_element(load.begin() + 1, load.begin() + m + 1);
        if (max_load < best_max_load) {
            best_max_load = max_load;
        }
        solution_found = true;
        return;
    }

    for (int t = 1; t <= m; ++t) {
        if (can_teach[t][k] && !has_conflict(t, k)) {
            assignment[k] = t;
            load[t]++;

            if (load[t] < best_max_load) {
                try_assignment(k + 1);
            }

            load[t]--;
            assignment[k] = 0;
        }
    }
}

int main()
{
    std::cin >> m >> n;

    can_teach.resize(m + 1, vector<bool>(n + 1, false));
    conflicts.resize(n + 1, vector<bool>(n + 1, false));
    assignment.resize(n + 1, 0);
    load.resize(m + 1, 0);

    for (int i = 1; i <= m; ++i) {
        int k;
        std::cin >> k;
        for (int j = 0; j < k; ++j) {
            int course;
            std::cin >> course;
            can_teach[i][course] = true;
        }
    }

    int num_conflicts;
    std::cin >> num_conflicts;
    for (int i = 0; i < num_conflicts; ++i) {
        int c1, c2;
        std::cin >> c1 >> c2;
        conflicts[c1][c2] = conflicts[c2][c1] = true;
    }

    try_assignment(1);

    if (solution_found) {
        std::cout << best_max_load << std::endl;
    } else {
        std::cout << -1 << std::endl;
    }

    return 0;
}
