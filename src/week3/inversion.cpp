/*
Description
Given a sequence of integers a1, a2, ..., an.
A pair (i, j) is call an inversion if i < j and ai > aj.
Compute the number Q of inversions
Input
Line 1: contains a positive integer n (1 <= n <= 10^6)
Line 2: contains a1, a2, ..., an (0 <= ai<= 10^6)
Output
Write the value Q modulo 10^9+7

Example
Input
6
3 2 4 5 6 1
Output
6
*/

#include <iostream>
#include <vector>

constexpr int MOD = 1e9 + 7;

int merge(std::vector<int>& a, int left, int right) {
    if(right - left <= 1) {
        return 0;
    }
    int mid = left + (right - left) / 2;
    int count = (merge(a, left, mid) + merge(a, mid, right)) % MOD;

    std::vector<int> buf{ };
    int i = left, j = mid;
    while(i < mid && j < right) {
        if(a[i] <= a[j]) {
            buf.push_back(a[i++]);
        } else {
            count = (count + (mid - i)) % MOD;
            buf.push_back(a[j++]);
        }
    }

    while(i < mid) {
        buf.push_back(a[i++]);
    }
    while(j < right) {
        buf.push_back(a[j++]);
    }

    std::copy(buf.begin(), buf.end(), a.begin() + left);

    return count;
}

int main() {
    int n;
    std::cin >> n;
    auto seq = std::vector<int>(n);
    auto buf = std::vector<int>(n);

    for(auto& x : seq) {
        std::cin >> x;
    }

    std::cout << merge(seq, 0, n) << std::endl;

    return 0;
}
