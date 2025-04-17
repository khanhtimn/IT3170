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

using std::stack;
using std::vector;

vector<vector<int>> readMatrix()
{
    int n, m;
    std::cin >> n >> m;

    vector<vector<int>> matrix(n, vector<int>(m));
    for (int i = 0; i < n; i++) {
        for (int j = 0; j < m; j++) {
            std::cin >> matrix[i][j];
        }
    }

    return matrix;
}

int largestRectangleInHistogram(const vector<int>& heights)
{
    int m = heights.size();
    vector<int> left(m), right(m);
    stack<int> s;

    for (int i = 0; i < m; i++) {
        while (!s.empty() && heights[s.top()] >= heights[i]) {
            s.pop();
        }
        left[i] = s.empty() ? -1 : s.top();
        s.push(i);
    }

    while (!s.empty())
        s.pop();

    for (int i = m - 1; i >= 0; i--) {
        while (!s.empty() && heights[s.top()] >= heights[i]) {
            s.pop();
        }
        right[i] = s.empty() ? m : s.top();
        s.push(i);
    }

    int maxArea = 0;
    for (int i = 0; i < m; i++) {
        maxArea = std::max(maxArea, heights[i] * (right[i] - left[i] - 1));
    }

    return maxArea;
}

int findLargestBlackSubrectangle(const vector<vector<int>>& matrix)
{
    if (matrix.empty())
        return 0;

    int n = matrix.size();
    int m = matrix[0].size();

    vector<int> heights(m, 0);

    int maxArea = 0;

    for (int i = 0; i < n; i++) {
        for (int j = 0; j < m; j++) {
            heights[j] = matrix[i][j] == 0 ? 0 : heights[j] + 1;
        }

        maxArea = std::max(maxArea, largestRectangleInHistogram(heights));
    }

    return maxArea;
}

int main()
{
    vector<vector<int>> matrix = readMatrix();

    int result = findLargestBlackSubrectangle(matrix);

    std::cout << result;

    return 0;
}
