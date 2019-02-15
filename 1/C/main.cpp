#include <bits/stdc++.h>

using namespace std;

struct node {
    int value, prior;
    node *l, *r;
    int num;

    node(int value, int prior) {
        this->value = value;
        this->prior = prior;
        this->l = nullptr;
        this->r = nullptr;
        this->num = 1;
    }
};

typedef node *pnode;

int n;
pnode root = nullptr;

int cnt(pnode it) {
    return it ? it->num : 0;
}

void update(pnode it) {
    if (it) {
        it->num = cnt(it->l) + cnt(it->r) + 1;
    }
}

void split(pnode cur, int value, pnode &l, pnode &r) {
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

void divide(pnode cur, int value, pnode &l, pnode &r) {
    if (cur == nullptr) {
        l = r = nullptr;
    } else {
        if (cnt(cur->l) >= value) {
            divide(cur->l, value, l, cur->l);
            r = cur;
        } else {
            divide(cur->r, value - cnt(cur->l) - 1, cur->r, r);
            l = cur;
        }
    }
    update(cur);
}

void insert(pnode it, pnode &cur) {
    if (cur == nullptr) {
        cur = it;
    } else if (cur->prior < it->prior) {
        split(cur, it->value, it->l, it->r);
        cur = it;
    } else if (cur->value > it->value) {
        insert(it, cur->l);
    } else {
        insert(it, cur->r);
    }
    update(cur);
}

void merge(pnode &cur, pnode l, pnode r) {
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

void init() {
    for (int i = 1; i <= n; i++) {
        auto it = new node(i, rand());
        insert(it, root);
    }
}

void gfs(pnode it) {
    if (it) {
        gfs(it->l);
        cout << it->value << " ";
        gfs(it->r);
    }
}

int main() {
    ios_base::sync_with_stdio(false);
    srand(static_cast<unsigned int>(time(nullptr)));
    int m;
    cin >> n >> m;
    init();
    while (m--) {
        int l, r;
        cin >> l >> r;
        pnode t1, t2, t3, t4;
        divide(root, l - 1, t1, t2);
        divide(t2, r - l + 1, t3, t4);
        merge(root, t3, t1);
        merge(root, root, t4);
    }
    gfs(root);
    return 0;
}
