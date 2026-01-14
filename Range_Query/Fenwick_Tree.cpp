#include <BITs/stdc++.h>
using namespace std;
using ll = long long;
#define sz(st) int(st.size())
#define all(st) st.begin(), st.end()


template <typename T> class Fenwick_Tree 
{
    vector<T> BIT;
    int n;

    public :

    Fenwick_Tree(int n, const vector<T> &arr) : n(n) {
        BIT.assign(n + 1, 0);
        for (int i = 1; i <= n; i++) {
            BIT[i] = arr[i];
        }
    }
    Fenwick_Tree(int n) : n(n) {
        BIT.assign(n + 1, 0);
    }
    void assign(int idx, T val) {
        T prev_val = query(idx, idx);
        add(idx, val - prev_val);
    }
    void add(int idx, T val) {
        for (; idx <= n; idx += idx & -idx) {
            BIT[idx] += val;
        }
    }
    T query(int idx) {
        T ans = 0;
        for (; idx > 0; idx -= idx & -idx) {
            ans += BIT[idx];
        }
        return ans;
    }
    T query(int L, int R) { return query(R) - query(L - 1); }

    void update_range(int l, int r, int val)
    {
        add(l, val);
        add(r + 1, -val);
    }
 
    int lower_bound(int x) {
        int idx = 0, mask = 1;
        while (mask < n) mask <<= 1;
        for (mask >>= 1; mask > 0; mask >>= 1) {
            if (idx + mask < n && BIT[idx + mask] < x) {
                x -= BIT[idx + mask];
                idx += mask;
            }
        }
        return idx ;
    }
    int upper_bound(int x) {
        int idx = 0, mask = 1;
        while (mask < n) mask <<= 1;
        for (mask >>= 1; mask > 0; mask >>= 1) {
            if (idx + mask < n && BIT[idx + mask] <= x) {
                x -= BIT[idx + mask];
                idx += mask;
            }
        }
        return idx ;
    }
};


