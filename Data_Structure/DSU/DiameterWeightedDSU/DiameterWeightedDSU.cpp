/*
 * Topic: Diameter Weighted DSU
 * Description: Diameter Weighted DSU is an advanced DSU structure that efficiently tracks both
 *   connectivity and the diameter in graphs or trees with weighted edges as
 *   components are dynamically united.
 * edge weights. Usage: DiameterWeightedDSU dsu(N); dsu.unite(u, v, w); cout << dsu.get_diameter(dsu.find(u)) << endl;
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
    vector<vector<pair<int, int>>> g;  // (neighbor, weight)
    vector<int> tin, tout;
    vector<int> dep;              // store weighted depth
    vector<pair<int, int>> flat;  // (depth, node)
    SparseTable<pair<int, int>> st;

    LCA() {}

    LCA(int n) : timer(0), g(n + 5), tin(n + 5), tout(n + 5), dep(n + 5), flat(2 * (n + 5)) {}

    void add_edge(int u, int v, int w) {
        g[u].push_back({v, w});
        g[v].push_back({u, w});
    }

    void dfs(int v, int p) {
        flat[timer] = {dep[v], v};
        tin[v] = timer++;
        for (auto [u, w] : g[v]) {
            if (u == p) continue;
            dep[u] = dep[v] + w;
            dfs(u, v);
            flat[timer++] = {dep[v], v};
        }
        tout[v] = timer;
    }

    void build(int root) {
        timer = 0;
        if (flat.size() != 2 * g.size()) flat.assign(2 * g.size(), {0, 0});
        dep.assign(g.size(), 0);
        dfs(root, 0);
        vector<pair<int, int>> used_flat(flat.begin(), flat.begin() + timer);
        st = SparseTable<pair<int, int>>(used_flat);
    }

    int get_lca(int u, int v) {
        auto [l, r] = minmax(tin[u], tin[v]);
        return st.query(l, r).second;
    }

    int get_dist(int u, int v) {
        int w = get_lca(u, v);
        return dep[u] + dep[v] - 2LL * dep[w];
    }
};

// Time Complexity: O(log N) per merge due to LCA queries
// Space Complexity: O(N)
struct DiameterWeightedDSU {
    int cnt;
    int diam{};
    vector<int> p, sz;
    vector<pair<int, int>> far;

    struct RB {
        int b;
        int a;
        int old_x;
        int old_y;
        int old_diam;
        int old_sz_a;
    };

    vector<RB> st;
    LCA* lca;

    DiameterWeightedDSU() {}

    DiameterWeightedDSU(int n, LCA* lca_ptr) : cnt(n), p(n + 5), sz(n + 5, 1), far(n + 5), lca(lca_ptr) {
        iota(p.begin(), p.end(), 0);
        for (int i = 1; i <= n; i++) far[i] = {i, i};
        diam = 0;
    }

    // Time Complexity: O(log N)
    int find(int v) {
        while (v != p[v]) v = p[v];
        return v;
    }

    // Time Complexity: O(log N)
    bool merge(int a, int b) {
        a = find(a), b = find(b);
        if (a == b) return false;
        if (sz[a] < sz[b]) swap(a, b);
        RB rb;
        rb.b = b;
        rb.a = a;
        rb.old_x = far[a].first;
        rb.old_y = far[a].second;
        rb.old_diam = diam;
        rb.old_sz_a = sz[a];
        cnt--;
        sz[a] += sz[b];
        p[b] = a;
        auto [x1, y1] = far[a];
        int d1 = lca->get_dist(x1, y1);
        auto [x2, y2] = far[b];
        int d2 = lca->get_dist(x2, y2);
        int mx = d1;
        int mx1 = x1, mx2 = y1;
        if (d2 > mx) {
            mx = d2;
            mx1 = x2;
            mx2 = y2;
        }
        for (int u : {x1, y1}) {
            for (int v : {x2, y2}) {
                int dist = lca->get_dist(u, v);
                if (dist > mx) {
                    mx = dist;
                    mx1 = u;
                    mx2 = v;
                }
            }
        }
        diam = max(diam, mx);
        far[a] = {mx1, mx2};
        st.push_back(rb);
        return true;
    }

    // Time Complexity: O(1)
    void rollback() {
        if (st.empty()) return;
        RB rb = st.back();
        st.pop_back();
        int b = rb.b;
        int a = rb.a;
        p[b] = b;
        sz[a] = rb.old_sz_a;
        far[a] = {rb.old_x, rb.old_y};
        diam = rb.old_diam;
        cnt++;
    }
};
