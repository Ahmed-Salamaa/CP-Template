/* Topic: Heavy Light Decomposition (Minimal)
 * Description: Heavy-Light Decomposition (HLD) decomposes a tree into a set of disjoint paths,
 *   allowing path queries and updates to be processed using a Segment Tree in
 *   O(log^2 N) time.
 * Usage: HLD<false> hld(G, root); auto paths = hld.queryPath(u, v);
 */
#include "../../core.h"

template <bool VAL_ON_EDGE = false>
struct HLD {
    int n, timer;
    vector<int> par, root, pos, sz, child, dep;

    void dfs_sz(int u, int p, int d, const vector<vector<int>>& adj) {
        par[u] = p;
        dep[u] = d;
        sz[u] = 1;
        child[u] = 0;
        for (int v : adj[u]) {
            if (v == p) continue;
            dfs_sz(v, u, d + 1, adj);
            sz[u] += sz[v];
            if (sz[v] > sz[child[u]]) child[u] = v;
        }
    }

    void dfs_hld(int u, int p, int r, const vector<vector<int>>& adj) {
        root[u] = r;
        pos[u] = ++timer;
        if (child[u]) dfs_hld(child[u], u, r, adj);
        for (int v : adj[u]) {
            if (v != p && v != child[u]) dfs_hld(v, u, v, adj);
        }
    }

    // Time Complexity: O(N)
    // Space Complexity: O(N)
    HLD(const vector<vector<int>>& adj, int treeRoot = 1) {
        n = sz(adj) - 1;
        timer = 0;
        par.assign(n + 1, 0);
        root.assign(n + 1, 0);
        pos.assign(n + 1, 0);
        sz.assign(n + 1, 0);
        child.assign(n + 1, 0);
        dep.assign(n + 1, 0);
        dfs_sz(treeRoot, 0, 0, adj);
        dfs_hld(treeRoot, 0, treeRoot, adj);
    }

    // Returns contiguous ranges [l, r] covering the path from u to v.
    // Time Complexity: O(log N)
    vector<pair<int, int>> queryPath(int u, int v) {
        vector<pair<int, int>> ans;
        while (root[u] != root[v]) {
            if (dep[root[u]] < dep[root[v]]) swap(u, v);
            ans.push_back({pos[root[u]], pos[u]});
            u = par[root[u]];
        }
        if (dep[u] > dep[v]) swap(u, v);
        if (!VAL_ON_EDGE)
            ans.push_back({pos[u], pos[v]});
        else if (u != v)
            ans.push_back({pos[u] + 1, pos[v]});
        return ans;
    }
};