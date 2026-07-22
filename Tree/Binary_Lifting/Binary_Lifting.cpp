/*
 * Topic: Tree - Binary Lifting
 * Description: Binary Lifting precomputes the 2^k-th ancestor of each node, enabling O(log N)
 *   ancestor queries and facilitating efficient Lowest Common Ancestor and path
 *   queries.
 */
#include "../../core.h"

struct Binary_Lifting {
    int n, root, LOG;
    vector<vector<int>> adj;
    vector<vector<int>> up;
    vector<int> depth;

    Binary_Lifting(int n, int root = 1) : n(n), root(root) {
        LOG = __lg(n + 5) + 2;
        adj.assign(n + 5, vector<int>());
        up.assign(n + 5, vector<int>(LOG, -1));
        depth.assign(n + 5, 0);
    }

    void add_edge(int u, int v) {
        adj[u].push_back(v);
        adj[v].push_back(u);
    }

    void dfs(int u, int p) {
        depth[u] = (p == -1 ? 0 : depth[p] + 1);
        up[u][0] = (p == -1 ? u : p);
        for (int i = 1; i < LOG; i++) { up[u][i] = up[up[u][i - 1]][i - 1]; }
        for (int v : adj[u]) {
            if (v != p) { dfs(v, u); }
        }
    }

    void build() { dfs(root, -1); }

    int query(int u, int k) {
        if (depth[u] < k) return -1;
        for (int i = 0; i < LOG; i++) {
            if ((k >> i) & 1) { u = up[u][i]; }
        }
        return u;
    }
};
