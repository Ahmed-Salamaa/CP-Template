#include <vector>
using namespace std;

template<typename T>
struct FenwickTree3D {
    int n, m, p;
    vector<vector<vector<T>>> bit;

    FenwickTree3D(int rows, int cols, int depths) : n(rows), m(cols), p(depths),
        bit(n + 1, vector<vector<T>>(m + 1, vector<T>(p + 1, T(0)))) {}

    void update(int x, int y, int z, T delta) {
        for (int i = x; i <= n; i += i & -i)
            for (int j = y; j <= m; j += j & -j)
                for (int k = z; k <= p; k += k & -k)
                    bit[i][j][k] += delta;
    }

    T query(int x, int y, int z) {
        T res = 0;
        for (int i = x; i > 0; i -= i & -i)
            for (int j = y; j > 0; j -= j & -j)
                for (int k = z; k > 0; k -= k & -k)
                    res += bit[i][j][k];
        return res;
    }

    T query(int x1, int y1, int z1, int x2, int y2, int z2) {
        return query(x2, y2, z2)
             - query(x1 - 1, y2, z2) - query(x2, y1 - 1, z2) - query(x2, y2, z1 - 1)
             + query(x1 - 1, y1 - 1, z2) + query(x1 - 1, y2, z1 - 1) + query(x2, y1 - 1, z1 - 1)
             - query(x1 - 1, y1 - 1, z1 - 1);
    }
};
