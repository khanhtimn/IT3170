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

using std::vector;

const int MOD = 1000000007;

int merge_count(vector<int>& a, vector<int>& buff, int left, int right)
{
    if (right - left <= 1)
        return 0;
    int mid = left + (right - left) / 2;
    int inversion_count = (merge_count(a, buff, left, mid) % MOD + merge_count(a, buff, mid, right) % MOD) % MOD;

    int i = left, j = mid, k = left;
    while (i < mid || j < right) {
        if (i < mid && (j == right || a[i] <= a[j])) {
            buff[k++] = a[i++];
        } else {
            inversion_count = (inversion_count % MOD + (mid - i) % MOD) % MOD;
            buff[k++] = a[j++];
        }
    }
    for (int t = left; t < right; ++t) {
        a[t] = buff[t];
    }
    return inversion_count;
}

int main()
{
    std::ios::sync_with_stdio(false);
    std::cin.tie(nullptr);

    int n;
    std::cin >> n;
    vector<int> a(n), tmp(n);
    for (int i = 0; i < n; ++i) {
        std::cin >> a[i];
    }

    int result = merge_count(a, tmp, 0, n);
    std::cout << result << std::endl;
    return 0;
}
