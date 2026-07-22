/*
 * Topic: Dinic (Short Template)
 * Description: A compact and slightly optimized implementation of Dinic's Algorithm for
 *   computing Maximum Flow.
 * Usage:
 *    Dinic<int> dinic(n);
 *    dinic.addEdge(u, v, cap);
 *    int max_flow = dinic.calc(s, t);
 */
#include "../../../core.h"

// Time Complexity: O(V^2 E), O(E sqrt(V)) for unit networks
// Space Complexity: O(V + E)
template <typename T>
struct Dinic {
    struct Edge {
        int to, rev;
        T c, oc;

        T flow() const { return oc - c; }
    };

    vector<int> lvl, ptr, q;
    vector<vector<Edge>> adj;

    Dinic(int n) : lvl(n), ptr(n), q(n), adj(n) {}

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

    bool leftOfMinCut(int a) const { return lvl[a] != 0; }
};
