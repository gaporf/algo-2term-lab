#include <iostream>
#include <vector>
#include <math.h>

void dfs(size_t v, size_t p, size_t k, size_t &timer, std::vector<size_t> &tin,
         std::vector<size_t> &tout, std::vector<std::vector<size_t>> &up,
         std::vector<std::vector<size_t>> &g) {
    up[v][0] = p;
    tin[v] = timer++;
    for (size_t i = 1; i < k; i++) {
        up[v][i] = up[up[v][i - 1]][i - 1];
    }
    for (auto &to : g[v]) {
        if (to != p) {
            dfs(to, v, k, timer, tin, tout, up, g);
        }
    }
    tout[v] = timer++;
}

bool is_ancestor(size_t a, size_t b, std::vector<size_t> &tin, std::vector<size_t> &tout) {
    return tin[a] <= tin[b] && tout[a] >= tout[b];
}

size_t lca(size_t u, size_t v, size_t k, std::vector<size_t> &tin, std::vector<size_t> &tout,
           std::vector<std::vector<size_t>> &up) {
    if (is_ancestor(u, v, tin, tout)) {
        return u;
    } else if (is_ancestor(v, u, tin, tout)) {
        return v;
    } else {
        for (size_t i = k - 1; i <= k; i--) {
            if (!is_ancestor(up[u][i], v, tin, tout)) {
                u = up[u][i];
            }
        }
        return up[u][0];
    }
}

size_t get(size_t u, size_t v, size_t root, size_t k, std::vector<size_t> &tin,
           std::vector<size_t> &tout, std::vector<std::vector<size_t>> &up) {
    if (is_ancestor(root, u, tin, tout) && is_ancestor(root, v, tin, tout)) {
        return lca(u, v, k, tin, tout, up);
    } else if (is_ancestor(root, u, tin, tout) != is_ancestor(root, v, tin, tout)) {
        return root;
    } else {
        size_t uv = lca(u, v, k, tin, tout, up),
                ur = lca(u, root, k, tin, tout, up),
                vr = lca(v, root, k, tin, tout, up);
        if (is_ancestor(ur, vr, tin, tout)) {
            ur = vr;
        }
        if (is_ancestor(uv, ur, tin, tout)) {
            uv = ur;
        }
        return uv;
    }
}

bool solve() {
    size_t n;
    std::cin >> n;
    if (n == 0) {
        return false;
    }
    std::vector<std::vector<size_t>> g(n);
    for (size_t i = 1; i < n; i++) {
        size_t from, to;
        std::cin >> from >> to;
        from--;
        to--;
        g[from].push_back(to);
        g[to].push_back(from);
    }
    auto k = static_cast<size_t>(log2(static_cast<double>(n))) + 1;
    std::vector<std::vector<size_t>> up(n, std::vector<size_t>(k));
    size_t timer = 0;
    std::vector<size_t> tin(n), tout(n);
    dfs(0, 0, k, timer, tin, tout, up, g);
    size_t m;
    std::cin >> m;
    size_t root = 0;
    while (m-- > 0) {
        char c;
        std::cin >> c;
        if (c == '?') {
            size_t u, v;
            std::cin >> u >> v;
            std::cout << get(u - 1, v - 1, root, k, tin, tout, up) + 1 << std::endl;
        } else {
            std::cin >> root;
            root--;
        }
    }
    return true;
}

int main() {
    std::ios_base::sync_with_stdio(false);
    while (solve()) {}
    return 0;
}
