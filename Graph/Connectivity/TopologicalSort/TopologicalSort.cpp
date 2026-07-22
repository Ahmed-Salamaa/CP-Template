/*
 * Topic: TopologicalSort
 * Description: Topological Sort linearly orders the vertices of a Directed Acyclic Graph (DAG)
 *   such that every directed edge goes from earlier to later vertices. It is crucial
 *   for scheduling tasks with dependencies, resolving build orders, and course
 *   prerequisites.
 * Usage:
 *    TopologicalSort solver(N);
 *    solver.add_edge(u, v);
 *    solver.dfs(1);
 */
#include "../../../core.h"

// Time Complexity: O(V + E)
// Space Complexity: O(V + E)
struct TopologicalSort {
    int n;
    vector<vector<int>> adj;
    vector<int> vis, ans;
    bool bad;

    TopologicalSort(int n) : n(n), adj(n + 1), vis(n + 1) {}

    void add_edge(int u, int v) { adj[u].push_back(v); }

    void dfs(int v) {
        vis[v] = 1;
        for (int u : adj[v]) {
            if (vis[u] == 0)
                dfs(u);
            else if (vis[u] == 1)
                bad = true;
        }
        vis[v] = 2;
        ans.push_back(v);
    }

    bool sort() {
        fill(all(vis), 0);
        ans.clear();
        bad = false;
        for (int i = 1; i <= n; ++i)
            if (!vis[i]) dfs(i);
        reverse(all(ans));
        return !bad;
    }
};
