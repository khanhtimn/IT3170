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

inline bool check(const std::vector<int>& pos, int C, int D) {
    int num_subs = 1;
    int last = pos[0];
    for(size_t i = 1; i < pos.size() && num_subs < C; ++i) {
        if(pos[i] - last >= D) {
            last = pos[i];
            ++num_subs;
        }
    }
    return num_subs >= C;
}

int find_max(std::vector<int>& a, int C) {
    std::sort(a.begin(), a.end());

    int low = 0;
    int high = a.back() - a.front();
    while(low < high) {
        int mid = low + (high - low + 1) / 2;
        if(check(a, C, mid)) {
            low = mid;
        } else {
            high = mid - 1;
        }
    }
    return low;
}

int main() {
    int T;
    std::cin >> T;
    while(T--) {
        int N, C;
        std::cin >> N >> C;
        auto seq = std::vector<int>(N);
        for(auto& x : seq) {
            std::cin >> x;
        }

        std::cout << find_max(seq, C) << std::endl;
    }
    return 0;
}