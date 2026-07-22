## Gaussian Elimination

**Topic:** Gaussian Elimination - Solves system of N linear equations using an augmented matrix

**Time Complexity:** O(n^3)
**Space Complexity:** O(n^2)

```cpp
//
// Description:
// Finds the roots of a system of linear equations represented as an augmented matrix.
// Uses Gaussian Elimination with partial pivoting for numerical stability.
//
// How to use it:
// vector<vector<double>> equations = {{2.0, 1.0, -1.0, 8.0}, {-3.0, -1.0, 2.0, -11.0}, {-2.0, 1.0, 2.0, -3.0}};
// vector<double> roots = GaussianElimination::gauss_solve(3, equations);
#include "../../../core.h"

namespace GaussianElimination {
            vector<double> gauss_solve(int n, vector<vector<double>> aug) {
        for (int i = 0; i < n - 1; i++) {
            int l = i;
            for (int j = i + 1; j < n; j++)
                if (fabs(aug[j][i]) > fabs(aug[l][i]))
                    l = j;

            for (int k = i; k <= n; k++)
                swap(aug[i][k], aug[l][k]);

            for (int j = i + 1; j < n; j++)
                for (int k = n; k >= i; k--)
                    aug[j][k] -= aug[i][k] * aug[j][i] / aug[i][i];
        }
        vector<double> ans(n);
        for (int j = n - 1; j >= 0; j--) {
            double t = 0;
            for (int k = j + 1; k < n; k++) {
                t += aug[j][k] * ans[k];
            }
            ans[j] = (aug[j][n] - t) / (aug[j][j]);
        }
        return ans;
    }
}
```

## GaussianEliminationReal

**Description:** * Topic: Gaussian Elimination Real - Solve linear equations systems with real coefficients * Description: Uses Gaussian elimination to find solutions to a system of linear equations. * Can detect if there is a unique solution, infinite solutions, or no solution. * Usage: * vector<vector<double>> a = {{2, 1, -1, 8}, {-3, -1, 2, -11}, {-2, 1, 2, -3}}; * vector<double> ans; * int type = GaussianEliminationReal::gauss(a, ans);

**Time Complexity:** O(N * M^2) where N is equations, M is variables
**Space Complexity:** O(N * M)

```cpp
#include "../../../core.h"

namespace GaussianEliminationReal {
    const double eps = 1e-9;

            // Returns: 1 for unique solution, 2 for infinite solutions, -1 for no solution
    int gauss(vector<vector<double>>& a, vector<double>& ans) {
        if (a.empty()) return -1;
        int n = sz(a);
        int m = (int)a[0].size() - 1;
        vector<int> pos(m, -1);
        double det = 1;
        int rank = 0;
        
        for (int col = 0, row = 0; col < m && row < n; ++col) {
            int mx = row;
            for (int i = row; i < n; i++) {
                if (abs(a[i][col]) > abs(a[mx][col])) mx = i;
            }
            if (abs(a[mx][col]) < eps) {
                det = 0;
                continue;
            }
            for (int i = col; i <= m; i++) swap(a[row][i], a[mx][i]);
            if (row != mx) det = -det;
            det *= a[row][col];
            pos[col] = row;
            for (int i = 0; i < n; i++) {
                if (i != row && abs(a[i][col]) > eps) {
                    double c = a[i][col] / a[row][col];
                    for (int j = col; j <= m; j++) a[i][j] -= a[row][j] * c;
                }
            }
            ++row;
            ++rank;
        }
        
        ans.assign(m, 0);
        for (int i = 0; i < m; i++) {
            if (pos[i] != -1) ans[i] = a[pos[i]][m] / a[pos[i]][i];
        }
        for (int i = 0; i < n; i++) {
            double sum = 0;
            for (int j = 0; j < m; j++) sum += ans[j] * a[i][j];
            if (abs(sum - a[i][m]) > eps) return -1; // no solution
        }
        for (int i = 0; i < m; i++) {
            if (pos[i] == -1) return 2; // infinite solutions
        }
        return 1; // unique solution
    }
}
```

## NonNegativeNVarLinearDiophantineEquation

**Description:** * Topic: Non-Negative N-Variable LDE (Coin Change via Dijkstra) * Description: Finds the number of solutions to sum(a[i] * x[i]) = k, where x[i] >= 0. * Also known as the Frobenius Coin Problem on a Modulo Graph. * Usage: * vector<int> a = {3, 5, 7}; * NonNegativeNVarLinearDiophantineEquation::LDE solver(a); * int count = solver.count(10, 20);

**Time Complexity:** O(min(a[i]) * N * log(min(a[i]))) for preprocessing
**Space Complexity:** O(min(a[i]))

```cpp
#include "../../../core.h"

namespace NonNegativeNVarLinearDiophantineEquation {

            struct LDE {
        int n, x;
        vector<int> d;
        const int INF = 1e18;

        LDE(const vector<int>& a) {
            n = sz(a);
            if (n == 0) { x = 0; return; }
            x = *min_element(all(a));
            if (x <= 0) return;
            
            d.assign(x, INF);
            d[0] = 0;
            priority_queue<pair<int, int>, vector<pair<int, int>>, greater<pair<int, int>>> q;
            q.push({0, 0});
            
            while (!q.empty()) {
                auto [val, u] = q.top();
                q.pop();
                
                if (d[u] != val) continue;
                for (int i = 0; i < n; ++i) {
                    int tmp = val + a[i];
                    int v = tmp % x;
                    if (d[v] > tmp) {
                        d[v] = tmp;
                        q.push({tmp, v});
                    }
                }
            }
        }

        // Time Complexity: O(1)
        // Space Complexity: O(1)
        bool can(int k) const {
            if (x <= 0) return false;
            return d[k % x] <= k;
        }

        // Time Complexity: O(min(a[i]))
        // Space Complexity: O(1)
        int count(int l, int r) const {
            if (x <= 0 || l > r) return 0;
            int ans = 0;
            for (int i = 0; i < x; i++) {
                if (d[i] <= r) ans += (r - d[i]) / x + 1;
                if (d[i] <= l - 1) ans -= (l - 1 - d[i]) / x + 1;
            }
            return ans;
        }
    };
}
```

