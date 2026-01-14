#include <bits/stdc++.h>
using namespace std;
using ll = long long;
#define sz(st) int(st.size())
#define all(st) st.begin(), st.end()

template<typename T>
struct FenwickTree2D {
    int n, m;
    vector<vector<T>> bit;

    FenwickTree2D(int rows, int cols) {
        n = rows;
        m = cols;
        bit.assign(n + 1, vector<T>(m + 1, 0));
    }

    // Update value at (x, y) by delta
    void update(int x, int y, T delta) {
        for (int i = x; i <= n; i += i & -i) {
            for (int j = y; j <= m; j += j & -j) {
                bit[i][j] += delta;
            }
        }
    }

    // Query sum from (1,1) to (x,y)
    T query(int x, int y) {
        T res = 0;
        for (int i = x; i > 0; i -= i & -i) {
            for (int j = y; j > 0; j -= j & -j) {
                res += bit[i][j];
            }
        }
        return res;
    }

    // Query sum in rectangle (x1,y1) to (x2,y2)
    T query(int x1, int y1, int x2, int y2) {
        return query(x2, y2) - query(x1 - 1, y2) - query(x2, y1 - 1) + query(x1 - 1, y1 - 1);
    }
};
