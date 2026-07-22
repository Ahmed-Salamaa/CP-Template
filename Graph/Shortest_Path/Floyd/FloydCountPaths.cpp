/*
 * Topic: Floyd-Warshall (Count Paths)
 * Description: A modification of the Floyd-Warshall algorithm that not only computes shortest
 *   paths but also counts the number of such paths between all pairs of nodes. It is
 *   useful in network analysis and reliability optimization to understand the
 *   redundancy of shortest routes.
 * Usage:
 *    FloydCountPaths fw;
 *    fw.countPaths(adj_matrix);
 *    // adj_matrix[i][j] contains the number of paths from i to j
 */
#include "../../../core.h"

struct FloydCountPaths {
    // Time Complexity: O(V^3)
    // Space Complexity: O(1) auxiliary space (modifies input in-place)
    void countPaths(vector<vector<int>>& adj) {
        int n = sz(adj);
        for (int k = 0; k < n; k++) {
            for (int i = 0; i < n; i++) {
                for (int j = 0; j < n; j++) adj[i][j] += adj[i][k] * adj[k][j];
            }
        }
    }
};
