/*
Description
A project has n tasks 1,. . ., n. Task i has duration d(i) to be completed (i=1,. . ., n). There are precedence constraints between tasks represented by a set Q of pairs: for each (i,j)  in Q, task j cannot be started before the completion of task i. Compute the earliest completion time  of the project.

Input
Line 1: contains n and m (1 <= n <= 10^4, 1 <= m <= 200000)
Line 2: contains d(1),. . ., d(n) (1 <= d(i) <= 1000)
Line i+3 (i=1,. . ., m) : contains i and j : task j cannot be started to execute before the completion of task i

Output
Write the earliest completion time of the project.

Example
Input
9 13
5 3 1 2 6 4 3 1 4
1 3
1 5
1 6
2 1
2 3
3 5
4 1
4 2
4 6
5 8
7 9
9 5
9 8

Output
18
*/

#include <algorithm>
#include <iostream>
#include <queue>
#include <vector>

using std::cin;
using std::cout;
using std::endl;
using std::queue;
using std::vector;

struct Task {
    int duration;
    vector<int> successors;
};

void add_dependency(vector<Task>& tasks, vector<int>& in_degree, int from, int to)
{
    tasks[from].successors.push_back(to);
    in_degree[to]++;
}

vector<int> topological_sort(const vector<Task>& tasks, vector<int>& in_degree, int num_tasks)
{
    vector<int> order;
    queue<int> ready_tasks;

    for (int i = 1; i <= num_tasks; ++i) {
        if (in_degree[i] == 0) {
            ready_tasks.push(i);
        }
    }

    while (!ready_tasks.empty()) {
        int current = ready_tasks.front();
        ready_tasks.pop();
        order.push_back(current);

        for (int next : tasks[current].successors) {
            in_degree[next]--;
            if (in_degree[next] == 0) {
                ready_tasks.push(next);
            }
        }
    }

    return order;
}

void read_input(vector<Task>& tasks, vector<int>& in_degree, int num_tasks, int num_dependencies)
{
    for (int i = 1; i <= num_tasks; ++i) {
        cin >> tasks[i].duration;
    }

    for (int i = 0; i < num_dependencies; ++i) {
        int from, to;
        cin >> from >> to;
        add_dependency(tasks, in_degree, from, to);
    }

    for (int i = 1; i <= num_tasks; ++i) {
        if (tasks[i].successors.empty()) {
            add_dependency(tasks, in_degree, i, num_tasks + 1);
        }
    }
}

int find_earliest_completion_time(const vector<Task>& tasks,
    vector<int>& in_degree,
    int num_tasks)
{
    vector<int> completion_time(num_tasks + 2, 0);
    vector<int> order = topological_sort(tasks, in_degree, num_tasks);

    for (int task : order) {
        for (int next : tasks[task].successors) {
            completion_time[next] = std::max(
                completion_time[next],
                completion_time[task] + tasks[task].duration);
        }
    }

    return completion_time[num_tasks + 1];
}

int main()
{
    std::ios::sync_with_stdio(false);
    std::cin.tie(nullptr);

    int num_tasks, num_dependencies;
    cin >> num_tasks >> num_dependencies;

    vector<Task> tasks(num_tasks + 2);
    vector<int> in_degree(num_tasks + 2, 0);

    read_input(tasks, in_degree, num_tasks, num_dependencies);
    cout << find_earliest_completion_time(tasks, in_degree, num_tasks) << endl;

    return 0;
}
