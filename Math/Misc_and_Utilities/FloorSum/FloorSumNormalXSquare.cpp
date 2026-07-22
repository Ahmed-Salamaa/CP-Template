// Topic: Floor Sum Normal X Square - Calculates floor sum along with x * floor sum and floor sum squared
//
// Description:
// Similar to standard floor sum, but concurrently evaluates:
// f: sum_{x=0}^{n} floor((a*x + b) / c)
// g: sum_{x=0}^{n} x * floor((a*x + b) / c)
// h: sum_{x=0}^{n} [floor((a*x + b) / c)]^2
//
// How to use it:
// FloorSumNormalXSquare::dat ans = FloorSumNormalXSquare::query(a, b, c, n);
// cout << ans.f << " " << ans.g << " " << ans.h << endl;
#include "../../../core.h"
const int MOD = 1e9 + 7;

// Time Complexity: O(log(b))
// Space Complexity: O(1)
int binpow(int a, int b, int m) {
    a %= m;
    int res = 1;
    while (b > 0) {
        if (b & 1) res = res * a % m;
        a = a * a % m;
        b >>= 1;
    }
    return res;
}

const int inv2 = binpow(2, MOD - 2, MOD);
const int inv6 = binpow(6, MOD - 2, MOD);

// Time Complexity: O(1)
// Space Complexity: O(1)
int norm(int x) { return ((x % MOD) + MOD) % MOD; }

// Time Complexity: O(1)
// Space Complexity: O(1)
int mul(int a, int b) { return ((a % MOD) * (b % MOD)) % MOD; }

struct dat {
    int f, g, h;

    dat(int f = 0, int g = 0, int h = 0) : f(f), g(g), h(h) {}
};

// Time Complexity: O(log(max(a, c)))
// Space Complexity: O(log(max(a, c)))
// f: sum_{x=0}^{n} floor((a*x + b) / c)
// g: sum_{x=0}^{n} x * floor((a*x + b) / c)
// h: sum_{x=0}^{n} [floor((a*x + b) / c)]^2
dat query(int a, int b, int c, int n) {
    if (a == 0) {
        int d = b / c;
        int F = mul(n + 1, d);
        int G = mul(mul(d, n), (n + 1));
        G = mul(G, inv2);
        int H = mul(n + 1, d);
        H = mul(H, d);
        return {norm(F), norm(G), norm(H)};
    }
    dat nxt;
    int f, g, h;
    if (a >= c || b >= c) {
        nxt = query(a % c, b % c, c, n);
        int term1 = mul(mul(n, n + 1), inv2);
        f = norm(nxt.f + mul(term1, a / c) + mul(n + 1, b / c));
        int term2 = mul(mul(n, n + 1), mul(2 * n + 1, inv6));
        g = norm(nxt.g + mul(a / c, term2) + mul(mul(b / c, n), (n + 1) * inv2));
        h = norm(nxt.h + 2 * mul(b / c, nxt.f) + 2 * mul(a / c, nxt.g) +
                 mul(mul(a / c, a / c), mul(mul(n, n + 1), mul(2 * n + 1, inv6))) + mul(mul(b / c, b / c), (n + 1)) +
                 mul(mul(a / c, b / c), mul(n, n + 1)));
        return {f, g, h};
    }
    int m = (a * n + b) / c;
    nxt = query(c, c - b - 1, a, m - 1);
    f = norm(mul(m, n) - nxt.f);
    int temp = norm(mul(m, mul(n, n + 1)) - nxt.h - nxt.f);
    g = norm(mul(temp, inv2));
    h = norm(mul(n, mul(m, m + 1)) - 2 * nxt.g - 2 * nxt.f - f);
    return {f, g, h};
}
