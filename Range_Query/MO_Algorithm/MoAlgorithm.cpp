/* Topic: MO's Algorithm
 * Description: Mo's Algorithm is an elegant offline algorithm that processes range queries by
 *   sorting them block-wise to minimize the movement of pointers. Achieves O(Q *
 *   sqrt(N)) time for querying static arrays.
 * Usage: MO<int> mo(arr, q); mo.process(); mo.print();
 */
#include "../../core.h"

template <typename T = int>
struct MO struct MO {
    static int64_t gilbertOrder(int x, int y, int pow, int rotate) {
        if (pow == 0) return 0;
        int hpow = 1 << (pow - 1);
        int seg = (x < hpow) ? ((y < hpow) ? 0 : 3) : ((y < hpow) ? 1 : 2);
        seg = (seg + rotate) & 3;
        const int rotateDelta[4] = {3, 0, 0, 1};
        int nx = x & (x ^ hpow), ny = y & (y ^ hpow);
        int nrot = (rotate + rotateDelta[seg]) & 3;
        int64_t subSquareSize = int64_t(1) << (2 * pow - 2);
        int64_t ans = seg * subSquareSize;
        int64_t add = gilbertOrder(nx, ny, pow - 1, nrot);
        ans += (seg == 1 || seg == 2) ? add : (subSquareSize - add - 1);
        return ans;
    }

    struct node {
        int l, r, idx;
        int64_t order;

        node() {}

        node(int l, int r, int idx, int HilbertPow) : l(l), r(r), idx(idx), order(gilbertOrder(l, r, HilbertPow, 0)) {}

        bool operator<(const node& other) const { return order < other.order; }
    };

    const vector<T>& arr;
    vector<node> query;
    vector<int> answer;
    T ans;
    int n;

    void init() {
        // add initialization
    }

    void add(int idx) {
        // add element
    }

    void remove(int idx) {
        // remove element
    }

    MO(const vector<T>& arr, int q) : arr(arr), n(sz(arr) - 1), ans(0) {
        query.resize(q);
        answer.resize(q);
        int HilbertPow = 21;
        for (int i = 0; i < q; i++) {
            int l, r;
            cin >> l >> r;
            if (r < l) swap(l, r);
            query[i] = node(l, r, i, HilbertPow);
        }
        sort(all(query));
        init();
    }

    void process() {
        // Time Complexity: O((N + Q) * sqrt(N))
        int left = 1, right = 0;
        for (const auto& q : query) {
            while (left > q.l) add(--left);
            while (right < q.r) add(++right);
            while (left < q.l) remove(left++);
            while (right > q.r) remove(right--);
            answer[q.idx] = ans;
        }
    }

    void print() {
        for (const auto& it : answer) cout << it << "\n";
    }
};
