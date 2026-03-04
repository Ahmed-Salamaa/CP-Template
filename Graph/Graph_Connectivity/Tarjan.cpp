#include <bits/stdc++.h>
using namespace std;
#include <bits/stdc++.h>
using namespace std;

/*
    Tarjan Algorithm
    ----------------
    - Finds Strongly Connected Components (SCC) in O(N + M)
    - Works for directed graphs
    - Also builds condensed DAG of components if needed
*/

template <typename T = int>
struct Tarjan {

    int n;                          // number of nodes
    int timer = 0;                  // DFS timer
    int scc_count = 0;              // number of SCCs found

    vector<vector<T>> adj;          // adjacency list
    vector<int> dfs_num;            // discovery time of node
    vector<int> dfs_low;            // lowest reachable discovery time
    vector<bool> in_stack;          // is node currently in stack
    vector<int> comp;               // component id for each node

    stack<T> st;                    // stack for Tarjan
    vector<vector<T>> scc;          // list of SCCs
    vector<vector<T>> dag;          // condensed graph (SCC DAG)

    // Constructor
    Tarjan(int nodes) {
        n = nodes;
        adj.assign(n + 1, {});
        dfs_num.assign(n + 1, 0);
        dfs_low.assign(n + 1, 0);
        in_stack.assign(n + 1, false);
        comp.assign(n + 1, 0);
    }

    // Add edge (directed by default)
    void add_edge(T u, T v) {
        adj[u].push_back(v);
    }

    // Core DFS function
    void dfs(T u) {

        dfs_num[u] = dfs_low[u] = ++timer;
        st.push(u);
        in_stack[u] = true;

        for (T v : adj[u]) {

            if (!dfs_num[v]) {
                // Tree edge
                dfs(v);
                dfs_low[u] = min(dfs_low[u], dfs_low[v]);
            }
            else if (in_stack[v]) {
                // Back edge
                dfs_low[u] = min(dfs_low[u], dfs_num[v]);
            }
        }

        // If u is root of SCC
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

    // Run Tarjan on all nodes
    void build_scc() {

        for (int i = 1; i <= n; i++)
            if (!dfs_num[i])
                dfs(i);
    }

    // Build condensed DAG of SCCs
    void build_dag() {

        dag.assign(scc_count + 1, {});
        set<pair<int,int>> used; // avoid multi edges

        for (int u = 1; u <= n; u++) {
            for (auto v : adj[u]) {

                int cu = comp[u];
                int cv = comp[v];

                if (cu != cv && !used.count({cu, cv})) {
                    dag[cu].push_back(cv);
                    used.insert({cu, cv});
                }
            }
        }
    }

    // Getters
    int get_scc_count() { return scc_count; }

    vector<int> get_component_array() { return comp; }

    vector<vector<T>> get_scc() { return scc; }

    vector<vector<T>> get_dag() { return dag; }
};