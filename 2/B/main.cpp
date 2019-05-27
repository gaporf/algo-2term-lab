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
        return "none";
    }

 private:
    size_t capacity,
            n;
    vector<vector<pair<V, U>>> values;

    template<typename T>
    size_t get_hash(T key) {
        return 0;
    }

    size_t get_hash(string const &key) {
        unsigned int ans = 0,
                p = 1;
        for (auto ch : key) {
            ans += p * (ch - 'a' + 1);
            p *= 53;
        }
        return ans % capacity;
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

hash_table<string, string> mp;

int main() {
    freopen("map.in", "r", stdin);
    freopen("map.out", "w", stdout);
    ios_base::sync_with_stdio(false);
    string str;
    while (cin >> str) {
        if (str == "put") {
            string key, value;
            cin >> key >> value;
            mp.put_element(key, value);
        } else if (str == "get") {
            string key;
            cin >> key;
            cout << mp.get_element(key) << endl;
        } else {
            string key;
            cin >> key;
            mp.delete_element(key);
        }
    }
    return 0;
}
