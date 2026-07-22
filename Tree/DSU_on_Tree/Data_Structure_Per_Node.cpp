/*
 * Topic: Tree - DSU on Tree
 * Description: DSU on Tree (Data Structure Per Node) is an offline technique that merges data
 *   structures from child nodes into parent nodes to efficiently answer subtree
 *   queries.
 */
#include "../../core.h"

struct Data_Structure_Per_Node {
    int n;
    vector<vector<int>> adj;
    vector<int> bg, sz, ans;

    Data_Structure_Per_Node(int n) : n(n), adj(n + 5), bg(n + 5, -1), sz(n + 5, 0), ans(n + 5, 0) {}

    void add_edge(int u, int v) {
        adj[u].push_back(v);
        adj[v].push_back(u);
    }

    void dfs1(int u, int p) {
        bg[u] = -1;
        sz[u] = 1;
        for (auto& v : adj[u]) {
            if (v == p) continue;
            dfs1(v, u);
            sz[u] += sz[v];
            if (bg[u] == -1 || sz[bg[u]] < sz[v]) { bg[u] = v; }
        }
    }

    void dfs2(int u, int p) {
        for (auto& v : adj[u]) {
            if (v == p) continue;
            dfs2(v, u);
        }
        if (bg[u] != -1) {
            // swap your data with your big child : swap(freq[u], freq[bg[u]])
        }

        // insert yourself : freq[c[u]]++

        for (auto& v : adj[u]) {
            if (v == p || v == bg[u]) continue;
            // merge your data structure with your child data structure
        }
        // calculate answer
    }
};
