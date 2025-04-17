/*
Một mê cung hình chữ nhật được biểu diễn bởi 0-1 ma trận NxM trong đó:
A[i,j] = 1 thể hiện ô (i,j) là tường gạch và A[i,j] = 0 thể hiện ô (i,j) là ô trống, có thể di chuyển vào.
Từ 1 ô trống, ta có thể di chuyển sang 1 trong 4 ô lân cận (lên trên, xuống dưới, sang trái, sang phải)
nếu ô đó là ô trống. Xuất phát từ 1 ô trống trong mê cung, hãy tìm đường ngắn nhất thoát ra khỏi mê cung.

Input
Dòng 1: ghi 4 số nguyên dương n, m, r, c trong đó n và m tương ứng là số hàng và cột của ma trận A (1 <= n,m <= 999)
r, c tương ứng là chỉ số hàng, cột của ô xuất phát.
Dòng i+1 (i=1,...,n): ghi dòng thứ i của ma trận A

Output
Ghi giá số bước cần di chuyển ngắn nhất để thoát ra khỏi mê cung,
hoặc ghi giá trị -1 nếu không tìm thấy đường đi nào thoát ra khỏi mê cung.

Ví dụ

Input
8 12 5 6
1 1 0 0 0 0 1 0 0 0 0 1
1 0 0 0 1 1 0 1 0 0 1 1
0 0 1 0 0 0 0 0 0 0 0 0
1 0 0 0 0 0 1 0 0 1 0 1
1 0 0 1 0 0 0 0 0 1 0 0
1 0 1 0 1 0 0 0 1 0 1 0
0 0 0 0 1 0 1 0 0 0 0 0
1 0 1 1 0 1 1 1 0 1 0 1

Output
7
*/

#include <iostream>
#include <queue>
#include <utility>
#include <vector>

using std::pair;
using std::queue;
using std::vector;

const int dx[] = { -1, 1, 0, 0 };
const int dy[] = { 0, 0, -1, 1 };

int solve(vector<vector<int>>& maze, int n, int m, int r, int c)
{
    vector<vector<bool>> visited(n, vector<bool>(m, false));

    queue<pair<pair<int, int>, int>> q;

    q.push({ { r, c }, 0 });
    visited[r][c] = true;

    while (!q.empty()) {
        auto u = q.front();
        q.pop();

        int r0 = u.first.first;
        int c0 = u.first.second;
        int steps = u.second;

        if (r0 == 0 || r0 == n - 1 || c0 == 0 || c0 == m - 1) {
            return steps + 1;
        }

        for (int i = 0; i < 4; i++) {
            int r1 = r0 + dx[i];
            int c1 = c0 + dy[i];

            if (r1 >= 0 && r1 < n && c1 >= 0 && c1 < m && maze[r1][c1] == 0 && !visited[r1][c1]) {
                visited[r1][c1] = true;
                q.push({ { r1, c1 }, steps + 1 });
            }
        }
    }

    return -1;
}

int main()
{
    int n, m, r, c;
    std::cin >> n >> m >> r >> c;

    r--;
    c--;

    vector<vector<int>> maze(n, vector<int>(m));
    for (int i = 0; i < n; i++) {
        for (int j = 0; j < m; j++) {
            std::cin >> maze[i][j];
        }
    }

    if (maze[r][c] == 1) {
        std::cout << -1 << std::endl;
        return 0;
    }

    if (r == 0 || r == n - 1 || c == 0 || c == m - 1) {
        std::cout << 1 << std::endl;
        return 0;
    }

    int result = solve(maze, n, m, r, c);
    std::cout << result << std::endl;

    return 0;
}
