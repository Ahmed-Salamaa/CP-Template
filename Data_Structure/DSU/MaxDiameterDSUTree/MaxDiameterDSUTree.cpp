/*
 * Topic: Max Diameter DSU Tree
 * Description: Max Diameter DSU Tree is a DSU variant that maintains the diameter of connected
 *   components as edges are added, tracking the longest paths of merged trees.
 * tree construction or merging. Usage: MaxDiameterDSUTree dsu(N); dsu.unite(u, v); cout << dsu.get_max_diameter() <<
 * endl;
 */
#include "../../../core.h"

// Time Complexity: O(1) query, O(N log N) build
// Space Complexity: O(N log N)
template <class T>
struct SparseTable {
    int n, LOG;
    vector<vector<T>> st;

    SparseTable() {}

    SparseTable(vector<T> a) {
        n = a.size();
        LOG = bit_width(static_cast<unsigned int>(a.size())) + 1;
        st = vector<vector<T>>(LOG, vector<T>(n));
        st[0] = a;
        for (int j = 1; j < LOG; j++) {
            for (int i = 0; i + (1 << j) - 1 < n; i++) { st[j][i] = min(st[j - 1][i], st[j - 1][i + (1 << (j - 1))]); }
        }
    }

    T query(int l, int r) {
        int k = __lg(r - l + 1);
        return min(st[k][l], st[k][r - (1 << k) + 1]);
    }
};

// Time Complexity: O(1) query, O(N log N) build
// Space Complexity: O(N log N)
struct LCA {
    int timer;
    vector<vector<int>> g;
    vector<int> tin, tout, dep;
    vector<pair<int, int>> flat;
    SparseTable<pair<int, int>> st;

    LCA() {}

    LCA(int n) : timer(0), g(n + 5), tin(n + 5), tout(n + 5), dep(n + 5), flat(2 * (n + 5)) {}

    void add_edge(int u, int v) {
        g[u].push_back(v);
        g[v].push_back(u);
    }

    void dfs(int v, int p) {
        flat[timer] = {dep[v], v};
        tin[v] = timer++;
        for (int u : g[v]) {
            if (u == p) continue;
            dep[u] = dep[v] + 1;
            dfs(u, v);
            flat[timer++] = {dep[v], v};
        }
        tout[v] = timer;
    }

    void build(int root) {
        dfs(root, 0);
        st = SparseTable<pair<int, int>>(flat);
    }

    int get_lca(int u, int v) {
        auto [l, r] = minmax(tin[u], tin[v]);
        return st.query(l, r).second;
    }

    int get_dist(int u, int v) { return dep[u] + dep[v] - 2 * dep[get_lca(u, v)]; }
};

// Time Complexity: O(log N) per merge
// Space Complexity: O(N)
struct MaxDiameterDSUTree {
    int cnt, diam{};
    vector<int> p, sz;
    vector<pair<int, int>> far;
    vector<array<int, 3>> st;
    LCA* lca;

    MaxDiameterDSUTree() {}

    MaxDiameterDSUTree(int n, LCA* lca_ptr) : cnt(n), p(n + 5), sz(n + 5, 1), far(n + 5), lca(lca_ptr) {
        iota(p.begin(), p.end(), 0);
        for (int i = 1; i <= n; i++) far[i] = {i, i};
    }

    // Time Complexity: O(log N)
    int find(int v) {
        while (v != p[v]) v = p[v];
        return v;
    }

    // Time Complexity: O(log N) due to find and LCA queries
    bool merge(int a, int b) {
        a = find(a), b = find(b);
        if (a == b) return false;
        if (sz[a] < sz[b]) swap(a, b);
        cnt--;
        sz[a] += sz[b];
        p[b] = a;
        auto [x1, y1] = far[a];
        int d1 = lca->get_dist(x1, y1);
        auto [x2, y2] = far[b];
        int d2 = lca->get_dist(x2, y2);
        int mx = -1, mx1, mx2;
        if (d1 > mx) mx = d1, mx1 = x1, mx2 = y1;
        if (d2 > mx) mx = d2, mx1 = x2, mx2 = y2;
        for (int u : {x1, y1}) {
            for (int v : {x2, y2}) {
                int dist = lca->get_dist(u, v);
                if (dist > mx) mx = dist, mx1 = u, mx2 = v;
            }
        }
        diam = max(diam, mx);
        far[a] = {mx1, mx2};
        st.push_back({b, x1, y1});
        return true;
    }

    // Time Complexity: O(1)
    void rollback() {
        if (st.empty()) return;
        auto [b, x1, x2] = st.back();
        st.pop_back();
        int a = p[b];
        cnt++;
        sz[a] -= sz[b];
        far[a] = {x1, x2};
        p[b] = b;
    }
};
