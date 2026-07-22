/*
 * Topic: Bellman-Ford (Basic)
 * Description: A basic, minimal implementation of the Bellman-Ford algorithm.
 * Usage:
 *    BellmanFordBasic bf(n);
 *    bf.addEdge(u, v, w);
 *    vector<int> distances = bf.solve(source);
 */
#include "../../../core.h"

struct BellmanFordBasic {
    struct Edge {
        int a, b;
        int cost;
    };

    int n;
    vector<Edge> edges;

    BellmanFordBasic(int nodes) : n(nodes) {}

    void addEdge(int u, int v, int w) { edges.push_back({u, v, w}); }

    // Time Complexity: O(V * E)
    // Space Complexity: O(V)
    vector<int> solve(int source) {
        vector<int> d(n, 2e18);
        d[source] = 0;
        for (int i = 0; i < n - 1; ++i) {
            for (const Edge& e : edges) {
                if (d[e.a] < 2e18) { d[e.b] = min(d[e.b], d[e.a] + e.cost); }
            }
        }
        return d;
    }
};
