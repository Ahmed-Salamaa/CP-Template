#include "../core.h"

/** Topic: Hashing Segment Tree */
template <typename T = int, bool B = false>
struct HashSegmentTree {
    int n, size;
    vector<pair<T, T>> tree;
    const int p1 = 313, p2 = 1013;
    const int m1 = 1e9 + 7, m2 = 1e9 + 9;
    vector<T> pw1, pw2, i1, i2;
    T iv1 = 492012783, iv2 = 487660419;

    inline T add(T a, T b, T m) {
        return (a + b) % m;
    }

    inline T mul(T a, T b, T m) {
        return (1LL * a * b) % m;
    }

    HashSegmentTree(int n = 0) : n(n) {
        size = 1;
        while (size < n) size <<= 1;

        tree.assign(2 * size, {0, 0});
        pw1.assign(2 * size, 0);
        pw2.assign(2 * size, 0);
        i1.assign(2 * size, 0);
        i2.assign(2 * size, 0);

        pw1[0] = pw2[0] = i1[0] = i2[0] = 1;
        for (int i = 1; i < 2 * size; i++) {
            pw1[i] = mul(pw1[i - 1], p1, m1);
            pw2[i] = mul(pw2[i - 1], p2, m2);
            i1[i] = mul(i1[i - 1], iv1, m1);
            i2[i] = mul(i2[i - 1], iv2, m2);
        }
    }

    inline pair<T, T> merge(const pair<T, T>& a, const pair<T, T>& b) {
        return {add(a.first, b.first, m1), add(a.second, b.second, m2)};
    }

    void build(const vector<T>& v, int x, int l, int r) {
        if (B ? l >= (int)v.size() : l > (int)v.size()) return;
        if (l == r) {
            tree[x] = {mul(v[l - !B], pw1[x], m1), mul(v[l - !B], pw2[x], m2)};
            return;
        }
        int m = (l + r) / 2;
        build(v, 2 * x, l, m);
        build(v, 2 * x + 1, m + 1, r);
        tree[x] = merge(tree[2 * x], tree[2 * x + 1]);
    }

    void build(const vector<T>& v) {
        build(v, 1, 1, size);
    }

    void build(const string& s) {
        vector<T> v(s.size());
        for (int i = 0; i < (int)s.size(); i++) {
            v[i] = s[i] - 'a' + 1;
        }
        build(v);
    }

    void update(int i, T v, int x, int l, int r) {
        if (l == r) {
            tree[x] = {mul(v, pw1[x], m1), mul(v, pw2[x], m2)};
            return;
        }
        int m = (l + r) / 2;
        if (i <= m) {
            update(i, v, 2 * x, l, m);
        } else {
            update(i, v, 2 * x + 1, m + 1, r);
        }
        tree[x] = merge(tree[2 * x], tree[2 * x + 1]);
    }

    void update(int i, T v) {
        update(i, v, 1, 1, size);
    }

    pair<T, T> query(int l, int r, int x, int lx, int rx) {
        if (lx > r || l > rx) return {0, 0};
        if (lx >= l && rx <= r) return tree[x];
        int m = (lx + rx) / 2;
        return merge(query(l, r, 2 * x, lx, m), query(l, r, 2 * x + 1, m + 1, rx));
    }

    pair<T, T> query(int l, int r) {
        auto [h1, h2] = query(l, r, 1, 1, size);
        return {mul(h1, i1[l - 1], m1), mul(h2, i2[l - 1], m2)};
    }
};
