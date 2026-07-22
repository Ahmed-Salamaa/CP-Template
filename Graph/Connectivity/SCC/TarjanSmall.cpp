/*
 * Topic: Tarjan SCC
 * Description: A compact and simplified version of Tarjan's Strongly Connected Components
 *   algorithm. It provides the same linear-time SCC decomposition but uses tighter,
 *   more competitive programming-friendly code structures.
 * Usage:
 *    TarjanSCC solver(N);
 *    solver.add_edge(u, v);
 *    solver.build();
 */
#include "../../../core.h"

struct TarjanSCC {
    // Time Complexity: O(V + E)
    // Space Complexity: O(V + E)
    int n, ndfn;
    vector<vector<int>> adj, dag, comps;
    vector<int> comp, inStack, lowLink, dfn, deg;
    stack<int> st;

    TarjanSCC(int n) : n(n), ndfn(0), adj(n), comp(n), inStack(n, 0), lowLink(n, 0), dfn(n, -1), deg(n, 0) {}

    void add_edge(int u, int v) { adj[u].push_back(v); }

    void tarjan(int u) {
        dfn[u] = lowLink[u] = ndfn++;
        inStack[u] = 1;
        st.push(u);
        for (auto& v : adj[u]) {
            if (dfn[v] == -1) {
                tarjan(v);
                lowLink[u] = min(lowLink[u], lowLink[v]);
            } else if (inStack[v]) {
                lowLink[u] = min(lowLink[u], dfn[v]);
            }
        }
        if (dfn[u] == lowLink[u]) {
            int x = -1;
            comps.emplace_back(vector<int>());
            while (x != u) {
                x = st.top();
                st.pop();
                inStack[x] = 0;
                comps.back().emplace_back(x);
                comp[x] = sz(comps) - 1;
            }
        }
    }

    void genDag() {
        dag.assign(sz(comps), vector<int>());
        for (int u = 0; u < n; u++) {
            for (auto& v : adj[u]) {
                if (comp[u] != comp[v]) {
                    dag[comp[u]].emplace_back(comp[v]);
                    deg[comp[v]]++;
                }
            }
        }
    }

    void build() {
        for (int i = 0; i < n; i++)
            if (dfn[i] == -1) tarjan(i);
        genDag();
    }
};
