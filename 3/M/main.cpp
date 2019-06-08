#include <iostream>
#include <vector>
#include <math.h>

void dfs(size_t v, std::vector<std::vector<size_t>> &g, std::vector<size_t> &e,
         std::vector<size_t> &num, std::vector<size_t> &get_num, size_t deep) {
    num[v] = e.size();
    get_num.push_back(v);
    e.push_back(deep);
    for (auto &to : g[v]) {
        dfs(to, g, e, num, get_num, deep + 1);
        e.push_back(deep);
        get_num.push_back(v);
    }
}

uint64_t get(size_t v, size_t u, std::vector<size_t> &num, std::vector<size_t> &get_num,
             std::vector<std::vector<std::pair<size_t, size_t>>> &sparse, std::vector<size_t> &p,
             std::vector<size_t> &o) {
    v = num[v];
    u = num[u];
    if (v > u) {
        std::swap(v, u);
    }
    size_t k = p[u - v + 1],
            j = o[k];
    auto ans = std::min(sparse[v][k], sparse[u - j + 1][k]);
    return get_num[ans.second];
}

int main() {
    std::ios_base::sync_with_stdio(false);
    size_t n, m;
    std::cin >> n >> m;
    std::vector<std::vector<size_t>> g(n);
    std::vector<size_t> e, num(n), get_num;
    for (size_t i = 1; i < n; i++) {
        size_t from;
        std::cin >> from;
        g[from].push_back(i);
    }
    dfs(0, g, e, num, get_num, 0);
    size_t N = e.size();
    size_t k = static_cast<size_t>(log2(N)) + 1;
    std::vector<std::vector<std::pair<size_t, size_t>>>
            sparse(N, std::vector<std::pair<size_t, size_t>>(k));
    std::vector<size_t> get_p(N + 1), get_o(k);
    get_p[1] = 0;
    get_o[0] = 1;
    for (size_t i = 2; i <= N; i++) {
        get_p[i] = get_p[i / 2] + 1;
    }
    for (size_t i = 1; i < k; i++) {
        get_o[i] = get_o[i - 1] * 2;
    }
    for (size_t i = 0; i < N; i++) {
        sparse[i][0] = {e[i], i};
    }
    for (size_t j = 1, res = 1; j < k; j++, res *= 2) {
        for (size_t i = 0; i + res < N; i++) {
            sparse[i][j] = std::min(sparse[i][j - 1], sparse[i + res][j - 1]);
        }
    }
    uint64_t a_1, a_2;
    std::cin >> a_1 >> a_2;
    uint64_t x, y, z;
    std::cin >> x >> y >> z;
    uint64_t sum = 0,
            u = get(a_1, a_2, num, get_num, sparse, get_p, get_o);
    sum = u;
    for (size_t i = 1; i < m; i++) {
        uint64_t a_3 = (x * a_1 + y * a_2 + z) % n,
                a_4 = (x * a_2 + y * a_3 + z) % n;
        a_1 = a_3;
        a_2 = a_4;
        u = get((a_1 + u) % n, a_2, num, get_num, sparse, get_p, get_o);
        sum += u;
    }
    std::cout << sum << std::endl;
    return 0;
}
