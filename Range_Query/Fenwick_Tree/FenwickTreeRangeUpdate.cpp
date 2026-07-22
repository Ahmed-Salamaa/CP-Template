/* Topic: Fenwick Tree Range Update
 * Description: Adapts the standard Fenwick Tree to allow both range additions and point/range
 *   queries by maintaining dual trees. Serves as a lightweight alternative to a Lazy
 *   Segment Tree for sum queries.
 * Usage: FenwickTreeRangeUpdate<int> ftru; ftru.init(n); ftru.addRange(l, r, val); ftru.queryRange(l, r);
 */
#include "../../core.h"

namespace FenwickTreeRangeUpdateSpace {

    template<typename T = int>
    struct FenwickTreeRangeUpdate {
        int N;
        vector<T> m, c;

        // Time Complexity: O(N)
        // Space Complexity: O(N)
        void init(int x) {
            N = x;
            m.assign(N + 1, 0);
            c.assign(N + 1, 0);
        }

        void add(int pos, T mVal, T cVal) {
            for (++pos; pos <= N; pos += pos & -pos) {
                m[pos] += mVal;
                c[pos] += cVal;
            }
        }

        // Time Complexity: O(log N)
        // Space Complexity: O(1)
        T get(int pos) {
            T ret = 0;
            int x = pos;
            for (pos++; pos > 0; pos -= pos & -pos) {
                ret += m[pos] * x + c[pos];
            }
            return ret;
        }

        // 0-indexed range addition [l, r]
        // Time Complexity: O(log N)
        // Space Complexity: O(1)
        void addRange(int l, int r, T value) {
            add(l, value, -value * (l - 1));
            add(r + 1, -value, value * r);
        }

        // 0-indexed range query [l, r]
        // Time Complexity: O(log N)
        // Space Complexity: O(1)
        T queryRange(int l, int r) {
            return get(r) - get(l - 1);
        }
    };

} // namespace FenwickTreeRangeUpdateSpace
