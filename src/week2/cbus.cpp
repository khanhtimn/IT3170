/*
There are n passengers 1, 2, …, n. The passenger i want to travel from point i to point i + n (i = 1,2,…,n).
There is a bus located at point 0 and has k places for transporting the passengers (it means at any time, there are at most k passengers on the bus).
You are given the distance matrix c in which c(i,j) is the traveling distance from point i to point j (i, j = 0,1,…, 2n).
Compute the shortest route for the bus, serving n passengers and coming back to point 0.
Input
Line 1 contains n and k (1≤n≤11,1≤k≤10)
Line i+1 (i=1,2,…,2n+1) contains the (i−1)th line of the matrix c (rows and columns are indexed from 0,1,2,..,2n).
Output
Unique line contains the length of the shortest route.

Example
Input
3  2
0  8  5  1  10  5  9
9  0  5  6  6  2  8
2  2  0  3  8  7  2
5  3  4  0  3  2  7
9  6  8  7  0  9  10
3  8  10  6  5  0  2
3  4  4  5  2  2  0
Output
25
*/

#include <climits>
#include <iostream>
#include <vector>

using std::vector;

int n, k;
vector<vector<int>> c;
vector<int> x;
vector<bool> appear;
int load = 0;
int cost = 0;
int c_min = INT_MAX;
int min_edge = INT_MAX;

bool is_valid(int v)
{
    if (appear[v])
        return false;

    if (v <= n) {
        return load < k;
    } else {
        return appear[v - n];
    }
}

void try_route(int pos)
{
    if (pos > 2 * n) {
        int total_cost = cost + c[x[pos - 1]][0];
        if (total_cost < c_min) {
            c_min = total_cost;
        }
        return;
    }

    for (int v = 1; v <= 2 * n; v++) {
        if (is_valid(v)) {
            x[pos] = v;
            appear[v] = true;

            int old_cost = cost;
            cost += c[x[pos - 1]][v];

            if (v <= n)
                load++;
            else
                load--;

            int lower_bound = cost + (2 * n + 1 - pos) * min_edge;
            if (lower_bound < c_min) {
                try_route(pos + 1);
            }

            appear[v] = false;
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
    appear.resize(2 * n + 1, false);

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

    std::cout << c_min << std::endl;

    return 0;
}
