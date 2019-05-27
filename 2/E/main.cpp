#include <iostream>
#include <vector>
#include <cmath>
#include <random>
#include <ctime>

const int32_t MAX_VALUE = 1000 * 1000 * 1000;

static size_t n;
static size_t m;
static std::vector<int32_t> arr;
static std::vector<char> cl;
static std::vector<std::vector<std::pair<size_t, size_t>>> g;
static std::vector<int32_t> value;

static void print_char(unsigned char r) {
    std::cout << std::hex << (r / 16) << (r % 16);
}

static void print_int(int32_t a) {
    for (auto j = 0; j < 4; j++) {
        std::cout << std::hex << ((a & 255) / 16) << ((a & 255) % 16);
        a >>= 8;
    }
}

static void put(unsigned char r, int32_t a) {
    std::cout << "31";
    print_char(r);
    print_int(a);
}

static void mul(unsigned char r1, unsigned char r2, unsigned char r3, unsigned char r4) {
    std::cout << "03";
    print_char(r1);
    print_char(r2);
    print_char(r3);
    print_char(r4);
}

static void mod(unsigned char r1, unsigned char r2, unsigned char r3, unsigned char r4) {
    std::cout << "05";
    print_char(r1);
    print_char(r2);
    print_char(r3);
    print_char(r4);
}

static void add(unsigned char r1, unsigned char r2, unsigned char r3) {
    std::cout << "01";
    print_char(r1);
    print_char(r2);
    print_char(r3);
}

static void load(unsigned char r1, unsigned char r2) {
    std::cout << "30";
    print_char(r1);
    print_char(r2);
}

static void press_f() {
    std::cout << "ff";
}

template<class V, class U>
struct hash_table {
    hash_table() : capacity(1), n(0) {
        values = std::vector<std::vector<std::pair<V, U>>>(1);
    }

    void put_element(V key, U value) {
        if (n > 10 * capacity) {
            expand();
        }
        size_t index = get_hash(key);
        bool find = false;
        for (size_t i = 0; i < values[index].size(); i++) {
            if (values[index][i].first == key) {
                values[index][i].second = value;
                find = true;
                break;
            }
        }
        if (!find) {
            n++;
            values[index].emplace_back(key, value);
        }
    }

    U get_element(V key) {
        size_t index = get_hash(key);
        for (size_t i = 0; i < values[index].size(); i++) {
            if (values[index][i].first == key) {
                return values[index][i].second;
            }
        }
        return false;
    }

 private:
    size_t capacity;
    size_t n;
    std::vector<std::vector<std::pair<V, U>>> values;

    size_t get_hash(std::pair<size_t, size_t> key) {
        return key.first * key.second % capacity;
    }

    void expand() {
        capacity *= 2;
        std::vector<std::vector<std::pair<V, U >>> new_values(capacity);
        for (const auto &i : values) {
            for (auto j : i) {
                size_t index = get_hash(j.first);
                new_values[index].emplace_back(j);
            }
        }
        swap(values, new_values);
    }
};

static bool is_prime(int32_t number) {
    if (number == 1) {
        return false;
    }
    for (auto i = 2; i <= sqrt(number); i++) {
        if (number % i == 0) {
            return false;
        }
    }
    return true;
}

static int32_t get_random(int32_t l, int32_t r, std::mt19937 &rand_number) {
    return static_cast<int32_t>(rand_number() % (r - l) + l);
}

static bool dfs(size_t v, size_t p) {
    cl[v] = 'g';
    for (auto vertex : g[v]) {
        if (vertex.first != p) {
            if (cl[vertex.first] == 'w') {
                if (dfs(vertex.first, v)) {
                    return true;
                }
            } else {
                return true;
            }
        }
    }
    cl[v] = 'b';
    return false;
}

static void dfs_second(size_t v, size_t p) {
    for (auto vertex : g[v]) {
        size_t to = vertex.first;
        if (to == p) {
            continue;
        }
        auto num = static_cast<int32_t>(vertex.second);
        value[to] = num - value[v];
        dfs_second(to, v);
    }
}

static bool check_value(int32_t a1, int32_t a2, int32_t b1, int32_t b2, int32_t p) {
    hash_table<std::pair<size_t, size_t>, bool> mp;
    for (size_t i = 0; i < arr.size(); i++) {
        size_t index1 = ((static_cast<int64_t>(a1) * arr[i] + b1) % p) % m;
        size_t index2 = ((static_cast<int64_t>(a2) * arr[i] + b2) % p) % m;
        if (mp.get_element(std::make_pair(index1, index2))) {
            return false;
        }
        mp.put_element(std::make_pair(index1, index2), true);
        g[index1].push_back(std::make_pair(index2, i));
        g[index2].push_back(std::make_pair(index1, i));
    }
    for (size_t i = 0; i < m; i++) {
        cl[i] = 'w';
    }
    for (size_t i = 0; i < m; i++) {
        if (cl[i] == 'w') {
            if (dfs(i, MAX_VALUE)) {
                return false;
            }
        }
    }
    value.resize(m);
    for (size_t i = 0; i < m; i++) {
        cl[i] = false;
    }
    for (size_t i = 0; i < m; i++) {
        if (!cl[i]) {
            value[i] = 0;
            dfs_second(i, MAX_VALUE);
        }
    }
    put(1, a1);
    put(2, b1);
    put(3, a2);
    put(4, b2);
    put(5, p);
    put(6, static_cast<int32_t>(m));
    put(7, 125);
    put(8, 4);
    mul(0, 1, 9, 10);
    mod(9, 5, 11, 10);
    add(11, 2, 12);
    mod(12, 5, 13, 255);
    mod(13, 6, 14, 255);
    mul(0, 3, 15, 16);
    mod(15, 5, 17, 16);
    add(17, 4, 18);
    mod(18, 5, 19, 255);
    mod(19, 6, 19, 255);
    mul(14, 8, 20, 21);
    add(20, 7, 21);
    load(22, 21);
    mul(19, 8, 23, 24);
    add(23, 7, 24);
    load(25, 24);
    add(22, 25, 0);
    press_f();
    for (size_t i = 0; i < m; i++) {
        print_int(value[i]);
    }
    return true;
}

int main() {
    std::ios_base::sync_with_stdio(false);
    std::cin >> n;
    m = 5 * n;
    cl.resize(m);
    arr.resize(n);
    for (size_t i = 0; i < n; i++) {
        std::cin >> arr[i];
    }
    int32_t a1, b1, a2, b2, p;
    std::mt19937 rand_number(static_cast<unsigned long>(time(nullptr) * time(nullptr)));
    while (true) {
        a1 = get_random(1, MAX_VALUE, rand_number);
        a2 = get_random(1, MAX_VALUE, rand_number);
        b1 = get_random(1, MAX_VALUE, rand_number);
        b2 = get_random(1, MAX_VALUE, rand_number);
        while (true) {
            p = get_random(static_cast<int32_t>(m + 1), MAX_VALUE, rand_number);
            if (is_prime(p)) {
                break;
            }
        }
        g.resize(m);
        if (check_value(a1, a2, b1, b2, p)) {
            break;
        }
        g.clear();
    }
    return 0;
}
