#include <iostream>
#include <vector>

using namespace std;

template<class V, class U>
struct hash_table {
    hash_table() : capacity(1), n(0) {
        values = vector<vector<pair<V, U>>>(1);
    }

    void put_element(V key, U value) {
        n++;
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
            values[index].emplace_back(key, value);
        }
    }

    void delete_element(V key) {
        size_t index = get_hash(key);
        for (size_t i = 0; i < values[index].size(); i++) {
            if (values[index][i].first == key) {
                n--;
                values[index].erase(values[index].begin() + i);
                break;
            }
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
    size_t capacity,
            n;
    vector<vector<pair<V, U>>> values;

    template<typename T>
    size_t get_hash(T key) {
        return 0;
    }

    size_t get_hash(int key) {
        key %= (int) capacity;
        if (key < 0) {
            key += (int) capacity;
        }
        return (size_t) key % capacity;
    }

    void expand() {
        capacity *= 2;
        vector<vector<pair<V, U>>> new_values(capacity);
        for (auto i : values) {
            for (auto j : i) {
                size_t index = get_hash(j.first);
                new_values[index].emplace_back(j);
            }
        }
        swap(values, new_values);
    }
};

hash_table<int, bool> mp;

int main() {
    freopen("set.in", "r", stdin);
    freopen("set.out", "w", stdout);
    ios_base::sync_with_stdio(false);
    string str;
    while (cin >> str) {
        if (str == "insert") {
            int a;
            cin >> a;
            mp.put_element(a, true);
        } else if (str == "exists") {
            int a;
            cin >> a;
            if (mp.get_element(a)) {
                cout << "true" << endl;
            } else {
                cout << "false" << endl;
            }
        } else {
            int a;
            cin >> a;
            mp.delete_element(a);
        }
    }
    return 0;
}
