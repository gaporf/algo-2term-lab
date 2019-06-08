#include <iostream>
#include <vector>

void build(size_t v, size_t l, size_t r, std::vector<std::vector<size_t>> &arr, std::string &line) {
    if (l == r) {
        for (size_t i = 0; i < 4; i++) {
            arr[i][v] = i;
        }
    } else {
        size_t mid = (l + r) / 2;
        build(2 * v + 1, l, mid, arr, line);
        build(2 * v + 2, mid + 1, r, arr, line);
        for (size_t i = 0; i < 4; i++) {
            size_t u = arr[i][2 * v + 1];
            if (u == 0) {
                if (line[mid] == '/' && line[mid + 1] == '*') {
                    arr[i][v] = arr[2][2 * v + 2];
                } else {
                    arr[i][v] = arr[0][2 * v + 2];
                }
            } else if (u == 1) {
                if (line[mid] == '*' && line[mid + 1] == '/') {
                    arr[i][v] = arr[3][2 * v + 2];
                } else {
                    arr[i][v] = arr[1][2 * v + 2];
                }
            } else if (u == 2) {
                arr[i][v] = arr[1][2 * v + 2];
            } else if (u == 3) {
                arr[i][v] = arr[0][2 * v + 2];
            }
        }
    }
}

size_t get(size_t v, size_t l, size_t r, size_t pos, std::vector<std::vector<size_t>> &arr,
           std::string &line, int8_t cond = 0) {
    if (l == r) {
        if (cond != 0 || (l + 1 < line.length() && line[l] == '/' && line[l + 1] == '*')) {
            return 1;
        } else {
            return 0;
        }
    } else {
        size_t mid = (l + r) / 2;
        if (pos <= mid) {
            return get(2 * v + 1, l, mid, pos, arr, line, cond);
        } else {
            size_t u = arr[cond][2 * v + 1];
            if (u == 0) {
                if (line[mid] == '/' && line[mid + 1] == '*') {
                    return get(2 * v + 2, mid + 1, r, pos, arr, line, 2);
                } else {
                    return get(2 * v + 2, mid + 1, r, pos, arr, line, 0);
                }
            } else if (u == 1) {
                if (line[mid] == '*' && line[mid + 1] == '/') {
                    return get(2 * v + 2, mid + 1, r, pos, arr, line, 3);
                } else {
                    return get(2 * v + 2, mid + 1, r, pos, arr, line, 1);
                }
            } else if (u == 2) {
                return get(2 * v + 2, mid + 1, r, pos, arr, line, 1);
            } else {
                return get(2 * v + 2, mid + 1, r, pos, arr, line, 0);
            }
        }
    }
}

void update(size_t v, size_t l, size_t r, size_t pos,
            std::vector<std::vector<size_t>> &arr, std::string &line) {
    if (l != r) {
        size_t mid = (l + r) / 2;
        if (pos <= mid) {
            update(2 * v + 1, l, mid, pos, arr, line);
        } else {
            update(2 * v + 2, mid + 1, r, pos, arr, line);
        }
        for (size_t i = 0; i < 4; i++) {
            size_t u = arr[i][2 * v + 1];
            if (u == 0) {
                if (line[mid] == '/' && line[mid + 1] == '*') {
                    arr[i][v] = arr[2][2 * v + 2];
                } else {
                    arr[i][v] = arr[0][2 * v + 2];
                }
            } else if (u == 1) {
                if (line[mid] == '*' && line[mid + 1] == '/') {
                    arr[i][v] = arr[3][2 * v + 2];
                } else {
                    arr[i][v] = arr[1][2 * v + 2];
                }
            } else if (u == 2) {
                arr[i][v] = arr[1][2 * v + 2];
            } else if (u == 3) {
                arr[i][v] = arr[0][2 * v + 2];
            }
        }
    }
}

int main() {
    std::ios_base::sync_with_stdio(false);
    size_t n;
    std::cin >> n;
    std::string line;
    std::cin >> line;
    std::vector<std::vector<size_t>> arr(4, std::vector<size_t>(4 * n));
    build(0, 0, n - 1, arr, line);
    size_t m;
    std::cin >> m;
    while (m-- > 0) {
        int8_t type;
        std::cin >> type;
        if (type == '1') {
            size_t u;
            char c;
            std::cin >> u >> c;
            u--;
            line[u] = c;
            update(0, 0, n - 1, u, arr, line);
        } else {
            size_t u;
            std::cin >> u;
            std::cout << (get(0, 0, n - 1, u - 1, arr, line) != 0 ? "Yes\n" : "No\n");
        }
    }
    return 0;
}
