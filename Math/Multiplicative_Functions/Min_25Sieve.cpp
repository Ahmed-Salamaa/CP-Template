// Topic: Min-25 Sieve - [Sublinear sieve for prefix sums of multiplicative functions]
#include "../../../core.h"

/*
 * Description: The Min-25 Sieve is an advanced number-theoretic sieve used to compute the
 *   prefix sum of multiplicative functions in sublinear time. It is highly useful
 *   for efficiently answering queries up to large limits like 10^11.
 * NOTE: This template uses standard integers with manual modulo arithmetic for performance and simplicity. If the
 * problem involves extremely complex chaining of operations or you want to avoid manual modulo bugs, you can easily
 * swap the `int` types back to a `modint` template. This mathematical template provides an efficient implementation for
 * Min-25 Sieve. It is commonly used in Advanced Number Theory problems.
 *
 * Usage:
 * Min_25Sieve_Algorithm solver;
 * auto result = solver.solve(...);
 */
// Time Complexity: O(N^(3/4) / log N)
// Space Complexity: O(sqrt(N))
struct Min_25Sieve_Algorithm {
    static const int N = 2e5 + 9, D = 3;  // N >= 2 * sqrt(n)
    static const int mod = 1e9 + 7;
    using ll = int;
    using T = int;

    T power(T base, T exp) {
        T res = 1;
        base %= mod;
        while (exp > 0) {
            if (exp % 2 == 1) res = (1LL * res * base) % mod;
            base = (1LL * base * base) % mod;
            exp /= 2;
        }
        return res;
    }

    T inv(T n) { return power(n, mod - 2); }

    /**
    Let f(x) be a multiplicative function and
    when p is a prime, f(p) has a polynomial representation
    and f(p^k) can be calculated quickly

    We can compute the prefix sum of f(n) in O((n^(3/4)) / logn) using Min_25 sieve
    Takes ~1s for n = 10^10 (deg = 3) in CF
    unfold the loops over poly to make the code faster
    Tutorial: https://www.luogu.com.cn/problem/solution/P5325
    **/
    int primes[N], p, deg;
    T pref[D][N];
    T poly[D];  // polynomial representation of f(p)

    void sieve(int n) {
        vector<bool> f(n + 1, false);
        p = 0;
        for (int i = 2; i <= n; i++) {
            if (!f[i]) {
                primes[++p] = i;
                T cur = 1;
                // pref[k][p] = sum of primes[i]^k s.t. i <= p
                for (int k = 0; k < deg; k++) {
                    pref[k][p] = (pref[k][p - 1] + cur) % mod;
                    cur = (1LL * cur * i) % mod;
                }
            }
            for (int j = 1; j <= p and primes[j] * i <= n; j++) {
                f[i * primes[j]] = 1;
                if (i % primes[j] == 0) break;
            }
        }
    }

    int r, id1[N], id2[N];
    ll n;

    int get_id(ll x) { return x <= r ? id1[x] : id2[n / x]; }

    int tot;
    T g[D][N];
    ll a[N];

    // f(p^k) in O(1)
    inline T eval(int p, int k, ll pw) {  // pw = p^k, pw <= n
        T ans = pw % mod;
        return (1LL * ans * (ans - 1 + mod)) % mod;
    }

    // no memorization needed!
    // sum of f(i) s.t. spf[i] > primes[j]
    T yo(ll x, int j) {
        if (primes[j] >= x) return 0;
        int id = get_id(x);
        T ans = 0;
        // initialization: ans = g(x) - g(primes[j])
        for (int k = 0; k < deg; k++) { ans = (ans + 1LL * poly[k] * g[k][id] % mod) % mod; }
        for (int k = 0; k < deg; k++) { ans = (ans - 1LL * poly[k] * pref[k][j] % mod + mod) % mod; }
        for (int i = j + 1; i <= p and primes[i] <= x / primes[i]; i++) {
            ll pw = primes[i];
            for (int e = 1; pw <= x; e++) {
                ans = (ans + 1LL * eval(primes[i], e, pw) * (yo(x / pw, i) + (e != 1)) % mod) % mod;
                if (!(pw <= x / primes[i])) break;
                pw *= primes[i];
            }
        }
        return ans;
    }

    // Min_25: Say my name
    // Dirichlet: You're heisenberg
    // Min_25: You're goddamn right
    // sum of f(i) for 1 <= i <= n
    // pol is the polynomial representation of f(p)
    T solve(ll _n, vector<T> pol) {
        n = _n;
        deg = sz(pol);
        for (int i = 0; i < deg; i++) { poly[i] = pol[i] % mod; }
        r = sqrt(n);
        while (1LL * r * r < n) ++r;
        while (1LL * r * r > n) --r;
        sieve(r);
        tot = 0;
        ll i = 1;
        while (i <= n) {
            ll x = n / i;
            ll j = n / x;
            a[++tot] = x;
            // initialization g[k][tot] = sum of i^k for 2 <= i <= x
            T z = x % mod;  // remove this mod if not needed
            for (int k = 0; k < deg; k++) {
                if (k == 0) { g[k][tot] = (z - 1 + mod) % mod; }
                if (k == 1) { g[k][tot] = (1LL * z * (z + 1) % mod * inv(2) % mod - 1 + mod) % mod; }
                if (k == 2) {
                    g[k][tot] = (1LL * z * (z + 1) % mod * (2 * z + 1) % mod * inv(6) % mod - 1 + mod) % mod;
                }
                // compute for larger values using lagrange if needed
            }
            if (x <= r)
                id1[x] = tot;
            else
                id2[n / x] = tot;
            i = j + 1;
        }
        // an integer x belongs to the array a iff for some integer z, n / z = x
        // g[k][i] = sum of prime^k for prime <= a[i]
        for (int i = 1; i <= p; i++) {
            for (int j = 1; j <= tot && primes[i] <= a[j] / primes[i]; j++) {
                int id = get_id(a[j] / primes[i]);
                T cur = 1;
                for (int k = 0; k < deg; k++) {
                    g[k][j] = (g[k][j] - 1LL * cur * (g[k][id] - pref[k][i - 1] + mod) % mod + mod) % mod;
                    cur = (1LL * cur * primes[i]) % mod;
                }
            }
        }
        return (yo(n, 0) + 1) % mod;  // add f(1)
    }
};
