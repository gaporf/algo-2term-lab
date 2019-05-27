#include <iostream>
#include <vector>
#include <queue>

static size_t n;
static std::vector<std::vector<size_t> > g;
static size_t head;
static std::vector<size_t> cnt;
static std::vector<size_t> size;
static std::vector<size_t> children;
static std::vector<size_t> type;
static std::vector<size_t> p;
static std::queue<size_t> q;

void end() {
    std::cout << "NO" << std::endl;
    exit(0);
}

void first_dfs(size_t v, size_t u = n) {
    for (auto vertex : g[v]) {
        if (vertex != u) {
            first_dfs(vertex, v);
            cnt[v] += cnt[vertex];
        }
    }
}

void second_dfs(size_t v, size_t u = n) {
    for (auto vertex : g[v]) {
        if (vertex != u) {
            second_dfs(vertex, v);
        }
    }
    if (g[v].size() == 2) {
        if (u == n) {
            if (cnt[g[v][0]] == cnt[g[v][1]]) {
                head = v;
                return;
            }
        } else {
            if (cnt[v] == n / 2 + n % 2) {
                head = v;
                return;
            }
        }
    }
}

void third_dfs(size_t v, size_t u = n) {
    size[v] = 1;
    p[v] = u;
    for (auto vertex : g[v]) {
        if (vertex != u) {
            third_dfs(vertex, v);
            children[v]++;
            size[v] += size[vertex];
        }
    }
    if (children[v] == 0) {
        q.push(v);
        type[v] = 1;
    }
    cnt[v] = children[v];
}

void get_type() {
    while (!q.empty()) {
        size_t v = q.front();
        size_t par = p[v];
        if (par != n) {
            cnt[par]--;
            if (cnt[par] == 0) {
                size_t u = q.back();
                if (children[par] != children[u] || size[par] != size[u]) {
                    type[par] = type[u] + 1;
                } else {
                    type[par] = type[u];
                }
                q.push(par);
            }
        }
        q.pop();
    }
}

int main() {
    std::ios_base::sync_with_stdio(false);
    std::cin >> n;
    if (n % 2 == 0) {
        end();
    }
    g.resize(n);
    for (size_t i = 0; i + 1 != n; i++) {
        size_t from, to;
        std::cin >> from >> to;
        from--;
        to--;
        g[from].push_back(to);
        g[to].push_back(from);
    }
    cnt.resize(n, 1);
    head = n;
    first_dfs(0);
    second_dfs(0);
    if (head == n) {
        end();
    }
    type.resize(n);
    p.resize(n);
    size.resize(n);
    children.resize(n, 0);
    third_dfs(head);
    get_type();
    if (type[g[head][0]] == type[g[head][1]]) {
        std::cout << "YES" << std::endl;
    } else {
        std::cout << "NO" << std::endl;
    }
    return 0;
}
