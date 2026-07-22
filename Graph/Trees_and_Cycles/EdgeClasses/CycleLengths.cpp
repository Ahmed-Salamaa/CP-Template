// Topic: Cycle Lengths Multiplication - Multiplies lengths of all simple cycles in an undirected graph
#include "../../../core.h"

struct CycleLengths {
    // Time Complexity: O(V + E)
    // Space Complexity: O(V + E)
    int n, timer;
    vector<vector<int>> adj;
    vector<int> tin, depth;
    vector<bool> vis;

    CycleLengths(int n) : n(n), timer(0), adj(n), tin(n, 0), depth(n, 0), vis(n, false) {}

    void add_edge(int u, int v) {
        adj[u].push_back(v);
        adj[v].push_back(u);
    }

    __int128 dfs(int u, int p = -1) {
        vis[u] = true;
        tin[u] = ++timer;
        depth[u] = (p == -1 ? 1 : depth[p] + 1);
        __int128 res = 1;
        for (int v : adj[u]) {
            if (vis[v] && tin[u] < tin[v]) {
                continue;
            }
            if (!vis[v]) {
                res *= dfs(v, u);
            } else if (v != p) {
                res *= (depth[u] - depth[v] + 1);
            }
        }
        return res;
    }
    
    __int128 build() {
        __int128 total = 1;
        for (int i = 0; i < n; i++) {
            if (!vis[i]) total *= dfs(i, -1);
        }
        return total;
    }
};
