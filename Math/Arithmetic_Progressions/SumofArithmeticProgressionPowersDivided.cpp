// Topic: Sum of AP Powers - [Computes sums of powers of AP]
// NOTE: This template requires basic combinatorics. You must also include Math/Combinatorics/Basics/Combinatorics.cpp.
#include <cassert>

#include "../../../core.h"

/*
 * Description: A specialized mathematical formula or algorithm that computes the sum of powers
 *   of an arithmetic progression divided by another term. Useful in complex analytic
 *   number theory summations.
 *
 * NOTE: This template uses standard integers with manual modulo arithmetic for performance and simplicity. If the
 * problem involves extremely complex chaining of operations or you want to avoid manual modulo bugs, you can easily
 * swap the `int` types back to a `modint` template.
 *
 * Usage:
 * SumofArithmeticProgressionPowersDivided_Algorithm solver;
 * auto result = solver.power(...);
 */
// Time Complexity: O(K log A)
// Space Complexity: O(K)
struct SumofArithmeticProgressionPowersDivided_Algorithm {
    static const int N = 25, mod = 1e9 + 7;

    long long power(long long n, long long k) {
        long long ans = 1 % mod;
        n %= mod;
        if (n < 0) n += mod;
        while (k) {
            if (k & 1) ans = (long long)ans * n % mod;
            n = (long long)n * n % mod;
            k >>= 1;
        }
        return ans;
    }

    // generates the polynomial explicitely
    vector<int> Lagrange(vector<int>& x, vector<int>& y) {
        int n = sz(x);
        vector<int> ans(n, 0);
        vector<int> all_p(n + 1, 0);  //(x - x0) * (x - x1) * ... * (x - x(n-1))
        all_p[0] = 1;
        for (int i = 0; i < n; i++) {
            for (int j = n; j >= 0; j--) {
                all_p[j] = all_p[j] * (mod - x[i]) % mod;
                if (j) all_p[j] = (all_p[j] + all_p[j - 1]) % mod;
            }
        }
        for (int i = 0; i < n; i++) {
            vector<int> up(n);  // all / (x - xi)
            int rem = all_p[n];
            for (int j = n - 1; j >= 0; --j) {
                up[j] = rem;
                rem = (all_p[j] + rem * x[i]) % mod;
            }
            int down = 1;
            for (int j = 0; j < n; j++)
                if (i != j) down = down * (x[i] - x[j] + mod) % mod;
            up.resize(n);
            down = power(down, mod - 2) * y[i] % mod;
            for (int j = 0; j < n; j++) ans[j] = (ans[j] + up[j] * down) % mod;
        }
        return ans;
    }

    int d[N][N];  // N > k1 + k2
    long long ncr[N][N];

    void prec() {
        for (int k = 0; k < N; k++) {
            vector<int> y, x;
            int sum = 0;
            y.push_back(0);
            x.push_back(0);
            for (int i = 1; i <= k + 1; i++) {
                sum = (sum + power(i, k)) % mod;
                y.push_back(sum);
                x.push_back(i);
            }
            auto p = Lagrange(x, y);
            assert(sz(p) > k + 1);
            for (int i = 0; i <= k + 1; i++) d[k][i] = p[i];
        }
        d[0][0] = 1;  // to satisfy 0^0 = 1
    }

    int power_sum(int n, int k) {
        assert(k >= 0);
        int ans = 0;
        for (int i = 0; i <= k + 1; i++) {
            ans += d[k][i] * power(n, i) % mod;
            ans %= mod;
        }
        return ans;
    }

    // \sum{x=0}^{n}{x^k1 * ((ax + b)/c)^k2} //floor division
    // 0^0 = 1
    // it solves for all _k1, _k2 s.t. _k1 + _k2 <= k1 + k2
    // (a * n + b) should fit in int
    // total complexity: O((k1 + k2)^3 * log(n) * log(mod)
    vector<vector<int>> solve(int k1, int k2, int a, int b, int c, int n) {
        if (n < 0) {
            vector<vector<int>> ret(k1 + k2 + 1, vector<int>(k1 + k2 + 1, 0));
            return ret;
        }
        if (!a) {
            vector<vector<int>> ret(k1 + k2 + 1, vector<int>(k1 + k2 + 1, 0));
            for (int _k1 = 0; _k1 <= k1 + k2; ++_k1) {
                ret[_k1][0] = power_sum(n, _k1);
                for (int _k2 = 1; _k1 + _k2 <= k1 + k2; ++_k2) {
                    ret[_k1][_k2] = (ret[_k1][0] * power(b / c, _k2)) % mod;
                }
            }
            return ret;
        }
        vector<vector<int>> ret(k1 + k2 + 1, vector<int>(k1 + k2 + 1, 0));
        assert(c != 0);
        if (a >= c) {
            vector<vector<int>> nxt = solve(k1, k2, a % c, b, c, n);
            for (int _k1 = 0; _k1 <= k1 + k2; ++_k1) {
                ret[_k1][0] = power_sum(n, _k1);
                for (int _k2 = 1; _k1 + _k2 <= k1 + k2; ++_k2) {
                    ret[_k1][_k2] = 0;
                    for (int y = 0; y <= _k2; ++y) {
                        ret[_k1][_k2] += ((ncr[_k2][y] * power(a / c, y)) % mod) * nxt[_k1 + y][_k2 - y];
                        ret[_k1][_k2] %= mod;
                    }
                }
            }
        } else if (b >= c) {
            vector<vector<int>> nxt = solve(k1, k2, a, b % c, c, n);
            for (int _k1 = 0; _k1 <= k1 + k2; ++_k1) {
                ret[_k1][0] = power_sum(n, _k1);
                for (int _k2 = 1; _k1 + _k2 <= k1 + k2; ++_k2) {
                    ret[_k1][_k2] = 0;
                    for (int y = 0; y <= _k2; ++y) {
                        ret[_k1][_k2] += ((ncr[_k2][y] * power(b / c, y)) % mod) * nxt[_k1][_k2 - y];
                        ret[_k1][_k2] %= mod;
                    }
                }
            }
        } else {
            int m = (a * n + b) / c;
            vector<vector<int>> nxt = solve(k1, k2, c, c - b - 1, a, m - 1);
            vector<int> psum(k1 + k2 + 1), pre(k1 + k2 + 1);
            for (int i = 0; i <= k1 + k2; ++i) psum[i] = power_sum(m - 1, i);
            for (int _k1 = 0; _k1 <= k1 + k2; ++_k1) {
                for (int i = 0; i <= k1 + k2; ++i) {
                    pre[i] = 0;
                    for (int j = 0; j <= _k1 + 1; ++j) {
                        pre[i] += (d[_k1][j] * nxt[i][j]) % mod;
                        pre[i] %= mod;
                    }
                }
                ret[_k1][0] = power_sum(n, _k1);
                for (int _k2 = 1; _k1 + _k2 <= k1 + k2; ++_k2) {
                    ret[_k1][_k2] = 0;
                    for (int i = 0; i < _k2; ++i) {
                        ret[_k1][_k2] += ((ncr[_k2][i] * psum[i]) % mod) * ret[_k1][0];
                        ret[_k1][_k2] -= ncr[_k2][i] * pre[i];
                        ret[_k1][_k2] %= mod;
                    }
                }
            }
        }
        return ret;
    }
};
