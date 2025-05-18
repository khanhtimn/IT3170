/*
Description
Given an undirected graph containing N vertices and M edges, find all the articulation points and the bridges in the graph.

Input
The first line consists of two space-separated integers denoting N and M,
M lines follow, each containing two space-separated integers X and Y denoting there is an edge between X and Y.

Output
One line consists of two integers denoting the number of articulation points and the number of bridges.

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

class Graph {
private:
    vector<vector<int>> adj;
    vector<int> discovery_time;
    vector<int> low_value;
    vector<int> parent;
    vector<bool> is_articulation_point;
    int num_vertices;
    int num_bridges;
    int current_time;

    void dfs(int vertex)
    {
        discovery_time[vertex] = low_value[vertex] = ++current_time;
        int num_children = 0;

        for (int neighbor : adj[vertex]) {
            if (discovery_time[neighbor] == 0) {
                num_children++;
                parent[neighbor] = vertex;
                dfs(neighbor);

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

public:
    Graph(int vertices)
        : num_vertices(vertices)
        , num_bridges(0)
        , current_time(0)
    {
        adj.resize(vertices + 1);
        discovery_time.resize(vertices + 1, 0);
        low_value.resize(vertices + 1, 0);
        parent.resize(vertices + 1);
        is_articulation_point.resize(vertices + 1, false);

        for (int i = 1; i <= vertices; ++i) {
            parent[i] = i;
        }
    }

    void add_edge(int u, int v)
    {
        adj[u].push_back(v);
        adj[v].push_back(u);
    }

    std::pair<int, int> find_articulation_points_and_bridges()
    {
        for (int i = 1; i <= num_vertices; ++i) {
            if (discovery_time[i] == 0) {
                dfs(i);
            }
        }

        int num_articulation_points = std::count(is_articulation_point.begin(),
            is_articulation_point.end(), true);

        return { num_articulation_points, num_bridges };
    }
};

int main()
{
    std::ios::sync_with_stdio(false);
    std::cin.tie(nullptr);

    int num_vertices, num_edges;
    cin >> num_vertices >> num_edges;

    Graph graph(num_vertices);

    for (int i = 0; i < num_edges; ++i) {
        int u, v;
        cin >> u >> v;
        graph.add_edge(u, v);
    }

    auto [num_articulation_points, num_bridges] = graph.find_articulation_points_and_bridges();
    cout << num_articulation_points << " " << num_bridges << endl;

    return 0;
}
