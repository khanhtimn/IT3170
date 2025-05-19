/*
Description
Given a sequence of n integers a=a1, . . ., an.
A subsequence of a consists of contiguous elements of a (for example, ai, ai+1, . . . ,aj). The weight of a subsequence is defined to be the sum of its elements. A subsequence is called even-subsequnce if its weight is even. Find the even-subsequence of a having largest weight.

Input
Line 1: contains a positive integer n (1 <= n <= 10^6)
Line 2: contains a1, . . ., an (-10^6 <= ai <= 10^6)

Output
The weight of the largest even-subsequence found, or write NOT_FOUND if no solution found.

Example
Input
4
1  2  -3  4
Output
4
*/

#include <algorithm>
#include <iostream>
#include <limits>
#include <vector>

using std::cin;
using std::cout;
using std::endl;
using std::vector;

const long long INF = std::numeric_limits<long long>::max();

long long find_max_even_subsequence(const vector<long long>& numbers)
{
    long long max_weight = -INF;
    long long min_odd_sum = INF;
    long long min_even_sum = 0;
    long long prefix_sum = 0;

    for (const long long num : numbers) {
        prefix_sum += num;

        if (prefix_sum % 2 == 0) {
            max_weight = std::max(max_weight, prefix_sum - min_even_sum);
            min_even_sum = std::min(min_even_sum, prefix_sum);
        } else {
            max_weight = std::max(max_weight, prefix_sum - min_odd_sum);
            min_odd_sum = std::min(min_odd_sum, prefix_sum);
        }
    }

    return max_weight;
}

int main()
{
    std::ios::sync_with_stdio(false);
    std::cin.tie(nullptr);

    int n;
    cin >> n;

    vector<long long> numbers(n);
    for (int i = 0; i < n; ++i) {
        cin >> numbers[i];
    }

    long long result = find_max_even_subsequence(numbers);
    if (result == -INF) {
        return 1;
    } else {
        cout << result << endl;
    }

    return 0;
}
