#include <iostream>
#include <vector>

using namespace std;

template<class V, class U>
struct hash_table {
    hash_table() : capacity(1), n(0), num(0), cur_number(0) {
        values = vector<vector<Position>>(1);
        numbers = vector<V>(1);
    }

    void put_element(V key, U value) {
        if (n > 10 * capacity) {
            expand();
        }
        size_t index = get_hash(key);
        bool find = false;
        for (size_t i = 0; i < values[index].size(); i++) {
            if (values[index][i].key == key) {
                values[index][i].value = value;
                find = true;
                break;
            }
        }
        if (!find) {
            n++;
            num++;
            numbers.push_back(key);
            Position &_prev = get(cur_number);
            if (_prev.number != 0) {
                _prev.next = num;
            }
            values[index].emplace_back(Position(key, value, num, cur_number, 0));
            cur_number = num;
        }
    }

    void delete_element(V key) {
        size_t index = get_hash(key);
        for (size_t i = 0; i < values[index].size(); i++) {
            if (values[index][i].key == key) {
                n--;
                Position &_prev = get(values[index][i].prev);
                Position &_next = get(values[index][i].next);
                if (_prev.number != 0 && _next.number == 0) {
                    _prev.next = 0;
                    cur_number = _prev.number;
                } else if (_prev.number == 0 && _next.number != 0) {
                    _next.prev = 0;
                } else if (_prev.number != 0 && _next.number != 0) {
                    _prev.next = _next.number;
                    _next.prev = _prev.number;
                } else {
                    cur_number = 0;
                }
                values[index].erase(values[index].begin() + i);
                break;
            }
        }
    }

    U get_element(V key) {
        size_t index = get_hash(key);
        for (size_t i = 0; i < values[index].size(); i++) {
            if (values[index][i].key == key) {
                return values[index][i].value;
            }
        }
        return "none";
    }

    U next_element(V key) {
        size_t index = get_hash(key);
        for (size_t i = 0; i < values[index].size(); i++) {
            if (values[index][i].key == key) {
                Position &_next = get(values[index][i].next);
                if (_next.number != 0) {
                    return _next.value;
                }
                break;
            }
        }
        return "none";
    }

    U prev_element(V key) {
        size_t index = get_hash(key);
        for (size_t i = 0; i < values[index].size(); i++) {
            if (values[index][i].key == key) {
                Position &_prev = get(values[index][i].prev);
                if (_prev.number != 0) {
                    return _prev.value;
                }
                break;
            }
        }
        return "none";
    }

 private:
    struct Position {
        Position() : number(0) {}

        Position(V key, U value, size_t number, size_t prev, size_t next) :
                key(std::move(key)),
                value(std::move(value)),
                number(number),
                prev(prev), next(next) {}

        V key;
        U value;
        size_t number, prev, next;
    };

    Position fake;
    size_t capacity,
            n,
            num,
            cur_number;
    vector<vector<Position>> values;
    vector<V> numbers;

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
        vector<vector<Position>> new_values(capacity);
        for (const auto &i : values) {
            for (auto j : i) {
                size_t index = get_hash(j.key);
                new_values[index].emplace_back(j);
            }
        }
        swap(values, new_values);
    }

    Position &get(size_t number) {
        if (number == 0) {
            return fake;
        } else {
            V key = numbers[number];
            size_t index = get_hash(key);
            for (size_t i = 0; i < values[index].size(); i++) {
                if (values[index][i].key == key) {
                    return values[index][i];
                }
            }
            return fake;
        }
    }
};

hash_table<string, string> mp;

int main() {
    ios_base::sync_with_stdio(false);
    freopen("linkedmap.in", "r", stdin);
    freopen("linkedmap.out", "w", stdout);
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
        } else if (str == "delete") {
            string key;
            cin >> key;
            mp.delete_element(key);
        } else if (str == "next") {
            string key;
            cin >> key;
            cout << mp.next_element(key) << endl;
        } else {
            string key;
            cin >> key;
            cout << mp.prev_element(key) << endl;
        }
    }
    return 0;
}
