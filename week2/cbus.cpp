#include <climits>
#include <iostream>
#include <vector>

using std::vector;

int n, k;
vector<vector<int>> c;
vector<int> x;
vector<bool> visited;
int load, cost = 0;
int min_cost, min_edge = INT_MAX;

bool is_valid(int v)
{
    if (visited[v])
        return false;

    if (v <= n) {
        return load < k;
    } else {
        return visited[v - n];
    }
}

void try_route(int pos)
{
    if (pos > 2 * n) {
        int total_cost = cost + c[x[pos - 1]][0];
        if (total_cost < min_cost) {
            min_cost = total_cost;
        }
        return;
    }

    for (int v = 1; v <= 2 * n; v++) {
        if (is_valid(v)) {
            x[pos] = v;
            visited[v] = true;

            int old_cost = cost;
            cost += c[x[pos - 1]][v];

            if (v <= n)
                load++;
            else
                load--;

            int lower_bound = cost + (2 * n + 1 - pos) * min_edge;
            if (lower_bound < min_cost) {
                try_route(pos + 1);
            }

            visited[v] = false;
            cost = old_cost;

            if (v <= n)
                load--;
            else
                load++;
        }
    }
}

int main()
{
    std::cin >> n >> k;

    c.resize(2 * n + 1, vector<int>(2 * n + 1));
    x.resize(2 * n + 1);
    visited.resize(2 * n + 1, false);

    for (int i = 0; i <= 2 * n; i++) {
        for (int j = 0; j <= 2 * n; j++) {
            std::cin >> c[i][j];
            if (i != j && c[i][j] < min_edge) {
                min_edge = c[i][j];
            }
        }
    }

    x[0] = 0;

    try_route(1);

    std::cout << min_cost << std::endl;

    return 0;
}
