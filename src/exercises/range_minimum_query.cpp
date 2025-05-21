/*
Description
Given a sequence of n integers a0,. . .,an-1.
We denote rmq(i,j) the minimum element of the sequence ai, ai+1, . . .,aj.
Given m pairs (i1, j1),. . .,(im, jm), compute the sum
Q = rmq(i1, j1) + . . . + rmq(im, jm)
Input
Line 1: n (1 <=  n <= 10^6)
Line 2: a0, . . . ,an-1 (1 <= ai <= 10^6)
line 3: m (1  <= m <= 10^6)
Line k+3 (k=1, . . ., m): ik, jk (0  <=  ik < jk< n)
Output
Write the value Q
Example
Input
16
2 4 6 1 6 8 7 3 3 5 8 9 1 2 6 4
4
1 5
0 9
1 15
6 10

Output
6
*/

#include <cmath>
#include <iostream>
#include <vector>

using std::cin;
using std::cout;
using std::endl;
using std::vector;

vector<vector<int>> build_sparse_table(const vector<int>& values)
{
    int n = values.size();
    int log_n = std::log2(n) + 1;
    vector<vector<int>> sparse_table(log_n, vector<int>(n, -1));

    for (int j = 0; j < n; ++j) {
        sparse_table[0][j] = j;
    }

    for (int i = 1; (1 << i) <= n; ++i) {
        for (int j = 0; j + (1 << i) - 1 < n; ++j) {
            int left = sparse_table[i - 1][j];
            int right = sparse_table[i - 1][j + (1 << (i - 1))];
            sparse_table[i][j] = (values[left] < values[right]) ? left : right;
        }
    }

    return sparse_table;
}

int query_min_index(const vector<vector<int>>& sparse_table,
    const vector<int>& values,
    int left, int right)
{
    int k = std::log2(right - left + 1);
    int power = 1 << k;

    int left_min = sparse_table[k][left];
    int right_min = sparse_table[k][right - power + 1];

    return (values[left_min] < values[right_min]) ? left_min : right_min;
}

vector<int> read_values()
{
    int n;
    cin >> n;

    vector<int> values(n);
    for (int i = 0; i < n; ++i) {
        cin >> values[i];
    }

    return values;
}

long long process_queries(const vector<vector<int>>& sparse_table,
    const vector<int>& values)
{
    int m;
    cin >> m;

    long long sum = 0;
    for (int k = 0; k < m; ++k) {
        int left, right;
        cin >> left >> right;
        int min_index = query_min_index(sparse_table, values, left, right);
        sum += values[min_index];
    }

    return sum;
}

int main()
{
    std::ios::sync_with_stdio(false);
    std::cin.tie(nullptr);

    vector<int> values = read_values();
    vector<vector<int>> sparse_table = build_sparse_table(values);

    cout << process_queries(sparse_table, values) << endl;
    return 0;
}
