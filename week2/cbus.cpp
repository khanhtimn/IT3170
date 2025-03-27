#include <algorithm>
#include <climits>
#include <iostream>
#include <map>
#include <vector>

using std::vector;

int n, k;
vector<vector<int>> c;
vector<int> x;
vector<bool> appear;
int load = 0;
int cost = 0;
int best_cost = INT_MAX;

std::map<std::pair<int, long long>, int> memo;

bool is_valid(int v)
{
    if (appear[v])
        return false;

    if (v <= n) {
        return load < k;
    } else {
        return appear[v - n];
    }
}

long long get_state_hash()
{
    long long hash = 0;
    for (int i = 1; i <= 2 * n; i++) {
        if (appear[i]) {
            hash |= (1LL << i);
        }
    }
    return hash;
}

void try_route(int pos)
{
    if (pos > 2 * n) {
        int total_cost = cost + c[x[pos - 1]][0];
        if (total_cost < best_cost) {
            best_cost = total_cost;
        }
        return;
    }

    if (cost >= best_cost)
        return;

    std::pair<int, long long> state = { x[pos - 1], get_state_hash() };
    if (memo.find(state) != memo.end() && cost >= memo[state])
        return;
    memo[state] = cost;

    vector<std::pair<int, int>> candidates;
    for (int v = 1; v <= 2 * n; v++) {
        if (is_valid(v)) {
            candidates.push_back({ c[x[pos - 1]][v], v });
        }
    }

    std::sort(candidates.begin(), candidates.end());

    for (auto& candidate : candidates) {
        int v = candidate.second;
        x[pos] = v;
        appear[v] = true;

        int old_cost = cost;
        cost += c[x[pos - 1]][v];

        if (v <= n)
            load++;
        else
            load--;

        if (cost < best_cost) {
            try_route(pos + 1);
        }

        appear[v] = false;
        cost = old_cost;
        if (v <= n)
            load--;
        else
            load++;
    }
}

int main()
{
    std::ios_base::sync_with_stdio(false);
    std::cin.tie(nullptr);

    std::cin >> n >> k;

    c.resize(2 * n + 1, vector<int>(2 * n + 1));
    x.resize(2 * n + 1);
    appear.resize(2 * n + 1, false);

    for (int i = 0; i <= 2 * n; i++) {
        for (int j = 0; j <= 2 * n; j++) {
            std::cin >> c[i][j];
        }
    }

    x[0] = 0;

    try_route(1);

    std::cout << best_cost << std::endl;

    return 0;
}
