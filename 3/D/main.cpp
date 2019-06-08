#include <iostream>
#include <vector>

void build(size_t n, std::vector<int32_t> &arr, std::vector<int32_t> &segment) {
    for (size_t i = 0; i < n; i++) {
        segment[n - 1 + i] = arr[i];
    }
    for (size_t i = n - 1; i > 0;) {
        i--;
        segment[i] = std::min(segment[2 * i + 1], segment[2 * i + 2]);
    }
}

int32_t get(std::vector<int32_t> &segment, size_t n, size_t l, size_t r) {
    int32_t left_res = 1000000000,
        right_res = 1000000000;
    l += n - 1;
    r += n - 1;
    while (l < r) {
        if (l % 2 == 0) {
            left_res = std::min(left_res, segment[l]);
        }
        l = (l / 2);
        if (r % 2 == 1) {
            right_res = std::min(right_res, segment[r]);
        }
        r = (r / 2) - 1;
    }
    if (l == r) {
        left_res = std::min(left_res, segment[l]);
    }
    return std::min(left_res, right_res);
}

void set(std::vector<int32_t> &segment, size_t n, size_t i, int32_t x) {
    i += n - 1;
    segment[i] = x;
    if (i == 0) {
        return;
    }
    i = (i - 1) / 2;
    while (i > 0) {
        segment[i] = std::min(segment[2 * i + 1], segment[2 * i + 2]);
        i = (i - 1) / 2;
    }
    segment[0] = std::min(segment[1], segment[2]);
}

int main() {
    std::ios_base::sync_with_stdio(false);
    size_t n;
    std::cin >> n;
    std::vector<int32_t> arr(n);
    for (size_t i = 0; i < n; i++) {
        std::cin >> arr[i];
    }
    std::vector<int32_t> segment(2 * n);
    build(n, arr, segment);
    std::string query;
    while (std::cin >> query) {
        if (query == "min") {
            size_t l, r;
            std::cin >> l >> r;
            std::cout << get(segment, n, l - 1, r - 1) << std::endl;
        } else {
            size_t i;
            int32_t x;
            std::cin >> i >> x;
            set(segment, n, i - 1, x);
        }
    }
    return 0;
}
