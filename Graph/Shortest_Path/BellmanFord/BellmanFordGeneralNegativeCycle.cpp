/*
 * Topic: Bellman-Ford (General Negative Cycle)
 * Description: Bellman-Ford variation to detect if ANY negative weight cycle exists in the
 *   graph, regardless of reachability from a source.
 * it. Usage: BellmanFordGeneralNegativeCycle bf(n); bf.addEdge(u, v, w); vector<int> cycle = bf.getNegativeCycle(); if
 * (!cycle.empty()) { ... }
 */
#include "../../../core.h"

struct BellmanFordGeneralNegativeCycle {
    struct Edge {
        int a, b;
        int cost;
    };

    int n;
    vector<Edge> edges;

    BellmanFordGeneralNegativeCycle(int nodes) : n(nodes) {}

    void addEdge(int u, int v, int w) { edges.push_back({u, v, w}); }

    // Time Complexity: O(V * E)
    // Space Complexity: O(V)
    vector<int> getNegativeCycle() {
        vector<int> d(n, 0);  // 0 allows finding disconnected components negative cycles
        vector<int> p(n, -1);
        int x = -1;
        for (int i = 0; i < n; ++i) {
            x = -1;
            for (const Edge& e : edges) {
                if (d[e.a] + e.cost < d[e.b]) {
                    d[e.b] = max(-2000000000000000000LL, d[e.a] + e.cost);  // avoid underflow
                    p[e.b] = e.a;
                    x = e.b;
                }
            }
        }
        if (x == -1) {
            return {-1};
        } else {
            for (int i = 0; i < n; ++i) x = p[x];
            vector<int> cycle;
            for (int v = x;; v = p[v]) {
                cycle.push_back(v);
                if (v == x && sz(cycle) > 1) break;
            }
            reverse(all(cycle));
            return cycle;
        }
    }
};
