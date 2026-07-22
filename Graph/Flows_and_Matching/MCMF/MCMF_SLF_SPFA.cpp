/*
 * Topic: MCMF SLF SPFA
 * Description: Min-Cost Max-Flow using SPFA with Small Label First (SLF) optimization to
 *   drastically speed up the shortest path finding in the residual graph.
 * practice. Usage: MCMF_SLF_SPFA mcmf(n, s, t); mcmf.add_edge(u, v, cap, cost); auto [flow, cost] =
 * mcmf.min_cost_max_flow();
 */
#include "../../../core.h"

// Time Complexity: O(F * V * E) (worst case), usually much faster with SLF
// Space Complexity: O(V + E)
struct MCMF_SLF_SPFA {
    struct Edge {
        int u, v, flow = 0, cap = 0, cost = 0;

        Edge(int u, int v, int c, int cost) : u(u), v(v), cap(c), cost(cost) {}

        int rem() const { return cap - flow; }
    };

    int n, s, t, cost = 0, flow = 0;
    int oo = 2e15;
    vector<Edge> edges;
    vector<vector<int>> adj;
    vector<int> from;

    MCMF_SLF_SPFA(int n, int s, int t) : n(n), s(s), t(t) { adj.assign(n + 1, {}); }

    void addEdge(int u, int v, int w = 2e15, int edge_cost = 0, int undir = 0) {
        adj[u].push_back(sz(edges));
        edges.push_back(Edge(u, v, w, edge_cost));
        adj[v].push_back(sz(edges));
        edges.push_back(Edge(v, u, w * undir, -edge_cost));
    }

    void move() {
        while (bfs()) {
            int addflow = oo;
            for (int u = t; u != s; u = edges[from[u]].u) addflow = min(addflow, edges[from[u]].rem());
            for (int u = t; u != s; u = edges[from[u]].u) {
                int i = from[u];
                edges[i].flow += addflow;
                edges[i ^ 1].flow -= addflow;
                cost += edges[i].cost * addflow;
            }
            flow += addflow;
        }
    }

    bool bfs() {
        from.assign(n + 1, -1);
        vector<int> d(n + 1, oo);
        vector<int> state(n + 1, 2);
        deque<int> q;
        d[s] = 0;
        state[s] = 1;
        q.push_back(s);
        while (!q.empty()) {
            int u = q.front();
            q.pop_front();
            state[u] = 0;
            for (auto& ei : adj[u]) {
                auto& e = edges[ei];
                int v = e.v;
                if (e.rem() <= 0 || d[v] <= d[u] + e.cost) continue;
                d[v] = d[u] + e.cost;
                from[v] = ei;
                if (state[v] == 1) continue;
                if (state[v] == 0 || (!q.empty() && d[q.front()] > d[v]))
                    q.push_front(v);
                else
                    q.push_back(v);
                state[v] = 1;
            }
        }
        return from[t] != -1;
    }
};
