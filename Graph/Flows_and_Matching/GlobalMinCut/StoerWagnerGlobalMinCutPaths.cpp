/*
 * Topic: Stoer-Wagner
 * Description: Stoer-Wagner algorithm extended to output the exact partition of vertices that
 *   forms the Global Minimum Cut.
 * elements on one side of the cut. Usage: StoerWagnerPaths solver; auto [min_cut_value, side] =
 * solver.solve(adj_matrix);
 */
#include "../../../core.h"

// Time Complexity: O(V^3), can be optimized to O(V E + V^2 log V)
// Space Complexity: O(V^2)
struct StoerWagnerPaths {
    pair<int, vector<int>> solve(vector<vector<int>> mat) {
        pair<int, vector<int>> best = {numeric_limits<int>::max(), {}};
        int n = sz(mat);
        vector<vector<int>> co(n);
        for (int i = 0; i < n; i++) co[i] = {i};
        for (int ph = 1; ph < n; ph++) {
            vector<int> w = mat[0];
            size_t s = 0, t = 0;
            for (int it = 0; it < n - ph; it++) {
                w[t] = numeric_limits<int>::min();
                s = t, t = max_element(all(w)) - w.begin();
                for (int i = 0; i < n; i++) w[i] += mat[t][i];
            }
            best = min(best, {w[t] - mat[t][t], co[t]});
            co[s].insert(co[s].end(), co[t].begin(), co[t].end());
            for (int i = 0; i < n; i++) mat[s][i] += mat[t][i];
            for (int i = 0; i < n; i++) mat[i][s] = mat[s][i];
            mat[0][t] = numeric_limits<int>::min();
        }
        return best;
    }
};
