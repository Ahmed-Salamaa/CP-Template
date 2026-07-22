/*
 * Topic: Tarjan Articulation Points
 * Description: Tarjan's algorithm applied to discover articulation points in O(V+E) time using
 *   a single DFS traversal to track discovery and low-link values. It remains the
 *   standard efficient approach for cut-vertex detection.
 * Usage:
 *    TarjanArtPoint solver(N);
 *    solver.add_edge(u, v);
 *    solver.build();
 */
#include "../../../core.h"

struct TarjanArtPoint {
    // Time Complexity: O(V + E)
    // Space Complexity: O(V + E)
    int n, ndfn;
    vector<vector<int>> adj;
    vector<int> dfn, LowLink, ArtPoints;
    vector<bool> IsArtPoints;

    TarjanArtPoint(int n) : n(n), ndfn(0), adj(n), dfn(n, -1), LowLink(n, 0), IsArtPoints(n, false) {}

    void add_edge(int u, int v) {
        adj[u].push_back(v);
        adj[v].push_back(u);
    }

    void tarjan(int node, int parent) {
        dfn[node] = LowLink[node] = ndfn++;
        int child = 0;
        for (auto& val : adj[node]) {
            if (dfn[val] == -1) {
                child++;
                tarjan(val, node);
                LowLink[node] = min(LowLink[node], LowLink[val]);
                if (LowLink[val] >= dfn[node]) {
                    if (parent == -1 && child <= 1) continue;
                    IsArtPoints[node] = true;
                }
            } else if (parent != val) {
                LowLink[node] = min(LowLink[node], dfn[val]);
            }
        }
    }

    void build() {
        for (int i = 0; i < n; i++) {
            if (dfn[i] == -1) tarjan(i, -1);
        }
        for (int i = 0; i < n; i++) {
            if (IsArtPoints[i]) ArtPoints.push_back(i);
        }
    }
};
