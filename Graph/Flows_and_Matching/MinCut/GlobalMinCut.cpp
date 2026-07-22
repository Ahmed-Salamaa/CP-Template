/*
 * Topic: Stoer-Wagner Algorithm
 * Description: Computes the Global Minimum Cut of an undirected graph (the minimum weight of
 *   edges to remove to disconnect the graph into two components) using Stoer-Wagner.
 * Usage:
 *    int min_cut = GlobalMinCut::stoer_wagner(n, weight_matrix);
 */
#include "../../../core.h"

// Time Complexity: O(V^3)
// Space Complexity: O(V^2)
// Stoer–Wagner global min‐cut on an undirected weighted graph.
// weight[u][v] = weight of edge u–v (0 if none).
// Returns the weight of the minimum cut separating the vertices into two nonempty sets.
int stoer_wagner(int n, vector<vector<int>>& weight) {
    vector<bool> alive(n, true);
    vector<int> dist(n);
    int min_cut = 1e18;
    for (int phase = n; phase > 1; --phase) {
        fill(all(dist), 0);
        vector<bool> added(n, false);
        int prev = -1;
        for (int i = 0; i < phase; ++i) {
            int pick = -1;
            for (int j = 0; j < n; ++j) {
                if (alive[j] && !added[j] && (pick == -1 || dist[j] > dist[pick])) pick = j;
            }
            added[pick] = true;
            if (i == phase - 1) {
                if (dist[pick] < min_cut) min_cut = dist[pick];
                for (int v = 0; v < n; ++v) {
                    weight[prev][v] += weight[pick][v];
                    weight[v][prev] = weight[prev][v];
                }
                alive[pick] = false;
            } else {
                prev = pick;
                for (int v = 0; v < n; ++v) {
                    if (alive[v] && !added[v]) dist[v] += weight[pick][v];
                }
            }
        }
    }
    return min_cut;
}
