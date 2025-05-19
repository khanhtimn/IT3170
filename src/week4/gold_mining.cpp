/*
Description
The Kingdom ALPHA has n warehouses of golds located on a straight line and are numbered 1, 2,..., n.
The warehouse i has amount of ai (ai is non-negative integer) and is located at coordinate i (i=1,...,n).
The King of ALPHA opens a competition for hunters who are responsible to find a subset of gold warehouses having largest total amount of golds with respect to the condition that the distance between two selected warehouses must be greater than or equal to L1 and less than or equal to L2.

Input
Line 1 contains n, L1, and L2 (1≤n≤1000000,1≤L1≤L2≤n)
Line 2 contains n integers a1,a2,…,an

Output
Contains only one single integer denoting the total amount of golds of selected warehouses.
Example
Input
6 2 3
3 5 9 6 7 4

Output
19
*/

#include <algorithm>
#include <deque>
#include <iostream>
#include <vector>

using std::cin;
using std::cout;
using std::deque;
using std::endl;
using std::vector;

long long find_max_gold(const vector<int>& gold_amounts, int L1, int L2)
{
    const int n = gold_amounts.size();
    vector<long long> dp(n + 1, 0);
    deque<int> window;
    long long max_gold = 0;

    for (int i = 1; i <= n; ++i) {
        while (!window.empty() && window.front() < i - L2) {
            window.pop_front();
        }

        int j = i - L1;
        if (j >= 1) {
            while (!window.empty() && dp[window.back()] < dp[j]) {
                window.pop_back();
            }
            window.push_back(j);
        }

        dp[i] = gold_amounts[i - 1] + (window.empty() ? 0 : dp[window.front()]);
        max_gold = std::max(max_gold, dp[i]);
    }

    return max_gold;
}

int main()
{
    std::ios::sync_with_stdio(false);
    std::cin.tie(nullptr);

    int n, L1, L2;
    cin >> n >> L1 >> L2;

    vector<int> gold_amounts(n);
    for (int i = 0; i < n; ++i) {
        cin >> gold_amounts[i];
    }

    cout << find_max_gold(gold_amounts, L1, L2) << endl;
    return 0;
}
