#include <iostream>
#include <vector>

void build(size_t v, size_t l, size_t r, std::vector<size_t> &seg) {
    if (l == r) {
        seg[v] = l;
    } else {
        size_t mid = (l + r) / 2;
        build(2 * v + 1, l, mid, seg);
        build(2 * v + 2, mid + 1, r, seg);
        seg[v] = r;
    }
}

size_t get(size_t v, size_t l, size_t r, size_t x, std::vector<size_t> &seg) {
    if (l == r) {
        return l;
    } else {
        size_t mid = (l + r) / 2;
        if (mid + 1 <= x) {
            return get(2 * v + 2, mid + 1, r, x, seg);
        } else {
            if (seg[2 * v + 1] <= mid && seg[2 * v + 1] >= x) {
                return get(2 * v + 1, l, mid, x, seg);
            } else {
                return get(2 * v + 2, mid + 1, r, x, seg);
            }
        }
    }
}

void in(size_t v, size_t l, size_t r, size_t x, std::vector<size_t> &seg) {
    if (l == r) {
        seg[v] = l + 1;
    } else {
        size_t mid = (l + r) / 2;
        if (x <= mid) {
            in(2 * v + 1, l, mid, x, seg);
        } else {
            in(2 * v + 2, mid + 1, r, x, seg);
        }
        if (seg[2 * v + 2] <= r) {
            seg[v] = seg[2 * v + 2];
        } else if (seg[2 * v + 1] <= mid) {
            seg[v] = seg[2 * v + 1];
        } else {
            seg[v] = r + 1;
        }
    }
}

void out(size_t v, size_t l, size_t r, size_t x, std::vector<size_t> &seg) {
    if (l == r) {
        seg[v] = l;
    } else {
        size_t mid = (l + r) / 2;
        if (x <= mid) {
            out(2 * v + 1, l, mid, x, seg);
        } else {
            out(2 * v + 2, mid + 1, r, x, seg);
        }
        if (seg[2 * v + 2] <= r) {
            seg[v] = seg[2 * v + 2];
        } else if (seg[2 * v + 1] <= mid) {
            seg[v] = seg[2 * v + 1];
        } else {
            seg[v] = r + 1;
        }
    }
}

int main() {
    std::ios_base::sync_with_stdio(false);
    size_t n, m;
    std::cin >> n >> m;
    n++;
    std::vector<size_t> seg(4 * n);
    build(0, 0, n - 1, seg);
    for (size_t i = 0; i < m; i++) {
        std::string query;
        size_t x;
        std::cin >> query >> x;
        if (query == "enter") {
            size_t place = get(0, 0, n - 1, x - 1, seg);
            if (place == n - 1) {
                place = get(0, 0, n - 1, 0, seg);
            }
            in(0, 0, n - 1, place, seg);
            std::cout << place + 1 << std::endl;
        } else {
            out(0, 0, n - 1, x - 1, seg);
        }
    }
    return 0;
}
