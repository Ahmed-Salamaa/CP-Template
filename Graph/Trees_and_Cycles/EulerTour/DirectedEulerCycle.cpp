// Topic: Directed Euler Cycle - Finds an Eulerian cycle in a directed graph using Hierholzer's algorithm
#include "../../../core.h"

struct DirectedEulerCycle {
    // Time Complexity: O(V + E)
    // Space Complexity: O(V + E)
    int n, m;
    vector<vector<int>> adj, radj;
    vector<int> in, out, tour;

    DirectedEulerCycle(int n) : n(n), m(0), adj(n), radj(n), in(n, 0), out(n, 0) {}

    void add_edge(int u, int v) {
        adj[u].push_back(v);
        radj[v].push_back(u);
        out[u]++;
        in[v]++;
        m++;
    }

    void dfs1(int s, const vector<vector<int>> &g, vector<char> &vis) {
        stack<int> st;
        st.push(s);
        vis[s] = 1;
        while (!st.empty()) {
            int v = st.top(); st.pop();
            for (int u : g[v]) {
                if (!vis[u]) {
                    vis[u] = 1;
                    st.push(u);
                }
            }
        }
    }

    void dfs2(int v) {
        while (!adj[v].empty()) {
            int u = adj[v].back();
            adj[v].pop_back();
            dfs2(u);
        }
        tour.push_back(v);
    }

    bool get() {
        for (int i = 0; i < n; ++i) {
            if (in[i] != out[i]) return false;
        }

        int start = -1;
        for (int i = 0; i < n; ++i) {
            if (out[i] > 0) { start = i; break; }
        }
        if (start == -1) {
            tour = {0};
            return (m == 0);
        }
        vector<char> vis(n, 0);
        dfs1(start, adj, vis);
        for (int i = 0; i < n; ++i)
            if ((in[i] + out[i]) > 0 && !vis[i]) return false;

        fill(vis.begin(), vis.end(), 0);
        dfs1(start, radj, vis);
        for (int i = 0; i < n; ++i)
            if ((in[i] + out[i]) > 0 && !vis[i]) return false;

        tour.clear();
        dfs2(start);
        reverse(tour.begin(), tour.end());
        if ((int)tour.size() != m + 1) return false;
        return true;
    }
};
