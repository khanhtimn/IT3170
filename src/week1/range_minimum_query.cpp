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

#include <algorithm>
#include <cmath>
#include <iostream>
#include <vector>

struct SparseTable {
    SparseTable(const std::vector<int>& arr) {
        n = arr.size();
        int max_log = std::log2(n) + 1;
        st.assign(n, std::vector<int>(max_log));

        for(int i = 0; i < n; i++) {
            st[i][0] = arr[i];
        }

        for(int j = 1; (1 << j) <= n; j++) {
            for(int i = 0; i + (1 << j) <= n; i++) {
                st[i][j] = std::min(st[i][j - 1], st[i + (1 << (j - 1))][j - 1]);
            }
        }
    }

    int query(int L, int R) const {
        int len = R - L + 1;
        int k = std::log2(len);
        return std::min(st[L][k], st[R - (1 << k) + 1][k]);
    }

private:
    int n;
    std::vector<std::vector<int>> st;
};

int main() {
    int n;
    if(!(std::cin >> n))
        return 0;

    std::vector<int> a(n);
    for(int i = 0; i < n; ++i) {
        std::cin >> a[i];
    }

    SparseTable solver(a);

    int m;
    if(!(std::cin >> m))
        return 0;

    long long sum = 0;
    for(int i = 0; i < m; ++i) {
        int start, end;
        std::cin >> start >> end;
        sum += solver.query(start, end);
    }

    std::cout << sum << std::endl;
    return 0;
}
