#include <iostream>
#include <vector>
#include <math.h>

void dfs(size_t v, size_t p, size_t k, int32_t last, size_t deep,
         std::vector<std::vector<std::pair<size_t, int32_t>>> &g, std::vector<size_t> &h,
         std::vector<std::vector<std::pair<size_t, int32_t>>> &up) {
    h[v] = deep;
    up[v][0] = std::make_pair(p, last);
    for (size_t i = 1; i < k; i++) {
        size_t to = up[v][i - 1].first;
        up[v][i] = std::make_pair(
                up[to][i - 1].first,
                std::min(up[v][i - 1].second, up[to][i - 1].second));
    }
    for (auto &to : g[v]) {
        dfs(to.first, v, k, to.second, deep + 1, g, h, up);
    }
}

int32_t get(size_t x, size_t y, size_t k, size_t q, std::vector<size_t> &h,
            std::vector<std::vector<std::pair<size_t, int32_t>>> &up) {
    if (h[x] < h[y]) {
        std::swap(x, y);
    }
    int32_t ans = 1000000;
    for (size_t i = k - 1, res = q; res > 0; res /= 2, i--) {
        if (h[x] - h[y] >= res) {
            ans = std::min(ans, up[x][i].second);
            x = up[x][i].first;
        }
    }
    if (x == y) {
        return ans;
    }
    for (size_t i = k - 1; i < k; i--) {
        if (up[x][i].first != up[y][i].first) {
            ans = std::min(ans, up[x][i].second);
            ans = std::min(ans, up[y][i].second);
            x = up[x][i].first;
            y = up[y][i].first;
        }
    }
    ans = std::min(ans, std::min(up[x][0].second, up[y][0].second));
    return ans;
}

int main() {
    size_t n;
    std::cin >> n;
    std::vector<std::vector<std::pair<size_t, int32_t>>> g(n);
    std::vector<size_t> h(n);
    for (size_t i = 1; i < n; i++) {
        size_t p;
        int32_t num;
        std::cin >> p >> num;
        g[--p].emplace_back(i, num);
    }
    auto k = static_cast<size_t>(log2(static_cast<double>(n))) + 2,
            q = static_cast<size_t>(1);
    for (size_t i = 0; i < k - 1; i++) {
        q *= 2;
    }
    std::vector<std::vector<std::pair<size_t, int32_t>>>
            up(n, std::vector<std::pair<size_t, int32_t>>(k));
    dfs(0, 0, k, 1000000, 0, g, h, up);
    size_t m;
    std::cin >> m;
    while (m-- > 0) {
        size_t x, y;
        std::cin >> x >> y;
        x--;
        y--;
        std::cout << get(x, y, k, q, h, up) << std::endl;
    }
    return 0;
}
