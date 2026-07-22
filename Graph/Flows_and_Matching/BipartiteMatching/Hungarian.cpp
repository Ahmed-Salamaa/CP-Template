/*
 * Topic: Hungarian Algorithm (Kuhn-Munkres)
 * Description: Hungarian algorithm (Kuhn-Munkres) solves the Assignment Problem (Min-Cost
 *   Perfect Bipartite Matching) in O(V^3) time.
 * negate all costs before adding edges and negate the returned answer. Usage: Hungarian solver(n1, n2);
 *    solver.add_edge(u, v, cost); // 1-based indexing for u and v
 *    int min_cost = solver.min_cost();
 *    // trace assignments via solver.l or solver.r
 */
#include "../../../core.h"

// Time Complexity: O(V^3)
// Space Complexity: O(V^2)
// Solves minimum cost maximum matching (assignment problem).
// For finding maximum cost matching, negate all costs before adding edges and negate the returned answer.
struct Hungarian {
    int inf = 1e18;
    int n, start, finish;
    vector<vector<int>> c;
    vector<int> fx, fy, d;
    vector<int> l, r, arg, trace;
    queue<int> q;

    Hungarian(int n1, int n2) {
        n = max(n1, n2);
        c.assign(n + 1, vector<int>(n + 1, inf));
        fx.assign(n + 1, 0);
        fy.assign(n + 1, 0);
        d.assign(n + 1, 0);
        l.assign(n + 1, 0);
        r.assign(n + 1, 0);
        arg.assign(n + 1, 0);
        trace.assign(n + 1, 0);
    }

    void add_edge(int u, int v, int cost) { c[u][v] = min(c[u][v], cost); }

    inline int getC(int u, int v) { return c[u][v] - fx[u] - fy[v]; }

    void initBFS() {
        while (!q.empty()) q.pop();
        q.push(start);
        for (int i = 0; i <= n; ++i) trace[i] = 0;
        for (int v = 1; v <= n; ++v) {
            d[v] = getC(start, v);
            arg[v] = start;
        }
        finish = 0;
    }

    void findAugPath() {
        while (!q.empty()) {
            int u = q.front();
            q.pop();
            for (int v = 1; v <= n; ++v)
                if (!trace[v]) {
                    int w = getC(u, v);
                    if (!w) {
                        trace[v] = u;
                        if (!r[v]) {
                            finish = v;
                            return;
                        }
                        q.push(r[v]);
                    }
                    if (d[v] > w) {
                        d[v] = w;
                        arg[v] = u;
                    }
                }
        }
    }

    void subX_addY() {
        int delta = inf;
        for (int v = 1; v <= n; ++v)
            if (trace[v] == 0 && d[v] < delta) delta = d[v];
        fx[start] += delta;
        for (int v = 1; v <= n; ++v)
            if (trace[v]) {
                int u = r[v];
                fy[v] -= delta;
                fx[u] += delta;
            } else {
                d[v] -= delta;
            }
        for (int v = 1; v <= n; ++v)
            if (!trace[v] && !d[v]) {
                trace[v] = arg[v];
                if (!r[v]) {
                    finish = v;
                    return;
                }
                q.push(r[v]);
            }
    }

    void Enlarge() {
        do {
            int u = trace[finish];
            int nxt = l[u];
            l[u] = finish;
            r[finish] = u;
            finish = nxt;
        } while (finish);
    }

    int maximum_matching() {
        for (int u = 1; u <= n; ++u) {
            fx[u] = c[u][1];
            for (int v = 1; v <= n; ++v) fx[u] = min(fx[u], c[u][v]);
        }
        for (int v = 1; v <= n; ++v) {
            fy[v] = c[1][v] - fx[1];
            for (int u = 1; u <= n; ++u) fy[v] = min(fy[v], c[u][v] - fx[u]);
        }
        for (int u = 1; u <= n; ++u) {
            start = u;
            initBFS();
            while (!finish) {
                findAugPath();
                if (!finish) subX_addY();
            }
            Enlarge();
        }
        int ans = 0;
        for (int i = 1; i <= n; ++i) {
            if (c[i][l[i]] != inf)
                ans += c[i][l[i]];
            else
                l[i] = 0;
        }
        return ans;
    }
};
