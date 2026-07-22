/*
 * Topic: EulerTour
 * Description: Flattens a tree into an array using a DFS Euler Tour, allowing subtree queries
 *   to be transformed into contiguous range queries (e.g. for Segment Trees).
 * Usage:
 *    EulerTour solver(N);
 *    solver.add_edge(u, v);
 *    solver.dfs(1);
 */
#include "../../../core.h"

// Time Complexity: O(V + E)
// Space Complexity: O(V + E)
struct DirectedCycle {
    int n, m;
    vector<vector<int>> adj, radj;
    vector<int> in, out, tour;

    DirectedCycle(int n, int m) : n(n), m(m), adj(n), radj(n), in(n, 0), out(n, 0) {}

    void add_edge(int u, int v) {
        adj[u].push_back(v);
        radj[v].push_back(u);
        out[u]++;
        in[v]++;
    }

    // Time Complexity: O(V + E)
    // Space Complexity: O(V)
    void dfs1(int s, const vector<vector<int>>& g, vector<char>& vis) {
        stack<int> st;
        st.push(s);
        vis[s] = 1;
        while (!st.empty()) {
            int v = st.top();
            st.pop();
            for (int u : g[v]) {
                if (!vis[u]) {
                    vis[u] = 1;
                    st.push(u);
                }
            }
        }
    }

    // Time Complexity: O(V + E)
    // Space Complexity: O(V + E)
    void dfs2(int v) {
        while (!adj[v].empty()) {
            int u = adj[v].back();
            adj[v].pop_back();
            dfs2(u);
        }
        tour.push_back(v);
    }

    // Time Complexity: O(V + E)
    // Space Complexity: O(V + E)
    bool get() {
        for (int i = 0; i < n; ++i) {
            if (in[i] != out[i]) return false;
        }
        int start = -1;
        for (int i = 0; i < n; ++i) {
            if (out[i] > 0) {
                start = i;
                break;
            }
        }
        if (start == -1) {
            tour = {0};
            return (m == 0);
        }
        vector<char> vis(n, 0);
        dfs1(start, adj, vis);
        for (int i = 0; i < n; ++i)
            if ((in[i] + out[i]) > 0 && !vis[i]) return false;
        fill(all(vis), 0);
        dfs1(start, radj, vis);
        for (int i = 0; i < n; ++i)
            if ((in[i] + out[i]) > 0 && !vis[i]) return false;
        tour.clear();
        dfs2(start);
        reverse(all(tour));
        return sz(tour) == m + 1;
    }
};

// Time Complexity: O(V + E)
// Space Complexity: O(V + E)
struct DirectedPath {
    int n, m;
    vector<vector<int>> adj;
    vector<int> in, out, tour;
    int st, en;

    DirectedPath(int n, int m) : n(n), m(m), adj(n), in(n, 0), out(n, 0), st(-1), en(-1) {}

    void add_edge(int u, int v) {
        adj[u].push_back(v);
        out[u]++;
        in[v]++;
    }

    // Time Complexity: O(V)
    // Space Complexity: O(1)
    bool check_euler() {
        for (int i = 0; i < n; i++) {
            if (abs(in[i] - out[i]) > 1) return false;
            if (in[i] == out[i] + 1) {
                if (en != -1) return false;
                en = i;
            }
            if (out[i] == in[i] + 1) {
                if (st != -1) return false;
                st = i;
            }
        }
        if (st == -1 && en == -1) {
            for (int i = 0; i < n; ++i) {
                if (out[i] > 0) {
                    st = i;
                    break;
                }
            }
            if (st == -1) st = 0;
        }
        return true;
    }

    // Time Complexity: O(V + E)
    // Space Complexity: O(V + E)
    void dfs(int node) {
        while (out[node]) { dfs(adj[node][--out[node]]); }
        tour.emplace_back(node);
    }

    // Time Complexity: O(V + E)
    // Space Complexity: O(V + E)
    bool get() {
        if (!check_euler()) return false;
        dfs(st);
        if (sz(tour) != m + 1) return false;
        reverse(all(tour));
        return true;
    }
};

// Time Complexity: O((V + E) log E)
// Space Complexity: O(V + E)
struct UndirectedCycle {
    int n, m;
    vector<set<int>> adj;
    vector<int> degree, tour;

    UndirectedCycle(int n, int m) : n(n), m(m), adj(n), degree(n, 0) {}

    void add_edge(int u, int v) {
        adj[u].insert(v);
        adj[v].insert(u);
        degree[u]++;
        degree[v]++;
    }

    // Time Complexity: O((V + E) log E)
    // Space Complexity: O(V + E)
    void dfs(int node) {
        while (!adj[node].empty()) {
            auto it = prev(adj[node].end());
            int x = *it;
            adj[node].erase(it);
            if (adj[x].erase(node)) { dfs(x); }
        }
        tour.emplace_back(node);
    }

    // Time Complexity: O(V)
    // Space Complexity: O(1)
    bool check() {
        for (int i = 0; i < n; ++i) {
            if (degree[i] & 1) return false;
        }
        return true;
    }

    // Time Complexity: O(V + E)
    // Space Complexity: O(V + E)
    bool isConnected() {
        int start = -1;
        for (int i = 0; i < n; ++i) {
            if (degree[i] > 0) {
                start = i;
                break;
            }
        }
        if (start == -1) return true;
        vector<char> vis(n, 0);
        stack<int> st;
        st.push(start);
        vis[start] = 1;
        while (!st.empty()) {
            int v = st.top();
            st.pop();
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

    // Time Complexity: O((V + E) log E)
    // Space Complexity: O(V + E)
    bool get() {
        if (!check() || !isConnected()) return false;
        int start = -1;
        for (int i = 0; i < n; ++i) {
            if (degree[i] > 0) {
                start = i;
                break;
            }
        }
        if (start == -1) return (m == 0);
        tour.clear();
        dfs(start);
        reverse(all(tour));
        return sz(tour) == m + 1;
    }
};

// Time Complexity: O((V + E) log E)
// Space Complexity: O(V + E)
struct UndirectedPath {
    int n, m;
    vector<set<int>> adj;
    vector<int> degree, tour;

    UndirectedPath(int n, int m) : n(n), m(m), adj(n), degree(n, 0) {}

    void add_edge(int u, int v) {
        adj[u].insert(v);
        adj[v].insert(u);
        degree[u]++;
        degree[v]++;
    }

    // Time Complexity: O((V + E) log E)
    // Space Complexity: O(V + E)
    void dfs(int node) {
        while (!adj[node].empty()) {
            auto it = prev(adj[node].end());
            int x = *it;
            adj[node].erase(it);
            if (adj[x].erase(node)) { dfs(x); }
        }
        tour.emplace_back(node);
    }

    // Time Complexity: O(V)
    // Space Complexity: O(1)
    bool check() {
        int odds = 0;
        for (int i = 0; i < n; ++i) {
            if (degree[i] & 1) odds++;
        }
        return odds == 0 || odds == 2;
    }

    // Time Complexity: O(V + E)
    // Space Complexity: O(V + E)
    bool isConnected() {
        int start = -1;
        for (int i = 0; i < n; ++i) {
            if (degree[i] > 0) {
                start = i;
                break;
            }
        }
        if (start == -1) return true;
        vector<char> vis(n, 0);
        stack<int> st;
        st.push(start);
        vis[start] = 1;
        while (!st.empty()) {
            int v = st.top();
            st.pop();
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

    // Time Complexity: O((V + E) log E)
    // Space Complexity: O(V + E)
    bool get() {
        if (!check() || !isConnected()) return false;
        int start = -1;
        for (int i = 0; i < n; ++i) {
            if (degree[i] & 1) {
                start = i;
                break;
            }
        }
        if (start == -1) {
            for (int i = 0; i < n; ++i) {
                if (degree[i] > 0) {
                    start = i;
                    break;
                }
            }
        }
        if (start == -1) return (m == 0);
        tour.clear();
        dfs(start);
        reverse(all(tour));
        return sz(tour) == m + 1;
    }
};
