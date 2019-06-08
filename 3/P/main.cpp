#include <iostream>
#include <vector>
#include <math.h>
#include <queue>

void dfs0(size_t v, size_t p, size_t k, std::vector<std::vector<size_t>> &g,
          std::vector<size_t> &cnt, std::vector<std::vector<size_t>> &up,
          std::vector<size_t> &par, std::vector<bool> &used, std::queue<size_t> &q,
          std::vector<size_t> &tin, std::vector<size_t> &tout, size_t &timer) {
    tin[v] = timer++;
    up[v][0] = p;
    for (size_t i = 1; i < k; i++) {
        up[v][i] = up[up[v][i - 1]][i - 1];
    }
    par[v] = p;
    bool leap = true;
    for (auto &to : g[v]) {
        if (to != p) {
            dfs0(to, v, k, g, cnt, up, par, used, q, tin, tout, timer);
            cnt[v] += cnt[to];
            leap = false;
        }
    }
    if (leap) {
        q.push(v);
        used[v] = true;
    }
    tout[v] = timer++;
}

void dfs(size_t v, std::vector<std::vector<size_t>> &g, std::vector<std::pair<size_t, size_t>> &num,
         std::vector<std::vector<int64_t>> &t, std::vector<bool> &used,
         std::vector<size_t> &par, std::vector<size_t> &next_vertex) {
    for (auto &to : g[v]) {
        if (par[v] != to) {
            dfs(to, g, num, t, used, par, next_vertex);
        }
    }
    if (!used[v]) {
        size_t cur_t = t.size(),
                cur_num = 0,
                u = v;
        for (; u != next_vertex[v]; u = par[u]) {
            num[u] = std::make_pair(cur_t, cur_num++);
            used[u] = true;
        }
        used[u] = true;
        num[u] = std::make_pair(cur_t, cur_num++);
        t.emplace_back(4 * cur_num, 0);
    }
}

bool is_parent(size_t a, size_t b, std::vector<size_t> &tin, std::vector<size_t> &tout) {
    return tin[b] >= tin[a] && tout[b] <= tout[a];
}

size_t lca(size_t v, size_t u, std::vector<std::vector<size_t>> &up,
           std::vector<size_t> &tin, std::vector<size_t> &tout) {
    if (is_parent(v, u, tin, tout)) {
        return v;
    } else if (is_parent(u, v, tin, tout)) {
        return u;
    } else {
        for (size_t k = up[v].size() - 1; k <= up[v].size(); k--) {
            if (!is_parent(up[v][k], u, tin, tout)) {
                v = up[v][k];
            }
        }
        return up[v][0];
    }
}

void
update(size_t v, size_t l, size_t r, size_t tl, size_t tr, std::vector<int64_t> &sum, int64_t d) {
    if (tl > tr) {
        return;
    } else if (l == tl && tr == r) {
        sum[v] += d;
    } else {
        size_t mid = (l + r) / 2;
        update(2 * v + 1, l, mid, tl, std::min(tr, mid), sum, d);
        update(2 * v + 2, mid + 1, r, std::max(mid + 1, tl), tr, sum, d);
    }
}

void add(size_t v, size_t w, std::vector<std::pair<size_t, size_t>> &num, std::vector<size_t> &par,
         std::vector<std::vector<int64_t>> &t, std::vector<size_t> &next_vertex,
         std::vector<size_t> &tin, std::vector<size_t> &tout, int64_t d) {
    while (v != w) {
        size_t u = next_vertex[v];
        if (is_parent(u, w, tin, tout)) {
            update(0, 0, t[num[v].first].size() / 4 - 1,
                   num[v].second, num[w].second - 1, t[num[v].first], d);
            v = w;
        } else {
            update(0, 0, t[num[v].first].size() / 4 - 1,
                   num[v].second, t[num[v].first].size() / 4 - 1, t[num[v].first], d);
            v = par[u];
        }
    }
}

void
add(size_t v, size_t u, int64_t d, std::vector<std::vector<size_t>> &up, std::vector<size_t> &par,
    std::vector<std::pair<size_t, size_t>> &num, std::vector<std::vector<int64_t>> &t,
    std::vector<size_t> &tin, std::vector<size_t> &tout, std::vector<size_t> &next_vertex) {
    size_t w = lca(v, u, up, tin, tout);
    add(v, w, num, par, t, next_vertex, tin, tout, d);
    add(u, w, num, par, t, next_vertex, tin, tout, d);
    update(0, 0, t[num[w].first].size() / 4 - 1, num[w].second, num[w].second, t[num[w].first], d);
}

size_t get_next(size_t v, std::vector<size_t> &next_vertex) {
    if (next_vertex[v] == v) {
        return v;
    } else {
        return next_vertex[v] = get_next(next_vertex[v], next_vertex);
    }
}

int64_t get(size_t v, size_t l, size_t r, size_t pos, std::vector<int64_t> &sum) {
    if (l == r) {
        return sum[v];
    } else {
        size_t mid = (l + r) / 2;
        if (pos <= mid) {
            return sum[v] + get(2 * v + 1, l, mid, pos, sum);
        } else {
            return sum[v] + get(2 * v + 2, mid + 1, r, pos, sum);
        }
    }
}

int64_t
get(size_t v, std::vector<std::pair<size_t, size_t>> &num, std::vector<std::vector<int64_t>> &t) {
    return get(0, 0, t[num[v].first].size() / 4 - 1, num[v].second, t[num[v].first]);
}

int main() {
    std::ios_base::sync_with_stdio(false);
    size_t n;
    std::cin >> n;
    std::vector<std::vector<size_t>> g(n);
    for (size_t i = 1; i < n; i++) {
        size_t from, to;
        std::cin >> from;
        std::cin >> to;
        --from;
        --to;
        g[from].push_back(to);
        g[to].push_back(from);
    }
    std::vector<size_t> cnt(n, 1);
    auto k = static_cast<size_t>(log2(static_cast<double>(n))) + 2;
    std::vector<std::vector<size_t>> up(n, std::vector<size_t>(k));
    std::vector<size_t> par(n);
    std::vector<bool> used(n, false);
    std::queue<size_t> que;
    size_t timer = 0;
    std::vector<size_t> tin(n), tout(n);
    dfs0(0, 0, k, g, cnt, up, par, used, que, tin, tout, timer);
    std::vector<size_t> next_vertex(n);
    while (!que.empty()) {
        auto v = que.front();
        que.pop();
        if (v > 0) {
            if (cnt[v] * 2 > cnt[par[v]]) {
                next_vertex[v] = par[v];
            } else {
                next_vertex[v] = v;
            }
            if (!used[par[v]]) {
                que.push(par[v]);
                used[par[v]] = true;
            }
        } else {
            next_vertex[0] = 0;
        }
    }
    for (size_t i = 0; i < n; i++) {
        get_next(i, next_vertex);
    }
    std::vector<std::pair<size_t, size_t>> num(n);
    std::vector<std::vector<int64_t>> t;
    used.assign(n, false);
    dfs(0, g, num, t, used, par, next_vertex);
    size_t m;
    std::cin >> m;
    while (m-- > 0) {
        char c;
        std::cin >> c;
        if (c == '+') {
            size_t v, u;
            int64_t d;
            std::cin >> v >> u >> d;
            v--;
            u--;
            add(v, u, d, up, par, num, t, tin, tout, next_vertex);
        } else {
            size_t v;
            std::cin >> v;
            v--;
            std::cout << get(v, num, t) << std::endl;
        }
    }
    return 0;
}
