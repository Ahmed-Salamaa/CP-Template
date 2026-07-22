// Topic: Direct To Have DAG - Orients undirected edges such that the resulting directed graph is a DAG
#include "../../../core.h"

struct DirectToHaveDAG {
    // Time Complexity: O(V + E)
    // Space Complexity: O(V + E)
    int n, timer;
    vector<vector<int>> adj;
    vector<pair<int,int>> e;
    vector<int> tin;
    vector<bool> vis;

    DirectToHaveDAG(int n) : n(n), timer(0), adj(n), tin(n, 0), vis(n, false) {}

    void add_edge(int u, int v) {
        adj[u].push_back(v);
        adj[v].push_back(u);
        e.emplace_back(u, v);
    }

    void dfs(int u) {
        vis[u] = true;
        tin[u] = ++timer;
        for (int v : adj[u]) {
            if (!vis[v]) dfs(v);
        }
    }

    vector<pair<int,int>> get_dag_edges() {
        for (int i = 0; i < n; ++i) {
            if (!vis[i]) dfs(i);
        }
        vector<pair<int,int>> res;
        for (auto &ed : e) {
            int a = ed.first, b = ed.second;
            if (tin[a] < tin[b]) res.emplace_back(a, b);
            else res.emplace_back(b, a);
        }
        return res;
    }
};
