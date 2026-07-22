/*
 * Topic: Tree - Heavy Light Decomposition
 * Description: Heavy-Light Decomposition partitions a tree into a set of disjoint paths (heavy
 *   chains), enabling path queries and updates in O(log^2 N) time using segment
 *   trees.
 */
#include "../../core.h"

template <bool EDGE_UPDATES = false>
struct HLD {
    int n, timer;
    vector<vector<int>> adj;
    vector<int> depth, root, pos, heavy, sz, parent;

    HLD(int n)
        : n(n),
          timer(0),
          adj(n + 5),
          depth(n + 5),
          root(n + 5),
          pos(n + 5),
          heavy(n + 5, -1),
          sz(n + 5),
          parent(n + 5) {}

    void add_edge(int u, int v) {
        adj[u].push_back(v);
        adj[v].push_back(u);
    }

    void dfs_sz(int u, int p) {
        depth[u] = depth[p] + 1;
        sz[u] = 1;
        parent[u] = p;
        for (auto& v : adj[u]) {
            if (v == p) continue;
            dfs_sz(v, u);
            sz[u] += sz[v];
        }
        heavy[u] = -1;
        for (auto& v : adj[u]) {
            if (v == p) continue;
            if (heavy[u] == -1 || sz[v] > sz[heavy[u]]) { heavy[u] = v; }
        }
    }

    void decompose(int u, int p) {
        pos[u] = timer++;
        if (heavy[u] != -1) {
            root[heavy[u]] = root[u];
            decompose(heavy[u], u);
        }
        for (auto& v : adj[u]) {
            if (v == p || heavy[u] == v) continue;
            root[v] = v;
            decompose(v, u);
        }
    }

    void build(int u = 1) {
        dfs_sz(u, 0);
        root[u] = u;
        decompose(u, 0);
    }

    void upd_path(int u, int v, long long nw_val) {
        while (root[u] != root[v]) {
            if (depth[root[u]] < depth[root[v]]) swap(u, v);
            // seg.upd(0, 0, n - 1, pos[root[u]], pos[u], nw_val);
            u = parent[root[u]];
        }
        if (depth[u] > depth[v]) swap(u, v);
        if (EDGE_UPDATES) {
            if (u != v) {
                // seg.upd(0, 0, n - 1, pos[u] + 1, pos[v], nw_val);
            }
        } else {
            // seg.upd(0, 0, n - 1, pos[u], pos[v], nw_val);
        }
    }

    long long query_path(int u, int v) {
        long long res = 0;
        while (root[u] != root[v]) {
            if (depth[root[u]] < depth[root[v]]) swap(u, v);
            // res = max(seg.query(0, 0, n - 1, pos[root[u]], pos[u]), res);
            u = parent[root[u]];
        }
        if (depth[u] > depth[v]) swap(u, v);
        if (EDGE_UPDATES) {
            if (u != v) {
                // res = max(res, seg.query(0, 0, n - 1, pos[u] + 1, pos[v]));
            }
        } else {
            // res = max(res, seg.query(0, 0, n - 1, pos[u], pos[v]));
        }
        return res;
    }
};
