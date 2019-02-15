#include <bits/stdc++.h>

using namespace std;

struct node {
    int place, prior, num, real;
    node *l, *r;

    node(int place, int real, int prior) {
        this->place = place;
        this->real = real;
        this->prior = prior;
        this->num = 1;
        this->l = nullptr;
        this->r = nullptr;
    }
};

typedef node *ptr;

ptr root = nullptr,
        anotherRoot = nullptr;
vector<int> values;
int ans = 0;

int cnt(ptr it) {
    return (it != nullptr) ? it->num : 0;
}

void update(ptr it) {
    if (it) {
        it->num = cnt(it->l) + 1 + cnt(it->r);
    }
}

void split(ptr cur, int place, ptr &l, ptr &r) {
    if (cur == nullptr) {
        l = r = nullptr;
    } else if (cur->place > place) {
        split(cur->l, place, l, cur->l);
        r = cur;
    } else {
        split(cur->r, place, cur->r, r);
        l = cur;
    }
    update(cur);
}

void divide(ptr cur, int value, ptr &l, ptr &r) {
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

void insert(ptr it, ptr &cur) {
    if (cur == nullptr) {
        cur = it;
    } else if (it->prior > cur->prior) {
        split(cur, it->place, it->l, it->r);
        cur = it;
    } else if (it->place < cur->place) {
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

void init(int n, int m) {
    for (int i = 1; i <= n + m + 1; i++) {
        ptr item = new node(i, 0, rand());
        insert(item, root);
        item = new node(i, 0, rand());
        insert(item, anotherRoot);
    }
}

int getNext(ptr cur, int value) {
    if (cur == nullptr) {
        return -1;
    } else if (cur->place == value) {
        return cur->place;
    } else if (cur->place < value) {
        return getNext(cur->r, value);
    } else {
        int u = getNext(cur->l, value);
        if (u != -1) {
            return u;
        } else {
            return cur->place;
        }
    }
}

void erase(ptr &cur, int place) {
    if (cur->place == place) {
        merge(cur, cur->l, cur->r);
    } else if (cur->place > place) {
        erase(cur->l, place);
    } else {
        erase(cur->r, place);
    }
}

void dfs(ptr cur) {
    if (cur == nullptr) {
        return;
    }
    dfs(cur->l);
    values.push_back(cur->real);
    if (values.back() != 0) {
        ans = static_cast<int>(values.size());
    }
    dfs(cur->r);
}

int main() {
    srand(static_cast<unsigned int>(time(nullptr)));
    int n, m;
    cin >> n >> m;
    init(n, m);
    for (int i = 1; i <= n; i++) {
        int pos;
        cin >> pos;
        int u = getNext(anotherRoot, pos);
        erase(anotherRoot, u);
        ptr item = new node(pos, i, rand());
        if (u == pos) {
            ptr t1, t2, t3, t4;
            divide(root, pos - 1, t1, t2);
            divide(t2, 1, t3, t4);
            merge(root, t1, item);
            merge(root, root, t4);
        } else {
            ptr t1, t2, t3, t4, t5, t6;
            divide(root, pos - 1, t1, t2);
            divide(t2, u - pos, t3, t4);
            divide(t4, 1, t5, t6);
            merge(root, t1, item);
            merge(root, root, t3);
            merge(root, root, t6);
        }
    }
    dfs(root);
    cout << ans << endl;
    for (int i = 0; i < ans; i++) {
        cout << values[i] << " ";
    }
    cout << endl;
    return 0;
}
