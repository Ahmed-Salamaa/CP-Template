/*
 * Topic: Stoer-Wagner
 * Description: Stoer-Wagner algorithm for finding the exact value of the Global Minimum Cut in
 *   an undirected graph in O(V E + V^2 log V) time.
 * Usage:
 *    StoerWagnerValue solver;
 *    int min_cut = solver.solve(n, weight_matrix);
 */
#include "../../../core.h"

// Time Complexity: O(V^3), can be optimized to O(V E + V^2 log V)
// Space Complexity: O(V^2)
struct StoerWagnerValue {
    int solve(int n, vector<vector<int>> weight) {
        vector<bool> alive(n, true);
        vector<int> best(n), dist(n);
        int min_cut = numeric_limits<int>::max();
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
};
