/*
 * Topic: Tree - Centroid Decomposition
 * Description: Centroid Decomposition is a divide-and-conquer technique on trees that
 *   repeatedly removes the tree's centroid to recursively solve path-related queries
 *   in O(N log N) time.
 */
#include "../../core.h"

struct CentroidDecomposition {
    int n;
    vector<vector<int>> adj;
    vector<int> sz;
    vector<int> parent;
    vector<int> level;
    vector<bool> removed;
    int centroid_root = -1;

    int dfs_size(int u, int p) {
        sz[u] = 1;
        for (int v : adj[u]) {
            if (v == p || removed[v]) continue;
            sz[u] += dfs_size(v, u);
        }
        return sz[u];
    }

    int dfs_centroid(int u, int p, int total) {
        for (int v : adj[u]) {
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
        for (int v : adj[c]) {
            if (!removed[v]) build_cd(v, c, depth + 1);
        }
    }

    CentroidDecomposition(int n) : n(n) {
        adj.resize(n + 1);
        sz.resize(n + 1);
        parent.assign(n + 1, -1);
        level.resize(n + 1);
        removed.assign(n + 1, false);
        centroid_root = -1;
    }

    CentroidDecomposition(int n, const vector<vector<int>>& graph, int root = 1) : n(n), adj(graph) {
        sz.resize(n + 1);
        parent.assign(n + 1, -1);
        level.resize(n + 1);
        removed.assign(n + 1, false);
        centroid_root = -1;
        build(root);
    }

    void add_edge(int u, int v) {
        adj[u].push_back(v);
        adj[v].push_back(u);
    }

    void delete_edge(int u, int v) {
        auto itu = find(adj[u].begin(), adj[u].end(), v);
        if (itu != adj[u].end()) adj[u].erase(itu);
        auto itv = find(adj[v].begin(), adj[v].end(), u);
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

    int get_subtree_centroid(int x) {
        int total = dfs_size(x, -1);
        return dfs_centroid(x, -1, total);
    }
};
