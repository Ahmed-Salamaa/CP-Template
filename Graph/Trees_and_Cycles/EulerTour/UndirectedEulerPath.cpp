// Topic: Undirected Euler Path - Finds an Eulerian path in an undirected graph using Hierholzer's algorithm
#include "../../../core.h"

struct UndirectedEulerPath {
    // Time Complexity: O(V + E log E)
    // Space Complexity: O(V + E)
    int n, m;
    vector<set<int>> adj;
    vector<int> degree;
    vector<int> tour;

    UndirectedEulerPath(int n) : n(n), m(0), adj(n), degree(n, 0) {}

    void add_edge(int u, int v) {
        adj[u].insert(v);
        adj[v].insert(u);
        degree[u]++;
        degree[v]++;
        m++;
    }

    void dfs(int node) {
        while (!adj[node].empty()) {
            auto it = prev(adj[node].end());
            int x = *it;
            adj[node].erase(it);
            if (adj[x].erase(node)) {
                dfs(x);
            }
        }
        tour.emplace_back(node);
    }

    bool check(int &start) {
        int odd_count = 0;
        start = -1;
        for (int i = 0; i < n; ++i) {
            if (degree[i] & 1) {
                odd_count++;
                if (start == -1) start = i;
            }
        }
        return odd_count == 0 || odd_count == 2;
    }

    bool isConnected(int start) {
        if (start == -1) {
            for (int i = 0; i < n; ++i) {
                if (degree[i] > 0) { start = i; break; }
            }
            if (start == -1) return true;
        }

        vector<char> vis(n, 0);
        stack<int> st;
        st.push(start);
        vis[start] = 1;
        while (!st.empty()) {
            int v = st.top(); st.pop();
            for (int u : adj[v]) {
                if (!vis[u]) {
                    vis[u] = 1;
                    st.push(u);
                }
            }
        }

        for (int i = 0; i < n; ++i) {
            if (degree[i] > 0 && !vis[i]) return false;
        }
        return true;
    }

    bool get() {
        int start;
        if (!check(start)) return false;
        if (!isConnected(start)) return false;

        if (start == -1) {
            for (int i = 0; i < n; ++i) {
                if (degree[i] > 0) { start = i; break; }
            }
        }
        if (start == -1) {
            return (m == 0);
        }

        tour.clear();
        dfs(start);
        reverse(tour.begin(), tour.end());
        return (int)tour.size() == m + 1;
    }
};
