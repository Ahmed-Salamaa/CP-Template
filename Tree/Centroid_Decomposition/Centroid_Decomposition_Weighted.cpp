/*
 * Topic: Tree - Centroid Decomposition
 * Description: Weighted Centroid Decomposition applies centroid decomposition to trees with
 *   edge weights, handling path distance queries and aggregate properties
 *   efficiently.
 */
#include "../../core.h"

struct CentroidDecompositionWeighted {
    int n;
    vector<vector<pair<int, int>>> adj;
    vector<int> sz;
    vector<int> parent;
    vector<int> level;
    vector<bool> removed;
    int centroid_root = -1;

    int dfs_size(int u, int p) {
        sz[u] = 1;
        for (auto& edge : adj[u]) {
            int v = edge.first;
            if (v == p || removed[v]) continue;
            sz[u] += dfs_size(v, u);
        }
        return sz[u];
    }

    int dfs_centroid(int u, int p, int total) {
        for (auto& edge : adj[u]) {
            int v = edge.first;
            if (v == p || removed[v]) continue;
            if (sz[v] > total / 2) return dfs_centroid(v, u, total);
        }
        return u;
    }

    void build_cd(int entry, int p, int depth) {
        int total = dfs_size(entry, -1);
        int c = dfs_centroid(entry, -1, total);
        parent[c] = p;
        level[c] = depth;
        removed[c] = true;
        if (p == -1) centroid_root = c;
        for (auto& edge : adj[c]) {
            int v = edge.first;
            if (!removed[v]) build_cd(v, c, depth + 1);
        }
    }

    CentroidDecompositionWeighted(int n) : n(n) {
        adj.resize(n + 1);
        sz.resize(n + 1);
        parent.assign(n + 1, -1);
        level.resize(n + 1);
        removed.assign(n + 1, false);
        centroid_root = -1;
    }

    CentroidDecompositionWeighted(int n, const vector<vector<pair<int, int>>>& graph, int root = 1) : n(n), adj(graph) {
        sz.resize(n + 1);
        parent.assign(n + 1, -1);
        level.resize(n + 1);
        removed.assign(n + 1, false);
        centroid_root = -1;
        build(root);
    }

    void add_edge(int u, int v, int w) {
        adj[u].push_back({v, w});
        adj[v].push_back({u, w});
    }

    void delete_edge(int u, int v) {
        auto itu = find_if(adj[u].begin(), adj[u].end(), [v](const pair<int, int>& edge) { return edge.first == v; });
        if (itu != adj[u].end()) adj[u].erase(itu);
        auto itv = find_if(adj[v].begin(), adj[v].end(), [u](const pair<int, int>& edge) { return edge.first == u; });
        if (itv != adj[v].end()) adj[v].erase(itv);
    }

    void build(int root = 1) {
        sz.assign(n + 1, 0);
        parent.assign(n + 1, -1);
        level.assign(n + 1, 0);
        removed.assign(n + 1, false);
        centroid_root = -1;
        build_cd(root, -1, 0);
    }

    int get_parent(int u) { return parent[u]; }

    vector<vector<int>> get_tree() {
        vector<vector<int>> new_graph(n + 1);
        for (int i = 1; i <= n; i++) {
            if (parent[i] != -1) {
                new_graph[i].push_back(parent[i]);
                new_graph[parent[i]].push_back(i);
            }
        }
        return new_graph;
    }

    int get_level(int u) { return level[u]; }

    bool is_ancestor(int u, int v) {
        while (v != -1) {
            if (v == u) return true;
            v = parent[v];
        }
        return false;
    }

    int get_root() const { return centroid_root; }
};
