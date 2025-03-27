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
