#include <bits/stdc++.h>

using namespace std;

struct node {
    int num, value, prior;
    node *l, *r;

    node(int value, int prior) {
        this->value = value;
        this->prior = prior;
        this->l = nullptr;
        this->r = nullptr;
        this->num = 1;
    }
};

typedef node *ptr;

ptr root = nullptr;

int cnt(ptr it) {
    return (it) ? it->num : 0;
}

void update(ptr it) {
    if (it) {
        it->num = cnt(it->l) + 1 + cnt(it->r);
    }
}

void split(ptr cur, int value, ptr &l, ptr &r) {
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

void insert(ptr it, ptr &cur) {
    if (cur == nullptr) {
        cur = it;
    } else if (it->prior > cur->prior) {
        split(cur, it->value, it->l, it->r);
        cur = it;
    } else if (it->value < cur->value) {
        insert(it, cur->l);
    } else {
        insert(it, cur->r);
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

void erase(ptr &cur, int value) {
    if (cur->value == value) {
        merge(cur, cur->l, cur->r);
    } else if (cur->value > value) {
        erase(cur->l, value);
    } else {
        erase(cur->r, value);
    }
    update(cur);
}

int find(int value, ptr cur) {
    if (value <= cnt(cur->l)) {
        return find(value, cur->l);
    } else {
        value -= cnt(cur->l);
        if (value == 1) {
            return cur->value;
        } else {
            return find(value - 1, cur->r);
        }
    }
}

int main() {
    ios_base::sync_with_stdio(false);
    srand(static_cast<unsigned int>(time(nullptr)));
    int n;
    cin >> n;
    int amount = 0;
    while (n--) {
        string str;
        int u;
        cin >> str >> u;
        if (str == "+1" || str == "1") {  // Why, mr Nigmatullin?
            auto it = new node(u, rand());
            insert(it, root);
            amount++;
        } else if (str == "0") {
            cout << find(amount - u + 1, root) << endl;
        } else if (str == "-1") {
            erase(root, u);
            amount--;
        }
    }
    return 0;
}
