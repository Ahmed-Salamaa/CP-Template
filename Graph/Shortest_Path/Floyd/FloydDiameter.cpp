/*
 * Topic: Floyd-Warshall (Graph Diameter)
 * Description: Uses the Floyd-Warshall algorithm's output to find the diameter of a graph,
 *   which is the longest of all shortest paths between any two nodes. This helps in
 *   analyzing the worst-case communication delay in a network.
 * Usage:
 *    FloydDiameter fw;
 *    int diameter = fw.diameter(matrix);
 */
#include "../../../core.h"

struct FloydDiameter {
    // Time Complexity: O(V^3)
    // Space Complexity: O(1) auxiliary space (modifies input in-place)
    int diameter(vector<vector<int>>& adj) {
        int n = sz(adj);
        for (int i = 0; i < n; i++) {
            for (int j = 0; j < n; j++) {
                for (int k = 0; k < n; k++) { adj[j][k] = min(adj[j][k], adj[j][i] + adj[i][k]); }
            }
        }
        int graph_diameter = 0;
        for (int i = 0; i < n; i++) {
            for (int j = 0; j < n; j++) {
                if (adj[i][j] < 2e9)  // Assuming 2e9 is infinity
                    graph_diameter = max(graph_diameter, adj[i][j]);
            }
        }
        return graph_diameter;
    }
};
