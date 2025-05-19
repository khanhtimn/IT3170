/*
Description
Given a positive integer n and n positive integers a1, a2, ..., an.
Compute the number of positive integer solutions to the equation:
a1X1 + a2X2 + . . . + anXn = M

Input
Dòng 1: n và M
Dòng 2: a
1
, a
2
, ..., a
n
Output
   Số nghiệm nguyên dương
Ví dụ
Input
3 5
1 1 1
Output
6
*/

#include <iostream>
#include <vector>

int countSolutions(int a[], int n, int M, int pos)
{
    if (M == 0 && pos == n)
        return 1;
    if (M <= 0 || pos == n)
        return 0;

    int solutions = 0;
    for (int i = 1; i <= M; i++) {
        solutions += countSolutions(a, n, M - a[pos] * i, pos + 1);
    }

    return solutions;
}

int main()
{
    int n, M;
    std::cin >> n >> M;
    std::vector<int> a(n);
    for (int i = 0; i < n; i++)
        std::cin >> a[i];

    std::cout << countSolutions(a.data(), n, M, 0) << std::endl;
    return 0;
}
