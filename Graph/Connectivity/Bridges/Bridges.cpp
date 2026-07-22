/*
 * Topic: Bridges
 * Description: An algorithm for identifying bridges (cut edges) in a graph, which are edges
 *   whose removal increases the number of connected components. This is vital for
 *   analyzing network vulnerabilities and single points of failure.
 * Usage:
 *    Bridges solver(N);
 *    solver.add_edge(u, v);
 *    solver.solve();
 */
#include "../../../core.h"

// Time Complexity: O(V + E)
// Space Complexity: O(V + E)
template <typename T = int>
struct Bridges {
    T dfs_timer;
    vector<vector<T>> adj;
    vector<pair<T, T>> bridges;
    vector<T> dfs_num, dfs_low, vis;
    vector<T> comp_id;
    vector<vector<T>> comp_adj;
    T comp_cnt = 0;

    Bridges(T n) {
        dfs_timer = 0;
        adj = vector<vector<T>>(n + 1);
        dfs_num = dfs_low = vis = vector<T>(n + 1);
    }

    void add_edge(T u, T v, bool bidirectional = true) {
        adj[u].push_back(v);
        if (bidirectional) adj[v].push_back(u);
    }

    void tarjan(T node, T parent) {
        dfs_num[node] = dfs_low[node] = ++dfs_timer;
        vis[node] = 1;
        for (auto child : adj[node]) {
            if (child == parent) continue;
            if (!dfs_num[child]) {
                tarjan(child, node);
                dfs_low[node] = min(dfs_low[node], dfs_low[child]);
                if (dfs_low[child] > dfs_num[node]) { bridges.push_back({node, child}); }
            } else if (vis[child]) {
                dfs_low[node] = min(dfs_low[node], dfs_num[child]);
            }
        }
    }

    void solve() {
        for (T i = 1; i < (T)sz(adj); ++i)
            if (!dfs_num[i]) tarjan(i, -1);
    }

    void make_scc_graph() {
        T n = (T)sz(adj) - 1;
        comp_id.assign(n + 1, 0);
        comp_cnt = 0;
        set<pair<T, T>> is_bridge;
        for (auto& e : bridges) { is_bridge.insert({min(e.first, e.second), max(e.first, e.second)}); }
        for (T i = 1; i <= n; ++i) {
            if (comp_id[i]) continue;
            ++comp_cnt;
            stack<T> st;
            st.push(i);
            comp_id[i] = comp_cnt;
            while (!st.empty()) {
                T u = st.top();
                st.pop();
                for (T v : adj[u]) {
                    if (is_bridge.count({min(u, v), max(u, v)})) continue;
                    if (!comp_id[v]) {
                        comp_id[v] = comp_cnt;
                        st.push(v);
                    }
                }
            }
        }
        comp_adj.assign(comp_cnt + 1, vector<T>());
        set<pair<T, T>> used;
        for (auto& e : bridges) {
            T u = e.first, v = e.second;
            T cu = comp_id[u], cv = comp_id[v];
            if (cu == cv) continue;
            T a = min(cu, cv), b = max(cu, cv);
            if (used.insert({a, b}).second) {
                comp_adj[a].push_back(b);
                comp_adj[b].push_back(a);
            }
        }
    }
};
