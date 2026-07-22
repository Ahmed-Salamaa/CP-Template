/* Topic: Segment Tree
 * Description: Standard Segment Tree provides O(log N) time point updates and range queries
 *   (like sum, min, max) over an array. Fundamental for answering dynamic array
 *   interval queries.
 * Usage: SegmentTree<int> st(n);
 */
#include "../../core.h"

template <typename T = int>
class SegmentTree {
    #define mid (start + (end - start) / 2)
    #define range_left  curr * 2, start, mid
    #define range_right curr * 2 + 1, mid + 1, end

private:
    struct Node {
        T val;
        Node operator=(const T rhs) {
            val = rhs;
            return *this;
        }
    };

    vector<Node> arr;
    Node def = {(T)(0)}; // change default
    int n = 1;

    Node calc(Node curr1, Node curr2) { // change operation
        Node ans;
        ans.val = curr1.val + curr2.val;
        return ans;
    }

    void build(int curr, int start, int end, const vector<T> &numbers) {
        if (start == end) { arr[curr] = numbers[start - 1]; return; }
        build(range_left, numbers);
        build(range_right, numbers);
        arr[curr] = calc(arr[2 * curr], arr[2 * curr + 1]);
    }

    Node query(int curr, int start, int end, int left, int right) {
        if (start > right || end < left) return def;
        if (left <= start && end <= right) return arr[curr];
        return calc(query(range_left, left, right), query(range_right, left, right));
    }

    void update(int curr, int start, int end, int idx, T val) {
        if (start == end) { arr[curr] = val; return; }
        if (idx <= mid) update(range_left, idx, val);
        else update(range_right, idx, val);
        arr[curr] = calc(arr[2 * curr], arr[2 * curr + 1]);
    }

public:
    // Time Complexity: Build O(N), Query O(log N), Update O(log N)
    // Space Complexity: O(N)
    SegmentTree(const vector<T> &numbers) {
        n = sz(numbers);
        int size = 1;
        while (size < n) size *= 2;
        arr.assign(2 * size, def);
        if (n > 0) build(1, 1, n, numbers);
    }

    SegmentTree(int _n, T val = 0) {
        n = _n;
        int size = 1;
        while (size < n) size *= 2;
        arr.assign(2 * size, def);
    }

    T query(int left, int right) {
        return query(1, 1, n, left, right).val;
    }

    void update(int idx, T val) {
        update(1, 1, n, idx, val);
    }

    #undef mid
    #undef range_left
    #undef range_right
};
