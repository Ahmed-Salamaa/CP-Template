/*
 * Topic: Bellman-Ford (Get Path)
 * Description: Bellman-Ford algorithm extended to reconstruct the shortest path to a specific
 *   target.
 * Usage:
 *    BellmanFordGetPath bf(n);
 *    bf.addEdge(u, v, w);
 *    vector<int> path = bf.solve(source, goal);
 */
#include "../../../core.h"

struct BellmanFordGetPath {
    struct Edge {
        int a, b;
        int cost;
    };

    int n;
    vector<Edge> edges;

    BellmanFordGetPath(int nodes) : n(nodes) {}

    void addEdge(int u, int v, int w) { edges.push_back({u, v, w}); }

    // Time Complexity: O(V * E)
    // Space Complexity: O(V)
    vector<int> solve(int source, int goal) {
        vector<int> d(n, 2e18);
        vector<int> p(n, -1);
        d[source] = 0;
        for (int i = 0; i < n - 1; i++) {
            bool any = false;
            for (const Edge& e : edges) {
                if (d[e.a] < 2e18) {
                    if (d[e.b] > d[e.a] + e.cost) {
                        d[e.b] = d[e.a] + e.cost;
                        p[e.b] = e.a;
                        any = true;
                    }
                }
            }
            if (!any) break;
        }
        if (d[goal] == 2e18) {
            return {-1};
        } else {
            vector<int> path;
            for (int cur = goal; cur != -1; cur = p[cur]) path.push_back(cur);
            reverse(all(path));
            return path;
        }
    }
};
