/*
 * Topic: N-Variable Linear Diophantine Equation
 * Description: Generalizes the Extended Euclidean algorithm to solve linear Diophantine
 *   equations with an arbitrary number of variables (e.g., a1x1 + a2x2 + ... = C).
 *   Common in coin change variations and strict subset weight problems.
 * It uses suffix GCDs to iteratively solve 2-variable LDEs.
 * Usage:
 * vector<int> a = {2, 3, 4};
 * vector<int> res = NVarLinearDiophantineEquation::findAnySolution(a, 5);
 */
#include "../../../core.h"

// Time Complexity: O(log(min(a, b)))
// Space Complexity: O(1)
int extendedEuclid(int a, int b, int& x, int& y) {
    int xx = y = 0;
    int yy = x = 1;
    while (b) {
        int q = a / b;
        int t = b;
        b = a % b;
        a = t;
        t = xx;
        xx = x - q * xx;
        x = t;
        t = yy;
        yy = y - q * yy;
        y = t;
    }
    return a;
}

// Time Complexity: O(log(min(a, b)))
// Space Complexity: O(1)
bool findAnySolution(int a, int b, int c, int& x0, int& y0, int& g) {
    if (a == 0 && b == 0) {
        if (c) return false;
        x0 = y0 = g = 0;
        return true;
    }
    g = extendedEuclid(abs(a), abs(b), x0, y0);
    if (c % g != 0) return false;
    x0 *= c / g;
    y0 *= c / g;
    if (a < 0) x0 *= -1;
    if (b < 0) y0 *= -1;
    return true;
}

// Time Complexity: O(N log(MAX))
// Space Complexity: O(N)
vector<int> findAnySolution(vector<int> a, int c) {
    int n = sz(a);
    vector<int> x;
    bool all_zero = true;
    for (int i = 0; i < n; i++) { all_zero &= (a[i] == 0); }
    if (all_zero) {
        if (c) return {};
        x.assign(n, 0);
        return x;
    }
    int g = 0;
    for (int i = 0; i < n; i++) { g = __gcd(g, a[i]); }
    if (c % g != 0) return {};
    if (n == 1) { return {c / a[0]}; }
    vector<int> suf_gcd(n);
    suf_gcd[n - 1] = a[n - 1];
    for (int i = n - 2; i >= 0; i--) { suf_gcd[i] = __gcd(suf_gcd[i + 1], a[i]); }
    int cur = c;
    for (int i = 0; i + 1 < n; i++) {
        int x0 = 0, y0 = 0, g_val = 0;
        bool ok = findAnySolution(a[i], suf_gcd[i + 1], cur, x0, y0, g_val);
        if (!ok) return {};
        int shift = abs(suf_gcd[i + 1] / g_val);
        x0 = ((x0 % shift) + shift) % shift;
        x.push_back(x0);
        cur -= a[i] * x0;
    }
    x.push_back(a[n - 1] == 0 ? 0 : cur / a[n - 1]);
    return x;
}
