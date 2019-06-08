#include <iostream>
#include <math.h>
#include <vector>

int32_t b[4][128][8][128][8];

int64_t
get(size_t num, size_t l, size_t r, size_t L, size_t R, bool c, std::vector<size_t> &p,
    std::vector<size_t> &o) {
    if (l > r) {
        std::swap(l, r);
    }
    if (L > R) {
        std::swap(L, R);
    }
    size_t k1 = p[r - l + 1],
            k2 = p[R - L + 1],
            j1 = o[k1],
            j2 = o[k2];
    if (!c) {
        return std::max(
                std::max(
                        b[num][l][k1][L][k2],
                        b[num][l][k1][R - j2 + 1][k2]),
                std::max(
                        b[num][r - j1 + 1][k1][L][k2],
                        b[num][r - j1 + 1][k1][R - j2 + 1][k2]));
    } else {
        return std::min(
                std::min(
                        b[num][l][k1][L][k2],
                        b[num][l][k1][R - j2 + 1][k2]),
                std::min(
                        b[num][r - j1 + 1][k1][L][k2],
                        b[num][r - j1 + 1][k1][R - j2 + 1][k2]));
    }
}

void
handle(std::vector<std::vector<int32_t>> &a, size_t num, size_t N, size_t M, bool c) {
    auto n = static_cast<size_t>(log2(N)) + 1,
            m = static_cast<size_t>(log2(M)) + 1;
    for (size_t i = 0; i < N; i++) {
        for (size_t j = 0; j < M; j++) {
            b[num][i][0][j][0] = a[i][j];
        }
    }
    for (size_t k = 1, J = 1; k < std::max(n, m); k++, J *= 2) {
        for (size_t i = 0; i + J < N; i++) {
            for (size_t j = 0; j < M; j++) {
                if (!c) {
                    b[num][i][k][j][0] =
                            std::max(b[num][i][k - 1][j][0], b[num][i + J][k - 1][j][0]);
                } else {
                    b[num][i][k][j][0] =
                            std::min(b[num][i][k - 1][j][0], b[num][i + J][k - 1][j][0]);
                }
            }
        }
        for (size_t j = 0; j + J < M; j++) {
            for (size_t i = 0; i < N; i++) {
                if (!c) {
                    b[num][i][0][j][k] =
                            std::max(b[num][i][0][j][k - 1], b[num][i][0][j + J][k - 1]);
                } else {
                    b[num][i][0][j][k] =
                            std::min(b[num][i][0][j][k - 1], b[num][i][0][j + J][k - 1]);
                }
            }
        }
    }
    for (size_t k1 = 1, j1 = 1; k1 < n; k1++, j1 *= 2) {
        for (size_t k2 = 1, j2 = 1; k2 < m; k2++, j2 *= 2) {
            for (size_t i = 0; i + j1 < N; i++) {
                for (size_t j = 0; j + j2 < M; j++) {
                    if (!c) {
                        b[num][i][k1][j][k2] = std::max(
                                std::max(
                                        b[num][i][k1 - 1][j][k2 - 1],
                                        b[num][i][k1 - 1][j + j2][k2 - 1]),
                                std::max(
                                        b[num][i + j1][k1 - 1][j][k2 - 1],
                                        b[num][i + j1][k1 - 1][j + j2][k2 - 1]));
                    } else {
                        b[num][i][k1][j][k2] = std::min(
                                std::min(
                                        b[num][i][k1 - 1][j][k2 - 1],
                                        b[num][i][k1 - 1][j + j2][k2 - 1]),
                                std::min(
                                        b[num][i + j1][k1 - 1][j][k2 - 1],
                                        b[num][i + j1][k1 - 1][j + j2][k2 - 1]));
                    }
                }
            }
        }
    }
}

int main() {
    std::ios_base::sync_with_stdio(false);
    size_t N, M;
    std::cin >> N >> M;
    std::vector<std::vector<int32_t> >
            a1(N, std::vector<int32_t>(M)),
            a2(N, std::vector<int32_t>(M)),
            a3(N, std::vector<int32_t>(M)),
            a4(N, std::vector<int32_t>(M));
    for (size_t i = 0; i < N; i++) {
        for (size_t j = 0; j < M; j++) {
            int32_t x1, y1, x2, y2;
            std::cin >> x1 >> y1 >> x2 >> y2;
            if (x1 > x2) {
                std::swap(x1, x2);
            }
            if (y1 > y2) {
                std::swap(y1, y2);
            }
            a1[i][j] = x1;
            a2[i][j] = x2;
            a3[i][j] = y1;
            a4[i][j] = y2;
        }
    }
    handle(a1, 0, N, M, false);
    handle(a2, 1, N, M, true);
    handle(a3, 2, N, M, false);
    handle(a4, 3, N, M, true);
    size_t Q;
    std::cin >> Q;
    int64_t A, B, u;
    std::cin >> A >> B >> u;
    int64_t const MOD = 1000000007;
    std::vector<size_t> p(128),
            o(8);
    p[1] = 0;
    o[0] = 1;
    for (size_t i = 1; i < 8; i++) {
        o[i] = o[i - 1] * 2;
    }
    for (size_t i = 2; i <= 127; i++) {
        p[i] = p[i / 2] + 1;
    }
    int64_t ans = 0;
    for (size_t i = 0; i < Q; i++) {
        int64_t u1 = (A * u + B) % MOD,
                u2 = (A * u1 + B) % MOD,
                u3 = (A * u2 + B) % MOD,
                u4 = (A * u3 + B) % MOD;
        int64_t x1 = get(0, u1 % N, u3 % N, u2 % M, u4 % M, false, p, o),
                x2 = get(1, u1 % N, u3 % N, u2 % M, u4 % M, true, p, o),
                y1 = get(2, u1 % N, u3 % N, u2 % M, u4 % M, false, p, o),
                y2 = get(3, u1 % N, u3 % N, u2 % M, u4 % M, true, p, o);
        u = u4;
        if (x1 <= x2 && y1 <= y2) {
            ans = ((x2 - x1) * (y2 - y1) + ans) % MOD;
        }
    }
    std::cout << ans << std::endl;
    return 0;
}
