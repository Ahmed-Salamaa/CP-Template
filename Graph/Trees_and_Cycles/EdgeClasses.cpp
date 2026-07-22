/*
 * Topic: Graph - Trees and Cycles
 * Description: Classifies edges in a graph using DFS traversal.
 */
#include "../../core.h"

namespace EdgeClasses {

// Time Complexity: O(V + E)
// Space Complexity: O(V + E)
struct DirectedEdgeClassifier {
    int n, timer;
    std::vector<std::vector<int>> adj;
    std::vector<int> tin, tout;
    std::vector<bool> vis;

    DirectedEdgeClassifier(int n) : n(n), timer(0), adj(n), tin(n, 0), tout(n, 0), vis(n, false) {}

    void add_edge(int u, int v) {
        adj[u].push_back(v);
    }

    // Time Complexity: O(V + E)
    // Space Complexity: O(V)
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
    
    // Time Complexity: O(V + E)
    // Space Complexity: O(V)
    void run() {
        for (int i = 0; i < n; ++i) {
            if (!vis[i]) dfs(i);
        }
    }
};

// Time Complexity: O(V + E)
// Space Complexity: O(V + E)
struct UndirectedEdgeClassifier {
    int n, timer;
    std::vector<std::vector<int>> adj;
    std::vector<int> tin, depth;
    std::vector<bool> vis;

    UndirectedEdgeClassifier(int n) : n(n), timer(0), adj(n), tin(n, 0), depth(n, 0), vis(n, false) {}

    void add_edge(int u, int v) {
        adj[u].push_back(v);
        adj[v].push_back(u);
    }

    // Time Complexity: O(V + E)
    // Space Complexity: O(V)
    void dfs(int u, int p = -1) {
        vis[u] = true;
        tin[u] = ++timer;
        depth[u] = (p == -1 ? 1 : depth[p] + 1);
        for (int v : adj[u]) {
            if (vis[v] && tin[u] < tin[v]) {
                // forward edge
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

    // Time Complexity: O(V + E)
    // Space Complexity: O(V)
    void run() {
        for (int i = 0; i < n; ++i) {
            if (!vis[i]) dfs(i);
        }
    }
};

} // namespace EdgeClasses
