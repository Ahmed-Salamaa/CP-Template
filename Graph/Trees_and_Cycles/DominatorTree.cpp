/*
 * Topic: Graph - Trees and Cycles
 * Description: Builds a Dominator Tree for a directed graph.
 */
#include "../../core.h"

// Time Complexity: O((V + E) log V)
// Space Complexity: O(V + E)
struct DominatorTree {
    int n, T;
    std::vector<std::vector<int>> g, rg, bucket, domTree;
    std::vector<int> sdom, par, idom, dsu, label, pdom, id, rev_;

    DominatorTree(int n) : n(n), T(0), g(n + 1), rg(n + 1), bucket(n + 1), domTree(n + 1),
                           sdom(n + 1), par(n + 1), idom(n + 1), dsu(n + 1), label(n + 1),
                           pdom(n + 1), id(n + 1, 0), rev_(n + 1, 0) {}

    void add_edge(int u, int v) {
        g[u].push_back(v);
    }

    // Time Complexity: O(log V) on average
    // Space Complexity: O(log V) for recursion depth
    int find_(int u, int x = 0) {
        if (u == dsu[u]) return x ? -1 : u;
        int v = find_(dsu[u], x + 1);
        if (v < 0) return u;
        if (sdom[label[dsu[u]]] < sdom[label[u]]) label[u] = label[dsu[u]];
        dsu[u] = v;
        return x ? v : label[u];
    }

    // Time Complexity: O(V + E)
    // Space Complexity: O(V)
    void dfs(int u) {
        id[u] = ++T;
        rev_[T] = u;
        label[T] = sdom[T] = dsu[T] = T;
        for (int v : g[u]) {
            if (!id[v]) {
                dfs(v);
                par[id[v]] = id[u];
            }
            if (id[v]) rg[id[v]].push_back(id[u]);
        }
    }

    // Time Complexity: O((V + E) log V)
    // Space Complexity: O(V + E)
    void build(int root) {
        dfs(root);
        int valid_nodes = T;

        for (int i = valid_nodes; i >= 1; --i) {
            for (int v : rg[i]) {
                int u = find_(v);
                sdom[i] = std::min(sdom[i], sdom[u]);
            }
            if (i > 1) bucket[sdom[i]].push_back(i);

            for (int w : bucket[i]) {
                int v = find_(w);
                if (sdom[v] == sdom[w]) idom[w] = sdom[w];
                else idom[w] = v;
            }

            if (i > 1) dsu[i] = par[i];
        }
        pdom[root] = root;
        for (int i = 2; i <= valid_nodes; ++i) {
            if (idom[i] != sdom[i]) idom[i] = idom[idom[i]];
            int u = rev_[idom[i]];
            int v = rev_[i];
            pdom[v] = u;
            domTree[u].push_back(v);
            domTree[v].push_back(u);
        }
    }
};
