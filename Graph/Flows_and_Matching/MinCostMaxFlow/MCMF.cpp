/*
 * Topic: Minimum Cost Maximum Flow (MCMF)
 * Description: Min-Cost Max-Flow (MCMF) algorithm finds the maximum flow that incurs the
 *   absolute minimum cost, using Successive Shortest Path algorithm with SPFA or
 *   Dijkstra with potentials.
 * Usage:
 *    MCMF mcmf(n, s, t);
 *    mcmf.add_edge(u, v, cap, cost);
 *    auto [flow, cost] = mcmf.min_cost_max_flow();
 */
#include "../../../core.h"

// Time Complexity: O(F * V * E) in worst case (where F is the flow size)
// Space Complexity: O(V + E)
struct MCMF {
    struct Edge {
        int u, v;
        int flow, cap, cost;

        Edge(int u, int v, int c, int cost) : u(u), v(v), flow(0), cap(c), cost(cost) {}

        int rem() const { return cap - flow; }
    };

    int n, s, t;
    int cost = 0, flow = 0, need;
    vector<Edge> edges;
    vector<vector<int>> adj;
    vector<int> from;
    const int OO = 1e18;

    MCMF(int n, int s, int t, int need) : n(n), s(s), t(t), need(need) {
        adj.assign(n, {});
        from.assign(n, -1);
    }

    // undir = 1 for undirected edge with cost
    void add_edge(int u, int v, int w, int edge_cost, int undir = 0) {
        adj[u].push_back(sz(edges));
        edges.emplace_back(u, v, w, edge_cost);
        adj[v].push_back(sz(edges));
        edges.emplace_back(v, u, w * undir, -edge_cost);
    }

    bool bfs() {
        deque<int> q;
        vector<int> dist(n, OO);
        vector<int> inq(n, 0);
        fill(all(from), -1);
        dist[s] = 0;
        inq[s] = 1;
        q.push_back(s);
        while (!q.empty()) {
            int u = q.front();
            q.pop_front();
            inq[u] = 0;
            for (int ei : adj[u]) {
                auto& e = edges[ei];
                int v = e.v;
                if (e.rem() <= 0 || dist[v] <= dist[u] + e.cost) continue;
                dist[v] = dist[u] + e.cost;
                from[v] = ei;
                if (!inq[v]) {
                    inq[v] = 1;
                    if (!q.empty() && dist[v] < dist[q.front()])
                        q.push_front(v);
                    else
                        q.push_back(v);
                }
            }
        }
        return from[t] != -1;
    }

    void go() {
        while (flow < need && bfs()) {
            int u = t;
            int addf = OO;
            while (u != s) {
                Edge& e = edges[from[u]];
                addf = min(addf, e.rem());
                u = e.u;
            }
            if (flow + addf > need) addf = need - flow;
            u = t;
            while (u != s) {
                int ei = from[u];
                edges[ei].flow += addf;
                edges[ei ^ 1].flow -= addf;
                cost += edges[ei].cost * addf;
                u = edges[ei].u;
            }
            flow += addf;
        }
    }
};
