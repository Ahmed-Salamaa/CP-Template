/*
 * Topic: Tree - LCA
 * Description: LCA with Euler Tour computes the Lowest Common Ancestor by reducing the problem
 *   to a Range Minimum Query (RMQ) over an Euler tour array in O(1) time after O(N
 *   log N) preprocessing.
 */
#include "../../core.h"

template <typename T>
struct SparseTable {
    int n, LOG;
    vector<vector<T>> st;

    SparseTable() {}

    SparseTable(const vector<T>& a) {
        n = a.size();
        LOG = __lg(n) + 2;
        st.assign(LOG, vector<T>(n));
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

struct LCA_Euler_Tour {
    int n, timer;
    vector<vector<int>> g;
    vector<int> tin, tout, dep;
    vector<pair<int, int>> flat;
    SparseTable<pair<int, int>> st;

    LCA_Euler_Tour(int n) : n(n), timer(0) {
        g.assign(n + 5, vector<int>());
        tin.assign(n + 5, 0);
        tout.assign(n + 5, 0);
        dep.assign(n + 5, 0);
        flat.assign(2 * n + 10, {0, 0});
    }

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

    void build(int root = 1) {
        timer = 0;
        dfs(root, -1);
        vector<pair<int, int>> actual_flat(flat.begin(), flat.begin() + timer);
        st = SparseTable<pair<int, int>>(actual_flat);
    }

    int get_lca(int u, int v) {
        int l = tin[u];
        int r = tin[v];
        if (l > r) swap(l, r);
        return st.query(l, r).second;
    }

    int distance(int u, int v) { return dep[u] + dep[v] - 2 * dep[get_lca(u, v)]; }
};
