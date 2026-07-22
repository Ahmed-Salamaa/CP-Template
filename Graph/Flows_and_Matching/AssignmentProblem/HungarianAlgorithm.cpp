/*
 * Topic: Hungarian Algorithm
 * Description: Hungarian algorithm to solve the classic Assignment Problem, matching N workers
 *   to N jobs to minimize total cost in O(N^3).
 * Usage:
 *    Hungarian<int> solver;
 *    vector<vector<int>> cost = {{...}, {...}};
 *    auto [min_cost, assignment] = solver.solve(cost);
 *    // assignment[j] contains the left node assigned to right node j
 */
#include "../../../core.h"

// Time Complexity: O(N^2 * M)
// Space Complexity: O(N * M)
template <typename T>
struct Hungarian {
    pair<T, vector<int>> solve(const vector<vector<T>>& a) {
        if (a.empty()) return {0, {}};
        int n = sz(a) + 1, m = a[0].size() + 1;
        vector<T> u(n, 0), v(m, 0);
        vector<int> p(m, 0);
        for (int i = 1; i < n; i++) {
            p[0] = i;
            int j0 = 0;
            vector<T> dist(m, numeric_limits<T>::max());
            vector<int> pre(m, -1);
            vector<bool> done(m, false);
            do {
                done[j0] = true;
                int i0 = p[j0], j1 = 0;
                T delta = numeric_limits<T>::max();
                for (int j = 1; j < m; j++) {
                    if (!done[j]) {
                        T cur = a[i0 - 1][j - 1] - u[i0] - v[j];
                        if (cur < dist[j]) {
                            dist[j] = cur;
                            pre[j] = j0;
                        }
                        if (dist[j] < delta) {
                            delta = dist[j];
                            j1 = j;
                        }
                    }
                }
                for (int j = 0; j < m; j++) {
                    if (done[j]) {
                        u[p[j]] += delta;
                        v[j] -= delta;
                    } else
                        dist[j] -= delta;
                }
                j0 = j1;
            } while (p[j0]);
            while (j0) {
                int j1 = pre[j0];
                p[j0] = p[j1];
                j0 = j1;
            }
        }
        vector<int> ans(n - 1, -1);
        for (int j = 1; j < m; j++)
            if (p[j]) ans[p[j] - 1] = j - 1;
        return {-v[0], ans};
    }
};
