/*
 * Topic: Tarjan Bridges
 * Description: Tarjan's linear-time DFS-based algorithm specifically tailored for finding
 *   bridges by comparing discovery times and lowest reachable times (low-link
 *   values). Highly efficient for static undirected graphs.
 * Usage:
 *    TarjanBridges solver(N);
 *    solver.add_edge(u, v);
 *    solver.build();
 */
#include "../../../core.h"

struct TarjanBridges {
    // Time Complexity: O(V + E)
    // Space Complexity: O(V + E)
    int n, ndfn;
    vector<vector<int>> adj;
    vector<int> dfn, LowLink;
    vector<pair<int, int>> bridges;

    TarjanBridges(int n) : n(n), ndfn(0), adj(n), dfn(n, -1), LowLink(n, 0) {}

    void add_edge(int u, int v) {
        adj[u].push_back(v);
        adj[v].push_back(u);
    }

    void tarjan(int node, int parent) {
        dfn[node] = LowLink[node] = ndfn++;
        for (auto& val : adj[node]) {
            if (dfn[val] == -1) {
                tarjan(val, node);
                LowLink[node] = min(LowLink[node], LowLink[val]);
            } else if (parent != val) {
                LowLink[node] = min(LowLink[node], dfn[val]);
            }
        }
        if (LowLink[node] == dfn[node] && ~parent) { bridges.emplace_back(parent, node); }
    }

    void build() {
        for (int i = 0; i < n; i++) {
            if (dfn[i] == -1) { tarjan(i, -1); }
        }
    }
};
