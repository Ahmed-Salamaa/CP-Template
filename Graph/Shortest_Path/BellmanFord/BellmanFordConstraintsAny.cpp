/*
 * Topic: Bellman-Ford (System of Difference Constraints - Any Solution)
 * Description: Bellman-Ford algorithm variant adapted for solving general system of difference
 *   constraints. It is used to find a set of values satisfying a system of
 *   inequalities with any type of bounds.
 * determining if none exists. Usage: BellmanFordConstraintsAny bf; bf.setNumVariables(n); bf.addConstraint(u, v, C); if
 * (bf.isSolvable()) { ... }
 */
#include "../../../core.h"

struct BellmanFordConstraintsAny {
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
                if (dist[u] < dist[v] + C) {
                    dist[u] = dist[v] + C;
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
    vector<int> getAnyValues() const {
        vector<int> dist(num_vars, 0);
        int m = sz(constraints);
        for (int i = 0; i < num_vars - 1; ++i) {
            for (int j = 0; j < m; ++j) {
                int u = constraints[j][0];
                int v = constraints[j][1];
                int C = constraints[j][2];
                dist[u] = max(dist[u], dist[v] + C);
            }
        }
        return dist;
    }
};
