#include <iostream>
#include <string>
#include <vector>

const size_t N = 500000;
const size_t Q = 20;
const size_t R = 524288;
size_t p[N], h[N];
size_t up[N][Q];

void add(size_t a, size_t b) {
    p[b] = a;
    h[b] = h[a] + 1;
    up[b][0] = a;
    for (size_t j = 1, res = 2; j < Q; j++, res *= 2) {
        up[b][j] = up[up[b][j - 1]][j - 1];
    }
}

size_t lca(size_t a, size_t b) {
    if (h[a] < h[b]) {
        std::swap(a, b);
    }
    for (size_t i = Q - 1, res = R; res > 0; i--, res /= 2) {
        if (h[a] - h[b] >= res) {
            a = up[a][i];
        }
    }
    if (a == b) {
        return a;
    }
    for (size_t i = Q - 1; i < Q; i--) {
        if (up[a][i] != up[b][i]) {
            a = up[a][i];
            b = up[b][i];
        }
    }
    return p[a];
}

int main() {
    size_t k;
    scanf("%zd", &k);
    p[0] = -1;
    h[0] = 0;
    for (size_t i = 0; i < Q; i++) {
        up[i][0] = 0;
    }
    for (size_t i = 0; i < k; i++) {
        char query[4];
        size_t a, b;
        scanf("%s%zd%zd", query, &a, &b);
        --a;
        --b;
        if (query[0] == 'A') {
            add(a, b);
        } else {
            printf("%zd\n", lca(a, b) + 1);
        }
    }
    return 0;
}
