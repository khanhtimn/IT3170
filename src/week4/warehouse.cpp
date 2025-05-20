/*
Description
A truck is planned to arrive at some stations among N stations 1, 2, . . ., N located on a line. Station i (i = 1,…,N) has coordinate i and has following information
ai: amount of goods
ti: pickup time duration for taking goods

The route of the truck is a sequence of stations x1 < x2 < . . . < xk (1 ≤ xj ≤ N, j = 1,…, k). Due to technical constraints, the distance between two consecutive stations that the truck arrives xi and xi+1 is less than or equal to D and the total pickup time duration cannot exceed T. Find a route for the truck such that total amount of goods picked up is maximal.

Input
Line 1: N, T, D (1 <= N <= 1000, 1 <= T <= 100, 1 <= D <= 10)
Line 2: a1,a2,…,aN (1 <= ai <= 10)
Line 3: t1,t2,…,tN (1 <= ti <= 10)

Output
Write the total amount of goods that the truck picks up in the route.
Example
Input
6 6 2
6 8 5 10 11 6
1 2 2 3 3 2

Output
24
*/

#include <algorithm>
#include <iostream>
#include <vector>

using std::cin;
using std::cout;
using std::endl;
using std::vector;

int find_max_goods(const vector<int>& goods, const vector<int>& times, int max_time, int max_distance)
{
    const int n = goods.size();
    vector<vector<int>> dp(n + 1, vector<int>(max_time + 1, 0));
    int max_goods = 0;

    for (int i = 1; i <= n; ++i) {
        for (int time = times[i - 1]; time <= max_time; ++time) {
            for (int j = std::max(0, i - max_distance); j < i; ++j) {
                dp[i][time] = std::max(dp[i][time], dp[j][time - times[i - 1]] + goods[i - 1]);
            }
            max_goods = std::max(max_goods, dp[i][time]);
        }
    }

    return max_goods;
}

int main()
{
    std::ios::sync_with_stdio(false);
    std::cin.tie(nullptr);

    int n, max_time, max_distance;
    cin >> n >> max_time >> max_distance;

    vector<int> goods(n);
    vector<int> times(n);

    for (int i = 0; i < n; ++i) {
        cin >> goods[i];
    }
    for (int i = 0; i < n; ++i) {
        cin >> times[i];
    }

    cout << find_max_goods(goods, times, max_time, max_distance) << endl;
    return 0;
}