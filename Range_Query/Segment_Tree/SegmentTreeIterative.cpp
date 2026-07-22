/* Topic: Iterative Segment Tree
 * Description: A non-recursive, array-based implementation of a Segment Tree that is generally
 *   faster and uses less memory. Often preferred for its low constant factor in
 *   update and query scenarios.
 * Usage: SegmentTreeIterative<int> st(n); st.update(idx, val); st.query(l, r);
 */
#include "../../core.h"
template <typename T = int>
struct SegmentTreeIterative {
  int n;
  vector<T> tree;
  T def = 0;
  T calc(T a, T b) { return a + b; }
  SegmentTreeIterative(int n) : n(n), tree(2 * n, def) {}
  SegmentTreeIterative(const vector<T>& arr) {
    n = sz(arr);
    tree.assign(2 * n, def);
    for (int i = 0; i < n; i++) tree[n + i] = arr[i];
    for (int i = n - 1; i > 0; --i) tree[i] = calc(tree[i << 1], tree[i << 1 | 1]);
  }
  void update(int p, T value) {
    for (tree[p += n] = value; p > 1; p >>= 1) tree[p >> 1] = calc(tree[p], tree[p ^ 1]);
  }
  T query(int l, int r) {
    T resL = def, resR = def;
    for (l += n, r += n + 1; l < r; l >>= 1, r >>= 1) {
      if (l & 1) resL = calc(resL, tree[l++]);
      if (r & 1) resR = calc(tree[--r], resR);
    }
    return calc(resL, resR);
  }
  // Time Complexity: Build O(N), Query O(log N), Update O(log N)
  // Space Complexity: O(N)
};