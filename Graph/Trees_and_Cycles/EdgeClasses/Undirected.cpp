// Topic: Edge Classes Undirected - Classifies edges into tree and backward edges in an undirected graph
#include "../../../core.h"

struct UndirectedEdgeClasses {
    // Time Complexity: O(V + E)
    // Space Complexity: O(V + E)
    int n, timer;
    vector<vector<int>> adj;
    vector<int> tin;
    vector<bool> vis;

    UndirectedEdgeClasses(int n) : n(n), timer(0), adj(n), tin(n, 0), vis(n, false) {}

    void add_edge(int u, int v) {
        adj[u].push_back(v);
        adj[v].push_back(u);
    }

    void dfs(int u, int p = -1) {
        vis[u] = true;
        tin[u] = ++timer;
        for (int v : adj[u]) {
            if (vis[v] && tin[u] < tin[v]) {
                // forward edge (already visited from the other side)
                continue;
            }
            if (!vis[v]) {
                // tree edge
                dfs(v, u);
            } else if (v != p) {
                // backward edge
            }
        }
    }

    void build() {
        for (int i = 0; i < n; i++) {
            if (!vis[i]) dfs(i);
        }
    }
};
