/*
Description
Given N elements (2≤ N ≤100,000) on a straight line at positions x1,…, xN (0 ≤ xi ≤1,000,000,000).
The distance of a subset of N elements is defined to be the minimum distance between two elements.
Find the subset of N given elements containing exactly C elements such that the distance is maximal.
Input
The first line contains a positive integer T (1 <= T <= 20) which is the number of test cases.
Subsequent lines are T test cases with the following format:
Line 1: Two space-separated integers: N and C
Lines 2: contains  x1, x2, . . . , xN

Output
For each test case output one integer: the distance of the subset found.

Example
input
1
5 3
1 2 8 4 9
output
3

Explain: Jonh can put his 3 cows in the stalls at positions 1, 4 and 8, resulting in a minimum distance of 3.
*/

#include <algorithm>
#include <iostream>
#include <vector>

using std::vector;

bool can_place(const vector<int>& pos, int C, int D)
{
    int placed = 1;
    int last = pos[0];
    for (int i = 1; i < (int)pos.size() && placed < C; ++i) {
        if (pos[i] - last >= D) {
            last = pos[i];
            ++placed;
        }
    }
    return placed >= C;
}

int find_max_distance(vector<int> positions, int C)
{
    std::sort(positions.begin(), positions.end());

    int low = 0;
    int high = positions.back() - positions.front();
    while (low < high) {
        int mid = low + (high - low + 1) / 2;
        if (can_place(positions, C, mid)) {
            low = mid;
        } else {
            high = mid - 1;
        }
    }
    return low;
}

int main()
{
    std::ios::sync_with_stdio(false);
    std::cin.tie(nullptr);

    int T;
    std::cin >> T;
    while (T--) {
        int N, C;
        std::cin >> N >> C;
        vector<int> positions(N);
        for (int i = 0; i < N; ++i) {
            std::cin >> positions[i];
        }

        std::cout << find_max_distance(std::move(positions), C) << std::endl;
    }
    return 0;
}
