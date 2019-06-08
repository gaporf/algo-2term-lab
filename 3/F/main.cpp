#include <iostream>
#include <vector>
#include <math.h>

int32_t R;
size_t n, m;
int32_t arr[200000][2][2],
        mul[400000][2][2];

void build() {
    for (size_t i = 0; i < n; i++) {
        for (size_t j = 0; j < 2; j++) {
            for (size_t k = 0; k < 2; k++) {
                mul[n - 1 + i][j][k] = arr[i][j][k];
            }
        }
    }
    for (size_t i = n - 1; i > 0;) {
        i--;
        for (size_t j = 0; j < 2; j++) {
            for (size_t k = 0; k < 2; k++) {
                mul[i][j][k] = 0;
            }
        }
        for (size_t j = 0; j < 2; j++) {
            for (size_t k = 0; k < 2; k++) {
                for (size_t l = 0; l < 2; l++) {
                    mul[i][j][k] =
                            (mul[i][j][k] + (mul[2 * i + 1][j][l] * mul[2 * i + 2][l][k])) % R;
                }
            }
        }
    }
}

void get_ans(size_t l, size_t r) {
    l += n - 1;
    r += n - 1;
    int32_t left_res[2][2] = {1, 0, 0, 1},
            right_res[2][2] = {1, 0, 0, 1};
    while (l < r) {
        if (l % 2 == 0) {
            int32_t cur[2][2] = {0, 0, 0, 0};
            for (size_t i = 0; i < 2; i++) {
                for (size_t j = 0; j < 2; j++) {
                    for (size_t k = 0; k < 2; k++) {
                        cur[i][j] = (cur[i][j] + left_res[i][k] * mul[l][k][j]) % R;
                    }
                }
            }
            for (size_t i = 0; i < 2; i++) {
                for (size_t j = 0; j < 2; j++) {
                    left_res[i][j] = cur[i][j];
                }
            }
        }
        l = (l / 2);
        if (r % 2 == 1) {
            int32_t cur[2][2] = {0, 0, 0, 0};
            for (size_t i = 0; i < 2; i++) {
                for (size_t j = 0; j < 2; j++) {
                    for (size_t k = 0; k < 2; k++) {
                        cur[i][j] = (cur[i][j] + mul[r][i][k] * right_res[k][j]) % R;
                    }
                }
            }
            for (size_t i = 0; i < 2; i++) {
                for (size_t j = 0; j < 2; j++) {
                    right_res[i][j] = cur[i][j];
                }
            }
        }
        r = (r / 2) - 1;
    }
    if (l == r) {
        int32_t cur[2][2] = {0, 0, 0, 0};
        for (size_t i = 0; i < 2; i++) {
            for (size_t j = 0; j < 2; j++) {
                for (size_t k = 0; k < 2; k++) {
                    cur[i][j] = (cur[i][j] + left_res[i][k] * mul[l][k][j]) % R;
                }
            }
        }
        for (size_t i = 0; i < 2; i++) {
            for (size_t j = 0; j < 2; j++) {
                left_res[i][j] = cur[i][j];
            }
        }
    }
    int32_t cur[2][2] = {0, 0, 0, 0};
    for (size_t i = 0; i < 2; i++) {
        for (size_t j = 0; j < 2; j++) {
            for (size_t k = 0; k < 2; k++) {
                cur[i][j] = (cur[i][j] + left_res[i][k] * right_res[k][j]) % R;
            }
        }
    }
    for (size_t i = 0; i < 2; i++) {
        for (size_t j = 0; j < 2; j++) {
            left_res[i][j] = cur[i][j];
        }
    }
    printf("%d %d\n%d %d\n\n", left_res[0][0], left_res[0][1], left_res[1][0], left_res[1][1]);
}

int main() {
    scanf("%d%zd%zd", &R, &n, &m);
    for (size_t i = 0; i != n; i++) {
        for (size_t j = 0; j < 2; j++) {
            for (size_t k = 0; k < 2; k++) {
                scanf("%d", &arr[i][j][k]);
                arr[i][j][k] %= R;
            }
        }
    }
    build();
    for (size_t i = 0; i < m; i++) {
        size_t l, r;
        scanf("%zd%zd", &l, &r);
        get_ans(l - 1, r - 1);
    }
    return 0;
}
