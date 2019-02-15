#include <bits/stdc++.h>

using namespace std;

typedef long long ll;

const ll MOD = static_cast<ll> (1e9);

struct node {
    ll value, sum;
    int prior;
    node *l, *r;

    node(ll value, int prior) {
        this->value = this->sum = value;
        this->prior = prior;
        this->l = this->r = nullptr;
    }
};

typedef node *ptr;

ptr root = nullptr;

ll sum(ptr it) {
    return (it != nullptr) ? it->sum : 0;
}

void update(ptr &it) {
    if (it) {
        it->sum = sum(it->l) + it->value + sum(it->r);
    }
}

void split(ptr cur, ll value, ptr &l, ptr &r) {
    if (cur == nullptr) {
        l = r = nullptr;
    } else if (cur->value > value) {
        split(cur->l, value, l, cur->l);
        r = cur;
    } else {
        split(cur->r, value, cur->r, r);
        l = cur;
    }
    update(cur);
}

bool exists(ptr cur, ll value) {
    if (cur == nullptr) {
        return false;
    } else if (cur->value > value) {
        return exists(cur->l, value);
    } else if (cur->value == value) {
        return true;
    } else {
        return exists(cur->r, value);
    }
}

void insert(ptr &cur, ptr it) {
    if (cur == nullptr) {
        cur = it;
    } else if (it->prior > cur->prior) {
        split(cur, it->value, it->l, it->r);
        cur = it;
    } else if (cur->value > it->value) {
        insert(cur->l, it);
    } else if (cur->value < it->value) {
        insert(cur->r, it);
    }
    update(cur);
}

void merge(ptr &cur, ptr l, ptr r) {
    if (l == nullptr) {
        cur = r;
    } else if (r == nullptr) {
        cur = l;
    } else if (l->prior > r->prior) {
        merge(l->r, l->r, r);
        cur = l;
    } else {
        merge(r->l, l, r->l);
        cur = r;
    }
    update(cur);
}

ll getSum(ll value) {
    ptr t1, t2;
    split(root, value, t1, t2);
    ll ans = sum(t1);
    merge(root, t1, t2);
    return ans;
}

int main() {
    ios_base::sync_with_stdio(false);
    srand(static_cast<unsigned int>(time(nullptr)));
    int n;
    cin >> n;
    ll last = -1;
    while (n--) {
        string str;
        cin >> str;
        if (str == "+") {
            ll num;
            cin >> num;
            if (last != -1) {
                num = (num + last) % MOD;
            }
            if (!exists(root, num)) {
                ptr it = new node(num, rand());
                insert(root, it);
            }
            last = -1;
        } else {
            ll l, r;
            cin >> l >> r;
            last = getSum(r) - getSum(l - 1);
            cout << last << endl;
        }
    }
    return 0;
}
