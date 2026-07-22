/*
 * Topic: Can Exist In Max Matching
 * Description: Determines whether a specific edge can be part of ANY valid Maximum Bipartite
 *   Matching in the graph.
 * Tarjan's SCC. Usage: CanExistInMaxMatching solver(n, m); solver.add_edge(u, v); solver.solve(); bool possible =
 * solver.is_possible(u, v);
 */
#include "../../../core.h"

// Time Complexity: O(E * sqrt(V)) for matching + O(V + E) for Tarjan SCC
// Space Complexity: O(V + E)
struct CanExistInMaxMatching {
    int n, m, snt, dnt, top;
    vector<int> scc, dfn, low, st;
    vector<bool> vis;
    vector<int> ma, lk;
    vector<vector<int>> g, g2;

    CanExistInMaxMatching(int n, int m) : n(n), m(m) {
        int max_nodes = n + m + 10;
        scc.assign(max_nodes, 0);
        dfn.assign(max_nodes, 0);
        low.assign(max_nodes, 0);
        st.assign(max_nodes, 0);
        vis.assign(max_nodes, false);
        ma.assign(max_nodes, 0);
        lk.assign(max_nodes, 0);
        g.assign(n + 1, {});
        g2.assign(max_nodes, {});
        snt = dnt = top = 0;
    }

    bool dfs(int x) {
        for (int i : g[x]) {
            if (!vis[i]) {
                vis[i] = true;
                if (ma[i] == 0 || dfs(ma[i])) {
                    ma[i] = x;
                    return true;
                }
            }
        }
        return false;
    }

    void tarjan(int u) {
        dfn[u] = low[u] = ++dnt;
        st[top++] = u;
        vis[u] = true;
        for (int i : g2[u]) {
            if (!dfn[i]) {
                tarjan(i);
                low[u] = min(low[u], low[i]);
            } else if (vis[i]) {
                low[u] = min(low[u], dfn[i]);
            }
        }
        if (low[u] == dfn[u]) {
            for (int v = -1; v != u;) {
                v = st[--top];
                vis[v] = false;
                scc[v] = u;
            }
        }
    }

    vector<vector<int>> solve(const vector<pair<int, int>>& relations) {
        for (auto& [l, r] : relations) { g[l].push_back(r); }
        for (int i = 1; i <= n; i++) {
            fill(all(vis), false);
            dfs(i);
        }
        for (int i = 1; i <= m; i++) lk[ma[i]] = i;
        for (int i = 1; i <= n; i++) {
            g2[0].push_back(i);
            for (int j : g[i]) g2[i].push_back(n + j);
        }
        for (int i = 1; i <= m; i++) {
            g2[n + i].push_back(ma[i]);
            g2[n + i].push_back(n + m + 1);
        }
        for (int i = 1; i <= n; i++) {
            if (!lk[i]) g2[m + n + 1].push_back(i);
        }
        fill(all(vis), false);
        for (int i = 1; i <= n + m + 1; i++) {
            if (!dfn[i]) tarjan(i);
        }
        vector<vector<int>> ans(n + 1);
        for (int i = 1; i <= n; ++i) {
            for (int j : g[i]) {
                if (scc[i] == scc[n + j]) ans[i].push_back(j);
            }
            sort(ans[i].begin(), ans[i].end());
        }
        return ans;
    }
};
