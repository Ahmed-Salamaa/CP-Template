/* Topic: 2D Sparse Table
 * Description: Extends the Sparse Table to two dimensions for rapid queries over a grid. Used
 *   to find the min/max or GCD of arbitrary rectangular submatrices in O(1) time.
 * Usage: SparseTable2D<int> st; st.build(mat); st.query(x1, y1, x2, y2);
 */
#include "../../core.h"

template <typename T>
struct SparseTable2D {
    int n, m, lgN, lgM;
    vector<vector<vector<vector<T>>>> st;
    function<T(T, T)> combine;

    void init(int _n, int _m, function<T(T, T)> F) {
        n = _n;
        m = _m;
        lgN = __lg(n);
        lgM = __lg(m);
        combine = F;
        st.assign(n, vector<vector<vector<T>>>(m, vector<vector<T>>(lgN + 1, vector<T>(lgM + 1))));
    }

    void build(const vector<vector<T>>& a) {
        // Time Complexity: O(N * M * log N * log M)
        // Space Complexity: O(N * M * log N * log M)
        for (int i = 0; i < n; i++) {
            for (int j = 0; j < m; j++) { st[i][j][0][0] = a[i][j]; }
        }
        for (int a_lg = 0; a_lg <= lgN; a_lg++) {
            for (int b_lg = 0; b_lg <= lgM; b_lg++) {
                if (a_lg + b_lg == 0) continue;
                for (int i = 0; i + (1 << a_lg) <= n; i++) {
                    for (int j = 0; j + (1 << b_lg) <= m; j++) {
                        if (!a_lg) {
                            st[i][j][a_lg][b_lg] =
                                combine(st[i][j][a_lg][b_lg - 1], st[i][j + (1 << (b_lg - 1))][a_lg][b_lg - 1]);
                        } else {
                            st[i][j][a_lg][b_lg] =
                                combine(st[i][j][a_lg - 1][b_lg], st[i + (1 << (a_lg - 1))][j][a_lg - 1][b_lg]);
                        }
                    }
                }
            }
        }
    }

    T query(int x1, int y1, int x2, int y2) {
        // Time Complexity: O(1)
        // Precondition: Operation must be idempotent
        int kx = 31 - __builtin_clz(x2 - x1 + 1);
        int ky = 31 - __builtin_clz(y2 - y1 + 1);
        T ans1 = combine(st[x1][y1][kx][ky], st[x2 - (1 << kx) + 1][y1][kx][ky]);
        T ans2 = combine(st[x1][y2 - (1 << ky) + 1][kx][ky], st[x2 - (1 << kx) + 1][y2 - (1 << ky) + 1][kx][ky]);
        return combine(ans1, ans2);
    }
};
