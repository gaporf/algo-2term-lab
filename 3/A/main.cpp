#include <iostream>
#include <cstring>

const unsigned int N = 16777217;

unsigned int arr[N];

int main() {
    memset(arr, 0, sizeof(arr));
    unsigned int m, q;
    unsigned int a, b, cur;
    scanf("%d%d%d%d", &m, &q, &a, &b);
    cur = b;
    for (unsigned int i = 0; i < m; i++) {
        unsigned int add, l, r;
        add = cur >> 8;
        cur = cur * a + b;
        l = (cur >> 8) + 1;
        cur = cur * a + b;
        r = (cur >> 8) + 1;
        cur = cur * a + b;
        if (l > r) {
            arr[r] += add;
            arr[l + 1] -= add;
        } else {
            arr[l] += add;
            arr[r + 1] -= add;
        }
    }
    unsigned int j = 0;
    for (unsigned int i = 1; i < N; i++) {
        j += arr[i];
        arr[i] = j + arr[i - 1];
    }
    unsigned int ans = 0;
    for (unsigned int i = 0; i < q; i++) {
        unsigned int l, r;
        l = (cur >> 8) + 1;
        cur = cur * a + b;
        r = (cur >> 8) + 1;
        cur = cur * a + b;
        if (l > r) {
            ans += arr[l] - arr[r - 1];
        } else {
            ans += arr[r] - arr[l - 1];
        }
    }
    printf("%u", ans);
    return 0;
}
