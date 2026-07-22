/*
 * Topic: Graph - MaxDiameterDSUTree
 * Description: A specialized Disjoint Set Union (DSU) structure that dynamically maintains the
 *   maximum diameter of the components as trees are merged. It solves complex
 *   interactive query problems involving distances in dynamically growing forests.
 */
#include "../core.h"

// Time Complexity: O(1) query, O(N log N) build
// Space Complexity: O(N log N)
template <class T>
struct SparseTable {
    int n, LOG;
    std::vector<std::vector<T>> st;
 
    SparseTable() {}
 
    SparseTable(std::vector<T> a) {
        n = a.size();
        LOG = std::bit_width(a.size()) + 1;
        st = std::vector<std::vector<T>>(LOG, std::vector<T>(n));
        st[0] = a;
        for (int j = 1; j < LOG; j++) {
            for (int i = 0; i + (1 << j) - 1 < n; i++) {
                st[j][i] = std::min(st[j - 1][i], st[j - 1][i + (1 << (j - 1))]);
            }
        }
    }
 
    T query(int l, int r) {
        int k = std::__lg(r - l + 1);
        return std::min(st[k][l], st[k][r - (1 << k) + 1]);
    }
};

// Time Complexity: O(1) query, O(N log N) build
// Space Complexity: O(N log N)
struct LCA {
    int timer;
    std::vector<std::vector<int>> g;
    std::vector<int> tin, tout, dep;
    std::vector<std::pair<int, int>> flat;
    SparseTable<std::pair<int, int>> st;
 
    LCA() {}
    LCA(int n) : timer(0), g(n), tin(n), tout(n), dep(n), flat(2 * n) {}
 
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
        dfs(root, -1);
        st = SparseTable<std::pair<int, int>>(flat);
    }
 
    int get_lca(int u, int v) {
        auto [l, r] = std::minmax(tin[u], tin[v]);
        return st.query(l, r).second;
    }
 
    int get_dist(int u, int v) {
        return dep[u] + dep[v] - 2 * dep[get_lca(u, v)];
    }
};

// Time Complexity: O(log N) per merge
// Space Complexity: O(N)
struct MaxDiameterDSUTree {
    int cnt, diam{};
    std::vector<int> p, sz;
    std::vector<std::pair<int, int>> far;
    std::vector<std::array<int, 3>> st;
    LCA* lca;
 
    MaxDiameterDSUTree() {}
 
    MaxDiameterDSUTree(int n, LCA* lca_ptr) : cnt(n), p(n), sz(n, 1), far(n), lca(lca_ptr) {
        std::iota(p.begin(), p.end(), 0);
        for (int i = 0; i < n; i++) far[i] = {i, i};
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
        if (sz[a] < sz[b]) std::swap(a, b);
        cnt--; sz[a] += sz[b]; p[b] = a;
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
        diam = std::max(diam, mx);
        far[a] = {mx1, mx2};
        st.push_back({b, x1, y1});
        return true;
    }
 
    // Time Complexity: O(1)
    void rollback() {
        if (st.empty()) return;
        auto [b, x1, x2] = st.back(); st.pop_back();
        int a = p[b];
        cnt++; sz[a] -= sz[b]; far[a] = {x1, x2}; p[b] = b;
    }
};
