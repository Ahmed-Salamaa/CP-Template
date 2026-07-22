/*
 * Topic: Floyd-Warshall Algorithm
 * Description: Floyd-Warshall algorithm is a fundamental dynamic programming technique for
 *   finding all-pairs shortest paths in a dense graph. It is used when we need to
 *   compute shortest distances between every pair of vertices, supporting negative
 *   edge weights but not negative cycles.
 * time. Usage: Floyd<int> fw(n); fw.add_edge(u, v, w); fw.shortest_path();
 *    // access distances in fw.dist[u][v]
 */
#include "../../../core.h"

// Time Complexity: O(N^3)
// Space Complexity: O(N^2)
template <typename T = int, int Base = 0>
struct Floyd {
    int n;
    const T INF = 1LL << 60;
    vector<vector<T>> dist;
    vector<vector<int>> par;

    // Time Complexity: O(N^2)
    // Space Complexity: O(N^2)
    Floyd(int n = 0) : n(n) {
        dist.assign(n + Base, vector<T>(n + Base, INF));
        par.assign(n + Base, vector<int>(n + Base, -1));
        for (int i = Base; i < n + Base; i++) dist[i][i] = 0;
    }

    // Time Complexity: O(N^3)
    // Space Complexity: O(N^2)
    Floyd(int n, const vector<vector<T>>& D) : n(n), dist(D) {
        par.assign(n + Base, vector<int>(n + Base, -1));
        build();
    }

    // Time Complexity: O(1)
    // Space Complexity: O(1) per edge
    void add_edge(int u, int v, T w) {
        if (w < dist[u][v]) {
            dist[u][v] = w;
            par[u][v] = u;
        }
    }

    // Time Complexity: O(N^3)
    // Space Complexity: O(1)
    void build() {
        for (int i = Base; i < n + Base; i++)
            for (int u = Base; u < n + Base; u++)
                for (int v = Base; v < n + Base; v++)
                    if (dist[u][i] != INF && dist[i][v] != INF) {
                        if (dist[u][i] + dist[i][v] < dist[u][v]) {
                            dist[u][v] = dist[u][i] + dist[i][v];
                            par[u][v] = par[i][v];
                        }
                    }
    }

    // Time Complexity: O(1)
    // Space Complexity: O(1)
    T get_dist(int u, int v) { return dist[u][v]; }

    // Time Complexity: O(N)
    // Space Complexity: O(N)
    vector<int> get_path(int u, int v) {
        if (dist[u][v] == INF) return {};
        vector<int> path;
        for (int curr = v; curr != -1 && curr != u; curr = par[u][curr]) { path.push_back(curr); }
        path.push_back(u);
        reverse(all(path));
        return path;
    }

    // Time Complexity: O(N^2)
    // Space Complexity: O(1)
    T diameter() {
        T mx = 0;
        for (int i = Base; i < n + Base; i++)
            for (int j = Base; j < n + Base; j++)
                if (dist[i][j] != INF) mx = max(mx, dist[i][j]);
        return mx;
    }

    // Time Complexity: O(N)
    // Space Complexity: O(1)
    bool has_negative_cycle() {
        for (int i = Base; i < n + Base; i++)
            if (dist[i][i] < 0) return true;
        return false;
    }

    // Time Complexity: O(N)
    // Space Complexity: O(1)
    bool negative_cycle_effect(int u, int v) {
        for (int i = Base; i < n + Base; i++) {
            if (dist[i][i] < 0 && dist[u][i] != INF && dist[i][v] != INF) return true;
        }
        return false;
    }

    // Time Complexity: O(N^2)
    // Space Complexity: O(N^2)
    vector<vector<int>> scc() {
        vector<int> comp(n + Base, -1);
        int cnt = 0;
        for (int i = Base; i < n + Base; i++) {
            if (comp[i] == -1) {
                comp[i] = cnt++;
                for (int j = Base; j < n + Base; j++) {
                    if (dist[i][j] != INF && dist[j][i] != INF) comp[j] = comp[i];
                }
            }
        }
        vector<vector<int>> comp_graph(cnt, vector<int>(cnt, 0));
        for (int i = Base; i < n + Base; i++) {
            for (int j = Base; j < n + Base; j++) {
                if (dist[i][j] != INF) { comp_graph[comp[i]][comp[j]]++; }
            }
        }
        return comp_graph;
    }
};

// Time Complexity: O(N^3)
// Space Complexity: O(N^2)
template <typename T = int, int Base = 0>
struct FloydPaths {
    int n;
    vector<vector<T>> adj;

    FloydPaths(int n = 0) : n(n), adj(n + Base, vector<T>(n + Base, 0)) {}

    void add_edge(int u, int v, T w = 1) { adj[u][v] += w; }

    void build() {
        for (int k = Base; k < n + Base; k++)
            for (int i = Base; i < n + Base; i++)
                for (int j = Base; j < n + Base; j++) adj[i][j] += adj[i][k] * adj[k][j];
    }
};

// Time Complexity: O(N^3)
// Space Complexity: O(N^2)
template <int Base = 0>
struct FloydTransitiveClosure {
    int n;
    vector<vector<bool>> adj;

    FloydTransitiveClosure(int n = 0) : n(n), adj(n + Base, vector<bool>(n + Base, false)) {}

    void add_edge(int u, int v) { adj[u][v] = true; }

    void build() {
        for (int k = Base; k < n + Base; k++)
            for (int i = Base; i < n + Base; i++)
                for (int j = Base; j < n + Base; j++) adj[i][j] = adj[i][j] | (adj[i][k] & adj[k][j]);
    }
};

// Time Complexity: O(N^3)
// Space Complexity: O(N^2)
template <typename T = int, int Base = 0>
struct FloydMinMaxEdge {
    int n;
    const T INF = 1LL << 60;
    vector<vector<T>> dist;

    FloydMinMaxEdge(int n = 0) : n(n), dist(n + Base, vector<T>(n + Base, INF)) {
        for (int i = Base; i < n + Base; i++) dist[i][i] = 0;
    }

    void add_edge(int u, int v, T w) { dist[u][v] = min(dist[u][v], w); }

    void build() {
        for (int k = Base; k < n + Base; k++)
            for (int i = Base; i < n + Base; i++)
                for (int j = Base; j < n + Base; j++) dist[i][j] = min(dist[i][j], max(dist[i][k], dist[k][j]));
    }
};

// Time Complexity: O(N^3)
// Space Complexity: O(N^2)
template <typename T = int, int Base = 0>
struct FloydMaxMinEdge {
    int n;
    const T NINF = -(1LL << 60);
    vector<vector<T>> dist;

    FloydMaxMinEdge(int n = 0) : n(n), dist(n + Base, vector<T>(n + Base, NINF)) {
        for (int i = Base; i < n + Base; i++) dist[i][i] = 0;
    }

    void add_edge(int u, int v, T w) { dist[u][v] = max(dist[u][v], w); }

    void build() {
        for (int k = Base; k < n + Base; k++)
            for (int i = Base; i < n + Base; i++)
                for (int j = Base; j < n + Base; j++) dist[i][j] = max(dist[i][j], min(dist[i][k], dist[k][j]));
    }
};
