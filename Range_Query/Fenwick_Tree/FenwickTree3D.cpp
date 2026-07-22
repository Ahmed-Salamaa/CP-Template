/* Topic: 3D Fenwick Tree
 * Description: Extends the Fenwick Tree to three dimensions for handling updates and queries
 *   within spatial volumes. Applied in 3D geometry problems or maintaining dynamic
 *   spatial aggregates.
 * Usage: FenwickTree3D<int> ft3(n, m, p); ft3.update(x, y, z, val); ft3.query(x1, y1, z1, x2, y2, z2);
 */
#include "../../core.h"

namespace FenwickTree3DSpace {

    template<typename T = int>
    struct FenwickTree3D {
        int n, m, p;
        vector<vector<vector<T>>> bit;

        // Time Complexity: O(N * M * P)
        // Space Complexity: O(N * M * P)
        FenwickTree3D(int rows, int cols, int depths) {
            n = rows;
            m = cols;
            p = depths;
            bit.assign(n + 1, vector<vector<T>>(m + 1, vector<T>(p + 1, 0)));
        }

        // 1-indexed point update
        // Time Complexity: O(log N * log M * log P)
        // Space Complexity: O(1)
        void update(int x, int y, int z, T delta) {
            for (int i = x; i <= n; i += i & -i) {
                for (int j = y; j <= m; j += j & -j) {
                    for (int k = z; k <= p; k += k & -k) {
                        bit[i][j][k] += delta;
                    }
                }
            }
        }

        // Time Complexity: O(log N * log M * log P)
        // Space Complexity: O(1)
        T query(int x, int y, int z) {
            T res = 0;
            for (int i = x; i > 0; i -= i & -i) {
                for (int j = y; j > 0; j -= j & -j) {
                    for (int k = z; k > 0; k -= k & -k) {
                        res += bit[i][j][k];
                    }
                }
            }
            return res;
        }

        // 1-indexed range query
        // Time Complexity: O(log N * log M * log P)
        // Space Complexity: O(1)
        T query(int x1, int y1, int z1, int x2, int y2, int z2) {
            return query(x2, y2, z2)
                 - query(x1 - 1, y2, z2) - query(x2, y1 - 1, z2) - query(x2, y2, z1 - 1)
                 + query(x1 - 1, y1 - 1, z2) + query(x1 - 1, y2, z1 - 1) + query(x2, y1 - 1, z1 - 1)
                 - query(x1 - 1, y1 - 1, z1 - 1);
        }
    };

} // namespace FenwickTree3DSpace
