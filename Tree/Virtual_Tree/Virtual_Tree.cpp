/*
 * Topic: Tree - Virtual Tree
 * Description: Virtual Tree (Auxiliary Tree) reduces a tree to only a set of important nodes
 *   and their LCAs, preserving structural properties to answer queries in time
 *   proportional to the subset size.
 */
#include "../../core.h"

struct Virtual_Tree {
    int n, LOG;
    int timer;
    vector<int> in, out, dep;
    vector<vector<int>> up;
    vector<vector<int>> adj, virtual_tree;

    Virtual_Tree(int n) : n(n), LOG(20), timer(0) {
        in.assign(n + 5, 0);
        out.assign(n + 5, 0);
        dep.assign(n + 5, 0);
        up.assign(n + 5, vector<int>(LOG, 0));
        adj.assign(n + 5, vector<int>());
        virtual_tree.assign(n + 5, vector<int>());
    }

    void pre_dfs(int u, int p) {
        in[u] = ++timer;

        dep[u] = dep[p] + 1;
        up[u][0] = p;
        for (int x = 1; x < LOG; ++x) { up[u][x] = up[up[u][x - 1]][x - 1]; }

        for (auto& v : adj[u]) {
            if (v != p) { pre_dfs(v, u); }
        }

        out[u] = timer;
    }

    int lca(int u, int v) {
        if (dep[u] < dep[v]) swap(u, v);
        int k = dep[u] - dep[v];
        for (int x = 0; x < LOG; ++x) {
            if ((k >> x) & 1) { u = up[u][x]; }
        }
        if (u == v) return u;
        for (int x = LOG - 1; x >= 0; --x) {
            if (up[u][x] != up[v][x]) {
                u = up[u][x];
                v = up[v][x];
            }
        }
        assert(up[u][0] == up[v][0]);
        return up[u][0];
    }

    bool isAncestor(int p, int u) { return in[u] >= in[p] && in[u] <= out[p]; }

    void virtualTree(vector<int>& vec) {
        int k = vec.size();
        sort(vec.begin(), vec.end(), [&](int x, int y) { return in[x] < in[y]; });

        for (int i = 1; i < k; ++i) { vec.emplace_back(lca(vec[i - 1], vec[i])); }

        sort(vec.begin(), vec.end(), [&](int x, int y) { return in[x] < in[y]; });
        vec.erase(unique(vec.begin(), vec.end()), vec.end());

        stack<int> st;
        for (auto& x : vec) {
            while (st.size() && !isAncestor(st.top(), x)) st.pop();
            if (st.size()) { virtual_tree[st.top()].emplace_back(x); }
            st.push(x);
        }
    }
};
