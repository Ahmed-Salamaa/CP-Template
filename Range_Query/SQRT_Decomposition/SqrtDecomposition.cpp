/* Topic: SQRT Decomposition
 * Description: Square Root (SQRT) Decomposition divides an array into blocks of size sqrt(N) to
 *   process updates and queries in O(sqrt(N)) time. A versatile alternative to
 *   Segment Trees for operations that are hard to merge.
 * Usage: SQRT<int> sq(arr); sq.query(l, r); sq.update(l, r, val);
 */
#include "../../core.h"

template <typename T = int>
struct SQRT struct SQRT {
    int block, n;
    int MAX_Block;
    T def;
    vector<T> arr, blockAnswer;

    T operation(const T& curr1, const T& curr2) {
        // add operation here
        return curr1 + curr2;
    }

    void rebuild(int b) {
        int l = b * block;
        int r = min(n - 1, (b + 1) * block - 1);
        if (l >= n) return;
        blockAnswer[b] = def;
        for (int i = l; i <= r; i++) { blockAnswer[b] = operation(blockAnswer[b], arr[i]); }
    }

    void init() {
        // Time Complexity: O(N)
        for (int i = 0; i < MAX_Block; i++) { rebuild(i); }
    }

    SQRT(vector<T> _arr) : arr(_arr) {
        def = 0;
        n = sz(arr);
        block = sqrt(n + 5);
        MAX_Block = n / block + 5;
        blockAnswer.assign(MAX_Block, def);
        init();
    }

    SQRT(int _n) : n(_n) {
        def = 0;
        arr = vector<T>(n + 5, def);
        block = sqrt(n + 5);
        MAX_Block = n / block + 5;
        blockAnswer.assign(MAX_Block, def);
        init();
    }

    T query(int l, int r) {
        // Time Complexity: O(sqrt(N))
        T ans = def;
        int bl = l / block;
        int br = r / block;
        if (bl == br) {
            for (int i = l; i <= r; i++) { ans = operation(ans, arr[i]); }
            return ans;
        }
        int endL = (bl + 1) * block - 1;
        for (int i = l; i <= endL; i++) { ans = operation(ans, arr[i]); }
        int startR = br * block;
        for (int i = startR; i <= r; i++) { ans = operation(ans, arr[i]); }
        for (int b = bl + 1; b <= br - 1; b++) { ans = operation(ans, blockAnswer[b]); }
        return ans;
    }

    void update(int l, int r, T val) {
        // Time Complexity: O(N) (Take care, naive block update)
        int bl = l / block;
        int br = r / block;
        if (bl == br) {
            for (int i = l; i <= r; i++) arr[i] = operation(arr[i], val);
            rebuild(bl);
            return;
        }
        int endL = (bl + 1) * block - 1;
        for (int i = l; i <= endL; i++) arr[i] = operation(arr[i], val);
        rebuild(bl);
        int startR = br * block;
        for (int i = startR; i <= r; i++) arr[i] = operation(arr[i], val);
        rebuild(br);
        for (int b = bl + 1; b <= br - 1; b++) {
            int L = b * block;
            int R = min(n - 1, (b + 1) * block - 1);
            for (int i = L; i <= R; i++) arr[i] = operation(arr[i], val);
            rebuild(b);
        }
    }

    void update(int idx, T val) {
        // Time Complexity: O(sqrt(N))
        update(idx, idx, val);
    }
};
