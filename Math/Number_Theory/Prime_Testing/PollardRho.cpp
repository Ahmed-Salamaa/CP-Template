/*
 * Topic: Pollard Rho
 * Description: Pollard's rho algorithm is a remarkably fast randomized integer factorization
 *   algorithm that finds non-trivial prime factors of composite numbers in
 *   O(N^(1/4)). It handles numbers up to 10^18 efficiently when combined with the
 *   Miller-Rabin primality test.
 * O(N^(1/4)) to find a non-trivial factor. NOTE: This template uses standard integers with manual modulo arithmetic for
 * performance and simplicity. If the problem involves extremely complex chaining of operations or you want to avoid
 * manual modulo bugs, you can easily swap the `int` types back to a `modint` template. Usage: vector<int> factors =
 * PollardRho::factorize(N);
 */
#include <cassert>

#include "../../../core.h"

// Time Complexity: O(N^(1/4))
// Space Complexity: O(log(N))
inline uint64_t mul(uint64_t a, uint64_t b, uint64_t m) { return (uint64_t)((__uint128_t)a * b % m); }

uint64_t power(uint64_t base, uint64_t exp, uint64_t mod) {
    uint64_t res = 1;
    base %= mod;
    while (exp > 0) {
        if (exp % 2 == 1) res = mul(res, base, mod);
        base = mul(base, base, mod);
        exp /= 2;
    }
    return res;
}

bool suspect(const uint64_t& a, const uint64_t& s, uint64_t d, const uint64_t& n) {
    uint64_t x = power(a, d, n);
    if (x == 1) return true;
    for (unsigned int r = 0; r < s; r++) {
        if (x == n - 1) return true;
        x = mul(x, x, n);
    }
    return false;
}

bool is_prime(const uint64_t& n) {
    if (n <= 1 || (n > 2 && (~n & 1))) return false;
    uint64_t d = n - 1, s = 0;
    while (~d & 1) {
        s++;
        d >>= 1;
    }
    static const uint64_t a_big[] = {2, 325, 9375, 28178, 450775, 9780504, 1795265022};
    static const uint64_t a_smol[] = {2, 7, 61};
    if (n <= 4759123141LL) {
        for (auto&& p : a_smol) {
            if (p >= n) break;
            if (!suspect(p, s, d, n)) return false;
        }
    } else {
        for (auto&& p : a_big) {
            if (p >= n) break;
            if (!suspect(p, s, d, n)) return false;
        }
    }
    return true;
}

uint64_t rho_pollard(const uint64_t& n) {
    if (~n & 1) return 2u;
    static random_device rng;
    uniform_int_distribution<uint64_t> rr(1, n - 1);
    for (;;) {
        uint64_t c_ = rr(rng), g = 1, r = 1, m = 500;
        uint64_t y = rr(rng), xx = 0, c = c_, ys = 0, q = 1;
        while (g == 1) {
            xx = y;
            for (unsigned int i = 1; i <= r; i++) {
                y = mul(y, y, n);
                y = (y + c) % n;
            }
            uint64_t k = 0;
            g = 1;
            while (k < r && g == 1) {
                for (unsigned int i = 1; i <= min(m, (uint64_t)(r - k)); i++) {
                    ys = y;
                    y = mul(y, y, n);
                    y = (y + c) % n;
                    uint64_t diff = xx > y ? xx - y : y - xx;
                    q = mul(q, diff, n);
                }
                g = gcd(q, n);
                k += m;
            }
            r *= 2;
        }
        if (g == n) g = 1;
        while (g == 1) {
            ys = mul(ys, ys, n);
            ys = (ys + c) % n;
            uint64_t diff = xx > ys ? xx - ys : ys - xx;
            g = gcd(diff, n);
        }
        if (g != n && is_prime(g)) return g;
    }
    assert(69 == 420);
}

template <typename T>
vector<T> factorize(T n) {
    if (n < 2) return {};
    if (is_prime(n)) return {n};
    T d = rho_pollard(static_cast<uint64_t>(n));
    vector<T> l = factorize(d), r = factorize(n / d);
    l.insert(l.end(), all(r));
    return l;
}
