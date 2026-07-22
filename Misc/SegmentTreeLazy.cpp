/*
 * Topic: Misc - Misc
 * Description: Segment Tree with Lazy Propagation enables efficient range updates and range
 *   queries in O(log N) time by deferring updates until necessary. Essential for
 *   problems requiring both continuous interval modifications and aggregate queries.
 */
#include "../core.h"

// -------------------------------------------------------------------------------------
// Lazy propagation range change point query
// used for problems like
// 1 l r x y   change all elements in range from l to r has value x to y
// 2 idx       query the value at index idx
struct LazyBinTreeUniversal {
    using T = int;
    using TMod = std::array<uint8_t, 101>;
    size_t size;
    std::vector<T> t;
    std::vector<TMod> mod;
    size_t h;

    LazyBinTreeUniversal(size_t N) : size(N), t(2 * N), mod(N, modInit()), h(32 - __builtin_clz(N)) {}

    LazyBinTreeUniversal(const std::vector<T> &other)
        : size(other.size()), t(2 * other.size()), mod(other.size(), modInit()), h(32 - __builtin_clz(other.size())) {
        std::copy(other.begin(), other.end(), t.begin() + size);
    }

    void apply(size_t p, const TMod &up_mod) {
        if (p < size) {
            if (mod[p][0] == 0) {
                mod[p] = up_mod;
            } else {
                mod[p] = combineMod(mod[p], up_mod);
            }
        } else {
            t[p] = combineValue(t[p], up_mod);
        }
    }

    void push(size_t p) {
        for (size_t s = h; s > 0; s--) {
            size_t pos = p >> s;
            if (mod[pos][0] != 0) {
                apply(2 * pos, mod[pos]);
                apply(2 * pos + 1, mod[pos]);
                mod[pos] = modInit();
            }
        }
    }

    void modify(size_t l, size_t r, const TMod &value) {
        if (l == r)
            return;
        l += size;
        r += size;
        push(l);
        push(r - 1);
        while (l < r) {
            if (l & 1) {
                apply(l, value);
                l++;
            }
            if (r & 1) {
                r--;
                apply(r, value);
            }
            l /= 2;
            r /= 2;
        }
    }

    T query(size_t p) {
        p += size;
        push(p);
        return t[p];
    }

  private:
    TMod combineMod(const TMod &mod, const TMod &up_mod) {
        TMod result = {};
        for (size_t i = 0; i < result.size(); i++) {
            result[i] = up_mod[mod[i]];
        }
        return result;
    }

    T combineValue(T value, const TMod &up_mod) { return up_mod[value]; }

    T init() { return 0; }

    TMod modInit() {
        TMod result = {};
        std::iota(result.begin(), result.end(), 0);
        return result;
    }
};

// -------------------------------------------------------------------------------------
// Standard Range Minimum Segment Tree with Lazy Propagation
struct SegmentTree {
    int sz;
    vector<int> seg;
    vector<int> lazy;
    int ret = 1e9;

    SegmentTree(int n) {
        sz = 1;
        while (sz < n)
            sz *= 2;
        seg.resize(2 * sz);
        lazy.resize(2 * sz);
    }

    int merge(int a, int b) { return min(a, b); }

    void build_tree(const vector<int>& s, int node, int l, int r) {
        if (l > r)
            return;

        if (l == r) {
            if (l < s.size()) seg[node] = s[l];
            return;
        }

        int mid = (l + r) / 2;

        build_tree(s, node * 2, l, mid);
        build_tree(s, node * 2 + 1, mid + 1, r);

        seg[node] = merge(seg[node * 2], seg[node * 2 + 1]);
    }

    void update_tree(int node, int l, int r, int lx, int rx, int value) {
        if (lazy[node] != 0) {
            seg[node] += lazy[node];
            if (l != r) {
                lazy[node * 2] += lazy[node];
                lazy[node * 2 + 1] += lazy[node];
            }
            lazy[node] = 0;
        }

        if (l > r || l > rx || r < lx)
            return;

        if (l >= lx && r <= rx) {
            seg[node] += value;
            if (l != r) {
                lazy[node * 2] += value;
                lazy[node * 2 + 1] += value;
            }
            return;
        }

        int mid = (l + r) / 2;

        update_tree(node * 2, l, mid, lx, rx, value);
        update_tree(node * 2 + 1, mid + 1, r, lx, rx, value);

        seg[node] = merge(seg[node * 2], seg[node * 2 + 1]);
    }

    int query_tree(int node, int l, int r, int lx, int rx) {
        if (l > r || l > rx || r < lx)
            return ret;

        if (lazy[node] != 0) {
            seg[node] += lazy[node];
            if (l != r) {
                lazy[node * 2] += lazy[node];
                lazy[node * 2 + 1] += lazy[node];
            }
            lazy[node] = 0;
        }

        if (l >= lx && r <= rx)
            return seg[node];

        int mid = (l + r) / 2;

        int q1 = query_tree(node * 2, l, mid, lx, rx);
        int q2 = query_tree(node * 2 + 1, mid + 1, r, lx, rx);

        return merge(q1, q2);
    }
};

// -------------------------------------------------------------------------------------
// Segment Tree with Lazy Propagation for sum, sqsum, cubesum
struct SegmentTreeMath {
    struct Node {
        long long sum, sqsum, cubesum;
        Node(long long x = 0) {
            long long mod = 1e9 + 7;
            sum = x;
            sqsum = x * x % mod;
            cubesum = x * x % mod * x % mod;
        }
    };

    int sz;
    vector<Node> tree;
    vector<long long> lazy;
    long long mod = 1e9 + 7;

    SegmentTreeMath(int n) {
        sz = 1;
        while (sz < n)
            sz *= 2;
        tree = vector<Node>(2 * sz);
        lazy = vector<long long>(2 * sz, 0); // for lazy propagation
    }

    void propagate(int idx, int lx, int rx) {
        if (!lazy[idx])
            return;
        tree[idx].cubesum += 3 * lazy[idx] * tree[idx].sqsum % mod + 3 * lazy[idx] * lazy[idx] % mod * tree[idx].sum % mod +
                             lazy[idx] * lazy[idx] % mod * lazy[idx] % mod * (rx - lx + 1) % mod;
        tree[idx].cubesum %= mod;

        tree[idx].sqsum += 2 * lazy[idx] * tree[idx].sum % mod + lazy[idx] * lazy[idx] % mod * (rx - lx + 1) % mod;
        tree[idx].sqsum %= mod;

        tree[idx].sum += lazy[idx] * (rx - lx + 1) % mod;
        tree[idx].sum %= mod;

        if (lx != rx) {
            lazy[2 * idx] += lazy[idx];
            lazy[2 * idx + 1] += lazy[idx];
        }
        lazy[idx] = 0;
    }

    void merge(Node &res, Node &a, Node &b) {
        res.sum = (a.sum + b.sum) % mod;
        res.sqsum = (a.sqsum + b.sqsum) % mod;
        res.cubesum = (a.cubesum + b.cubesum) % mod;
    }

    void build(vector<long long> &v, int idx, int lx, int rx) {
        if (lx > (int)v.size())
            return;
        if (lx == rx) {
            tree[idx] = Node(v[lx]);
            return;
        }
        int mid = (lx + rx) / 2;
        build(v, 2 * idx, lx, mid);
        build(v, 2 * idx + 1, mid + 1, rx);
        merge(tree[idx], tree[2 * idx], tree[2 * idx + 1]);
    }

    void update(int l, int r, long long val, int idx, int lx, int rx) { // update range
        propagate(idx, lx, rx);
        if (l > rx || r < lx)
            return;
        if (l <= lx && rx <= r) {
            lazy[idx] = val;
            propagate(idx, lx, rx);
            return;
        }
        int mid = (lx + rx) / 2;
        update(l, r, val, 2 * idx, lx, mid);
        update(l, r, val, 2 * idx + 1, mid + 1, rx);
        merge(tree[idx], tree[2 * idx], tree[2 * idx + 1]);
    }

    Node query(int l, int r, int idx, int lx, int rx) {
        propagate(idx, lx, rx); // for lazy propagation
        if (lx > r or rx < l)
            return Node(0LL);
        if (lx >= l and rx <= r)
            return tree[idx];
        int mid = (lx + rx) / 2;
        Node left = query(l, r, 2 * idx, lx, mid);
        Node right = query(l, r, 2 * idx + 1, mid + 1, rx);
        Node res;
        merge(res, left, right);
        return res;
    }
};
