/*
 * Topic: Bellman-Ford (System of Difference Constraints - Max Solution)
 * Description: Bellman-Ford algorithm variation customized for difference constraint systems
 *   focusing on maximizing the variables. Useful when we need to find the maximum
 *   possible values that satisfy given inequalities.
 * assignment for variables relative to a base or determining if none exists. Usage: BellmanFordConstraintsMax bf;
 *    bf.setNumVariables(n);
 *    bf.addConstraint(u, v, C);
 *    if (bf.isSolvable()) { ... }
 */
#include "../../../core.h"

struct BellmanFordConstraintsMax {
    vector<array<int, 3>> constraints;  // {u, v, C} for X_u - X_v >= C
    int num_vars = 0;

    void setNumVariables(int n) { num_vars = n; }

    void addConstraint(int u, int v, int C) { constraints.push_back({u, v, static_cast<int>(C)}); }

    // Time Complexity: O(V * E)
    // Space Complexity: O(V + E)
    bool isSolvable() const {
        vector<int> dist(num_vars, 0);
        int m = sz(constraints);
        for (int i = 0; i < num_vars; ++i) {
            bool updated = false;
            for (int j = 0; j < m; ++j) {
                int u = constraints[j][0];
                int v = constraints[j][1];
                int C = constraints[j][2];
                if (dist[v] < dist[u] + C) {
                    dist[v] = dist[u] + C;
                    updated = true;
                }
            }
            if (!updated) return true;
            if (i == num_vars - 1) return false;
        }
        return true;
    }

    // Time Complexity: O(V * E)
    // Space Complexity: O(V + E)
    vector<int> getMaxValues() const {
        vector<int> dist(num_vars, 0);
        int m = sz(constraints);
        for (int i = 0; i < num_vars - 1; ++i) {
            for (int j = 0; j < m; ++j) {
                int u = constraints[j][0];
                int v = constraints[j][1];
                int C = constraints[j][2];
                dist[v] = max(dist[v], dist[u] + C);
            }
        }
        vector<int> result(num_vars);
        for (int i = 0; i < num_vars; ++i) result[i] = -dist[i];
        return result;
    }
};
