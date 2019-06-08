#include <iostream>
#include <vector>
#include <math.h>

uint32_t get_ans(size_t l, size_t r, std::vector<std::vector<uint32_t> > &dp,
                 std::vector<uint32_t> &p, std::vector<uint32_t> &o) {
    if (l > r) {
        std::swap(l, r);
    }
    uint32_t k = p[r - l + 1],
            j = o[k];
    return std::min(dp[l][k], dp[r - j + 1][k]);
}

int main() {
    std::ios_base::sync_with_stdio(false);
    size_t n, m;
    std::cin >> n >> m;
    std::vector<uint32_t> arr(n);
    std::cin >> arr[0];
    for (size_t i = 1; i < n; i++) {
        arr[i] = (arr[i - 1] * 23 + 21563) % 16714589;
    }
    size_t N = static_cast<size_t>(log2(n)) + 5;
    std::vector<uint32_t> p(n + 1),
            o(N);
    p[1] = 0;
    o[0] = 1;
    for (size_t i = 1; i < N; i++) {
        o[i] = o[i - 1] * 2;
    }
    for (size_t i = 2; i <= n; i++) {
        p[i] = p[i / 2] + 1;
    }
    std::vector<std::vector<uint32_t> > dp(n, std::vector<uint32_t>(N));
    for (size_t i = 0; i < n; i++) {
        dp[i][0] = arr[i];
    }
    for (size_t j = 1, k = 1; j < N; j++, k *= 2) {
        for (size_t i = 0; i + k < n; i++) {
            dp[i][j] = std::min(dp[i][j - 1], dp[i + k][j - 1]);
        }
    }
    size_t u, v, ans;
    std::cin >> u >> v;
    ans = get_ans(u - 1, v - 1, dp, p, o);
    for (size_t i = 2; i <= m; i++) {
        size_t new_u = ((17 * u + 751 + ans + 2 * (i - 1)) % n) + 1,
                new_v = ((13 * v + 593 + ans + 5 * (i - 1)) % n) + 1;
        u = new_u;
        v = new_v;
        ans = get_ans(u - 1, v - 1, dp, p, o);
    }
    std::cout << u << " " << v << " " << ans << std::endl;
    return 0;
}
