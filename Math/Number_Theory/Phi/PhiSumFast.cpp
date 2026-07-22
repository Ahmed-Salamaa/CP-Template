/*
 * Topic: Phi Sum Fast
 * Description: Uses a highly optimized technique, such as the Dirichlet convolution or Mertens
 *   trick, to compute the prefix sum of Euler's totient function in sublinear time,
 *   typically O(N^(2/3)). Crucial for advanced number-theoretic counting problems.
 * of phi(i) up to 1e11 or more. Usage: int total = fast_phi_sum(N);
 */
#include "../../../core.h"
// Time Complexity: O(N^(2/3)) or O(N^(3/4))
// Space Complexity: O(sqrt(N))
typedef __int128 i128;

inline i128 totient_sum(const int N) {
    int v = int(sqrt(N));
    while ((int)(v)*v < N) ++v;
    while ((int)(v)*v > N) --v;
    vector<int> primes;
    vector<int> s0(v + 1), s1(v + 1), l0(v + 1);
    vector<i128> l1(v + 1);
    const auto f = [&](int p, int e) -> int {
        int ret = p - 1;
        while (e > 1) --e, ret *= p;
        return ret;
    };
    const auto divide = [](int n, int d) -> int { return (int)(double(n) / d); };
    for (int i = 1; i <= v; ++i) s0[i] = i - 1, s1[i] = (int)(i) * (i + 1) / 2 - 1;
    for (int i = 1; i <= v; ++i) l0[i] = N / i - 1, l1[i] = i128(N / i) * (N / i + 1) / 2 - 1;
    for (int p = 2; p <= v; ++p) {
        if (s0[p] > s0[p - 1]) {
            primes.push_back(p);
            int q = (int)(p)*p, M = N / p, t0 = s0[p - 1], t1 = s1[p - 1];
            int t = v / p, u = min((int)v, N / q);
            for (int i = 1; i <= t; ++i) l0[i] -= (l0[i * p] - t0), l1[i] -= (l1[i * p] - t1) * p;
            for (int i = t + 1; i <= u; ++i) l0[i] -= (s0[divide(M, i)] - t0), l1[i] -= (s1[divide(M, i)] - t1) * p;
            for (int i = v; i >= q; --i) s0[i] -= (s0[divide(i, p)] - t0), s1[i] -= (s1[divide(i, p)] - t1) * p;
        }
    }
    for (int i = 1; i <= v; ++i) s1[i] -= s0[i];
    for (int i = 1; i <= v; ++i) l1[i] -= l0[i];
    for (auto it = primes.rbegin(); it != primes.rend(); ++it) {
        int p = *it;
        int q = (int)(p)*p, M = N / p, s = s1[p - 1];
        int t = v / p, u = min((int)v, N / q);
        for (int i = q; i <= v; ++i) s1[i] += (s1[divide(i, p)] - s) * f(p, 1);
        for (int i = u; i > t; --i) l1[i] += (s1[divide(M, i)] - s) * f(p, 1);
        for (int i = t; i >= 1; --i) l1[i] += (l1[i * p] - s) * f(p, 1);
    }
    for (int i = 1; i <= v; ++i) s1[i] += 1;
    for (int i = 1; i <= v; ++i) l1[i] += 1;
    const auto dfs = [&](auto&& self, int n, unsigned beg, int coeff) -> i128 {
        if (!coeff) return 0;
        i128 ret = i128(coeff) * (n > v ? l1[divide(N, n)] : s1[n]);
        for (unsigned i = beg; i < sz(primes); ++i) {
            int p = primes[i];
            int q = (int)(p)*p;
            if (q > n) break;
            int nn = divide(n, q);
            for (int e = 2; nn > 0; nn = divide(nn, p), ++e)
                ret += self(self, nn, i + 1, coeff * (f(p, e) - f(p, 1) * f(p, e - 1)));
        }
        return ret;
    };
    return dfs(dfs, N, 0, 1);
}
