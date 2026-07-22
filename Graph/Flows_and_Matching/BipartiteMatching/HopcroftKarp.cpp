/*
 * Topic: Hopcroft-Karp
 * Description: Hopcroft-Karp algorithm computes the Maximum Bipartite Matching in O(E sqrt(V))
 *   time, significantly faster than standard DFS matching.
 * Usage:
 *    HopcroftKarp hk(n, m);
 *    hk.add_edge(u, v); // 0-based indices: u in [0, n-1], v in [0, m-1]
 *    int max_match = hk.max_matching();
 */
#include "../../../core.h"

// Time Complexity: O(E * sqrt(V))
// Space Complexity: O(V + E)
struct HopcroftKarp {
    int n;
    vector<vector<int>> adj;
    vector<int> mu, mv, dist;

    HopcroftKarp(int n, int m) : n(n), adj(n), mu(n, -1), mv(m, -1), dist(n) {}

    void add_edge(int u, int v) { adj[u].push_back(v); }

    bool bfs() {
        queue<int> q;
        for (int u = 0; u < n; ++u) {
            if (mu[u] == -1) {
                dist[u] = 0;
                q.push(u);
            } else {
                dist[u] = -1;
            }
        }
        bool found = false;
        while (!q.empty()) {
            int u = q.front();
            q.pop();
            for (int v : adj[u]) {
                if (mv[v] == -1) {
                    found = true;
                } else if (dist[mv[v]] == -1) {
                    dist[mv[v]] = dist[u] + 1;
                    q.push(mv[v]);
                }
            }
        }
        return found;
    }

    bool dfs(int u) {
        for (int v : adj[u]) {
            if (mv[v] == -1 || (dist[mv[v]] == dist[u] + 1 && dfs(mv[v]))) {
                mu[u] = v;
                mv[v] = u;
                return true;
            }
        }
        dist[u] = -1;
        return false;
    }

    int max_matching() {
        int res = 0;
        while (bfs()) {
            for (int u = 0; u < n; ++u) {
                if (mu[u] == -1 && dfs(u)) { res++; }
            }
        }
        return res;
    }
};
