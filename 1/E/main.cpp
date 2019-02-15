#include <bits/stdc++.h>

using namespace std;

const int N = 30000;

struct node {
    int prior, num;
    node *l, *r, *p;

    node(int prior, int num) {
        this->prior = prior;
        this->num = num;
        this->l = nullptr;
        this->r = nullptr;
        this->p = nullptr;
    }
};

typedef node *pnode;

pnode root = nullptr,
        cur = nullptr,
        it;

struct vertex {
    int a, b, num;
};

struct answer {
    int p, l, r;
};

int n;
vector<vertex> arr;
vector<answer> ans;

void insert() {
    if (it->prior < cur->prior) {
        if (cur->p == nullptr) {
            it->r = cur;
            cur->p = it;
            root = it;
        } else {
            cur = cur->p;
            insert();
        }
    } else {
        it->r = cur->l;
        if (it->r != nullptr) {
            it->r->p = it;
        }
        cur->l = it;
        it->p = cur;
    }
}

void dfs(pnode root) {
    if (root->l != nullptr) {
        dfs(root->l);
        ans[root->num].l = root->l->num + 1;
        ans[root->l->num].p = root->num + 1;
    } else {
        ans[root->num].l = 0;
    }
    if (root->r != nullptr) {
        dfs(root->r);
        ans[root->num].r = root->r->num + 1;
        ans[root->r->num].p = root->num + 1;
    }
}

bool cmp(const vertex &a, const vertex &b) {
    return a.a > b.a;
}

int main() {
    srand(static_cast<unsigned int>(time(nullptr)));
    ios_base::sync_with_stdio(false);
    cin >> n;
    arr.resize(n);
    for (int i = 0; i < n; i++) {
        cin >> arr[i].a >> arr[i].b;
        arr[i].num = i;
    }
    sort(arr.begin(), arr.end(), cmp);
    for (int i = 0; i < n; i++) {
        it = new node(arr[i].b, arr[i].num);
        if (root == nullptr) {
            root = it;
        } else {
            insert();
        }
        cur = it;
    }
    cout << "YES" << endl;
    ans.resize(n);
    dfs(root);
    for (int i = 0; i < n; i++) {
        cout << ans[i].p << " " << ans[i].l << " " << ans[i].r << endl;
    }
    return 0;
}
