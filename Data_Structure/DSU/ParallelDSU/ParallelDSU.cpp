/*
 * Topic: Parallel DSU
 * Description: Implements DSU structures adapted for Parallel Binary Search, allowing
 *   concurrent processing of multiple
 * constraints or queries over edge additions. Usage: ParallelDSU dsu(N); dsu.add_edges_and_check(queries);
 */
#include "../../../core.h"

// Time Complexity: O(N log N) total work
// Space Complexity: O(N log N)
struct OfflineParallelDSU {
    int n;
    int LG;
    vector<vector<int>> P;
    vector<int> par, sz;

    OfflineParallelDSU(int _n) {
        n = _n;
        LG = bit_width(static_cast<unsigned int>(n)) + 1;
        P.assign(LG, vector<int>(n + 5));
        for (int k = 0; k < LG; ++k) { iota(P[k].begin(), P[k].end(), 0); }
        par.resize(n + 5);
        iota(par.begin(), par.end(), 0);
        sz.assign(n + 5, 1);
    }

    // Time Complexity: O(alpha(N))
    int get(int k, int x) { return P[k][x] == x ? x : P[k][x] = get(k, P[k][x]); }

    // Time Complexity: O(alpha(N))
    void join(int k, int u, int v) {
        u = get(k, u);
        v = get(k, v);
        if (u != v) P[k][u] = v;
    }

    // Time Complexity: O(alpha(N))
    int find(int x) { return par[x] == x ? x : par[x] = find(par[x]); }

    // Time Complexity: O(1)
    void add(int u, int v, int len) {
        if (len <= 0) return;
        int k = 31 - __builtin_clz(len);
        join(k, u, v);
        join(k, u + len - (1 << k), v + len - (1 << k));
    }

    // Time Complexity: O(N log N)
    void build() {
        for (int k = LG - 1; k > 0; --k) {
            for (int i = 1; i + (1 << k) - 1 <= n; ++i) {
                int r = get(k, i);
                if (i != r) {
                    join(k - 1, i, r);
                    join(k - 1, i + (1 << (k - 1)), r + (1 << (k - 1)));
                }
            }
        }
        for (int i = 1; i <= n; ++i) {
            int u = find(i), v = find(get(0, i));
            if (u != v) {
                par[u] = v;
                sz[v] += sz[u];
            }
        }
    }
};

// Time Complexity: Amortized O(N log N) total work
// Space Complexity: O(N log N)
struct OnlineParallelDSU {
    struct SimpleDSU {
        vector<int> p, sz;

        SimpleDSU(int n = 0) { init(n); }

        void init(int n) {
            p.resize(n + 5);
            iota(p.begin(), p.end(), 0);
            sz.assign(n + 5, 1);
        }

        int find(int x) { return x == p[x] ? x : p[x] = find(p[x]); }

        bool unite(int x, int y) {
            x = find(x), y = find(y);
            if (x == y) return false;
            if (sz[x] < sz[y]) swap(x, y);
            p[y] = x;
            sz[x] += sz[y];
            return true;
        }
    };

    vector<int> p, sz;
    vector<SimpleDSU> st;
    int cnt;

    OnlineParallelDSU(int n = 0) {
        if (n == 0) return;
        int LOG = 31 - __builtin_clz(n);
        p.resize(n + 5);
        iota(p.begin(), p.end(), 0);
        sz.assign(n + 5, 1);
        st.assign(LOG + 1, SimpleDSU(n));
        cnt = n;
    }

    // Time Complexity: O(alpha(N))
    int find(int x) { return x == p[x] ? x : p[x] = find(p[x]); }

    // Time Complexity: O(alpha(N))
    int size(int u) { return sz[find(u)]; }

    // Time Complexity: O(1)
    int count() const { return cnt; }

    bool unite(int u, int v) {
        u = find(u), v = find(v);
        if (u == v) return false;
        if (sz[u] < sz[v]) swap(u, v);
        p[v] = u;
        sz[u] += sz[v];
        cnt--;
        return true;
    }

    void join(int k, int u, int v) {
        if (!k) {
            unite(u, v);
            return;
        }
        if (st[k].unite(u, v)) {
            join(k - 1, u, v);
            join(k - 1, u + (1 << (k - 1)), v + (1 << (k - 1)));
        }
    }

    // Time Complexity: Amortized O(alpha(N)) per query over all queries
    void merge(int u, int v, int len) {
        if (!len) return;
        int k = 31 - __builtin_clz(len);
        join(k, u, v);
        join(k, u + len - (1 << k), v + len - (1 << k));
    }
};
