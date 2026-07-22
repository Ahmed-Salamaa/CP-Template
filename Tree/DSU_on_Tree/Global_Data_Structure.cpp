/*
 * Topic: Tree - DSU on Tree
 * Description: DSU on Tree (Global Data Structure) optimizes tree subtree queries by
 *   maintaining a single global data structure and intelligently adding or removing
 *   subtree elements.
 */
#include "../../core.h"

struct Global_Data_Structure {
    int n;
    vector<vector<int>> adj, nodes;
    vector<int> bg, sz, ans;

    Global_Data_Structure(int n) : n(n), adj(n + 5), nodes(n + 5), bg(n + 5, -1), sz(n + 5, 0), ans(n + 5, 0) {}

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

    void dfs2(int u, int p, bool keep) {
        for (auto v : adj[u]) {
            if (v != p && v != bg[u]) { dfs2(v, u, false); }
        }
        if (bg[u] != -1) {
            dfs2(bg[u], u, true);
            swap(nodes[bg[u]], nodes[u]);
        }
        nodes[u].emplace_back(u);
        // insert yourself in the data structure [only one global data structure]

        for (auto& v : adj[u]) {
            if (v == p || v == bg[u]) continue;
            for (auto& val : nodes[v]) {
                nodes[u].emplace_back(val);
                // insert node $val$ in the data structure
            }
        }
        // get your answer from data structure
        if (!keep) {
            for (auto& v : nodes[u]) {
                // erase node v from data structure
            }
        }
    }
};
