/*
 * Topic: Gaussian Elimination Modulo - Solve linear equations systems modulo prime
 * Description: Gaussian elimination performed over a finite field (modulo a prime) to solve
 *   systems of linear equations or find matrix determinants and inverses. Essential
 *   in combinatorial problems and expected value DP over grids.
 * Returns the number of free variables or -1 if no solution exists.
 * Usage:
 * vector<vector<int>> a = {{2, 1, 8}, {3, 4, 2}}; // equations under modulo
 * vector<int> ans;
 * int free_vars = GaussianEliminationMod::gauss(a, ans);
 */
#include "../../../core.h"
const int mod = 1e9 + 7;

inline int power(int n, int k) {
    int ans = 1 % mod;
    n %= mod;
    if (n < 0) n += mod;
    while (k) {
        if (k & 1) ans = (int)ans * n % mod;
        n = (int)n * n % mod;
        k >>= 1;
    }
    return ans;
}

// Time Complexity: O(N * M^2) where N is equations, M is variables
// Space Complexity: O(N * M)
// Returns: number of free variables, or -1 if no solution
int gauss(vector<vector<int>>& a, vector<int>& ans) {
    if (a.empty()) return -1;
    int n = sz(a), m = (int)a[0].size() - 1;
    vector<int> pos(m, -1);
    int free_var = 0;
    const int MODSQ = (int)mod * mod;
    int det = 1, rank = 0;
    for (int col = 0, row = 0; col < m && row < n; col++) {
        int mx = row;
        for (int k = row; k < n; k++)
            if (a[k][col] > a[mx][col]) mx = k;
        if (a[mx][col] == 0) {
            det = 0;
            continue;
        }
        for (int j = col; j <= m; j++) swap(a[mx][j], a[row][j]);
        if (row != mx) det = det == 0 ? 0 : mod - det;
        det = 1LL * det * a[row][col] % mod;
        pos[col] = row;
        int inv = power(a[row][col], mod - 2);
        for (int i = 0; i < n && inv; i++) {
            if (i != row && a[i][col]) {
                int x = ((int)a[i][col] * inv) % mod;
                for (int j = col; j <= m && x; j++) {
                    if (a[row][j]) a[i][j] = (MODSQ + a[i][j] - ((int)a[row][j] * x)) % mod;
                }
            }
        }
        row++;
        ++rank;
    }
    ans.assign(m, 0);
    for (int i = 0; i < m; i++) {
        if (pos[i] == -1)
            free_var++;
        else
            ans[i] = ((int)a[pos[i]][m] * power(a[pos[i]][i], mod - 2)) % mod;
    }
    for (int i = 0; i < n; i++) {
        int val = 0;
        for (int j = 0; j < m; j++) val = (val + ((int)ans[j] * a[i][j])) % mod;
        if (val != a[i][m]) return -1;  // no solution
    }
    return free_var;  // has solution
}
