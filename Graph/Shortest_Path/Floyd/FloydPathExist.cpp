/*
 * Topic: Floyd-Warshall (Path Existence / Transitive Closure)
 * Description: An application of Floyd-Warshall to compute the transitive closure of a graph,
 *   simply checking if a path exists between any two nodes. Useful for resolving
 *   reachability and connectivity queries.
 * Usage:
 *    FloydPathExist fw;
 *    fw.pathExist(adj_matrix); // adj_matrix is vector<vector<bool>>
 */
#include "../../../core.h"

struct FloydPathExist {
    // Time Complexity: O(V^3)
    // Space Complexity: O(1) auxiliary space (modifies input in-place)
    void pathExist(vector<vector<bool>>& adj) {
        int n = sz(adj);
        for (int k = 0; k < n; k++) {
            for (int i = 0; i < n; i++) {
                for (int j = 0; j < n; j++) adj[i][j] = (adj[i][j] | (adj[i][k] & adj[k][j]));
            }
        }
    }
};
