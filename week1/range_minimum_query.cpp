#include <algorithm>
#include <iostream>
#include <vector>

using std::vector;

vector<vector<int>> buildSparseTable(const vector<int>& arr)
{
    int n = arr.size();
    int log_n = 32 - __builtin_clz(n);
    vector<vector<int>> st(n, vector<int>(log_n));

    for (int i = 0; i < n; i++) {
        st[i][0] = arr[i];
    }

    for (int j = 1; (1 << j) <= n; j++) {
        for (int i = 0; i + (1 << j) - 1 < n; i++) {
            st[i][j] = std::min(st[i][j - 1], st[i + (1 << (j - 1))][j - 1]);
        }
    }

    return st;
}

int queryRMQ(const vector<vector<int>>& st, int left, int right)
{
    int len = right - left + 1;
    int log_len = 31 - __builtin_clz(len);

    return std::min(st[left][log_len], st[right - (1 << log_len) + 1][log_len]);
}

vector<int> readArray()
{
    int n;
    std::cin >> n;

    vector<int> arr(n);
    for (auto& x : arr) {
        std::cin >> x;
    }

    return arr;
}

long long processQueries(const vector<vector<int>>& st)
{
    int m;
    std::cin >> m;

    long long sum = 0;
    for (int k = 0; k < m; k++) {
        int i, j;
        std::cin >> i >> j;

        int min_val = queryRMQ(st, i, j);
        sum += min_val;
    }

    return sum;
}

int main()
{
    vector<int> arr = readArray();

    vector<vector<int>> sparseTable = buildSparseTable(arr);

    long long result = processQueries(sparseTable);
    std::cout << result << std::endl;

    return 0;
}
