/*
Description
The director of a hospital want to schedule a working plan for a nurse in a given period of N consecutive days 1,..., N. Due to the policy of the hospital, each nurse cannot work all the days 1,..., N. Instead, there must be days off in which the nurse need to take a rest. A working plan is a sequence of disjoint working periods. A working period of a nurse is defined to be a sequence of consecutive days on which the nurse must work and the length of the working period is the number of consecutive days of that working period. The hospital imposes two constraints:
Each nurse can take a rest only one day between two consecutive working periods. it means that if the nurse takes a rest today, then she has to work tomorrow (1)
The length of each working period must be greater or equal to K1 and less than or equal to K2

The director of the hospital want to know how many possible working plans satisfying above constraint?
Input
The input consists of one line which contains 3 positive integers N, K1, K2 (2 <= N <= 1000, K1 < K2 <= 400)

Output
The output consists of only one single integer M modulo 10^9 + 7
Where M is the total working plans satisfying the above constraints.

Example
Input
6 2 3
Output
4
*/

#include <iostream>
#include <vector>

using std::cin;
using std::cout;
using std::endl;
using std::vector;

constexpr int MOD = 1e9 + 7;

int count_working_plans(int total_days, int min_work_days, int max_work_days)
{
    vector<vector<int>> dp(total_days + 1, vector<int>(2, 0));

    dp[0][0] = dp[0][1] = 1;

    for (int day = 1; day <= total_days; ++day) {
        for (int work_days = min_work_days; work_days <= max_work_days; ++work_days) {
            if (day - work_days >= 0) {
                dp[day][1] = (dp[day][1] + dp[day - work_days][0]) % MOD;
            }
        }

        dp[day][0] = dp[day - 1][1];
    }

    return (dp[total_days][0] + dp[total_days][1]) % MOD;
}

int main()
{
    std::ios::sync_with_stdio(false);
    std::cin.tie(nullptr);

    int total_days, min_work_days, max_work_days;
    cin >> total_days >> min_work_days >> max_work_days;

    cout << count_working_plans(total_days, min_work_days, max_work_days) << endl;
    return 0;
}
