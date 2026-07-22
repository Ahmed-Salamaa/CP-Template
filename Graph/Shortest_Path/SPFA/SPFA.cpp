/*
 * Topic: Shortest Path Faster Algorithm (SPFA)
 * Description: Shortest Path Faster Algorithm (SPFA) is an improvement over Bellman-Ford using
 *   a queue to relax only the active nodes. It effectively handles graphs with
 *   negative weights and is generally faster on average for sparse graphs, though
 *   its worst-case remains O(VE).
 * sparse graphs. Usage: SPFA<int> spfa(n); spfa.add_edge(u, v, w); vector<int> dist = spfa.run(source);
 *    // spfa.neg_cycle indicates if a negative cycle was detected
 */
#include "../../../core.h"

// Time Complexity: O(N * M) average, O(N * M) worst case
// Space Complexity: O(N + M)
template <typename T = int>
struct SPFA {
    struct Edge {
        int v;
        T w;

        Edge(int v, T w) : v(v), w(w) {}

        bool operator<(const Edge& e) const { return w < e.w; }
    };

    int n;
    bool neg_cycle = false;
    const T INF = 1LL << 60;
    vector<vector<Edge>> adj;
    vector<T> dist;

    // Time Complexity: O(N)
    // Space Complexity: O(N)
    SPFA(int n) : n(n), adj(n + 1) {}

    // Time Complexity: O(1)
    // Space Complexity: O(1) per edge
    void add_edge(int u, int v, T w, bool directed = true) {
        adj[u].push_back(Edge(v, w));
        if (!directed) adj[v].push_back(Edge(u, w));
    }

    // Time Complexity: O(N * M)
    // Space Complexity: O(N)
    void calc(int src) {
        dist.assign(n + 1, INF);
        queue<int> q;
        vector<int> cnt(n + 1, 0), in_queue(n + 1, 0);
        dist[src] = 0;
        q.push(src);
        in_queue[src] = 1;
        while (!q.empty()) {
            int u = q.front();
            q.pop();
            in_queue[u] = 0;
            for (const auto& e : adj[u]) {
                if (dist[u] + e.w < dist[e.v]) {
                    dist[e.v] = dist[u] + e.w;
                    if (!in_queue[e.v]) {
                        q.push(e.v);
                        in_queue[e.v] = 1;
                        if (++cnt[e.v] >= n) {
                            neg_cycle = true;
                            return;
                        }
                    }
                }
            }
        }
    }
};
