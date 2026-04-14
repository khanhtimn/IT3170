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

#include <array>
#include <iostream>
#include <queue>
#include <vector>

struct Cell {
    bool blocked = false;
};

using Maze = std::vector<std::vector<Cell>>;

struct Position {
    int row = 0;
    int col = 0;
};

class MazeSolver {
public:
    MazeSolver() {
        std::cin >> rows_ >> cols_ >> start_.row >> start_.col;
        --start_.row;
        --start_.col;

        maze_.assign(rows_, std::vector<Cell>(cols_));
        for(int row{ }; row < rows_; ++row) {
            for(int col{ }; col < cols_; ++col) {
                int value;
                std::cin >> value;
                maze_[row][col].blocked = (value == 1);
            }
        }
    }

    int solve() const {
        if(!is_valid(start_) || maze_[start_.row][start_.col].blocked) {
            return -1;
        }

        if(is_border(start_)) {
            return 1;
        }

        std::vector<std::vector<int>> dist(rows_, std::vector<int>(cols_, -1));

        std::queue<Position> q;
        q.push(start_);
        dist[start_.row][start_.col] = 0;

        while(!q.empty()) {
            const Position current = q.front();
            q.pop();

            for(int dir = 0; dir < 4; ++dir) {
                Position next{ current.row + dx[dir], current.col + dy[dir] };

                if(!is_valid(next)) {
                    continue;
                }

                if(maze_[next.row][next.col].blocked) {
                    continue;
                }

                if(dist[next.row][next.col] != -1) {
                    continue;
                }

                dist[next.row][next.col] = dist[current.row][current.col] + 1;

                if(is_border(next)) {
                    return dist[next.row][next.col] + 1;
                }

                q.push(next);
            }
        }

        return -1;
    }

private:
    bool is_valid(const Position& pos) const {
        return pos.row >= 0 && pos.row < rows_ && pos.col >= 0 && pos.col < cols_;
    }

    bool is_border(const Position& pos) const {
        return pos.row == 0 || pos.row == rows_ - 1 || pos.col == 0 || pos.col == cols_ - 1;
    }

    static constexpr std::array<int, 4> dx{ -1, 1, 0, 0 };
    static constexpr std::array<int, 4> dy{ 0, 0, -1, 1 };

    int rows_ = 0;
    int cols_ = 0;
    Position start_;
    Maze maze_;
};

int main() {
    MazeSolver solver{ };
    std::cout << solver.solve() << std::endl;
    return 0;
}