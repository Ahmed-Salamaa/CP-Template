#include "../core.h"

/** Topic: Hashing with Lazy Segment Tree */
template <typename T = long long, int Base = 0>
struct Hash_LazySegmentTree {
    static const int HN = 1e6 + 5;
    long long p_pow[2][HN], pinv[2][HN];
    const long long mods[2] = {1000000007, 1000000009};
    const long long ps[2] = {31, 37};
    const long long pinvs[2] = {233333335, 972222231};

    bool initialized;

    void precompute() {
        if (initialized) return;
        initialized = true;
        p_pow[0][0] = p_pow[1][0] = pinv[0][0] = pinv[1][0] = 1;
        for (int i = 1; i < HN; i++) {
            for (int j : {0, 1}) {
                p_pow[j][i] = p_pow[j][i - 1] * ps[j] % mods[j];
                pinv[j][i] = pinv[j][i - 1] * pinvs[j] % mods[j];
            }
        }
    }

    string s;
    int n, sz;
    vector<T> lazy;
    vector<pair<T, T>> tree;
    
    struct Item {
        T len;
        pair<T, T> ans;
        Item() : ans({0, 0}), len(0) {}
        Item(pair<T, T> ans, T len) : ans(ans), len(len) {}
    };
    
    Hash_LazySegmentTree(int n, const string& s) : s(s), n(n) {
        initialized = false;
        sz = 1;
        while (sz < n) {
            sz *= 2;
        }
        lazy.assign(2 * sz, -1);
        tree.assign(2 * sz, {0, 0});
        precompute();
        build(1, 1, n);
    }
    
    pair<T, T> merge(const pair<T, T>& a, const pair<T, T>& b, T len) {
        return {(a.first + b.first * p_pow[0][len]) % mods[0], 
                (a.second + b.second * p_pow[1][len]) % mods[1]};
    }
    
    pair<T, T> lazy_process(T val, T len) {
        T h1 = val * ((p_pow[0][len] - 1 + mods[0]) % mods[0]) % mods[0] * pinvs[0] % mods[0];
        T h2 = val * ((p_pow[1][len] - 1 + mods[1]) % mods[1]) % mods[1] * pinvs[1] % mods[1];
        return {h1, h2};
    }
    
    void propagate(int u, int l, int r) {
        if (lazy[u] == -1) {
            return;
        }
        tree[u] = lazy_process(lazy[u], r - l + 1);
        if (l != r) {
            lazy[2 * u] = lazy[2 * u + 1] = lazy[u];
        }
        lazy[u] = -1;
    }
    
    void build(int u, int l, int r) {
        if (l == r) {
            tree[u] = {s[l - !Base] - 'a' + 1, s[l - !Base] - 'a' + 1};
            return;
        }
        int m = (l + r) / 2;
        build(2 * u, l, m);
        build(2 * u + 1, m + 1, r);
        tree[u] = merge(tree[2 * u], tree[2 * u + 1], m - l + 1);
    }
    
    void update(int u, int l, int r, int L, int R, T val) {
        propagate(u, l, r);
        if (l > R || r < L) {
            return;
        }
        if (l >= L && r <= R) {
            lazy[u] = val;
            propagate(u, l, r);
            return;
        }
        int m = (l + r) / 2;
        update(2 * u, l, m, L, R, val);
        update(2 * u + 1, m + 1, r, L, R, val);
        tree[u] = merge(tree[2 * u], tree[2 * u + 1], m - l + 1);
    }
    
    Item query(int u, int l, int r, int L, int R) {
        propagate(u, l, r);
        if (l > R || r < L) {
            return Item();
        }
        if (l >= L && r <= R) {
            return Item(tree[u], r - l + 1);
        }
        int m = (l + r) / 2;
        Item left = query(2 * u, l, m, L, R);
        Item right = query(2 * u + 1, m + 1, r, L, R);
        return Item(merge(left.ans, right.ans, left.len), left.len + right.len);
    }
    
    void update(int l, int r, T val) {
        update(1, 1, n, l, r, val);
    }
    
    void update(int idx, T val) {
        update(idx, idx, val);
    }
    
    pair<T, T> query(int l, int r) {
        return query(1, 1, n, l, r).ans;
    }
};
