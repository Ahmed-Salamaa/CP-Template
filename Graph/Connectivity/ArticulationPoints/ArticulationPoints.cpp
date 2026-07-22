/*
 * Topic: ArticulationPoints
 * Description: An algorithm to find articulation points (cut vertices) in a graph, meaning
 *   nodes whose removal disconnects the graph. Critical in identifying essential
 *   routing hubs or vulnerable servers in a network.
 * Usage:
 *    ArticulationPoints solver(N);
 *    solver.add_edge(u, v);
 *    solver.solve();
 */
#include "../../../core.h"

// Time Complexity: O(V + E)
// Space Complexity: O(V + E)
template <typename T = int>
struct ArticulationPoints {
    T dfs_timer;
    vector<T> dfs_num, dfs_low, vis;
    vector<vector<T>> adj;
    set<T> articulation_points;

    ArticulationPoints(T n) {
        dfs_timer = 0;
        adj = vector<vector<T>>(n + 1);
        dfs_num = dfs_low = vis = vector<T>(n + 1);
    }

    void add_edge(T u, T v) {
        adj[u].push_back(v);
        adj[v].push_back(u);
    }

    void tarjan(T node, T parent) {
        dfs_num[node] = dfs_low[node] = ++dfs_timer;
        vis[node] = 1;
        T children = 0;
        for (auto child : adj[node]) {
            if (child == parent) continue;
            if (!dfs_num[child]) {
                tarjan(child, node);
                dfs_low[node] = min(dfs_low[node], dfs_low[child]);
                if (dfs_low[child] >= dfs_num[node] && parent != -1) { articulation_points.insert(node); }
                ++children;
            } else if (vis[child]) {
                dfs_low[node] = min(dfs_low[node], dfs_num[child]);
            }
        }
        if (parent == -1 && children > 1) { articulation_points.insert(node); }
    }

    void solve() {
        for (T i = 1; i < (T)sz(adj); ++i)
            if (!dfs_num[i]) tarjan(i, -1);
    }
};
