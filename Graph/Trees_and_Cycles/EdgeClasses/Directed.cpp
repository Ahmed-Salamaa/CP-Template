// Topic: Edge Classes Directed - Classifies edges into tree, back, forward, and cross edges in a directed graph
#include "../../../core.h"

struct DirectedEdgeClasses {
    // Time Complexity: O(V + E)
    // Space Complexity: O(V + E)
    int n, timer;
    vector<vector<int>> adj;
    vector<int> tin, tout;
    vector<bool> vis;

    DirectedEdgeClasses(int n) : n(n), timer(0), adj(n), tin(n, 0), tout(n, 0), vis(n, false) {}

    void add_edge(int u, int v) {
        adj[u].push_back(v);
    }

    void dfs(int u, int p = -1) {
        vis[u] = true;
        tin[u] = ++timer;
        for (int v : adj[u]) {
            if (!vis[v]) {
                // tree edge
                dfs(v, u);
            } else {
                if (tin[v] < tin[u] && tout[v] == 0) {
                    // back edge
                } else if (tin[v] > tin[u] && tout[v] != 0) {
                    // forward edge
                } else if (tin[v] < tin[u] && tout[v] != 0) {
                    // cross edge
                }
            }
        }
        tout[u] = ++timer;
    }

    void build() {
        for (int i = 0; i < n; i++) {
            if (!vis[i]) dfs(i);
        }
    }
};
