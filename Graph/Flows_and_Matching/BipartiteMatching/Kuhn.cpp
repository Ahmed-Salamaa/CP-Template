/*
 * Topic: Kuhn's Algorithm
 * Description: Kuhn's Algorithm computes the Maximum Bipartite Matching using a simple DFS
 *   augmenting path approach in O(V E) time.
 * Usage:
 *    Kuhn solver(n, m);
 *    solver.add_edge(u, v);
 *    int max_match = solver.max_matching();
 */
#include "../../../core.h"

// Time Complexity: O(V * E)
// Space Complexity: O(V + E)
// Maximum Bipartite Matching
struct Kuhn {
    int n, m;
    vector<vector<int>> adj;
    vector<int> match;
    vector<bool> used;

    Kuhn(int _n, int _m) : n(_n), m(_m) {
        adj.resize(n + 1);
        match.assign(m + 1, -1);
        used.assign(n + 1, false);
    }

    void add_edge(int u, int v) { adj[u].push_back(v); }

    bool try_kuhn(int u) {
        if (used[u]) return false;
        used[u] = true;
        for (int v : adj[u]) {
            if (match[v] == -1 || try_kuhn(match[v])) {
                match[v] = u;
                return true;
            }
        }
        return false;
    }

    int maximum_matching() {
        int res = 0;
        for (int i = 1; i <= n; i++) {
            used.assign(n + 1, false);
            if (try_kuhn(i)) res++;
        }
        return res;
    }
};
