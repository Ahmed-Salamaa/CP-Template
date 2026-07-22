/* Topic: 1D Fenwick Tree (Binary Indexed Tree)
 * Description: Fenwick Tree (Binary Indexed Tree) is a highly efficient data structure for
 *   maintaining prefix sums and answering point update or range sum queries in O(log
 *   N) time.
 * Usage: 
 *   FenwickTree<int> ft; ft.init(n); ft.add(pos, val); ft.query(l, r);
 *   SparseFenwickTree<int> sft; sft.add(pos, val); sft.query(l, r);
 *   FenwickMultiset ms; ms.init(n); ms.insert(v); ms.count(v);
 */
#include "../../core.h"

namespace FenwickTreeSpace {

    template<typename T = int>
    struct FenwickTree {
        int n;
        vector<T> tree;
        int N = 1;
        
        // Time Complexity: O(N)
        // Space Complexity: O(N)
        void init(int _n) {
            n = _n;
            tree.assign(n + 1, 0);
            N = n ? (1 << (31 - __builtin_clz(n))) * 2 : 1;
        }
        
        // 0-indexed position
        // Time Complexity: O(log N)
        // Space Complexity: O(1)
        void add(int pos, T value) {
            for (int i = pos + 1; i <= n; i += i & -i) {
                tree[i] += value;
            }
        }
        
        // 0-indexed position
        // Time Complexity: O(log N)
        // Space Complexity: O(1)
        T get(int pos) {
            T sum = 0;
            for (int i = min(pos + 1, n); i > 0; i -= i & -i) {
                sum += tree[i];
            }
            return sum;
        }
        
        // 0-indexed inclusive range [l, r]
        // Time Complexity: O(log N)
        // Space Complexity: O(1)
        T query(int l, int r) {
            return get(r) - get(l - 1);
        }
        
        // Finds first index with prefix sum >= t
        // Time Complexity: O(log N)
        // Space Complexity: O(1)
        int lower_bound(T t) {
            T sum = 0;
            int pos = 0;
            for (int i = N; i > 0; i >>= 1) {
                if (pos + i <= n && sum + tree[pos + i] < t) {
                    sum += tree[pos + i];
                    pos += i;
                }
            }
            return pos; // 0-based index
        }
    };

    template<typename T = int>
    struct SparseFenwickTree {
        map<int, T> tree;
        int max_val;
        
        SparseFenwickTree(int max_val = 1e9 + 9) : max_val(max_val) {}

        // 0-indexed position
        // Time Complexity: O(log(MAX) * log(Operations))
        // Space Complexity: O(Operations * log(MAX))
        void add(int pos, T val) {
            for (int i = pos + 1; i < max_val; i += i & -i) {
                tree[i] += val;
            }
        }
        
        // Time Complexity: O(log(MAX) * log(Operations))
        // Space Complexity: O(1)
        T get(int pos) {
            T sum = 0;
            for (int i = pos + 1; i > 0; i -= i & -i) {
                if (tree.count(i)) {
                    sum += tree[i];
                }
            }
            return sum;
        }
        
        // Time Complexity: O(log(MAX) * log(Operations))
        // Space Complexity: O(1)
        T query(int l, int r) {
            return get(r) - get(l - 1);
        }
    };

    struct FenwickMultiset {
        FenwickTree<int> bit;
        int max_val;

        void init(int _max_val) { 
            max_val = _max_val;
            bit.init(max_val); 
        }
        
        // Time Complexity: O(log MAX)
        // Space Complexity: O(1)
        void insert(int v) { bit.add(v, 1); }
        void erase(int v) { if (count(v)) bit.add(v, -1); }
        int count(int v) { return bit.query(v, v); }
        int size() { return bit.get(max_val - 1); }
        int at(int k) { return bit.lower_bound(k + 1); }
        int order_of_key(int v) { return bit.get(v - 1); }
        int largerThanMe(int v) { return size() - bit.get(v); }
        int largerThanOrEqualMe(int v) { return size() - bit.get(v - 1); }
        int lessThanMe(int v) { return bit.get(v - 1); }
        int lessThanOrEqualMe(int v) { return bit.get(v); }
    };

} // namespace FenwickTreeSpace
