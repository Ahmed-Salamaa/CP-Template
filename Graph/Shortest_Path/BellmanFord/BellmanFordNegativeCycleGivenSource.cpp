/*
 * Topic: Bellman-Ford (Negative Cycle Reachable from Source)
 * Description: Bellman-Ford variation to detect if a negative weight cycle is reachable from a
 *   specific source vertex.
 * Usage:
 *    BellmanFordNegativeCycleGivenSource bf(n);
 *    bf.addEdge(u, v, w);
 *    vector<int> cycle = bf.getNegativeCycle(source);
 */
#include "../../../core.h"

struct BellmanFordNegativeCycleGivenSource {
    struct Edge {
        int a, b;
        int cost;
    };

    int n;
    vector<Edge> edges;

    BellmanFordNegativeCycleGivenSource(int nodes) : n(nodes) {}

    void addEdge(int u, int v, int w) { edges.push_back({u, v, w}); }

    // Time Complexity: O(V * E)
    // Space Complexity: O(V)
    vector<int> getNegativeCycle(int source) {
        vector<int> d(n, 2e18);
        d[source] = 0;
        vector<int> p(n, -1);
        int x = -1;
        for (int i = 0; i < n; ++i) {
            x = -1;
            for (const Edge& e : edges) {
                if (d[e.a] < 2e18) {
                    if (d[e.b] > d[e.a] + e.cost) {
                        d[e.b] = max(-2000000000000000000LL, d[e.a] + e.cost);
                        p[e.b] = e.a;
                        x = e.b;
                    }
                }
            }
        }
        if (x == -1) {
            return {-1};
        } else {
            int y = x;
            for (int i = 0; i < n; ++i) y = p[y];
            vector<int> path;
            for (int cur = y;; cur = p[cur]) {
                path.push_back(cur);
                if (cur == y && sz(path) > 1) break;
            }
            reverse(all(path));
            return path;
        }
    }
};
