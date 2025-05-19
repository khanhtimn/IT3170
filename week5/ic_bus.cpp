/*
Description
There are n cities 1, 2, ..., n. Between 2 cities i and j there can be 1 road (2-way) connecting them.
Each city i has bus route i where C[i] is the fare per ride and D[i] is the maximum number of cities that bus i can go to on a journey through the connecting roads.
Find a way to go from city 1 to city n with the least amount of money to pay

Input
Line 1: contains 2 positive integers n and m where n is the number of cities and m is the number of roads connecting the cities (1 <= n <= 5000, 1 <= m <= 10000)
Line i+1 (i = 1,2,...,n): contains 2 positive integers C[i] and D[i] (1 <= C[i] <= 10000, 1 <= D[i] <= 100)
Line n+1+i (i = 1, 2, ..., m): contains 2 positive integers i and j where there is a connecting road between cities i and j
Output
Minimum amount of money to spend to take the bus from city 1 to city n

Example
Input
6 6
10 2
30 1
50 1
20 3
30 1
20 1
1 2
1 3
1 5
2 4
2 5
4 6
Output
30

Explanation:
-Getting on bus 1 from city 1 to city 4 costs 10 dong
-Getting on bus 4 from city 4 to city 6 costs 20 dong
Total cost 10 + 20 = 30 dong
*/

#include <algorithm>
#include <iostream>
#include <queue>
#include <vector>

using std::cin;
using std::cout;
using std::endl;
using std::priority_queue;
using std::vector;

struct State {
    long long cost;
    int remaining_steps;
    int city;

    bool operator>(const State& other) const
    {
        return cost > other.cost;
    }
};

void read_bus_info(vector<long long>& fare, vector<int>& max_steps)
{
    int n = fare.size() - 1;
    for (int i = 1; i <= n; ++i) {
        cin >> fare[i] >> max_steps[i];
    }
}

void add_road(vector<vector<int>>& graph, int a, int b)
{
    graph[a].push_back(b);
    graph[b].push_back(a);
}

long long find_min_cost(const vector<vector<int>>& graph,
    const vector<long long>& fare,
    const vector<int>& max_steps,
    int start,
    int end)
{
    int n = graph.size() - 1;
    vector<vector<long long>> min_cost(n + 1, vector<long long>(n + 1, 1e18));

    priority_queue<State, vector<State>, std::greater<>> pq;
    min_cost[start][max_steps[start]] = fare[start];
    pq.push({ fare[start], max_steps[start], start });

    while (!pq.empty()) {
        auto [cost, steps, city] = pq.top();
        pq.pop();

        if (cost != min_cost[city][steps])
            continue;
        if (city == end)
            return cost;

        for (int next : graph[city]) {
            if (steps > 0) {
                if (min_cost[next][steps - 1] > cost) {
                    min_cost[next][steps - 1] = cost;
                    pq.push({ cost, steps - 1, next });
                }
                if (min_cost[next][max_steps[next]] > cost + fare[next]) {
                    min_cost[next][max_steps[next]] = cost + fare[next];
                    pq.push({ cost + fare[next], max_steps[next], next });
                }
            }
        }
    }
    return -1;
}

int main()
{
    std::ios::sync_with_stdio(false);
    std::cin.tie(nullptr);

    int num_cities, num_roads;
    cin >> num_cities >> num_roads;

    vector<vector<int>> graph(num_cities + 1);
    vector<long long> fare(num_cities + 1);
    vector<int> max_steps(num_cities + 1);

    read_bus_info(fare, max_steps);

    for (int i = 0; i < num_roads; ++i) {
        int a, b;
        cin >> a >> b;
        add_road(graph, a, b);
    }

    cout << find_min_cost(graph, fare, max_steps, 1, num_cities) << endl;
    return 0;
}
