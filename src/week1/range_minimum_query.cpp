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
#include <iostream>
#include <tuple>
#include <vector>

int min_in_range(const std::vector<int>& a, std::vector<std::tuple<int, int, int>>& pairs)
{
    int sum = 0;
    for (auto& p : pairs) {
        std::get<2>(p) = *std::min_element(a.begin() + std::get<0>(p), a.begin() + std::get<1>(p) + 1);
    }

    for (const auto& [start, end, min] : pairs) {
        sum += min;
    }
    return sum;
}

int main()
{
    int n;
    std::cin >> n;
    std::vector<int> a(n);

    for (int i = 0; i < n; ++i) {
        std::cin >> a[i];
    }

    int m;
    std::cin >> m;
    std::vector<std::tuple<int, int, int>> pairs;

    for (int i = 0; i < m; ++i) {
        int start, end;
        std::cin >> start;
        std::cin >> end;
        pairs.push_back(std::make_tuple(start, end, 0));
    }

    std::cout << min_in_range(a, pairs) << "\n";

    return 0;
}
