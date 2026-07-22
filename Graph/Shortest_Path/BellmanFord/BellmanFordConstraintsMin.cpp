/*
 * Topic: Bellman-Ford (System of Difference Constraints - Min Solution)
 * Description: Bellman-Ford algorithm adapted for minimizing variable values in a system of
 *   difference constraints. It helps find the smallest valid assignments satisfying
 *   multiple relational constraints.
 * assignment for variables relative to a base or determining if none exists. Usage: BellmanFordConstraintsMin bf;
 *    bf.setNumVariables(n);
 *    bf.addConstraint(u, v, C);
 *    if (bf.isSolvableMin()) { ... }
 */
#include "../../../core.h"

struct BellmanFordConstraintsMin {
    vector<array<int, 3>> constraints;  // {u, v, C} for X_u - X_v >= C
    int num_vars = 0;

    void setNumVariables(int n) { num_vars = n; }

    void addConstraint(int u, int v, int C) { constraints.push_back({(int)u, (int)v, C}); }

    // Time Complexity: O(V * E)
    // Space Complexity: O(V + E)
    bool isSolvableMin() const {
        vector<int> dist(num_vars, 0);
        int m = sz(constraints);
        for (int i = 0; i < num_vars; ++i) {
            bool updated = false;
            for (int j = 0; j < m; ++j) {
                int u = (int)constraints[j][0];
                int v = (int)constraints[j][1];
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
    vector<int> getMinValues() const {
        vector<int> dist(num_vars, 0);
        int m = sz(constraints);
        for (int i = 0; i < num_vars - 1; ++i) {
            for (int j = 0; j < m; ++j) {
                int u = (int)constraints[j][0];
                int v = (int)constraints[j][1];
                int C = constraints[j][2];
                dist[u] = max(dist[u], dist[v] + C);
            }
        }
        return dist;
    }
};
