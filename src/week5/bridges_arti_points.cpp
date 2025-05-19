/*
Description
Given an undirected graph containing N vertices and M edges, find all the
articulation points and the bridges in the graph.

Input
The first line consists of two space-separated integers denoting N and M,
M lines follow, each containing two space-separated integers X and Y denoting
there is an edge between X and Y.

Output
One line consists of two integers denoting the number of articulation points and
the number of bridges.

Example
Input
10 12
1 10
10 2
10 3
2 4
4 5
5 2
3 6
6 7
7 3
7 8
8 9
9 7
Output
4  3
*/

#include <algorithm>
#include <iostream>
#include <vector>

using std::cin;
using std::cout;
using std::endl;
using std::vector;

void dfs(int vertex,
    const vector<vector<int>>& adj,
    vector<int>& discovery_time,
    vector<int>& low_value,
    vector<int>& parent,
    vector<bool>& is_articulation_point,
    int& num_bridges,
    int& current_time)
{
    discovery_time[vertex] = low_value[vertex] = ++current_time;
    int num_children = 0;

    for (int neighbor : adj[vertex]) {
        if (discovery_time[neighbor] == 0) {
            num_children++;
            parent[neighbor] = vertex;
            dfs(neighbor,
                adj,
                discovery_time,
                low_value,
                parent,
                is_articulation_point,
                num_bridges,
                current_time);

            low_value[vertex] = std::min(low_value[vertex], low_value[neighbor]);

            if (discovery_time[vertex] < low_value[neighbor]) {
                num_bridges++;
            }

            if (parent[vertex] == vertex && num_children >= 2) {
                is_articulation_point[vertex] = true;
            }
            if (parent[vertex] != vertex && low_value[neighbor] >= discovery_time[vertex]) {
                is_articulation_point[vertex] = true;
            }
        } else if (parent[vertex] != neighbor) {
            low_value[vertex] = std::min(low_value[vertex], discovery_time[neighbor]);
        }
    }
}

std::pair<int, int> find_articulation_points_and_bridges(const vector<vector<int>>& adj, int num_vertices)
{
    vector<int> discovery_time(num_vertices + 1, 0);
    vector<int> low_value(num_vertices + 1, 0);
    vector<int> parent(num_vertices + 1);
    vector<bool> is_articulation_point(num_vertices + 1, false);
    int num_bridges = 0;
    int current_time = 0;

    for (int i = 1; i <= num_vertices; ++i) {
        parent[i] = i;
    }

    for (int i = 1; i <= num_vertices; ++i) {
        if (discovery_time[i] == 0) {
            dfs(i,
                adj,
                discovery_time,
                low_value,
                parent,
                is_articulation_point,
                num_bridges,
                current_time);
        }
    }

    int num_articulation_points = std::count(
        is_articulation_point.begin(), is_articulation_point.end(), true);

    return { num_articulation_points, num_bridges };
}

int main()
{
    std::ios::sync_with_stdio(false);
    std::cin.tie(nullptr);

    int num_vertices, num_edges;
    cin >> num_vertices >> num_edges;

    vector<vector<int>> adj(num_vertices + 1);

    for (int i = 0; i < num_edges; ++i) {
        int u, v;
        cin >> u >> v;
        adj[u].push_back(v);
        adj[v].push_back(u);
    }

    auto [num_articulation_points, num_bridges] = find_articulation_points_and_bridges(adj, num_vertices);
    cout << num_articulation_points << " " << num_bridges << endl;

    return 0;
}
