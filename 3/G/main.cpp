#include <iostream>
#include <vector>

const int64_t MIN = static_cast<int64_t>(-2000000000000000000);
const int64_t MAX = static_cast<int64_t>(2000000000000000000);

struct triple {
    triple() = default;

    triple(int64_t min_value, int64_t common_value, int64_t sum_value) :
            min_value(min_value), common_value(common_value), sum_value(sum_value) {}

    int64_t min_value, common_value, sum_value;
};

void build(size_t v, size_t l, size_t r, std::vector<int64_t> &arr, std::vector<triple> &seg) {
    if (l == r) {
        seg[v] = triple(arr[l], arr[l], 0);
    } else {
        size_t mid = (l + r) / 2;
        build(2 * v + 1, l, mid, arr, seg);
        build(2 * v + 2, mid + 1, r, arr, seg);
        seg[v] = triple(std::min(seg[2 * v + 1].min_value, seg[2 * v + 2].min_value), MIN, 0);
    }
}

int64_t get_min(size_t v, size_t l, size_t r, size_t tl, size_t tr, std::vector<triple> &seg) {
    if (tl > tr) {
        return MAX;
    } else if (l == tl && r == tr) {
        return seg[v].min_value;
    } else {
        if (seg[v].common_value != MIN) {
            return seg[v].common_value;
        } else {
            size_t mid = (l + r) / 2;
            return std::min(get_min(2 * v + 1, l, mid, tl, std::min(tr, mid), seg),
                            get_min(2 * v + 2, mid + 1, r, std::max(tl, mid + 1), tr, seg))
                   + seg[v].sum_value;
        }
    }
}

void push(size_t v, std::vector<triple> &seg) {
    seg[2 * v + 1].min_value = seg[2 * v + 2].min_value =
    seg[2 * v + 1].common_value = seg[2 * v + 2].common_value = seg[v].common_value;
    seg[2 * v + 1].sum_value = seg[2 * v + 2].sum_value = 0;
    seg[v].common_value = MIN;
}

void add(size_t v, size_t l, size_t r, size_t tl, size_t tr, int64_t x, std::vector<triple> &seg) {
    if (tl > tr) {
        return;
    } else if (l == tl && r == tr) {
        if (seg[v].common_value != MIN) {
            seg[v].common_value += x;
        } else {
            seg[v].sum_value += x;
        }
        seg[v].min_value += x;
    } else {
        if (seg[v].common_value != MIN) {
            push(v, seg);
        }
        size_t mid = (l + r) / 2;
        add(2 * v + 1, l, mid, tl, std::min(mid, tr), x, seg);
        add(2 * v + 2, mid + 1, r, std::max(tl, mid + 1), tr, x, seg);
        seg[v].min_value =
                std::min(seg[2 * v + 1].min_value, seg[2 * v + 2].min_value) + seg[v].sum_value;
    }
}

void set(size_t v, size_t l, size_t r, size_t tl, size_t tr, int64_t x, std::vector<triple> &seg) {
    if (tl > tr) {
        return;
    } else if (l == tl && r == tr) {
        seg[v].min_value = seg[v].common_value = x;
        seg[v].sum_value = 0;
    } else {
        if (seg[v].common_value != MIN) {
            push(v, seg);
        }
        size_t mid = (l + r) / 2;
        add(2 * v + 1, l, mid, l, mid, seg[v].sum_value, seg);
        add(2 * v + 2, mid + 1, r, mid + 1, r, seg[v].sum_value, seg);
        seg[v].sum_value = 0;
        set(2 * v + 1, l, mid, tl, std::min(mid, tr), x, seg);
        set(2 * v + 2, mid + 1, r, std::max(mid + 1, tl), tr, x, seg);
        seg[v].min_value = std::min(seg[2 * v + 1].min_value, seg[2 * v + 2].min_value);
    }
}

int main() {
    std::ios_base::sync_with_stdio(false);
    size_t n;
    std::cin >> n;
    std::vector<int64_t> arr(n);
    for (size_t i = 0; i < n; i++) {
        std::cin >> arr[i];
    }
    std::vector<triple> seg(4 * n);
    build(0, 0, n - 1, arr, seg);
    std::string query;
    while (std::cin >> query) {
        if (query == "min") {
            size_t l, r;
            std::cin >> l >> r;
            std::cout << get_min(0, 0, n - 1, l - 1, r - 1, seg) << std::endl;
        } else if (query == "add") {
            size_t i, j;
            int64_t x;
            std::cin >> i >> j >> x;
            add(0, 0, n - 1, i - 1, j - 1, x, seg);
        } else {
            size_t i, j;
            int64_t x;
            std::cin >> i >> j >> x;
            set(0, 0, n - 1, i - 1, j - 1, x, seg);
        }
    }
    return 0;
}
