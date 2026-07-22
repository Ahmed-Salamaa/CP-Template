/*
 * Topic: Bellman-Ford
 * Description: Bellman-Ford algorithm finds shortest paths from a single source in O(V E) time,
 *   and is capable of detecting negative weight cycles.
 * difference constraints. Usage: BellmanFord<int> bf(n); bf.add_edge(u, v, weight); if (!bf.has_negative_cycle(source))
 * { int d = bf.dist[target];
 *    }
 */
#include "../../../core.h"

// Time Complexity: O(N * M)
// Space Complexity: O(N + M)
template <typename T = int>
struct BellmanFord {
    struct Edge {
        T u, v, w;

        Edge(T u, T v, T w) : u(u), v(v), w(w) {}
    };

    bool was_processed = false;
    T n, m;
    const T INF = 1LL << 60;
    vector<T> dist, par;
    vector<Edge> edges;

    // Time Complexity: O(N)
    // Space Complexity: O(N)
    BellmanFord(T n) : n(n), m(0), dist(n + 1, INF), par(n + 1, -1) {}

    // Time Complexity: O(1)
    // Space Complexity: O(1) per edge
    void add_edge(T u, T v, T w) {
        edges.push_back({u, v, w});
        m++;
    }

    // Time Complexity: O(N * M) worst case, O(M) best case
    // Space Complexity: O(N)
    vector<T> shortest_path(T source) {
        dist.assign(n + 1, INF);
        dist[source] = 0;
        was_processed = true;
        for (T i = 0; i < n - 1; ++i) {
            bool updated = false;
            for (const auto& e : edges) {
                if (dist[e.u] != INF && dist[e.u] + e.w < dist[e.v]) {
                    dist[e.v] = dist[e.u] + e.w;
                    par[e.v] = e.u;
                    updated = true;
                }
            }
            if (!updated) break;
        }
        return dist;
    }

    // Time Complexity: O(N * M)
    // Space Complexity: O(N)
    T shortest_path(T source, T target) { return shortest_path(source)[target]; }

    // Time Complexity: O(N * M)
    // Space Complexity: O(N)
    bool negative_cycle(T source = 1) {
        if (!was_processed) shortest_path(source);
        for (const auto& e : edges)
            if (dist[e.u] != INF && dist[e.u] + e.w < dist[e.v]) return true;
        return false;
    }

    // Time Complexity: O(N * M)
    // Space Complexity: O(N)
    bool negative_cycle(T source, T target) {
        if (!was_processed) shortest_path(source);
        vector<bool> in_neg_cycle(n + 1, false);
        for (T i = 0; i < n; ++i) {
            for (const auto& e : edges) {
                if (dist[e.u] != INF && dist[e.u] + e.w < dist[e.v]) {
                    dist[e.v] = dist[e.u] + e.w;
                    in_neg_cycle[e.v] = true;
                }
                if (in_neg_cycle[e.u]) in_neg_cycle[e.v] = true;
            }
        }
        return in_neg_cycle[target];
    }

    // Time Complexity: O(N * M)
    // Space Complexity: O(N)
    vector<T> get_negative_cycle() {
        vector<T> d(n + 1, 0);
        vector<T> p(n + 1, -1);
        T x = -1;
        for (T i = 0; i < n; ++i) {
            x = -1;
            for (const auto& e : edges) {
                if (d[e.u] != INF && d[e.u] + e.w < d[e.v]) {
                    d[e.v] = max(-INF, d[e.u] + e.w);
                    p[e.v] = e.u;
                    x = e.v;
                }
            }
        }
        if (x == -1) return {};
        for (T i = 0; i < n; ++i) x = p[x];
        vector<T> cycle;
        for (T v = x;; v = p[v]) {
            cycle.push_back(v);
            if (v == x && sz(cycle) > 1) break;
        }
        reverse(all(cycle));
        return cycle;
    }

    // Time Complexity: O(N * M)
    // Space Complexity: O(N + M)
    vector<T> nodes_in_negative_cycle(T source = 1) {
        if (!was_processed) shortest_path(source);
        vector<T> ref = dist;
        for (T i = 0; i < n; ++i) {
            for (const auto& e : edges) {
                if (dist[e.u] != INF && dist[e.u] + e.w < dist[e.v]) { dist[e.v] = dist[e.u] + e.w; }
            }
        }
        vector<bool> in_neg(n + 1, false);
        queue<T> q;
        for (T i = 0; i <= n; ++i) {
            if (ref[i] != dist[i]) {
                in_neg[i] = true;
                q.push(i);
            }
        }
        vector<vector<T>> adj(n + 1);
        for (const auto& e : edges) adj[e.u].push_back(e.v);
        while (!q.empty()) {
            T u = q.front();
            q.pop();
            for (T v : adj[u]) {
                if (!in_neg[v]) {
                    in_neg[v] = true;
                    q.push(v);
                }
            }
        }
        vector<T> res;
        for (T i = 0; i <= n; ++i)
            if (in_neg[i]) res.push_back(i);
        return res;
    }

    // Time Complexity: O(N * M) to compute, O(N) to reconstruct
    // Space Complexity: O(N)
    vector<T> path(T source, T target) {
        if (!was_processed) shortest_path(source);
        vector<T> p;
        if (dist[target] == INF) return p;
        for (T i = target; i != -1; i = par[i]) p.push_back(i);
        reverse(all(p));
        return p;
    }
};

// Time Complexity: O(N * M)
// Space Complexity: O(N^2)
template <typename T = int>
struct MinMeanCycle {
    struct Edge {
        int u, v;
        T w;
    };

    int n;
    vector<Edge> edges;
    const T INF = 1LL << 60;

    MinMeanCycle(int n) : n(n) {}

    void add_edge(int u, int v, T w) { edges.push_back({u, v, w}); }

    // Returns minimum mean cycle or infinity if acyclic
    ld min_mean_cycle() {
        vector<vector<T>> dp(n + 1, vector<T>(n, INF));
        for (int v = 0; v < n; ++v) dp[0][v] = 0;
        for (int k = 1; k <= n; ++k) {
            for (const auto& e : edges) {
                if (dp[k - 1][e.u] != INF) { dp[k][e.v] = min(dp[k][e.v], dp[k - 1][e.u] + e.w); }
            }
        }
        ld mu = numeric_limits<ld>::infinity();
        for (int v = 0; v < n; ++v) {
            if (dp[n][v] == INF) continue;
            ld local_max = -numeric_limits<ld>::infinity();
            for (int k = 0; k < n; ++k) {
                if (dp[k][v] == INF) continue;
                ld mean = (ld)(dp[n][v] - dp[k][v]) / (n - k);
                local_max = max(local_max, mean);
            }
            mu = min(mu, local_max);
        }
        return mu;
    }
};

// Time Complexity: O(N * M)
// Space Complexity: O(N^2)
template <typename T = int>
struct MinDeltaCycle {
    struct Edge {
        int u, v;
        T w;
    };

    int n;
    vector<Edge> edges;
    const T INF = 1LL << 60;

    MinDeltaCycle(int n) : n(n) {}

    void add_edge(int u, int v, T w) { edges.push_back({u, v, w}); }

    // Finds the minimum total-weight (most negative) cycle-delta in the graph.
    T min_delta_negative_cycle() {
        vector<vector<T>> dp(n + 1, vector<T>(n, INF));
        for (int v = 0; v < n; ++v) dp[0][v] = 0;
        for (int k = 1; k <= n; ++k) {
            for (const auto& e : edges) {
                if (dp[k - 1][e.u] != INF) { dp[k][e.v] = min(dp[k][e.v], dp[k - 1][e.u] + e.w); }
            }
        }
        T min_delta = INF;
        for (int v = 0; v < n; ++v) {
            if (dp[n][v] == INF) continue;
            T temp = -(1LL << 60);
            for (int k = 0; k < n; ++k) {
                if (dp[k][v] == INF) continue;
                T delta = dp[n][v] - dp[k][v];
                temp = max(temp, delta);
            }
            min_delta = min(min_delta, temp);
        }
        return min_delta;
    }
};

// Difference Constraints
// Time Complexity: O(N * M)
// Space Complexity: O(N)
template <typename T = int>
struct DifferenceConstraints {
    struct Constraint {
        int u, v;
        T c;
    };

    int n;
    vector<Constraint> constraints;

    DifferenceConstraints(int n) : n(n) {}

    // X_u - X_v >= c
    void add_constraint(int u, int v, T c) { constraints.push_back({u, v, c}); }

    bool is_solvable() const {
        vector<T> dist(n, 0);
        int m = sz(constraints);
        for (int i = 0; i < n; ++i) {
            bool updated = false;
            for (int j = 0; j < m; ++j) {
                int u = constraints[j].u;
                int v = constraints[j].v;
                T c = constraints[j].c;
                if (dist[u] < dist[v] + c) {
                    dist[u] = dist[v] + c;
                    updated = true;
                }
            }
            if (!updated) return true;
            if (i == n - 1) return false;
        }
        return true;
    }

    vector<T> get_any_values() const {
        vector<T> dist(n, 0);
        int m = sz(constraints);
        for (int i = 0; i < n - 1; ++i) {
            for (int j = 0; j < m; ++j) {
                int u = constraints[j].u;
                int v = constraints[j].v;
                T c = constraints[j].c;
                dist[u] = max(dist[u], dist[v] + c);
            }
        }
        return dist;
    }

    vector<T> get_min_values() const { return get_any_values(); }

    vector<T> get_max_values() const {
        vector<T> dist(n, 0);
        int m = sz(constraints);
        for (int i = 0; i < n - 1; ++i) {
            for (int j = 0; j < m; ++j) {
                int u = constraints[j].u;
                int v = constraints[j].v;
                T c = constraints[j].c;
                dist[v] = max(dist[v], dist[u] + c);
            }
        }
        vector<T> res(n);
        for (int i = 0; i < n; ++i) res[i] = -dist[i];
        return res;
    }
};
