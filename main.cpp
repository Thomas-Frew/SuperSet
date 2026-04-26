#include <bits/stdc++.h>
#include <ext/pb_ds/assoc_container.hpp>
#include <ext/pb_ds/tree_policy.hpp>

using namespace __gnu_pbds;
using namespace std;

typedef long long ll;

template<typename K, typename C>
struct pairCmp {
    bool operator()(const pair<K,ll>& a, const pair<K,ll>& b) const {
        if (C()(a.first, b.first)) return true;
        if (C()(b.first, a.first)) return false;
        return a.second < b.second;
    }
};

template<typename K, typename V, typename C>
using osTree = tree<K, V, C, rb_tree_tag, tree_order_statistics_node_update>;

template<typename K, typename C = less<K>>
using osSet = osTree<K, null_type, C>;

template<typename K, typename C = less<K>>
using osMultiSet = osTree<pair<K,ll>, null_type, pairCmp<K,C>>;

ll FIRST_TIME = -1;
ll LAST_TIME = 1e9;

template <typename K, typename C = less<K>>
class SuperSet {
    ll t = 0;

    osMultiSet<K,C> data;
    osSet<K,C> keys;
    unordered_map<K,ll> keyFreq;

public:

    bool insert(K key) {
        bool existed = exists(key);

        data.insert({key, t++});
        keys.insert(key);
        keyFreq[key]++;

        return existed;
    }

    bool erase(K key) {
        if (!exists(key)) return false;

        auto it = data.lower_bound({key, FIRST_TIME});
        data.erase(it);

        if (--keyFreq[key] == 0) {
            keyFreq.erase(key);
            keys.erase(key);
        }

        return true;
    }

    bool eraseAll(K key) {
        if (!exists(key)) return false;

        auto it = data.lower_bound({key, FIRST_TIME});
        auto end = data.upper_bound({key, LAST_TIME});

        while (it != end) {
            it = data.erase(it);
        }

        keyFreq.erase(key);
        keys.erase(key);

        return true;
    }

    bool clear() {
        if (!keys.size()) return false;

        data.clear();
        keys.clear();
        keyFreq.clear();
        return true;
    }

    ll size() {
        return data.size();
    }

    ll uniqueSize() {
        return keys.size();
    }

    bool exists(K key) {
        return keyFreq.find(key) != keyFreq.end();
    }

    ll count(K key) {
        auto it = keyFreq.find(key);
        return it == keyFreq.end() ? 0 : it->second;
    }

    optional<K> first() {
        if (data.empty()) return nullopt;
        return data.begin()->first;
    }

    optional<K> last() {
        if (data.empty()) return nullopt;
        return prev(data.end())->first;
    }

    string print() {
        string result;
        for (auto &k : data) {
            result += to_string(k.first);
            result += " ";
        }
        return result;
    }

    ll less(K key) {
        return data.order_of_key({key, FIRST_TIME});
    }

    ll uniqueLess(K key) {
        return keys.order_of_key(key);
    }

    ll more(K key) {
        return data.size() - data.order_of_key({key, LAST_TIME});
    }

    ll uniqueMore(K key) {
        return keys.size() - (keys.order_of_key(key) + exists(key));
    }

    ll countBetween(K l, K r) {
        if (C()(r, l)) return 0;

        ll left = data.order_of_key({l, FIRST_TIME});
        ll right = data.order_of_key({r, LAST_TIME});

        return right - left;
    }

    ll uniqueCountBetween(K l, K r) {
        if (C()(r, l)) return 0;
        return keys.order_of_key(r) + exists(r) - keys.order_of_key(l);
    }

    optional<K> smallest(ll order) {
        if (order <= 0 || order > (ll)data.size())
            return nullopt;

        auto it = data.find_by_order(order-1);
        return it->first;
    }

    optional<K> uniqueSmallest(ll order) {
        if (order <= 0 || order > (ll)keys.size())
            return nullopt;

        auto it = keys.find_by_order(order-1);
        return *it;
    }

    optional<K> largest(ll order) {
        return smallest(data.size() - order + 1);
    }

    optional<K> uniqueLargest(ll order) {
        return uniqueSmallest(keys.size() - order + 1);
    }

    optional<K> median() {
        if (data.empty()) return nullopt;
        return smallest((data.size()+1)/2);
    }

    optional<K> medianUnique() {
        if (keys.empty()) return nullopt;
        return uniqueSmallest((keys.size()+1)/2);
    }

    optional<K> lowerBound(K key) {
        auto it = data.lower_bound({key, FIRST_TIME});
        if (it == data.end()) return nullopt;
        return it->first;
    }

    optional<K> upperBound(K key) {
        auto it = data.upper_bound({key, LAST_TIME});
        if (it == data.end()) return nullopt;
        return it->first;
    }

    optional<K> uniquePrev(K key) {
        auto it = keys.lower_bound(key);
        if (it == keys.begin()) return nullopt;
        --it;
        return *it;
    }

    optional<K> uniqueNext(K key) {
        auto it = keys.upper_bound(key);
        if (it == keys.end()) return nullopt;
        return *it;
    }

    optional<K> popFirst() {
        if (data.empty()) return nullopt;

        auto key = first();
        erase(*key);
        return key;
    }

    optional<K> popLast() {
        if (data.empty()) return nullopt;

        auto key = last();
        erase(*key);
        return key;
    }
};

enum Operation {
    INSERT = 0,
    ERASE = 1,
    ERASE_ALL = 2,
    CLEAR = 3,

    FIRST = 4,
    LAST = 5,

    LESS = 6,
    UNIQUE_LESS = 7,
    MORE = 8,
    UNIQUE_MORE = 9,

    SMALLEST = 10,
    UNIQUE_SMALLEST = 11,
    LARGEST = 12,
    UNIQUE_LARGEST = 13,

    COUNT = 14,
    COUNT_BETWEEN = 15,
    UNIQUE_COUNT_BETWEEN = 16,

    SIZE = 17,
    UNIQUE_SIZE = 18,

    MEDIAN = 19,
    UNIQUE_MEDIAN = 20,

    LOWER_BOUND = 21,
    UPPER_BOUND = 22,
    UNIQUE_PREV = 23,
    UNIQUE_NEXT = 24,

    POP_FIRST = 25,
    POP_LAST = 26,

    PRINT = 100
};

string opToStr(Operation op) {
    switch (op) {

        case PRINT: return "PRINT";

        case INSERT: return "INSERT";
        case ERASE: return "ERASE";
        case ERASE_ALL: return "ERASE-A";
        case CLEAR: return "CLEAR";

        case FIRST: return "FIRST";
        case LAST: return "LAST";

        case LESS: return "LESS";
        case UNIQUE_LESS: return "U-LESS";
        case MORE: return "MORE";
        case UNIQUE_MORE: return "U-MORE";

        case SMALLEST: return "SMLST";
        case UNIQUE_SMALLEST: return "U-SMLST";
        case LARGEST: return "LRGST";
        case UNIQUE_LARGEST: return "U-LRGST";

        case COUNT: return "COUNT";
        case COUNT_BETWEEN: return "CNT-BT";
        case UNIQUE_COUNT_BETWEEN: return "U-CNT-BT";

        case SIZE: return "SIZE";
        case UNIQUE_SIZE: return "U-SIZE";

        case MEDIAN: return "MEDIAN";
        case UNIQUE_MEDIAN: return "U-MEDN";

        case LOWER_BOUND: return "L-BOUND";
        case UPPER_BOUND: return "G-BOUND";

        case UNIQUE_PREV: return "U-PREV";
        case UNIQUE_NEXT: return "U-NEXT";

        case POP_FIRST: return "POP-FST";
        case POP_LAST: return "POP-LST";
    }

    return "UNKNOWN";
}

string padOpStr(string s, int lim) {
    while (s.size() < lim) s += ' ';
    return s;
}

template<typename K>
struct Printer {

    Operation op;
    any input;

    Printer(Operation o, any in)
        : op(o), input(move(in)) {}

    void printInput() const {

        cout << padOpStr("[" + opToStr(op) + "]", 12) << "input=";

        if (!input.has_value()) {
            cout << "?";
            return;
        }

        if (input.type() == typeid(ll))
            cout << any_cast<ll>(input);
        else if (input.type() == typeid(int))
            cout << any_cast<int>(input);
        else if (input.type() == typeid(string))
            cout << any_cast<string>(input);
        else
            cout << "?";
    }

    template<typename R>
    void operator()(const R& result) const {

        printInput();

        cout << " result=";

        if constexpr (is_same_v<R,bool>) {
            cout << (result ? "true" : "false");
        }
        else if constexpr (is_same_v<R,optional<K>>) {
            if (result) cout << *result;
            else cout << "?";
        }
        else {
            cout << result;
        }

        cout << "\n";
    }
};

template <typename K>
void solveSet() {

    SuperSet<K> ss;

    ll n;
    cin >> n;

    while (n--) {

        ll o;
        cin >> o;

        auto op = (Operation)o;

        if (op == PRINT) {
            Printer<K>{op, nullopt}(ss.print());
        }

        else if (op == INSERT) {
            K k; cin >> k;
            Printer<K>{op, k}(ss.insert(k));
        }

        else if (op == ERASE) {
            K k; cin >> k;
            Printer<K>{op, k}(ss.erase(k));
        }

        else if (op == ERASE_ALL) {
            K k; cin >> k;
            Printer<K>{op, k}(ss.eraseAll(k));
        }

        else if (op == CLEAR) {
            Printer<K>{op, nullopt}(ss.clear());
        }

        else if (op == FIRST) {
            Printer<K>{op, nullopt}(ss.first());
        }

        else if (op == LAST) {
            Printer<K>{op, nullopt}(ss.last());
        }

        else if (op == LESS) {
            K k; cin >> k;
            Printer<K>{op, k}(ss.less(k));
        }

        else if (op == UNIQUE_LESS) {
            K k; cin >> k;
            Printer<K>{op, k}(ss.uniqueLess(k));
        }

        else if (op == MORE) {
            K k; cin >> k;
            Printer<K>{op, k}(ss.more(k));
        }

        else if (op == UNIQUE_MORE) {
            K k; cin >> k;
            Printer<K>{op, k}(ss.uniqueMore(k));
        }

        else if (op == SMALLEST) {
            ll x; cin >> x;
            Printer<K>{op, x}(ss.smallest(x));
        }

        else if (op == UNIQUE_SMALLEST) {
            ll x; cin >> x;
            Printer<K>{op, x}(ss.uniqueSmallest(x));
        }

        else if (op == LARGEST) {
            ll x; cin >> x;
            Printer<K>{op, x}(ss.largest(x));
        }

        else if (op == UNIQUE_LARGEST) {
            ll x; cin >> x;
            Printer<K>{op, x}(ss.uniqueLargest(x));
        }

        else if (op == COUNT) {
            K k; cin >> k;
            Printer<K>{op, k}(ss.count(k));
        }

        else if (op == COUNT_BETWEEN) {
            K l, r;
            cin >> l >> r;
            Printer<K>{op, string(to_string(l)+" "+to_string(r))}
                (ss.countBetween(l,r));
        }

        else if (op == UNIQUE_COUNT_BETWEEN) {
            K l, r;
            cin >> l >> r;
            Printer<K>{op, string(to_string(l)+" "+to_string(r))}
                (ss.uniqueCountBetween(l,r));
        }

        else if (op == SIZE) {
            Printer<K>{op, nullopt}(ss.size());
        }

        else if (op == UNIQUE_SIZE) {
            Printer<K>{op, nullopt}(ss.uniqueSize());
        }

        else if (op == MEDIAN) {
            Printer<K>{op, nullopt}(ss.median());
        }

        else if (op == UNIQUE_MEDIAN) {
            Printer<K>{op, nullopt}(ss.medianUnique());
        }

        else if (op == LOWER_BOUND) {
            K k; cin >> k;
            Printer<K>{op, k}(ss.lowerBound(k));
        }

        else if (op == UPPER_BOUND) {
            K k; cin >> k;
            Printer<K>{op, k}(ss.upperBound(k));
        }

        else if (op == UNIQUE_PREV) {
            K k; cin >> k;
            Printer<K>{op, k}(ss.uniquePrev(k));
        }

        else if (op == UNIQUE_NEXT) {
            K k; cin >> k;
            Printer<K>{op, k}(ss.uniqueNext(k));
        }

        else if (op == POP_FIRST) {
            Printer<K>{op, nullopt}(ss.popFirst());
        }

        else if (op == POP_LAST) {
            Printer<K>{op, nullopt}(ss.popLast());
        }
    }
}

int main() {
    ios::sync_with_stdio(false);
    cin.tie(nullptr);

    solveSet<ll>();
}