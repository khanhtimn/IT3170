/*
Description
Given a directed graph G=(V,E) where V={1,. . ., N} is the number of nodes and the set E has M arcs.
Compute number of strongly connected components of G

Input
Line 1: two positive integers N and M (1 <= N <= 10^5, 1 <= M <= 10^6)
Line i+1 (i=1,. . ., M): contains two positive integers u and v which are endpoints of i-th arc

Output
Write the number of strongly connected components of G

Example
Input
8 13
1 2
1 8
2 3
2 6
3 6
4 3
4 6
5 4
6 5
7 1
7 2
7 6
8 7
Output
3
*/

#include <algorithm>
#include <iostream>
#include <vector>

using std::cin;
using std::cout;
using std::endl;
using std::vector;

void dfs_first(int vertex,
    const vector<vector<int>>& graph,
    vector<bool>& visited,
    vector<int>& finish_order)
{
    visited[vertex] = true;
    for (int neighbor : graph[vertex]) {
        if (!visited[neighbor]) {
            dfs_first(neighbor, graph, visited, finish_order);
        }
    }
    finish_order.push_back(vertex);
}

void dfs_second(int vertex,
    const vector<vector<int>>& transpose,
    vector<bool>& visited)
{
    visited[vertex] = true;
    for (int neighbor : transpose[vertex]) {
        if (!visited[neighbor]) {
            dfs_second(neighbor, transpose, visited);
        }
    }
}

int count_strongly_connected_components(const vector<vector<int>>& graph,
    const vector<vector<int>>& transpose,
    int num_vertices)
{
    vector<bool> visited(num_vertices + 1, false);
    vector<int> finish_order;
    int num_components = 0;

    // First DFS to get finish order
    for (int i = 1; i <= num_vertices; ++i) {
        if (!visited[i]) {
            dfs_first(i, graph, visited, finish_order);
        }
    }

    // Reset visited array
    std::fill(visited.begin(), visited.end(), false);

    // Second DFS on transpose graph
    for (auto it = finish_order.rbegin(); it != finish_order.rend(); ++it) {
        if (!visited[*it]) {
            dfs_second(*it, transpose, visited);
            ++num_components;
        }
    }

    return num_components;
}

int main()
{
    std::ios::sync_with_stdio(false);
    std::cin.tie(nullptr);

    int num_vertices, num_edges;
    cin >> num_vertices >> num_edges;

    vector<vector<int>> graph(num_vertices + 1);
    vector<vector<int>> transpose(num_vertices + 1);

    for (int i = 0; i < num_edges; ++i) {
        int from, to;
        cin >> from >> to;
        graph[from].push_back(to);
        transpose[to].push_back(from);
    }

    cout << count_strongly_connected_components(graph, transpose, num_vertices) << endl;
    return 0;
}
