// Topic: Count Solutions with Upper Bounds - [PIE to count solutions to linear equations with bounds]
#include "../../../core.h"

/*
 * Description: Computes the number of ways to satisfy a linear equation subject to specific
 *   constraints on variable ranges. Often useful in combinatorics, dynamic
 *   programming, or applying inclusion-exclusion principles.
 * This mathematical template provides an efficient implementation for Count Solutions with Upper Bounds.
 * It is commonly used in Advanced Number Theory problems.
 *
 * NOTE: This template uses standard integers with manual modulo arithmetic for performance and simplicity. If the
 * problem involves extremely complex chaining of operations or you want to avoid manual modulo bugs, you can easily
 * swap the `int` types back to a `modint` template.
 *
 * Usage:
 * NumberofSolutionstoaBasicLinearAlgebraicEquationwithVariableUpperBoundConstraints_Algorithm solver;
 * auto result = solver.modint(...);
 */
// Time Complexity: O(2^N)
// Space Complexity: O(1)
struct NumberofSolutionstoaBasicLinearAlgebraicEquationwithVariableUpperBoundConstraints_Algorithm {
    static const int mod = 1e9 + 7;
    using poly = vector<int>;

    int power(int base, int exp) {
        int res = 1;
        base %= mod;
        if (base < 0) base += mod;
        while (exp > 0) {
            if (exp % 2 == 1) res = (1LL * res * base) % mod;
            base = (1LL * base * base) % mod;
            exp /= 2;
        }
        return res;
    }

    int inverse(int n) { return power(n, mod - 2); }

    int eval(poly a, int x) {
        int ans = 0, p = 1;
        for (int b : a) {
            ans = (ans + 1LL * p * b) % mod;
            p = (1LL * p * x) % mod;
        }
        return ans;
    }

    static poly add(poly a, poly b) {
        if (sz(a) < sz(b)) a.resize(sz(b), 0);
        for (int i = 0; i < sz(b); i++) { a[i] = (a[i] + b[i]) % mod; }
        return a;
    }

    static poly sub(poly a, poly b) {
        if (sz(a) < sz(b)) a.resize(sz(b), 0);
        for (int i = 0; i < sz(b); i++) { a[i] = (a[i] - b[i] + mod) % mod; }
        return a;
    }

    static poly mul(poly a, poly b) {
        vector<int> r(sz(a) + sz(b) - 1, 0);
        for (int i = 0; i < sz(a); i++) {
            for (int j = 0; j < sz(b); j++) { r[i + j] = (r[i + j] + 1LL * a[i] * b[j]) % mod; }
        }
        return r;
    }

    vector<pair<int, int>> get(vector<int> a) {
        int n = sz(a);
        vector<pair<int, int>> ans;
        for (int mask = 0; mask < (1 << n); mask++) {
            int add = 0;
            int x = 1;
            for (int i = 0; i < n; i++) {
                if (mask >> i & 1) {
                    add += a[i];
                    x = -x;
                }
            }
            ans.push_back({add, x});
        }
        return ans;
    }

    // (x + n)(x + n - 1)...(x + n - k + 1);
    poly falling(int n, int k) {
        poly ans({1});
        int p = n % mod;
        for (int i = 0; i < k; i++) { ans = mul(ans, poly({(p - i % mod + mod) % mod, 1})); }
        return ans;
    }

    // returns the number of solutions to the equation x_0 + ... + x_(n-1) = s s.t. 0 <= x_i <= a_i
    // O(2^(n/2)*n^2)
    int yo(vector<int> a, int s) {
        int n = sz(a);
        for (auto& x : a) ++x;
        vector<int> l, r;
        for (int i = 0; i < n; i++) {
            if (i % 2 == 0)
                l.push_back(a[i]);
            else
                r.push_back(a[i]);
        }
        auto L = get(l);
        auto R = get(r);
        sort(all(L));
        sort(R.rbegin(), R.rend());
        int i = 0;
        poly f({0});
        int ans = 0;
        for (auto [x, y] : R) {
            while (i < sz(L) and L[i].first + x <= s) {
                if (L[i].second == 1)
                    f = add(f, falling(s + n - 1 - L[i].first, n - 1));
                else
                    f = sub(f, falling(s + n - 1 - L[i].first, n - 1));
                ++i;
            }
            ans = (ans + 1LL * eval(f, (-x % mod + mod) % mod) * (y + mod)) % mod;
        }
        for (int j = 1; j <= n - 1; j++) { ans = (1LL * ans * inverse(j)) % mod; }
        return ans;
    }
};
