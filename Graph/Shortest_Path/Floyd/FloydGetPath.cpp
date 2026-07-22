/*
 * Topic: Floyd-Warshall (Get Path)
 * Description: Floyd-Warshall algorithm extended with a parent or next-node array to
 *   reconstruct the actual sequence of vertices forming the shortest path. Essential
 *   when the actual route, not just the distance, is required.
 * Usage:
 *    FloydGetPath fw(adj_matrix, parent_matrix);
 *    fw.getPath(u, v);
 *    vector<int> path = fw.path;
 */
#include "../../../core.h"

struct FloydGetPath {
    vector<vector<int>> adj, parent;
    vector<int> path;

    FloydGetPath(const vector<vector<int>>& adjacency_matrix, const vector<vector<int>>& parent_matrix)
        : adj(adjacency_matrix), parent(parent_matrix) {}

    // Time Complexity: O(V)
    // Space Complexity: O(V) for the path recursion
    void getPath(int i, int j) {
        if (i != j) { getPath(i, parent[i][j]); }
        path.emplace_back(j);
    }
};
