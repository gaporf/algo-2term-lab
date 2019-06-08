#include <iostream>
#include <vector>
#include <cstring>

const int32_t ADD = 1000000;
const size_t MAX = 2000001;
std::vector<std::pair<size_t, size_t>> add[MAX];
std::vector<std::pair<size_t, size_t>> del[MAX];
std::pair<size_t, size_t> sum[4 * MAX];

void build(size_t v, size_t l, size_t r) {
    if (l != r) {
        size_t mid = (l + r) / 2;
        build(2 * v + 1, l, mid);
        build(2 * v + 2, mid + 1, r);
    }
    sum[v] = std::make_pair(0, 0);
}

void update(size_t v, size_t l, size_t r, size_t tl, size_t tr, size_t delta) {
    if (tl > tr) {
        return;
    } else if (tl == l && tr == r) {
        sum[v].first += delta;
        sum[v].second += delta;
    } else {
        size_t mid = (l + r) / 2;
        update(2 * v + 1, l, mid, tl, std::min(mid, tr), delta);
        update(2 * v + 2, mid + 1, r, std::max(mid + 1, tl), tr, delta);
        sum[v].second = std::max(sum[2 * v + 1].second, sum[2 * v + 2].second) + sum[v].first;
    }
}

size_t get(size_t v, size_t l, size_t r, size_t tl, size_t tr, size_t add = 0) {
    if (tl > tr) {
        return 0;
    } else if (tl == l && tr == r) {
        return sum[v].second + add;
    } else {
        size_t mid = (l + r) / 2;
        return std::max(get(2 * v + 1, l, mid, tl, std::min(tr, mid), add + sum[v].first),
                        get(2 * v + 2, mid + 1, r, std::max(mid + 1, tl), tr, add + sum[v].first));
    }
}

int main() {
    build(0, 0, MAX - 1);
    std::ios_base::sync_with_stdio(false);
    memset(sum, 0, sizeof(sum));
    size_t n;
    std::cin >> n;
    for (size_t i = 0; i < n; i++) {
        int32_t x1, y1, x2, y2;
        std::cin >> x1 >> y1 >> x2 >> y2;
        x1 += ADD;
        y1 += ADD;
        x2 += ADD;
        y2 += ADD;
        add[x1].emplace_back(y1, y2);
        del[x2].emplace_back(y1, y2);
    }
    size_t ans_max = 0;
    for (size_t i = 0; i < MAX; i++) {
        for (size_t j = 0; j < add[i].size(); j++) {
            update(0, 0, MAX - 1, add[i][j].first, add[i][j].second, 1);
        }
        ans_max = std::max(ans_max, get(0, 0, MAX - 1, 0, MAX - 1));
        for (size_t j = 0; j < del[i].size(); j++) {
            update(0, 0, MAX - 1, del[i][j].first, del[i][j].second, -1);
        }
    }
    std::cout << ans_max << std::endl;
    build(0, 0, MAX - 1);
    for (size_t i = 0; i < MAX; i++) {
        for (size_t j = 0; j < add[i].size(); j++) {
            update(0, 0, MAX - 1, add[i][j].first, add[i][j].second, 1);
        }
        if (get(0, 0, MAX - 1, 0, MAX - 1) == ans_max) {
            for (size_t j = 0; j < MAX; j++) {
                if (ans_max == get(0, 0, MAX - 1, j, j)) {
                    std::cout <<
                              static_cast<int32_t>(i) - ADD << " " <<
                              static_cast<int32_t>(j) - ADD << std::endl;
                    return 0;
                }
            }
        }
        for (size_t j = 0; j < del[i].size(); j++) {
            update(0, 0, MAX - 1, del[i][j].first, del[i][j].second, -1);
        }
    }
    return 0;
}
