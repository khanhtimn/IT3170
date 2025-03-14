
#include <iostream>
#include <vector>

using std::vector;

int solve(vector<vector<int>> &rect, int n, int m) { return -1; }

int main() {
  int n, m;
  vector<vector<int>> rect;

  std::cin >> n >> m;
  for (int i = 0; i < n; i++) {
    for (int j = 0; j < m; j++)
      std::cin >> rect[i][j];
  }

  std::cout << solve(rect, n, m) << std::endl;

  return 0;
}
