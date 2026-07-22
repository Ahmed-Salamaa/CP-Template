/*
 * Topic: Dijkstra's Algorithm
 * Description: Dijkstra's algorithm finds the shortest paths from a single source to all other
 *   vertices in a graph with non-negative edge weights in O(E log V) time.
 * Usage:
 *    Dijkstra<int> solver(n);
 *    solver.add_edge(u, v, w);
 *    vector<int> dist = solver.run(source);
 *    // or solver.get_path(source, target)
 */
#include "../../../core.h"

// Time Complexity: O(M * log(M))
// Space Complexity: O(N + M)
template <typename T = int>
struct Dijkstra {
    struct Edge {
        T v, w;

        Edge(T v = 0, T w = 0) : v(v), w(w) {}

        bool operator<(const Edge& e) const { return w > e.w; }
    };

    int n;
    const T INF = 1LL << 60;
    vector<vector<Edge>> adj;

    // Time Complexity: O(N)
    // Space Complexity: O(N)
    Dijkstra(int n) : n(n), adj(n + 1) {}

    // Time Complexity: O(1)
    // Space Complexity: O(1) per edge
    void add_edge(int u, int v, T w, bool undirected = true) {
        adj[u].push_back(Edge(v, w));
        if (undirected) adj[v].push_back(Edge(u, w));
    }

    // Time Complexity: O((N + M) * log(M))
    // Space Complexity: O(N + M)
    T min_cost(int src, int dest) {
        vector<T> dist(n + 1, INF);
        dist[src] = 0;
        priority_queue<Edge> pq;
        pq.push(Edge(src, 0));
        while (!pq.empty()) {
            Edge top = pq.top();
            pq.pop();
            T u = top.v, cost = top.w;
            if (cost > dist[u]) continue;
            for (const auto& edge : adj[u]) {
                if (dist[edge.v] > dist[u] + edge.w) {
                    dist[edge.v] = dist[u] + edge.w;
                    pq.push(Edge(edge.v, dist[edge.v]));
                }
            }
        }
        return dist[dest] == INF ? -1 : dist[dest];
    }

    // Time Complexity: O((N + M) * log(M))
    // Space Complexity: O(N + M)
    vector<T> get_dist(int src) {
        vector<T> dist(n + 1, INF);
        dist[src] = 0;
        priority_queue<Edge> pq;
        pq.push(Edge(src, 0));
        while (!pq.empty()) {
            Edge top = pq.top();
            pq.pop();
            T u = top.v, cost = top.w;
            if (cost > dist[u]) continue;
            for (const auto& edge : adj[u]) {
                if (dist[edge.v] > dist[u] + edge.w) {
                    dist[edge.v] = dist[u] + edge.w;
                    pq.push(Edge(edge.v, dist[edge.v]));
                }
            }
        }
        return dist;
    }

    // Time Complexity: O((N + M) * log(M))
    // Space Complexity: O(N + M)
    vector<int> get_path(int src, int dest) {
        vector<T> dist(n + 1, INF);
        vector<int> parent(n + 1, -1);
        dist[src] = 0;
        priority_queue<Edge> pq;
        pq.push(Edge(src, 0));
        while (!pq.empty()) {
            Edge top = pq.top();
            pq.pop();
            T u = top.v, cost = top.w;
            if (cost > dist[u]) continue;
            for (const auto& edge : adj[u]) {
                if (dist[edge.v] > dist[u] + edge.w) {
                    dist[edge.v] = dist[u] + edge.w;
                    parent[edge.v] = u;
                    pq.push(Edge(edge.v, dist[edge.v]));
                }
            }
        }
        vector<int> path;
        for (int i = dest; i != -1; i = parent[i]) path.push_back(i);
        reverse(all(path));
        return path;
    }
};
