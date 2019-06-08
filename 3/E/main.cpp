#include <iostream>
#include <vector>

void build(std::vector<int64_t> &sum, std::vector<int64_t> &arr, size_t n) {
    for (size_t i = 0; i < n; i++) {
        sum[n - 1 + i] = arr[i];
    }
    for (size_t i = n - 1; i > 0;) {
        i--;
        sum[i] = sum[2 * i + 1] + sum[2 * i + 2];
    }
}

int64_t get_sum(std::vector<int64_t> &sum, size_t n, size_t l, size_t r) {
    int64_t left_res = 0,
        right_res = 0;
    l += n - 1;
    r += n - 1;
    while (l < r) {
        if (l % 2 == 0) {
            left_res += sum[l];
        }
        l = (l / 2);
        if (r % 2 == 1) {
            right_res += sum[r];
        }
        r = (r / 2) - 1;
    }
    if (l == r) {
        left_res += sum[l];
    }
    return left_res + right_res;
}

void set(std::vector<int64_t> &sum, size_t n, size_t i, int64_t x) {
    i += n - 1;
    sum[i] = x;
    if (i == 0) {
        return;
    }
    i = (i - 1) / 2;
    while (i > 0) {
        sum[i] = sum[2 * i + 1] + sum[2 * i + 2];
        i = (i - 1) / 2;
    }
    sum[0] = sum[1] + sum[2];
}

int main() {
    std::ios_base::sync_with_stdio(false);
    size_t n;
    std::cin >> n;
    std::vector<int64_t> arr(n);
    for (size_t i = 0; i < n; i++) {
        std::cin >> arr[i];
    }
    std::vector<int64_t> sum(2 * n);
    build(sum, arr, n);
    std::string query;
    while (std::cin >> query) {
        if (query == "sum") {
            size_t l, r;
            std::cin >> l >> r;
            std::cout << get_sum(sum, n, l - 1, r - 1) << std::endl;
        } else {
            size_t i;
            int64_t x;
            std::cin >> i >> x;
            set(sum, n, i - 1, x);
        }
    }
    return 0;
}
