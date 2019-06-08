#include <iostream>
#include <vector>

const size_t N = 131072;
uint32_t a, b, cur = 0;

uint32_t nextRand17() {
    cur = cur * a + b;
    return cur >> 15;
}

uint32_t nextRand24() {
    cur = cur * a + b;
    return cur >> 8;
}

void build(size_t v, size_t l, size_t r, std::vector<uint32_t> &f,
           std::vector<std::vector<uint32_t>> &g) {
    if (l == r) {
        g[v] = {f[l]};
    } else {
        size_t mid = (l + r) / 2;
        build(2 * v + 1, l, mid, f, g);
        build(2 * v + 2, mid + 1, r, f, g);
        g[v].resize(g[2 * v + 1].size() + g[2 * v + 2].size());
        for (size_t i = 0, j = 0, k = 0;
             j < g[2 * v + 1].size() || k < g[2 * v + 2].size(); i++) {
            if (j == g[2 * v + 1].size()) {
                g[v][i] = g[2 * v + 2][k++];
            } else if (k == g[2 * v + 2].size()) {
                g[v][i] = g[2 * v + 1][j++];
            } else if (g[2 * v + 1][j] < g[2 * v + 2][k]) {
                g[v][i] = g[2 * v + 1][j++];
            } else {
                g[v][i] = g[2 * v + 2][k++];
            }
        }
    }
}

uint32_t get(size_t v, size_t l, size_t r, size_t tl, size_t tr, uint32_t x, uint32_t y,
             std::vector<std::vector<uint32_t>> &g) {
    if (tl > tr) {
        return 0;
    } else if (tl == l && tr == r) {
        if (g[v].back() < x || g[v][0] > y) {
            return 0;
        } else {
            int32_t l1 = -1,
                    r1 = static_cast<int32_t>(g[v].size()),
                    l2 = -1,
                    r2 = static_cast<int32_t>(g[v].size());
            while (l1 + 1 < r1) {
                auto mid = (r1 + l1) / 2;
                if (g[v][mid] < x) {
                    l1 = mid;
                } else {
                    r1 = mid;
                }
            }
            while (l2 + 1 < r2) {
                auto mid = (r2 + l2) / 2;
                if (g[v][mid] > y) {
                    r2 = mid;
                } else {
                    l2 = mid;
                }
            }
            return l2 - r1 + 1;
        }
    } else {
        size_t mid = (l + r) / 2;
        return get(2 * v + 1, l, mid, tl, std::min(mid, tr), x, y, g) +
               get(2 * v + 2, mid + 1, r, std::max(mid + 1, tl), tr, x, y, g);
    }
}

int main() {
    std::ios_base::sync_with_stdio(false);
    size_t q;
    std::cin >> q;
    std::cin >> a >> b;
    std::vector<uint32_t> f(N);
    for (size_t i = 0; i < N; i++) {
        f[i] = nextRand24();
    }
    std::vector<std::vector<uint32_t>> g(4 * N);
    build(0, 0, N - 1, f, g);
    uint32_t sum = 0;
    for (size_t i = 0; i < q; i++) {
        size_t l = nextRand17(),
                r = nextRand17();
        if (l > r) {
            std::swap(l, r);
        }
        uint32_t x = nextRand24(),
                y = nextRand24();
        if (x > y) {
            std::swap(x, y);
        }
        uint32_t c = get(0, 0, N - 1, l, r, x, y, g);
        sum += c;
        b += c;
    }
    std::cout << sum << std::endl;
    return 0;
}
