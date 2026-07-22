/* Topic: Segment Tree Beats
 * Description: Segment Tree Beats is an advanced modification of the Segment Tree capable of
 *   handling operations like `a_i = min(a_i, x)` across a range. Used for complex
 *   non-standard range updates while preserving time complexity.
 * Usage: SGTBeats st(n, a); st.update_min(l, r, x);
 */
#include "../../core.h"

// 0-indexed, queries on [l, r)
struct SGTBeats {
    const int inf = 1e18; // Change based on ll vs int
    int n, n0;
    vector<int> max_v, smax_v, max_c;
    vector<int> min_v, smin_v, min_c;
    vector<int> sum, len, ladd, lval;

    // Time Complexity: O(N) build, amortized O(log^2 N) for chmin/chmax updates, O(log N) queries
    // Space Complexity: O(N)
    SGTBeats(int n, int *a = nullptr) : n(n) {
        n0 = 1;
        while (n0 < n) n0 <<= 1;
        int sz = 4 * n0;
        max_v.resize(sz); smax_v.resize(sz); max_c.resize(sz);
        min_v.resize(sz); smin_v.resize(sz); min_c.resize(sz);
        sum.resize(sz); len.resize(sz); ladd.resize(sz); lval.resize(sz);

        for (int i = 0; i < 2 * n0; ++i) ladd[i] = 0, lval[i] = inf;
        len[0] = n0;
        for (int i = 0; i < n0 - 1; ++i) len[2 * i + 1] = len[2 * i + 2] = (len[i] >> 1);

        for (int i = 0; i < n; ++i) {
            max_v[n0 - 1 + i] = min_v[n0 - 1 + i] = sum[n0 - 1 + i] = (a != nullptr ? a[i] : 0);
            smax_v[n0 - 1 + i] = -inf;
            smin_v[n0 - 1 + i] = inf;
            max_c[n0 - 1 + i] = min_c[n0 - 1 + i] = 1;
        }
        for (int i = n; i < n0; ++i) {
            max_v[n0 - 1 + i] = smax_v[n0 - 1 + i] = -inf;
            min_v[n0 - 1 + i] = smin_v[n0 - 1 + i] = inf;
            max_c[n0 - 1 + i] = min_c[n0 - 1 + i] = 0;
        }
        for (int i = n0 - 2; i >= 0; i--) update(i);
    }

    void update_node_max(int k, int x) {
        sum[k] += (x - max_v[k]) * max_c[k];
        if (max_v[k] == min_v[k]) max_v[k] = min_v[k] = x;
        else if (max_v[k] == smin_v[k]) max_v[k] = smin_v[k] = x;
        else max_v[k] = x;
        if (lval[k] != inf && x < lval[k]) lval[k] = x;
    }
    void update_node_min(int k, int x) {
        sum[k] += (x - min_v[k]) * min_c[k];
        if (max_v[k] == min_v[k]) max_v[k] = min_v[k] = x;
        else if (smax_v[k] == min_v[k]) min_v[k] = smax_v[k] = x;
        else min_v[k] = x;
        if (lval[k] != inf && lval[k] < x) lval[k] = x;
    }
    void push(int k) {
        if (n0 - 1 <= k) return;
        if (lval[k] != inf) {
            updateall(2 * k + 1, lval[k]);
            updateall(2 * k + 2, lval[k]);
            lval[k] = inf;
            return;
        }
        if (ladd[k] != 0) {
            addall(2 * k + 1, ladd[k]);
            addall(2 * k + 2, ladd[k]);
            ladd[k] = 0;
        }
        if (max_v[k] < max_v[2 * k + 1]) update_node_max(2 * k + 1, max_v[k]);
        if (min_v[2 * k + 1] < min_v[k]) update_node_min(2 * k + 1, min_v[k]);
        if (max_v[k] < max_v[2 * k + 2]) update_node_max(2 * k + 2, max_v[k]);
        if (min_v[2 * k + 2] < min_v[k]) update_node_min(2 * k + 2, min_v[k]);
    }
    void update(int k) {
        sum[k] = sum[2 * k + 1] + sum[2 * k + 2];

        if (max_v[2 * k + 1] < max_v[2 * k + 2]) {
            max_v[k] = max_v[2 * k + 2];
            max_c[k] = max_c[2 * k + 2];
            smax_v[k] = max(max_v[2 * k + 1], smax_v[2 * k + 2]);
        } else if (max_v[2 * k + 1] > max_v[2 * k + 2]) {
            max_v[k] = max_v[2 * k + 1];
            max_c[k] = max_c[2 * k + 1];
            smax_v[k] = max(smax_v[2 * k + 1], max_v[2 * k + 2]);
        } else {
            max_v[k] = max_v[2 * k + 1];
            max_c[k] = max_c[2 * k + 1] + max_c[2 * k + 2];
            smax_v[k] = max(smax_v[2 * k + 1], smax_v[2 * k + 2]);
        }

        if (min_v[2 * k + 1] < min_v[2 * k + 2]) {
            min_v[k] = min_v[2 * k + 1];
            min_c[k] = min_c[2 * k + 1];
            smin_v[k] = min(smin_v[2 * k + 1], min_v[2 * k + 2]);
        } else if (min_v[2 * k + 1] > min_v[2 * k + 2]) {
            min_v[k] = min_v[2 * k + 2];
            min_c[k] = min_c[2 * k + 2];
            smin_v[k] = min(min_v[2 * k + 1], smin_v[2 * k + 2]);
        } else {
            min_v[k] = min_v[2 * k + 1];
            min_c[k] = min_c[2 * k + 1] + min_c[2 * k + 2];
            smin_v[k] = min(smin_v[2 * k + 1], smin_v[2 * k + 2]);
        }
    }
    void _update_min(int x, int a, int b, int k, int l, int r) {
        if (b <= l || r <= a || max_v[k] <= x) return;
        if (a <= l && r <= b && smax_v[k] < x) {
            update_node_max(k, x);
            return;
        }
        push(k);
        _update_min(x, a, b, 2 * k + 1, l, (l + r) / 2);
        _update_min(x, a, b, 2 * k + 2, (l + r) / 2, r);
        update(k);
    }
    void _update_max(int x, int a, int b, int k, int l, int r) {
        if (b <= l || r <= a || x <= min_v[k]) return;
        if (a <= l && r <= b && x < smin_v[k]) {
            update_node_min(k, x);
            return;
        }
        push(k);
        _update_max(x, a, b, 2 * k + 1, l, (l + r) / 2);
        _update_max(x, a, b, 2 * k + 2, (l + r) / 2, r);
        update(k);
    }
    void addall(int k, int x) {
        max_v[k] += x; if (smax_v[k] != -inf) smax_v[k] += x;
        min_v[k] += x; if (smin_v[k] != inf) smin_v[k] += x;
        sum[k] += len[k] * x;
        if (lval[k] != inf) lval[k] += x;
        else ladd[k] += x;
    }
    void updateall(int k, int x) {
        max_v[k] = x; smax_v[k] = -inf;
        min_v[k] = x; smin_v[k] = inf;
        max_c[k] = min_c[k] = len[k];
        sum[k] = x * len[k];
        lval[k] = x; ladd[k] = 0;
    }
    void _add_val(int x, int a, int b, int k, int l, int r) {
        if (b <= l || r <= a) return;
        if (a <= l && r <= b) { addall(k, x); return; }
        push(k);
        _add_val(x, a, b, 2 * k + 1, l, (l + r) / 2);
        _add_val(x, a, b, 2 * k + 2, (l + r) / 2, r);
        update(k);
    }
    void _update_val(int x, int a, int b, int k, int l, int r) {
        if (b <= l || r <= a) return;
        if (a <= l && r <= b) { updateall(k, x); return; }
        push(k);
        _update_val(x, a, b, 2 * k + 1, l, (l + r) / 2);
        _update_val(x, a, b, 2 * k + 2, (l + r) / 2, r);
        update(k);
    }

    int _query_max(int a, int b, int k, int l, int r) {
        if (b <= l || r <= a) return -inf;
        if (a <= l && r <= b) return max_v[k];
        push(k);
        return max(_query_max(a, b, 2 * k + 1, l, (l + r) / 2),
                   _query_max(a, b, 2 * k + 2, (l + r) / 2, r));
    }
    int _query_min(int a, int b, int k, int l, int r) {
        if (b <= l || r <= a) return inf;
        if (a <= l && r <= b) return min_v[k];
        push(k);
        return min(_query_min(a, b, 2 * k + 1, l, (l + r) / 2),
                   _query_min(a, b, 2 * k + 2, (l + r) / 2, r));
    }
    int _query_sum(int a, int b, int k, int l, int r) {
        if (b <= l || r <= a) return 0;
        if (a <= l && r <= b) return sum[k];
        push(k);
        return _query_sum(a, b, 2 * k + 1, l, (l + r) / 2) +
               _query_sum(a, b, 2 * k + 2, (l + r) / 2, r);
    }

    void update_min(int a, int b, int x) { _update_min(x, a, b, 0, 0, n0); }
    void update_max(int a, int b, int x) { _update_max(x, a, b, 0, 0, n0); }
    void add_val(int a, int b, int x) { _add_val(x, a, b, 0, 0, n0); }
    void update_val(int a, int b, int x) { _update_val(x, a, b, 0, 0, n0); }
    int query_max(int a, int b) { return _query_max(a, b, 0, 0, n0); }
    int query_min(int a, int b) { return _query_min(a, b, 0, 0, n0); }
    int query_sum(int a, int b) { return _query_sum(a, b, 0, 0, n0); }
};
