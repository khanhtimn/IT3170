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
