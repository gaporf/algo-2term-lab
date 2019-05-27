#include <iostream>
#include <vector>
#include <ctime>

using namespace std;

template<class V, class U>
struct Hash_table {
    explicit Hash_table() : _capacity(1), _n(0),
                            _a(static_cast<size_t>(rand())), _b(static_cast<size_t>(rand())),
                            _p(406586897), values(vector<vector<pair<V, U> > >(1)) {}


    void put_element(V key, U value) {
        if (_n > 10 * _capacity) {
            expand();
        }
        size_t index = get_hash(key);
        bool find = false;
        for (size_t i = 0; i < values[index].size(); i++) {
            if (values[index][i].first == key) {
                find = true;
                values[index][i].second = value;
                break;
            }
        }
        if (!find) {
            _n++;
            values[index].push_back(make_pair(key, value));
        }
    }

    bool exist_element(V key) {
        size_t index = get_hash(key);
        for (size_t i = 0; i < values[index].size(); i++) {
            if (values[index][i].first == key) {
                return true;
            }
        }
        return false;
    }

    void delete_element(V key) {
        size_t index = get_hash(key);
        for (size_t i = 0; i < values[index].size(); i++) {
            if (values[index][i].first == key) {
                _n--;
                values[index].erase(values[index].begin() + i);
                break;
            }
        }
    }

    U &get_element(V key) {
        size_t index = get_hash(key);
        for (size_t i = 1; i < values[index].size(); i++) {
            if (values[index][i].first == key) {
                return values[index][i].second;
            }
        }
        return values[index][0].second;
    }

    vector<V> get_keys() {
        vector<V> ans;
        for (size_t i = 0; i < values.size(); i++) {
            for (size_t j = 0; j < values[i].size(); j++) {
                ans.push_back(values[i][j].first);
            }
        }
        return ans;
    }

 private:
    size_t _capacity, _n, _a, _b, _p;
    vector<vector<pair<V, U> > > values;

    void expand() {
        _capacity *= 2;
        vector<vector<pair<V, U> > > new_values(_capacity);
        for (auto i : values) {
            for (auto j : i) {
                size_t index = get_hash(j.first);
                new_values[index].emplace_back(j);
            }
        }
        swap(values, new_values);
    }

    size_t get_hash(const string &str) {
        size_t hash = 0;
        size_t p = 1;
        for (auto ch : str) {
            hash += (ch - 'a' + 1) * p;
            p *= 53;
        }
        return ((hash * _a + _b) % _p) % _capacity;
    }
};

Hash_table<string, Hash_table<string, bool> > mp;

int main() {
    srand(static_cast<unsigned int>(time(nullptr)));
    freopen("multimap.in", "r", stdin);
    freopen("multimap.out", "w", stdout);
    ios_base::sync_with_stdio(false);

    string str;
    while (cin >> str) {
        if (str == "put") {
            string key, value;
            cin >> key >> value;
            if (!mp.exist_element(key)) {
                mp.put_element(key, Hash_table<string, bool>());
            }
            mp.get_element(key).put_element(value, true);
        } else if (str == "get") {
            string key;
            cin >> key;
            if (mp.exist_element(key)) {
                vector<string> values = mp.get_element(key).get_keys();
                cout << values.size() << " ";
                for (const auto &value : values) {
                    cout << value << " ";
                }
                cout << endl;
            } else {
                cout << 0 << endl;
            }
        } else if (str == "delete") {
            string key, value;
            cin >> key >> value;
            if (mp.exist_element(key)) {
                mp.get_element(key).delete_element(value);
            }
        } else if (str == "deleteall") {
            string key;
            cin >> key;
            mp.delete_element(key);
        }
    }
    return 0;
}
