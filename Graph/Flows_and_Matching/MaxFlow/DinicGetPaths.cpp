/*
 * Topic: Dinic Get Paths
 * Description: An extension of Dinic's algorithm that reconstructs and extracts the individual
 *   flow paths from the source to the sink after computing the max flow.
 * Usage:
 *    DinicGetPaths<int> dinic(n);
 *    dinic.addEdge(u, v, cap);
 *    int max_flow = dinic.calc(s, t);
 *    // paths extraction logic (depends on implementation)
 */
#include "../../../core.h"

// Time Complexity: O(V^2 E) for max flow, path extraction takes additional time bounded by flow value
// Space Complexity: O(V + E)
template <typename T>
struct DinicGetPaths {
    struct Edge {
        int to, rev;
        T c, oc;

        T flow() const { return oc - c; }
    };

    int n;
    vector<int> lvl, ptr, q;
    vector<vector<Edge>> adj;

    DinicGetPaths(int n) : n(n), lvl(n), ptr(n), q(n), adj(n) {}

    void addEdge(int a, int b, T c, T rcap = 0) {
        adj[a].push_back({b, (int)adj[b].size(), c, c});
        adj[b].push_back({a, (int)adj[a].size() - 1, rcap, rcap});
    }

    T dfs(int v, int t, T f) {
        if (v == t || f == 0) return f;
        for (int& i = ptr[v]; i < (int)adj[v].size(); i++) {
            Edge& e = adj[v][i];
            if (lvl[e.to] == lvl[v] + 1 && e.c > 0) {
                T pushed = dfs(e.to, t, min(f, e.c));
                if (pushed) {
                    e.c -= pushed;
                    adj[e.to][e.rev].c += pushed;
                    return pushed;
                }
            }
        }
        return 0;
    }

    T calc(int s, int t) {
        T flow = 0;
        q[0] = s;
        for (int L = 0; L <= 31; L++) {
            do {
                fill(all(lvl), 0);
                fill(all(ptr), 0);
                int qi = 0, qe = 0;
                q[qe++] = s;
                lvl[s] = 1;
                while (qi < qe && !lvl[t]) {
                    int v = q[qi++];
                    for (const Edge& e : adj[v]) {
                        if (!lvl[e.to] && (e.c >> (30 - L)) > 0) {
                            lvl[e.to] = lvl[v] + 1;
                            q[qe++] = e.to;
                        }
                    }
                }
                while (T pushed = dfs(s, t, numeric_limits<T>::max())) flow += pushed;
            } while (lvl[t]);
        }
        return flow;
    }

    vector<pair<vector<int>, T>> extract_paths(int s, int t) {
        vector<pair<vector<int>, T>> paths;
        vector<pair<int, int>> parent_edge(n);
        auto dfs2 = [&](auto& self, int v, vector<vector<pair<int, int>>>& g) -> T {
            if (v == t) return numeric_limits<T>::max();
            for (auto& pr : g[v]) {
                int u = pr.first, idx = pr.second;
                Edge& e = adj[v][idx];
                if (e.flow() <= 0) continue;
                if (parent_edge[u].first == -1) {
                    parent_edge[u] = {v, idx};
                    T pushed = self(self, u, g);
                    if (pushed > 0) { return min(pushed, e.flow()); }
                }
            }
            return 0;
        };
        while (true) {
            vector<vector<pair<int, int>>> g(n);
            for (int i = 0; i < n; i++) {
                for (int j = 0; j < (int)adj[i].size(); j++) {
                    if (adj[i][j].flow() > 0) { g[i].emplace_back(adj[i][j].to, j); }
                }
            }
            fill(all(parent_edge), make_pair(-1, -1));
            parent_edge[s] = {-2, -2};
            T pushed = dfs2(dfs2, s, g);
            if (pushed == 0) break;
            vector<int> path;
            int v = t;
            while (v != s) {
                auto [p, idx] = parent_edge[v];
                path.push_back(v);
                Edge& e = adj[p][idx];
                e.c += pushed;
                adj[v][e.rev].c -= pushed;
                v = p;
            }
            path.push_back(s);
            reverse(all(path));
            paths.emplace_back(path, pushed);
        }
        return paths;
    }
};
