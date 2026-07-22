/*
 * Topic: Tarjan
 * Description: Tarjan's strongly connected components algorithm is a fast DFS-based technique
 *   that identifies all SCCs in a directed graph in a single pass. It efficiently
 *   groups nodes that are mutually reachable, often used to compress graphs into
 *   DAGs.
 * Usage:
 *    Tarjan solver(N);
 *    solver.add_edge(u, v);
 *    solver.dfs(1);
 */
#include "../../../core.h"

// Time Complexity: O(V + E)
// Space Complexity: O(V + E)
template <typename T = int>
struct Tarjan {
    int n, timer = 0, scc_count = 0;
    vector<vector<T>> adj;
    vector<int> dfs_num, dfs_low, comp;
    vector<bool> in_stack;
    stack<T> st;
    vector<vector<T>> scc, dag;

    Tarjan(int nodes) {
        n = nodes;
        adj.assign(n + 1, {});
        dfs_num.assign(n + 1, 0);
        dfs_low.assign(n + 1, 0);
        in_stack.assign(n + 1, false);
        comp.assign(n + 1, 0);
    }

    void add_edge(T u, T v) { adj[u].push_back(v); }

    void dfs(T u) {
        dfs_num[u] = dfs_low[u] = ++timer;
        st.push(u);
        in_stack[u] = true;
        for (T v : adj[u]) {
            if (!dfs_num[v]) {
                dfs(v);
                dfs_low[u] = min(dfs_low[u], dfs_low[v]);
            } else if (in_stack[v]) {
                dfs_low[u] = min(dfs_low[u], dfs_num[v]);
            }
        }
        if (dfs_low[u] == dfs_num[u]) {
            scc_count++;
            scc.push_back({});
            while (true) {
                T v = st.top();
                st.pop();
                in_stack[v] = false;
                comp[v] = scc_count;
                scc.back().push_back(v);
                if (v == u) break;
            }
        }
    }

    void build_scc() {
        for (int i = 1; i <= n; i++)
            if (!dfs_num[i]) dfs(i);
    }

    void build_dag() {
        dag.assign(scc_count + 1, {});
        set<pair<int, int>> used;
        for (int u = 1; u <= n; u++) {
            for (auto v : adj[u]) {
                int cu = comp[u], cv = comp[v];
                if (cu != cv && !used.count({cu, cv})) {
                    dag[cu].push_back(cv);
                    used.insert({cu, cv});
                }
            }
        }
    }
};
