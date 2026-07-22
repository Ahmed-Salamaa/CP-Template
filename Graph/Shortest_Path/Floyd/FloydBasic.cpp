/*
 * Topic: Floyd-Warshall (Basic)
 * Description: The standard implementation of the Floyd-Warshall all-pairs shortest path
 *   algorithm. It serves as a foundational building block for dense graph
 *   reachability and distance queries.
 * Usage:
 *    FloydBasic fw;
 *    fw.shortestDistance(matrix);
 *    // matrix[i][j] contains shortest distance from i to j
 */
#include "../../../core.h"

struct FloydBasic {
    // Time Complexity: O(V^3)
    // Space Complexity: O(1) auxiliary space (modifies input in-place)
    void shortestDistance(vector<vector<int>>& matrix) {
        int n = sz(matrix);
        for (int i = 0; i < n; i++) {
            for (int j = 0; j < n; j++) {
                for (int k = 0; k < n; k++) matrix[j][k] = min(matrix[j][k], matrix[j][i] + matrix[i][k]);
            }
        }
    }
};
