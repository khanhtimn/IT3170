/*
A Rectangle of size n x mis divided into unit-square 1 x 1 with two colors black and white.
The rectangle is represented by a matrix A(nx m) in which A(i, j) = 1 means that the unit-square at row i, column jis black and A(i, j) = 0 means that the unit-square at row icolumn jis white.
Find the sub-rectangle of the given rectangle containing only black unit-squares such that the area is maximal.
Input
•Line 1: contains 2 positive integers nand m(1 <= n, m <= 1000)
•Line i+1 (i= 1,…, n): contains the ith of the matrix A
Output
Write the area of the sub-rectangle found.

Example
Input
4 4
0 1 1 1
1 1 1 0
1 1 0 0
1 1 1 0
Output
6
*/

#include <algorithm>
#include <iostream>
#include <stack>
#include <vector>

using Matrix = std::vector<std::vector<int>>;

struct Histogram {
    Histogram() {
        int n, m;
        std::cin >> n >> m;
        matrix_.assign(n, std::vector<int>(m));

        for(int i{ }; i < n; ++i) {
            for(int j{ }; j < m; ++j) {
                std::cin >> matrix_[i][j];
            }
        }
    }

    int max_area(const std::vector<int>& heights) {
        int m = heights.size();
        std::vector<int> left(m), right(m);
        std::stack<int> s;

        for(int i{ }; i < m; i++) {
            while(!s.empty() && heights[s.top()] >= heights[i]) {
                s.pop();
            }
            left[i] = s.empty() ? -1 : s.top();
            s.push(i);
        }

        while(!s.empty())
            s.pop();

        for(int i{ m - 1 }; i >= 0; i--) {
            while(!s.empty() && heights[s.top()] >= heights[i]) {
                s.pop();
            }
            right[i] = s.empty() ? m : s.top();
            s.push(i);
        }

        int maxArea = 0;
        for(int i = 0; i < m; i++) {
            maxArea = std::max(maxArea, heights[i] * (right[i] - left[i] - 1));
        }

        return maxArea;
    }

    int largest_rec() {
        if(matrix_.empty())
            return 0;

        int n = matrix_.size();
        int m = matrix_[0].size();

        std::vector<int> heights(m, 0);

        int maxArea = 0;

        for(int i{ }; i < n; i++) {
            for(int j{ }; j < m; j++) {
                heights[j] = matrix_[i][j] == 0 ? 0 : heights[j] + 1;
            }

            maxArea = std::max(maxArea, max_area(heights));
        }

        return maxArea;
    }

private:
    Matrix matrix_{ };
};

int main() {
    Histogram histogram;

    std::cout << histogram.largest_rec() << std::endl;

    return 0;
}
