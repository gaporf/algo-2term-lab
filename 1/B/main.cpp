#include <bits/stdc++.h>

using namespace std;

const int NONE = static_cast<int> (-2e9);

struct node {
    node *parent;
    vector<node *> son;
    vector<int> key;
    int n, maxValue;
};

typedef node *pnode;

pnode root = nullptr;
pnode a, b, c, d, e, f, t;

int strToNum(string str) {
    reverse(str.begin(), str.end());
    int ans = 0;
    for (int i = 0, j = 1; str[i] && str[i] != '-'; i++, j *= 10) {
        ans += (str[i] - '0') * j;
    }
    if (str[str.length() - 1] == '-') {
        ans *= -1;
    }
    return ans;
}

int getNum(int len, string req) {
    string str;
    for (int i = len; req[i]; i++) {
        str += req[i];
    }
    return strToNum(str);
}

pnode search(int x) {
    t = root;
    while (t != nullptr && t->n > 0) {
        if (t->n == 2) {
            if (t->key[0] < x) {
                t = t->son[1];
            } else {
                t = t->son[0];
            }
        } else {
            if (t->key[1] < x) {
                t = t->son[2];
            } else if (t->key[0] < x) {
                t = t->son[1];
            } else {
                t = t->son[0];
            }
        }
    }
    return t;
}

void updateKeys() {
    a = t->parent;
    while (a != nullptr) {
        for (int i = 0; i < a->n - 1; i++) {
            a->key[i] = a->son[i]->maxValue;
        }
        a->maxValue = a->son.back()->maxValue;
        a = a->parent;
    }
}

void init(pnode &t) {
    t = new node();
    t->parent = nullptr;
    while (!t->son.empty()) {
        t->son.pop_back();
    }
    while (!t->key.empty()) {
        t->key.pop_back();
    }
    t->n = 0;
    t->maxValue = 0;
}

void split() {
    if (t->n > 3) {
        init(a);
        a->son.push_back(t->son[2]);
        a->son.push_back(t->son[3]);
        a->key.push_back(t->key[2]);
        a->son[0]->parent = a;
        a->son[1]->parent = a;
        t->n = 2;
        a->n = 2;
        t->son.pop_back();
        t->son.pop_back();
        t->key.pop_back();
        t->key.pop_back();
        a->parent = t->parent;
        a->maxValue = a->son[1]->maxValue;
        t->maxValue = t->son[1]->maxValue;
        if (t->parent != nullptr) {
            t->parent->son.push_back(a);
            t->parent->key.push_back(t->son[0]->maxValue);
            t->parent->n++;
            for (int i = t->parent->n - 1; i > 0; i--) {
                if (t->parent->son[i]->maxValue < t->parent->son[i - 1]->maxValue) {
                    swap(t->parent->son[i], t->parent->son[i - 1]);
                }
            }
            for (int i = 0; i < t->parent->n - 1; i++) {
                t->parent->key[i] = t->parent->son[i]->maxValue;
            }
            t->parent->maxValue = t->parent->son.back()->maxValue;
            t = t->parent;
            split();
        } else {
            init(b);
            b->son.push_back(t);
            b->son.push_back(a);
            b->key.push_back(t->key[0]);
            t->parent = b;
            a->parent = b;
            b->n = 2;
            root = b;
            if (b->son[0]->key[0] > b->son[1]->key[0]) {
                swap(b->son[0], b->son[1]);
            }
            b->key[0] = b->son[0]->maxValue;
            b->maxValue = b->son[1]->maxValue;
            root = b;
        }
    }
}

void insert(int x) {
    if (root == nullptr) {
        init(root);
        root->maxValue = x;
        return;
    }
    pnode a = search(x);
    if (a != nullptr && a->maxValue == x) {
        return;
    }
    init(b);
    b->maxValue = x;
    if (a->parent == nullptr) {
        init(c);
        c->son.push_back(a);
        c->son.push_back(b);
        c->key.push_back(a->maxValue);
        a->parent = c;
        b->parent = c;
        c->n = 2;
        if (c->son[0]->maxValue > c->son[1]->maxValue) {
            swap(c->son[0], c->son[1]);
        }
        c->key[0] = c->son[0]->maxValue;
        c->maxValue = c->son[1]->maxValue;
        root = c;
    } else {
        c = a->parent;
        c->son.push_back(b);
        c->n++;
        b->parent = c;
        c->key.push_back(c->son[0]->maxValue);
        for (int i = c->n - 1; i > 0; i--) {
            if (c->son[i]->maxValue < c->son[i - 1]->maxValue) {
                swap(c->son[i], c->son[i - 1]);
            }
        }
        t = b;
        updateKeys();
        t = b->parent;
        split();
    }
    t = b;
    updateKeys();
}

void recover() {
    if (t->parent == nullptr) {
        return;
    }
    a = t->parent;
    if (a->n != 1) {
        return;
    }
    if (a->parent == nullptr) {
        root = t;
        root->parent = nullptr;
        return;
    }
    b = a->parent;
    for (int i = 0; i < b->n; i++) {
        if (b->son[i] == a) {
            if (i + 1 < b->n) {
                c = b->son[i + 1];
            } else {
                c = b->son[i - 1];
            }
        }
    }
    if (c->n == 2) {
        c->son.push_back(t);
        t->parent = c;
        c->n = 3;
        c->key.push_back(c->son[0]->maxValue);
        for (int i = c->n - 1; i > 0; i--) {
            if (c->son[i]->maxValue < c->son[i - 1]->maxValue) {
                swap(c->son[i], c->son[i - 1]);
            }
        }
        for (int i = 0; i < c->n - 1; i++) {
            c->key[i] = c->son[i]->maxValue;
        }
        c->maxValue = c->son[2]->maxValue;
        for (int i = 0; i < b->n; i++) {
            if (b->son[i] == a) {
                for (int j = i + 1; j < c->n; j++) {
                    b->son[j - 1] = b->son[j];
                }
                break;
            }
        }
        b->son.pop_back();
        b->key.pop_back();
        for (int i = 0; i < b->n - 1; i++) {
            b->key[i] = b->son[i]->maxValue;
        }
        b->maxValue = b->son.back()->maxValue;
        b->n--;
        t = c;
        recover();
    } else {
        if (c->maxValue > t->maxValue) {
            a->son.push_back(c->son[0]);
            a->son[1]->parent = a;
            a->key.push_back(a->son[0]->maxValue);
            a->maxValue = a->son[1]->maxValue;
            c->son[0] = c->son[1];
            c->son[1] = c->son[2];
            c->son.pop_back();
            c->key.pop_back();
            c->key[0] = c->son[0]->maxValue;
            c->maxValue = c->son[1]->maxValue;
            c->n = 2;
            a->n = 2;
        } else {
            a->son.push_back(c->son[2]);
            swap(a->son[0], a->son[1]);
            a->son[0]->parent = a;
            a->key.push_back(a->son[0]->maxValue);
            a->maxValue = a->son[1]->maxValue;
            c->son.pop_back();
            c->key.pop_back();
            c->maxValue = c->son[1]->maxValue;
            c->n = 2;
            a->n = 2;
        }
    }
}

void remove() {
    if (t->parent == nullptr) {
        root = nullptr;
        return;
    }
    a = t->parent;
    if (a->n == 3) {
        for (int i = 0; i < 3; i++) {
            if (a->son[i] == t) {
                for (int j = i + 1; j < 3; j++) {
                    a->son[j - 1] = a->son[j];
                }
            }
        }
        a->son.pop_back();
        a->key.pop_back();
        a->key[0] = a->son[0]->maxValue;
        a->maxValue = a->son[1]->maxValue;
        a->n = 2;
        t = a;
        updateKeys();
    } else {
        if (a->son[0] == t) {
            a->son[0] = a->son[1];
        }
        a->son.pop_back();
        a->key.pop_back();
        d = a->son[0];
        if (a->parent == nullptr) {
            root = d;
            root->parent = nullptr;
        } else {
            c = a->parent;
            for (int i = 0; i < c->n; i++) {
                if (c->son[i] == a) {
                    if (i + 1 < c->n) {
                        e = c->son[i + 1];
                    } else {
                        e = c->son[i - 1];
                    }
                }
            }
            if (e->n == 2) {
                e->son.push_back(d);
                d->parent = e;
                e->n = 3;
                e->key.push_back(e->son[0]->maxValue);
                for (int i = e->n - 1; i > 0; i--) {
                    if (e->son[i]->maxValue < e->son[i - 1]->maxValue) {
                        swap(e->son[i], e->son[i - 1]);
                    }
                }
                for (int i = 0; i < e->n - 1; i++) {
                    e->key[i] = e->son[i]->maxValue;
                }
                e->maxValue = e->son[2]->maxValue;
                for (int i = 0; i < c->n; i++) {
                    if (c->son[i] == a) {
                        for (int j = i + 1; j < c->n; j++) {
                            c->son[j - 1] = c->son[j];
                        }
                        break;
                    }
                }
                c->son.pop_back();
                c->key.pop_back();
                for (int i = 0; i < c->n - 1; i++) {
                    c->key[i] = c->son[i]->maxValue;
                }
                c->maxValue = c->son.back()->maxValue;
                c->n--;
                t = e;
                recover();
            } else {
                if (e->maxValue > d->maxValue) {
                    a->son.push_back(e->son[0]);
                    a->son[1]->parent = a;
                    a->key.push_back(a->son[0]->maxValue);
                    a->maxValue = a->son[1]->maxValue;
                    e->son[0] = e->son[1];
                    e->son[1] = e->son[2];
                    e->son.pop_back();
                    e->key.pop_back();
                    e->key[0] = e->son[0]->maxValue;
                    e->maxValue = e->son[1]->maxValue;
                    e->n = 2;
                    a->n = 2;
                } else {
                    a->son.push_back(e->son[2]);
                    swap(a->son[0], a->son[1]);
                    a->son[0]->parent = a;
                    a->key.push_back(a->son[0]->maxValue);
                    a->maxValue = a->son[1]->maxValue;
                    e->son.pop_back();
                    e->key.pop_back();
                    e->maxValue = e->son[1]->maxValue;
                    e->n = 2;
                    a->n = 2;
                }
            }
        }
        t = d;
        updateKeys();
    }
}

int next(int x) {
    a = search(x);
    if (a == nullptr) {
        return NONE;
    }
    if (a->maxValue > x) {
        return a->maxValue;
    }
    while (a != nullptr) {
        b = a;
        a = a->parent;
        if (a == nullptr) {
            break;
        }
        for (int i = 0; i + 1 < a->n; i++) {
            if (a->son[i] == b) {
                b = a->son[i + 1];
                while (b->n > 0) {
                    b = b->son[0];
                }
                return b->maxValue;
            }
        }
    }
    return NONE;
}

int prev(int x) {
    a = search(x);
    if (a == nullptr) {
        return NONE;
    }
    if (a->maxValue < x) {
        return a->maxValue;
    }
    while (a != nullptr) {
        b = a;
        a = a->parent;
        if (a == nullptr) {
            break;
        }
        for (int i = a->n - 1; i > 0; i--) {
            if (a->son[i] == b) {
                b = a->son[i - 1];
                while (b->n > 0) {
                    b = b->son.back();
                }
                return b->maxValue;
            }
        }
    }
    return NONE;
}

int main() {
    ios_base::sync_with_stdio(false);
    string req;
    while (getline(cin, req)) {
        if (req[0] == 'e') {
            int u = getNum(7, req);
            pnode ans = search(u);
            if (ans != nullptr && ans->maxValue == u) {
                cout << "true" << endl;
            } else {
                cout << "false" << endl;
            }
        } else if (req[0] == 'i') {
            int u = getNum(7, req);
            insert(u);
        } else if (req[0] == 'd') {
            int u = getNum(7, req);
            t = search(u);
            if (t != nullptr && t->maxValue == u) {
                remove();
            }
        } else if (req[0] == 'n') {
            int u = getNum(5, req);
            u = next(u);
            if (u == NONE) {
                cout << "none" << endl;
            } else {
                cout << u << endl;
            }
        } else if (req[0] == 'p') {
            int u = getNum(5, req);
            u = prev(u);
            if (u == NONE) {
                cout << "none" << endl;
            } else {
                cout << u << endl;
            }
        }
    }
    return 0;
}
