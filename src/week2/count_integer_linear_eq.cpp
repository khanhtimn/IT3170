/*
Description
Given a positive integer n and n positive integers a1, a2, ..., an.
Compute the number of positive integer solutions to the equation:
a1X1 + a2X2 + . . . + anXn = M

Input
Dòng 1: n và M
Dòng 2: a1, a2, ..., an
Output
   Số nghiệm nguyên dương
Ví dụ
Input
3 5
1 1 1
Output
6
*/

#include <iostream>
#include <numeric>
#include <vector>

int count_solutions(std::vector<int>& a, int n, int M) {
    int sumA = std::accumulate(a.begin(), a.end(), 0);
    if(M < sumA)
        return 0;

    int R = M - sumA;
    std::vector<long long> dp(R + 1, 0);
    dp[0] = 1;

    for(int coin : a) {
        for(int s = coin; s <= R; ++s) {
            dp[s] += dp[s - coin];
        }
    }
    return dp[R];
}

int main() {
    int n, M;
    std::cin >> n >> M;
    std::vector<int> a(n);
    for(int& x : a)
        std::cin >> x;

    std::cout << count_solutions(a, n, M) << std::endl;
    return 0;
}
