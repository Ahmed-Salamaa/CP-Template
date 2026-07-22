/* Topic: Sparse Table
 * Description: An immutable data structure that allows O(1) time range queries for idempotent
 *   operations like Minimum, Maximum, and GCD after an O(N log N) preprocessing
 *   step.
 * Usage: sparseTable<int> st; st.init(n, [](int a, int b) { return min(a, b); }); st.build(v); st.query(l, r);
 */
#include "../../core.h"

template <typename T>
struct sparseTable {
    int sz, lg;
    vector<vector<T>> t;
    function<T(T, T)> combine;

    void init(int n, function<T(T, T)> F) {
        sz = n;
        lg = __lg(n);
        combine = F;
        t.assign(lg + 1, vector<T>(n));
    }

    void build(const vector<T>& v) {
        for (int i = 0; i < sz; i++) { t[0][i] = v[i]; }
        for (int p = 1; p <= lg; p++) {
            for (int i = 0; i + (1 << p) <= sz; i++) { t[p][i] = combine(t[p - 1][i], t[p - 1][i + (1 << (p - 1))]); }
        }
    }

    T query(int l, int r) {
        assert(l <= r);
        int k = 31 - __builtin_clz(r - l + 1);
        return combine(t[k][l], t[k][r - (1 << k) + 1]);
    }

    T queryLog(int l, int r, T neutral) {
        assert(l <= r);
        T ret = neutral;
        while (l <= r) {
            int k = 31 - __builtin_clz(r - l + 1);
            ret = combine(ret, t[k][l]);
            l += (1 << k);
        }
        return ret;
    }
};
