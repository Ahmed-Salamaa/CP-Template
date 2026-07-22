/* Topic: 2D Fenwick Tree
 * Description: A multi-dimensional Fenwick Tree that calculates subgrid sums and point updates
 *   over a matrix in O(log^2 N) time. Ideal for dynamic 2D region queries.
 * Usage: 
 *   FenwickTree2D<int> ft2; ft2.init(n, m); ft2.add(x, y, val); ft2.query(r1, c1, r2, c2);
 *   OfflineFenwickTree2D<int> oft2(n, coords); oft2.add(x, y, val); oft2.query(r1, c1, r2, c2);
 *   FenwickTree2DRangeUpdate<int> ft2ru(n, m); ft2ru.add_range(r1, c1, r2, c2, val); ft2ru.query_range(r1, c1, r2, c2);
 */
#include "../../core.h"

namespace FenwickTree2DSpace {

    template<typename T = int>
    struct FenwickTree2D {
        vector<vector<T>> tree;
        int n, m;

        // Time Complexity: O(N * M)
        // Space Complexity: O(N * M)
        void init(int _n, int _m) {
            n = _n; m = _m;
            tree.assign(n + 1, vector<T>(m + 1, 0));
        }

        // 0-indexed position
        // Time Complexity: O(log N * log M)
        // Space Complexity: O(1)
        void add(int x, int y, T val) {
            for (int i = x + 1; i <= n; i += (i & (-i))) {
                for (int j = y + 1; j <= m; j += (j & (-j))) {
                    tree[i][j] += val;
                }
            }
        }

        // 0-indexed position
        // Time Complexity: O(log N * log M)
        // Space Complexity: O(1)
        T get(int x, int y) {
            T ret = 0;
            for (int i = x + 1; i > 0; i -= (i & (-i))) {
                for (int j = y + 1; j > 0; j -= (j & (-j))) {
                    ret += tree[i][j];
                }
            }
            return ret;
        }

        // 0-indexed range
        // Time Complexity: O(log N * log M)
        // Space Complexity: O(1)
        T query(int sx, int sy, int ex, int ey) {
            return get(ex, ey) - get(ex, sy - 1) - get(sx - 1, ey) + get(sx - 1, sy - 1);
        }
    };

    template <typename T = int>
    struct OfflineFenwickTree2D {
        int n;
        vector<vector<int>> vals;
        vector<vector<T>> bit;

        int ind(const vector<int> &v, int x) {
            return upper_bound(begin(v), end(v), x) - begin(v) - 1;
        }

        OfflineFenwickTree2D() : n(0) {}

        // n is the limit of the first dimension
        // Time Complexity: O(Q log^2 Q) for initialization
        // Space Complexity: O(Q log Q)
        OfflineFenwickTree2D(int _n, vector<array<int, 2>> &todo) : n(_n), vals(n + 1), bit(n + 1) {
            sort(begin(todo), end(todo), [](auto &a, auto &b) { return a[1] < b[1]; });

            for (int i = 1; i <= n; i++) vals[i].push_back(0);
            for (auto p : todo) {
                int r = p[0], c = p[1];
                r++, c++;
                for (; r <= n; r += r & -r) {
                    if (vals[r].empty() || vals[r].back() != c) vals[r].push_back(c);
                }
            }
            for (int i = 1; i <= n; i++) bit[i].assign(vals[i].size(), 0);
        }

        // 0-indexed position
        // Time Complexity: O(log N * log Q)
        // Space Complexity: O(1)
        void add(int r, int c, T val) {
            r++, c++;
            for (; r <= n; r += r & -r) {
                int i = ind(vals[r], c);
                for (; i < sz(bit[r]); i += i & -i) bit[r][i] += val;
            }
        }

        // 0-indexed prefix
        // Time Complexity: O(log N * log Q)
        // Space Complexity: O(1)
        T get(int r, int c) {
            r++, c++;
            T sum = 0;
            for (; r > 0; r -= r & -r) {
                int i = ind(vals[r], c);
                for (; i > 0; i -= i & -i) sum += bit[r][i];
            }
            return sum;
        }

        // 0-indexed range
        // Time Complexity: O(log N * log Q)
        // Space Complexity: O(1)
        T query(int r1, int c1, int r2, int c2) {
            return get(r2, c2) - get(r2, c1 - 1) - get(r1 - 1, c2) + get(r1 - 1, c1 - 1);
        }
    };

    template<typename T = int>
    struct FenwickTree2DRangeUpdate {
        vector<vector<T>> S[4];
        int n, m;

        // Time Complexity: O(N * M)
        // Space Complexity: O(N * M)
        void init(int _n, int _m) {
            n = _n; m = _m;
            for(int i = 0; i < 4; i++) {
                S[i].assign(n + 2, vector<T>(m + 2, 0));
            }
        }

        void add_point(int z, int x, int Y, T c) {
            for(; x <= n + 1; x += x & -x) {
                for(int y = Y; y <= m + 1; y += y & -y) {
                    S[z][x][y] += c;
                }
            }
        }

        // add c on [a, inf) x [b, inf)
        void add_suffix(int a, int b, T c) {
            add_point(0, a, b, c);
            add_point(1, a, b, -c * (a - 1));
            add_point(2, a, b, -c * (b - 1));
            add_point(3, a, b, c * (a - 1) * (b - 1));
        }

        // 1-indexed range update
        // Time Complexity: O(log N * log M)
        // Space Complexity: O(1)
        void add_range(int x1, int y1, int x2, int y2, T c) {
            add_suffix(x1, y1, c);
            add_suffix(x1, y2 + 1, -c);
            add_suffix(x2 + 1, y1, -c);
            add_suffix(x2 + 1, y2 + 1, c);
        }

        T get_point(int z, int x, int Y) {
            T res = 0;
            for(; x > 0; x -= x & -x) {
                for(int y = Y; y > 0; y -= y & -y) {
                    res += S[z][x][y];
                }
            }
            return res;
        }

        // get sum on [1, x] x [1, y]
        T get_prefix(int x, int y) {
            if (x <= 0 || y <= 0) return 0;
            return get_point(0, x, y) * x * y
                 + get_point(1, x, y) * y
                 + get_point(2, x, y) * x
                 + get_point(3, x, y);
        }

        // 1-indexed range query
        // Time Complexity: O(log N * log M)
        // Space Complexity: O(1)
        T query_range(int x1, int y1, int x2, int y2) {
            return get_prefix(x2, y2)
                 - get_prefix(x1 - 1, y2)
                 - get_prefix(x2, y1 - 1)
                 + get_prefix(x1 - 1, y1 - 1);
        }
    };

} // namespace FenwickTree2DSpace
