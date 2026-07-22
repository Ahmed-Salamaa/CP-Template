/*
 * Topic: Min Vertex Cover
 * Description: Finds the Minimum Vertex Cover in a Bipartite Graph by using Konig's theorem,
 *   which states it equals the Maximum Bipartite Matching.
 * matching. Usage: MinVertexCover mvc(n, m); mvc.add_edge(u, v); int match_size = mvc.max_matching(); auto [left_cover,
 * right_cover] = mvc.get_min_vertex_cover();
 */
#include "../../../core.h"

// Time Complexity: O(E * sqrt(V)) for matching, O(V + E) for vertex cover
// Space Complexity: O(V + E)
struct MinVertexCover {
    int n;
    vector<vector<int>> adj;
    vector<int> mu, mv, dist;

    MinVertexCover(int n, int m) : n(n), adj(n), mu(n, -1), mv(m, -1), dist(n) {}

    void add_edge(int u, int v) { adj[u].push_back(v); }

    bool bfs() {
        queue<int> q;
        for (int u = 0; u < n; ++u) {
            if (mu[u] == -1) {
                dist[u] = 0;
                q.push(u);
            } else {
                dist[u] = -1;
            }
        }
        bool found = false;
        while (!q.empty()) {
            int u = q.front();
            q.pop();
            for (int v : adj[u]) {
                if (mv[v] == -1) {
                    found = true;
                } else if (dist[mv[v]] == -1) {
                    dist[mv[v]] = dist[u] + 1;
                    q.push(mv[v]);
                }
            }
        }
        return found;
    }

    bool dfs(int u) {
        for (int v : adj[u]) {
            if (mv[v] == -1 || (dist[mv[v]] == dist[u] + 1 && dfs(mv[v]))) {
                mu[u] = v;
                mv[v] = u;
                return true;
            }
        }
        dist[u] = -1;
        return false;
    }

    int max_matching() {
        int res = 0;
        while (bfs()) {
            for (int u = 0; u < n; ++u) {
                if (mu[u] == -1 && dfs(u)) { res++; }
            }
        }
        return res;
    }

    // Left vertices are labeled 0..n-1, right vertices are labeled n..n+m-1.
    vector<int> min_vertex_cover() {
        max_matching();
        int m = sz(mv);
        vector<char> visL(n, false), visR(m, false);
        queue<int> q;
        for (int u = 0; u < n; ++u) {
            if (mu[u] == -1) {
                visL[u] = true;
                q.push(u);
            }
        }
        while (!q.empty()) {
            int u = q.front();
            q.pop();
            for (int v : adj[u]) {
                if (!visR[v] && mu[u] != v) {
                    visR[v] = true;
                    int u2 = mv[v];
                    if (u2 != -1 && !visL[u2]) {
                        visL[u2] = true;
                        q.push(u2);
                    }
                }
            }
        }
        vector<int> cover;
        for (int u = 0; u < n; ++u) {
            if (!visL[u]) cover.push_back(u);
        }
        for (int v = 0; v < m; ++v) {
            if (visR[v]) cover.push_back(n + v);
        }
        return cover;
    }
};
