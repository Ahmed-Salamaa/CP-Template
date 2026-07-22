/*
 * Topic: Floyd-Warshall (Min-Max Edge)
 * Description: A minimax variation of Floyd-Warshall designed to find a path that minimizes the
 *   maximum edge weight across all possible paths between every pair of vertices.
 *   This is ideal for minimizing the maximum bottlenecks, such as avoiding high-toll
 *   roads or risky connections.
 * on a path between all pairs of nodes (minimize bottleneck). Usage: FloydMinMaxEdge fw; fw.minMaxEdge(adj_matrix);
 */
#include "../../../core.h"

struct FloydMinMaxEdge {
    // Time Complexity: O(V^3)
    // Space Complexity: O(1) auxiliary space (modifies input in-place)
    void minMaxEdge(vector<vector<int>>& adj) {
        int n = sz(adj);
        for (int k = 0; k < n; k++) {
            for (int i = 0; i < n; i++) {
                for (int j = 0; j < n; j++) adj[i][j] = min(adj[i][j], max(adj[i][k], adj[k][j]));
            }
        }
    }
};
