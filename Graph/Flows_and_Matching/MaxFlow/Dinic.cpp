/*
 * Topic: Dinic's Algorithm
 * Description: Dinic's Algorithm computes the Maximum Flow in a network in O(V^2 E) time using
 *   level graphs and blocking flows. Extremely fast in practice.
 * Usage:
 *    Dinic dinic(n);
 *    dinic.add_edge(u, v, cap);
 *    int max_flow = dinic.max_flow(s, t);
 */
#include "../../../core.h"

// Time Complexity: O(V^2 E) for general graphs, O(E * sqrt(V)) for bipartite
// Space Complexity: O(V + E)
struct Dinic {
    struct Edge {
        int to, rev;
        int flow, w;
        int id;
    };

    int n, s, t, mxid;
    vector<int> d, flow_through;
    vector<int> done;
    vector<vector<Edge>> g;

    Dinic(int _n) {
        n = _n + 10;
        mxid = 0;
        g.resize(n);
    }

    void add_edge(int u, int v, int w, int id = -1) {
        Edge a = {v, (int)g[v].size(), 0, w, id};
        Edge b = {u, (int)g[u].size(), 0, 0, -2};  // for bidirectional edges cap(b) = w
        g[u].push_back(a);
        g[v].push_back(b);
        mxid = max(mxid, id);
    }

    bool bfs() {
        d.assign(n, -1);
        d[s] = 0;
        queue<int> q;
        q.push(s);
        while (!q.empty()) {
            int u = q.front();
            q.pop();
            for (auto& e : g[u]) {
                int v = e.to;
                if (d[v] == -1 && e.flow < e.w) {
                    d[v] = d[u] + 1;
                    q.push(v);
                }
            }
        }
        return d[t] != -1;
    }

    int dfs(int u, int flow) {
        if (u == t) return flow;
        for (int& i = done[u]; i < (int)g[u].size(); i++) {
            Edge& e = g[u][i];
            if (e.w <= e.flow) continue;
            int v = e.to;
            if (d[v] == d[u] + 1) {
                int nw = dfs(v, min(flow, e.w - e.flow));
                if (nw > 0) {
                    e.flow += nw;
                    g[v][e.rev].flow -= nw;
                    return nw;
                }
            }
        }
        return 0;
    }

    int max_flow(int _s, int _t) {
        s = _s;
        t = _t;
        int flow = 0;
        int inf = 1e18;
        while (bfs()) {
            done.assign(n, 0);
            while (int nw = dfs(s, inf)) flow += nw;
        }
        flow_through.assign(mxid + 10, 0);
        for (int i = 0; i < n; i++)
            for (auto e : g[i])
                if (e.id >= 0) flow_through[e.id] = e.flow;
        return flow;
    }

    vector<bool> vis;

    void mark_reachable(int u) {
        vis[u] = true;
        for (auto& e : g[u]) {
            if (!vis[e.to] && e.flow < e.w)  // residual capacity exists
                mark_reachable(e.to);
        }
    }

    // Time Complexity: O(V + E) after max flow
    // Space Complexity: O(V + E)
    vector<pair<int, int>> get_min_cut() {
        vis.assign(n, false);
        mark_reachable(s);
        vector<pair<int, int>> cut;
        for (int u = 0; u < n; ++u) {
            if (!vis[u]) continue;
            for (auto& e : g[u]) {
                if (!vis[e.to] && e.w > 0) { cut.emplace_back(u, e.to); }
            }
        }
        return cut;
    }

    // Time Complexity: O(V + E) typically
    // Space Complexity: O(V + E)
    vector<vector<int>> get_paths() {
        vector<vector<int>> res;
        while (1) {
            vector<int> path;
            int u = s;
            path.push_back(u);
            while (u != t) {
                bool found = false;
                for (auto& e : g[u]) {
                    if (e.flow > 0) {
                        e.flow--;
                        u = e.to;
                        path.push_back(u);
                        found = true;
                        break;
                    }
                }
                if (!found) break;
            }
            if (path.back() != t) break;
            res.push_back(path);
        }
        return res;
    }
};
