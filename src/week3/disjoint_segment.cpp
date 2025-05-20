/*
Description
Given a set of segments X = {(a1, b1), . . . , (an, bn)}
in which ai < bi are coordinates of the segment i on a line, i = 1, …, n.
Find a subset of X having largest cardinality in which no two segments of the subset intersect
Input
Line 1: contains a positive integer n (1 <= n <= 100000)
Line i+1 (i= 1,...,n): contains ai and bi (0 <= ai <= bi <= 1000000)
Output
Number of segments in the solution found.
Example
Input
6
0 10
3 7
6 14
9 11
12 15
17 19

Output
4
*/

#include <algorithm>
#include <iostream>
#include <utility>
#include <vector>

using segment = std::pair<int, int>;

bool compare_segments(const segment& a, const segment& b)
{
    if (a.second != b.second)
        return a.second < b.second;
    return a.first < b.first;
}

void sort_segments(std::vector<segment>& segments)
{
    std::sort(segments.begin(),
        segments.end(),
        compare_segments);
}

int main()
{
    std::ios::sync_with_stdio(false);
    std::cin.tie(nullptr);

    int n;
    std::cin >> n;
    std::vector<segment> segments;
    segments.reserve(n);

    for (int i = 0; i < n; ++i) {
        int a, b;
        std::cin >> a >> b;
        segments.emplace_back(a, b);
    }

    sort_segments(segments);

    int count = 0;
    int last = 0;
    for (auto& segment : segments) {
        if (segment.first > last) {
            ++count;
            last = segment.second;
        }
    }

    std::cout << count << std::endl;
    return 0;
}
