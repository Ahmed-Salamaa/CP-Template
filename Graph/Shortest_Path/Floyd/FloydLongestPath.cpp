/*
 * Topic: Floyd-Warshall (Longest Path)
 * Description: Floyd-Warshall adapted to find the longest simple paths between all pairs of
 *   nodes in a Directed Acyclic Graph (DAG) or graphs without positive cycles. It is
 *   commonly used in critical path analysis and scheduling problems.
 * max bottleneck edge). Usage: FloydLongestPath fw; fw.longestPath(adj_matrix);
 */
#include "../../../core.h"

struct FloydLongestPath {
    // Time Complexity: O(V^3)
    // Space Complexity: O(1) auxiliary space (modifies input in-place)
    void longestPath(vector<vector<int>>& adj) {
        int n = sz(adj);
        for (int k = 0; k < n; k++) {
            for (int i = 0; i < n; i++) {
                for (int j = 0; j < n; j++) { adj[i][j] = max(adj[i][j], max(adj[i][k], adj[k][j])); }
            }
        }
    }
};
