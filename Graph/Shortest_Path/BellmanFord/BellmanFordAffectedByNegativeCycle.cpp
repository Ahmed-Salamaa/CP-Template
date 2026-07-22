/*
 * Topic: Bellman-Ford (Affected Nodes)
 * Description: Bellman-Ford variation that finds all vertices whose shortest path distance is
 *   pushed to negative infinity due to being reachable from a negative cycle.
 * Usage:
 *    BellmanFordAffectedByNegativeCycle bf(n);
 *    bf.addEdge(u, v, weight);
 *    vector<int> affected = bf.solve(source);
 */
#include "../../../core.h"

struct BellmanFordAffectedByNegativeCycle {
    int n;
    vector<array<int, 3>> edges;

    BellmanFordAffectedByNegativeCycle(int nodes) : n(nodes) {}

    void addEdge(int u, int v, int w) { edges.push_back({u, v, w}); }

    // Time Complexity: O(V * E + V + E)
    // Space Complexity: O(V + E)
    vector<int> solve(int source) {
        const int INF = 1e18;
        vector<int> dist(n, INF);
        dist[source] = 0;
        for (int pass = 0; pass < n - 1; ++pass) {
            bool changed = false;
            for (auto& e : edges) {
                int u = (int)e[0], v = (int)e[1];
                int w = e[2];
                if (dist[u] < INF && dist[u] + w < dist[v]) {
                    dist[v] = dist[u] + w;
                    changed = true;
                }
            }
            if (!changed) break;
        }
        vector<int> ref = dist;
        for (int pass = 0; pass < n; ++pass) {
            for (auto& e : edges) {
                int u = (int)e[0], v = (int)e[1];
                int w = e[2];
                if (dist[u] < INF && dist[u] + w < dist[v]) { dist[v] = dist[u] + w; }
            }
        }
        vector<bool> neg_inf(n, false);
        queue<int> bfs;
        for (int i = 0; i < n; ++i) {
            if (ref[i] != dist[i]) {
                neg_inf[i] = true;
                bfs.push(i);
            }
        }
        vector<vector<int>> adj(n);
        for (auto& e : edges) {
            int u = (int)e[0], v = (int)e[1];
            adj[u].push_back(v);
        }
        while (!bfs.empty()) {
            int u = bfs.front();
            bfs.pop();
            for (int nxt : adj[u]) {
                if (!neg_inf[nxt]) {
                    neg_inf[nxt] = true;
                    bfs.push(nxt);
                }
            }
        }
        vector<int> nodes;
        for (int i = 0; i < n; ++i) {
            if (neg_inf[i]) { nodes.push_back(i); }
        }
        return nodes;
    }
};
