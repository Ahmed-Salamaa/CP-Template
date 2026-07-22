/*
 * Topic: DFS
 * Description: Depth-First Search (DFS) is a fundamental recursive graph traversal algorithm
 *   used to explore paths deeply before backtracking. It is widely applied for cycle
 *   detection, topological sorting, finding connected components, and solving mazes.
 * Usage:
 *    DFS solver(N);
 *    solver.add_edge(u, v);
 *    solver.dfs(1);
 */
#include "../../../core.h"

// Time Complexity: O(V + E)
// Space Complexity: O(V + E)
struct DFS {
    int n, timer;
    vector<vector<int>> adj;
    vector<int> vis, par, in, out, depth, subtree, color;

    DFS(int n) : n(n) {
        adj.resize(n + 1);
        vis.resize(n + 1);
        par.resize(n + 1);
        in.resize(n + 1);
        out.resize(n + 1);
        depth.resize(n + 1);
        subtree.resize(n + 1);
        color.resize(n + 1);
        timer = 0;
    }

    void add_edge(int u, int v) { adj[u].push_back(v); }

    bool dfs_visit(int u) {
        timer++;
        in[u] = timer;
        vis[u] = 1;
        subtree[u] = 1;
        bool res = true;
        for (auto v : adj[u]) {
            if (!vis[v]) {
                par[v] = u;
                depth[v] = depth[u] + 1;
                color[v] = 1 - color[u];
                res &= dfs_visit(v);
                subtree[u] += subtree[v];
            } else if (color[v] == color[u]) {
                res = false;
            }
        }
        timer++;
        out[u] = timer;
        return res;
    }

    bool dfs() {
        fill(all(vis), 0);
        timer = 0;
        bool ok = true;
        for (int i = 1; i <= n; ++i) {
            if (!vis[i]) { ok &= dfs_visit(i); }
        }
        return ok;
    }
};
