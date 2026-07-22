/*
 * Topic: Kosaraju SCC
 * Description: Kosaraju's algorithm is a classic two-pass DFS approach for finding Strongly
 *   Connected Components in a directed graph. While slightly slower than Tarjan's
 *   due to two traversals, it is highly intuitive and relies on transposing the
 *   graph.
 * Usage:
 *    KosarajuSCC solver(N);
 *    solver.add_edge(u, v);
 *    solver.build();
 */
#include "../../../core.h"

struct KosarajuSCC {
    // Time Complexity: O(V + E)
    // Space Complexity: O(V + E)
    int n;
    vector<vector<int>> adj, adj_rev, comps;
    vector<int> order, component;
    vector<bool> used;

    KosarajuSCC(int n) : n(n), adj(n), adj_rev(n), used(n, false) {}

    void add_edge(int u, int v) {
        adj[u].push_back(v);
        adj_rev[v].push_back(u);
    }

    void dfs1(int v) {
        used[v] = true;
        for (auto u : adj[v])
            if (!used[u]) dfs1(u);
        order.push_back(v);
    }

    void dfs2(int v) {
        used[v] = true;
        component.push_back(v);
        for (auto u : adj_rev[v])
            if (!used[u]) dfs2(u);
    }

    void build() {
        for (int i = 0; i < n; i++) {
            if (!used[i]) { dfs1(i); }
        }
        fill(all(used), false);
        reverse(all(order));
        for (auto v : order) {
            if (!used[v]) {
                dfs2(v);
                comps.emplace_back(component);
                component.clear();
            }
        }
    }
};
