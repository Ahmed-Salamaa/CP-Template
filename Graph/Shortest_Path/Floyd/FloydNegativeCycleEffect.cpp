/*
 * Topic: Floyd-Warshall (Negative Cycle Effect)
 * Description: Extends Floyd-Warshall to determine which specific pairs of nodes have their
 *   shortest path distances compromised by being reachable from a negative cycle.
 *   Useful for safely finding shortest paths in financial arbitrage or routing
 *   systems with negative costs.
 * destination. Usage: FloydNegativeCycleEffect fw; bool affected = fw.checkCycleEffect(adj_matrix, source, dest);
 */
#include "../../../core.h"

struct FloydNegativeCycleEffect {
    // Time Complexity: O(V) after Floyd-Warshall, overall O(V^3)
    // Space Complexity: O(1) auxiliary space
    bool checkCycleEffect(vector<vector<int>>& adj, int source, int dest) {
        int n = sz(adj);
        for (int i = 0; i < n; i++) {
            if (adj[i][i] < 0 && adj[source][i] < 2e9 && adj[i][dest] < 2e9)  // Check if cycle on path
                return true;
        }
        return false;
    }
};
