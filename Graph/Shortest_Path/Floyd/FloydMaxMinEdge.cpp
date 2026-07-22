/*
 * Topic: Floyd-Warshall (Max-Min Edge)
 * Description: A minimax variant of the Floyd-Warshall algorithm that finds a path between all
 *   pairs of nodes which maximizes the minimum edge weight along the path. Often
 *   applied in maximizing bandwidth or capacity across a network.
 * on a path between all pairs of nodes (maximize bottleneck). Usage: FloydMaxMinEdge fw; fw.maxMinEdge(adj_matrix);
 */
#include "../../../core.h"

struct FloydMaxMinEdge {
    // Time Complexity: O(V^3)
    // Space Complexity: O(1) auxiliary space (modifies input in-place)
    void maxMinEdge(vector<vector<int>>& adj) {
        int n = sz(adj);
        for (int k = 0; k < n; k++) {
            for (int i = 0; i < n; i++) {
                for (int j = 0; j < n; j++) adj[i][j] = max(adj[i][j], min(adj[i][k], adj[k][j]));
            }
        }
    }
};
