/* Topic: Segment Tree Lazy
 * Description: Enhances a Segment Tree to support efficient range modifications along with
 *   range queries. Uses a deferred update strategy to keep operations bounded by
 *   O(log N).
 * Usage: SegmentTreeLazy<int> st(n);
 */
#include "../../core.h"

template <typename T = int>
class SegmentTreeLazy {
    #define mid (start + (end - start) / 2)
    #define range_left  curr * 2, start, mid
    #define range_right curr * 2 + 1, mid + 1, end

private:
    struct Node {
        T val, lazy;
        bool is_lazy;
        Node operator=(const T rhs) {
            val = rhs;
            return *this;
        }
    };

    vector<Node> arr;
    Node def = {(T)0, (T)0, false}; // change default
    int n = 1;

    Node calc(Node curr1, Node curr2) { // change operation
        Node ans;
        ans.val = curr1.val + curr2.val;
        ans.lazy = def.lazy;
        ans.is_lazy = false;
        return ans;
    }

    void apply_lazy(int curr, int start, int end, T val) {
        arr[curr].val += val * (end - start + 1); // for sum
        arr[curr].lazy += val;
        arr[curr].is_lazy = true;
    }

    void propagate(int curr, int start, int end) {
        if (!arr[curr].is_lazy) return;
        T pending = arr[curr].lazy;
        if (start != end) {
            apply_lazy(range_left, pending);
            apply_lazy(range_right, pending);
        }
        arr[curr].lazy = def.lazy;
        arr[curr].is_lazy = false;
    }

    void build(int curr, int start, int end, const vector<T> &numbers) {
        if (start == end) { arr[curr] = numbers[start - 1]; return; }
        build(range_left, numbers);
        build(range_right, numbers);
        arr[curr] = calc(arr[2 * curr], arr[2 * curr + 1]);
    }

    Node query_range(int curr, int start, int end, int left, int right) {
        propagate(curr, start, end);
        if (start > right || end < left) return def;
        if (left <= start && end <= right) return arr[curr];
        return calc(query_range(range_left, left, right), query_range(range_right, left, right));
    }

    void update_range(int curr, int start, int end, int left, int right, T val) {
        propagate(curr, start, end);
        if (start > right || end < left) return;
        if (left <= start && end <= right) {
            apply_lazy(curr, start, end, val);
            return;
        }
        update_range(range_left, left, right, val);
        update_range(range_right, left, right, val);
        arr[curr] = calc(arr[2 * curr], arr[2 * curr + 1]);
    }

public:
    // Time Complexity: Build O(N), Query O(log N), Update O(log N)
    // Space Complexity: O(N)
    SegmentTreeLazy(const vector<T> &numbers) {
        n = sz(numbers);
        int size = 1;
        while (size < n) size *= 2;
        arr.assign(2 * size, def);
        if (n > 0) build(1, 1, n, numbers);
    }

    SegmentTreeLazy(int _n) {
        n = _n;
        int size = 1;
        while (size < n) size *= 2;
        arr.assign(2 * size, def);
    }

    T query_range(int left, int right) {
        return query_range(1, 1, n, left, right).val;
    }

    void update_range(int left, int right, T val) {
        update_range(1, 1, n, left, right, val);
    }

    #undef mid
    #undef range_left
    #undef range_right
};
