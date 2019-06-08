// I hate this code :(
#include <bits/stdc++.h>

const unsigned int N = 1000005;
const unsigned int M = 200005;

struct query {
    unsigned int l, r;
    unsigned int num;
};

static unsigned int k;
unsigned int mp[N];
unsigned int arr[M];
unsigned long long ans[M];
query req[M];

bool cmp(const query &a, const query &b) {
    if (a.l / k != b.l / k) {
        return a.l < b.l;
    } else {
        return a.r < b.r;
    }
}

int main() {
    unsigned int n, m;
    scanf("%u%u", &n, &m);
    k = static_cast<unsigned int>(sqrt(static_cast<double>(n)));
    for (size_t i = 0; i < n; i++) {
        scanf("%u", &arr[i]);
    }
    for (unsigned int i = 0; i < m; i++) {
        scanf("%u%u", &req[i].l, &req[i].r);
        req[i].l--;
        req[i].r--;
        req[i].num = i;
    }
    std::sort(req, req + m, cmp);
    unsigned long long cur = 0;
    memset(mp, 0, sizeof(mp));
    for (unsigned int i = req[0].l; i <= req[0].r; i++) {
        cur += (2 * mp[arr[i]]++ + 1) * arr[i];
    }
    for (unsigned int i = 0, l = req[0].l, r = req[0].r; i < m; i++) {
        while (l > req[i].l) {
            --l;
            cur += (2 * mp[arr[l]]++ + 1) * arr[l];
        }
        while (l < req[i].l) {
            cur -= (2 * mp[arr[l]]-- - 1) * arr[l];
            l++;
        }
        while (r > req[i].r) {
            cur -= (2 * mp[arr[r]]-- - 1) * arr[r];
            r--;
        }
        while (r < req[i].r) {
            ++r;
            cur += (2 * mp[arr[r]]++ + 1) * arr[r];
        }
        ans[req[i].num] = cur;
    }
    for (size_t i = 0; i < m; i++) {
        printf("%lld\n", ans[i]);
    }
    return 0;
}
