/*
 * Topic: Floyd-Warshall (Detect Negative Cycle)
 * Description: Floyd-Warshall algorithm specialized to detect the presence of negative-weight
 *   cycles in a graph. It works by checking if the shortest distance from a node to
 *   itself becomes negative, indicating an unbounded shortest path.
 * after running Floyd-Warshall. Usage: FloydNegativeCycle fw; bool has_cycle = fw.hasNegativeCycle(adj_matrix);
 */
#include "../../../core.h"

struct FloydNegativeCycle {
    // Time Complexity: O(V) after Floyd-Warshall, overall O(V^3)
    // Space Complexity: O(1) auxiliary space
    bool hasNegativeCycle(vector<vector<int>>& adj) {
        int n = sz(adj);
        for (int i = 0; i < n; i++) {
            if (adj[i][i] < 0)  // Should be < 0 since distance to self decreasing means negative cycle
                return true;
        }
        return false;
    }
};
