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
#include <vector>

int count(std::vector<std::pair<int, int>>& set) {
    int count{ }, last{ };

    auto cmp = [](const std::pair<int, int>& l, const std::pair<int, int>& r) {
        if(l.second != r.second) {
            return l.second < r.second;
        }
        return l.first < r.first;
    };
    std::sort(set.begin(), set.end(), cmp);

    for(auto& pair : set) {
        if(pair.first > last) {
            ++count;
            last = pair.second;
        }
    }

    return count;
}

int main() {
    int n;
    std::cin >> n;

    auto set = std::vector<std::pair<int, int>>(n);

    for(auto& pair : set) {
        std::cin >> pair.first >> pair.second;
    }

    std::cout << count(set) << std::endl;

    return 0;
}
