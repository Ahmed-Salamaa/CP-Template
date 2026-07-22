// Topic: Sum of nCi Fixed Large n - [Computes sum of combinations for fixed large n]
#include "../../../core.h"

/*
 * Description: Optimized mathematical technique to efficiently evaluate the sum of binomial
 *   coefficients up to a certain point for a very large N. Useful in probabilistic
 *   queries or counting subsets where direct calculation of all terms is too slow.
 * This mathematical template provides an efficient implementation for Sum of nCi Fixed Large n.
 * It is commonly used in Advanced Number Theory problems.
 *
 * Usage:
 * SumofnCiforaFixedLargen_Algorithm solver;
 * auto result = solver.mul_inv(...);
 */
// Time Complexity: O(N)
// Space Complexity: O(N)
struct SumofnCiforaFixedLargen_Algorithm {
#pragma GCC optimize("O3")
#pragma GCC target("avx")
#ifdef __x86_64__
#define NTT64
#endif
#define _rep(_1, _2, _3, _4, name, ...) name
#define rep2(i, n) rep3(i, 0, n)
#define rep3(i, a, b) rep4(i, a, b, 1)
#define rep4(i, a, b, c) for (int i = int(a); i < int(b); i += int(c))
#define rep(...) _rep(__VA_ARGS__, rep4, rep3, rep2, _)(__VA_ARGS__)
    using i64 = int;
    using u32 = unsigned;
    using u64 = unsigned int;
    using f80 = ld;
    // NOTE: Implement/Use the standard FFT/NTT polynomial template here.
    // namespace ntt
    using R = int;
    using R64 = i64;
    // NOTE: Implement/Use the standard FFT/NTT polynomial template here.
    ;

    int pow_mod(int b, int e, int mod) {
        int ret = 1;
        for (; e; e >>= 1, b = i64(b) * b % mod) {
            if (e & 1) ret = i64(ret) * b % mod;
        }
        return ret;
    }

    int binomial_sum_mod_p(int N, int K, int mod) {
        if (K == 0) return 1 % mod;
        if (N <= K) return pow_mod(2, N, mod);
        if (i64(K) * 2 > N) { return (pow_mod(2, N, mod) + i64(mod) - binomial_sum_mod_p(N, N - K - 1, mod)) % mod; }
        assert(N < mod);
        const int sqrt_K = sqrt(K);
        poly::set_mod(mod, sqrt_K);
        auto mod_invs = [&](vector<int>& f) {
            int n = sz(f);
            vector<int> ret(f);
            if (n > 0) {
                rep(i, 1, n) ret[i] = i64(ret[i - 1]) * ret[i] % mod;
                int inv = poly::mod_inv(ret[n - 1]);
                for (int i = n - 1; i > 0; --i) {
                    ret[i] = i64(ret[i - 1]) * inv % mod;
                    inv = i64(inv) * f[i] % mod;
                }
                ret[0] = inv;
            }
            return ret;
        };
        auto conv = [&](vector<int>& f) -> poly {
            int n = sz(f);
            const auto& ifacts = poly::ifacts;
            auto g = poly(f);
            rep(i, n) {
                int d = i64(ifacts[i]) * ifacts[(n - 1) - i] % mod;
                if ((n - 1 - i) & 1) d = mod - d;
                g[i] = i64(g[i]) * d % mod;
            }
            return g;
        };
        auto shift = [&](const poly& cf, const poly& f, i64 dx) {
            if ((dx %= mod) < 0) dx += mod;
            const int n = sz(f);
            const int a = i64(dx) * poly::mod_inv(sqrt_K) % mod;
            auto g = poly(2 * n);
            rep(i, sz(g)) g[i] = (i64(mod) + a + i - n) % mod;
            rep(i, sz(g)) if (g[i] == 0) g[i] = 1;
            g.coefs = mod_invs(g.coefs);
            auto ret = cf.middle_product(g);
            int prod = 1;
            rep(i, n) prod = i64(prod) * (i64(mod) + a + n - 1 - i) % mod;
            for (int i = n - 1; i >= 0; --i) {
                ret[i] = i64(ret[i]) * prod % mod;
                prod = i64(prod) * g[n + i] % mod * (i64(mod) + a + i - n) % mod;
            }
            if (dx % sqrt_K == 0) {
                int k = n - dx / sqrt_K;
                rep(i, k) ret[i] = f[n - k + i];
            }
            return ret.coefs;
        };
        using Pair = pair<vector<int>, vector<int> >;
        function<Pair(int)> rec = [&](int n) -> Pair {
            if (n == 1) { return Pair({N, N - sqrt_K}, {1, sqrt_K + 1}); }
            int nh = n >> 1;
            auto res = rec(nh);
            auto &f11 = res.first, &g11 = res.second;
            auto f = conv(f11), g = conv(g11);
            auto g12 = shift(g, g11, nh);
            auto g21 = shift(g, g11, i64(sqrt_K) * nh);
            auto g22 = shift(g, g11, i64(sqrt_K) * nh + nh);
            auto f12 = shift(f, f11, N - nh * i64(sqrt_K + 2));
            auto f21 = shift(f, f11, i64(sqrt_K) * nh);
            auto f22 = shift(f, f11, N - i64(2) * nh * (sqrt_K + 1));
            rep(i, nh + 1) { g11[i] = (i64(g11[i]) * f12[nh - i] + i64(g12[i]) * f11[i]) % mod; }
            rep(i, 1, nh + 1) { g11.push_back((i64(g21[i]) * f22[nh - i] + i64(g22[i]) * f21[i]) % mod); }
            f12 = shift(f, f11, nh);
            f22 = shift(f, f11, i64(sqrt_K) * nh + nh);
            rep(i, nh + 1) f11[i] = i64(f11[i]) * f12[i] % mod;
            rep(i, 1, nh + 1) f11.push_back(i64(f21[i]) * f22[i] % mod);
            if (n & 1) {
                rep(i, n) { g11[i] = (i64(g11[i]) + f11[i]) * (n + i64(i) * sqrt_K) % mod; }
                rep(i, n) { f11[i] = i64(f11[i]) * (i64(N) + mod - sqrt_K * i - n + 1) % mod; }
                vector<int> vals(n);
                rep(i, n) vals[i] = (i64(sqrt_K) * n + i + 1) % mod;
                if (i64(sqrt_K + 1) * n < mod) {
                    int prod = 1;
                    rep(i, n) prod = i64(prod) * vals[i] % mod;
                    auto invs = mod_invs(vals);
                    i64 s = 0;
                    rep(i, n) {
                        s += prod;
                        prod = i64(prod) * invs[i] % mod * (i64(N) + mod - i64(sqrt_K) * n - i) % mod;
                    }
                    g11.push_back(s % mod);
                    f11.push_back(prod);
                } else {
                    g11.push_back(0);
                    f11.push_back(0);
                }
            }
            return {f11, g11};
        };
        auto res = rec(sqrt_K);
        auto &f1 = res.first, &g1 = res.second;
        auto f2 = shift(conv(f1), f1, N - i64(sqrt_K) * (sqrt_K + 1));
        reverse(all(f2));
        f2.resize(sz(f2) - 1);
        f2 = mod_invs(f2);
        i64 ret = 0;
        rep(i, sqrt_K) {
            ret = (ret * f1[sqrt_K - 1 - i] + g1[sqrt_K - 1 - i]) % mod;
            ret = ret * f2[sqrt_K - 1 - i] % mod;
        }
        int prod = 1;
        rep(i, sqrt_K) { prod = i64(prod) * f1[i] % mod * f2[i] % mod; }
        const int rest = max(0, K - sqrt_K * sqrt_K);
        ret += prod;
        vector<int> invs(rest);
        rep(i, rest) invs[i] = i + 1 + sqrt_K * sqrt_K;
        invs = mod_invs(invs);
        rep(i, rest) {
            prod = i64(prod) * (N - sqrt_K * sqrt_K - i) % mod * invs[i] % mod;
            ret += prod;
        }
        ret %= mod;
        return ret;
    }

    // credit: Min_25
    void solve() {
        const u32 p = u32(-1) >> 1;  // 2^31
        printf("%d\n", binomial_sum_mod_p(2e9, 1e9, p));
    }
};
