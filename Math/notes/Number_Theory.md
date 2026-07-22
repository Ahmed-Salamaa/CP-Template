# From Number_Theory_Snippets.md
# Number Theory Snippets

This document contains a collection of essential number theory algorithms and their implementations in C++.

## Trial Division

```cpp
vector<int> factorize(int n) {
    vector<int> factorization;
    for (int d = 2; d * d <= n; d++) {
        while (n % d == 0) {
            factorization.push_back(d);
            n /= d;
        }
    }
    if (n > 1)
        factorization.push_back(n);
    return factorization;
}
```

## Euler's Totient Function (Phi) Range Sieve

```cpp
vector<int> compute_phi_range(int n) {
    vector<int> phi(n + 1);
    for (int i = 0; i <= n; i++)
        phi[i] = i;

    for (int i = 2; i <= n; i++) {
        if (phi[i] == i) {
            for (int j = i; j <= n; j += i)
                phi[j] -= phi[j] / i;
        }
    }
    return phi;
}
```

## Modular Multiplicative Inverse in a Range

```cpp
vector<int> compute_range_inverse(int mod) {
    vector<int> inv(mod);
    inv[1] = 1;
    for (int a = 2; a < mod; ++a)
        inv[a] = mod - (int)(mod / a) * inv[mod % a] % mod;
    return inv;
}
```

## Fermat's Modular Inverse

```cpp
int fast_power(long long a, int b, int m) {
    a %= m;
    int res = 1;
    while (b > 0) {
        if (b & 1) res = res * a % m;
        a = a * a % m;
        b >>= 1;
    }
    return res;
}

int compute_fermat_inverse(int a, int m) {
    return fast_power(a, m - 2, m);
}
```

## Binary GCD

```cpp
int binary_gcd(int a, int b) {
    if (!a || !b)
        return a | b;
    unsigned shift = __builtin_ctz(a | b);
    a >>= __builtin_ctz(a);
    do {
        b >>= __builtin_ctz(b);
        if (a > b)
            swap(a, b);
        b -= a;
    } while (b);
    return a << shift;
}
```

## Extended GCD for Negative Numbers

```cpp
int gcd_negative(int a, int b, int& x, int& y) {
    x = 1; y = 0;
    int x1 = 0, y1 = 1, a1 = a, b1 = b;
    while (b1) {
        int q = a1 / b1;
        tie(x, x1) = make_tuple(x1, x - q * x1);
        tie(y, y1) = make_tuple(y1, y - q * y1);
        tie(a1, b1) = make_tuple(b1, a1 - q * b1);
    }
    return a1;
}
```

## Möbius Function Sieve

```cpp
vector<int> build_mobius(int n) {
    vector<int> mob(n + 1, 0);
    mob[1] = 1;
    for (int i = 2; i <= n; i++) {
        mob[i]--;
        for (int j = i + i; j <= n; j += i) {
            mob[j] -= mob[i];
        }
    }
    return mob;
}
```

## LinearSieve

**Description:** Topic: Linear Sieve Computes prime numbers up to N and the Lowest Prime Factor (LPF) for each number in strictly O(N) time.

**Usage:**
```cpp
linear_sieve(MAXN);
vector<int> factors = get_factors(X); // Using LPF
```

**Time Complexity:** O(N)
**Space Complexity:** O(N)

```cpp
#include "../../../core.h"

namespace LinearSieve {
            const int N = 1e7 + 5;
    int Lpf[N];

    vector<int> build() {
        for (int i = 1; i < N; i++) Lpf[i] = i;
        vector<int> primes;
        for (int i = 2; i < N; i++) {
            if (Lpf[i] == i) {
                primes.emplace_back(i);
            }
            for (auto &val : primes) {
                if (i * val >= N) break;
                Lpf[i * val] = val;
                if (i % val == 0) break; // Optimization to keep linear time
            }
        }
        return primes;
    }
}

```

## SegmentedSieve

**Description:** Topic: Segmented Sieve Finds all primes in a large range [L, R] where R can be up to 10^12 but R - L is relatively small (e.g., 10^6).

**Usage:**
```cpp
vector<int> primes = segmented_sieve(L, R);
```

**Time Complexity:** O(sqrt(R) + (R-L+1) * log(log(R)))
**Space Complexity:** O(sqrt(R) + R - L + 1)

```cpp
#include "../../../core.h"

namespace SegmentedSieve {
            vector<int> build(int L, int R) {
        int lim = (int)sqrt(R);
        vector<bool> mark(lim + 1, true);
        vector<int> primes;
        for (int i = 2; i <= lim; ++i) {
            if (mark[i]) {
                primes.emplace_back(i);
                for (int j = i * i; j <= lim; j += i)
                    mark[j] = false;
            }
        }

        vector<bool> isPrime(R - L + 1, true);
        for (int i : primes) {
            for (int j = max(i * i, (L + i - 1) / i * i); j <= R; j += i)
                isPrime[j - L] = false;
        }
        if (L == 1)
            isPrime[0] = false;
            
        vector<int> ans_primes;
        for (int i = 0; i < R - L + 1; i++) {
            if (isPrime[i]) ans_primes.emplace_back(i + L);
        }
        return ans_primes;
    }
}

```

## FermatPrimality

**Description:** Topic: Fermat Primality Testing Randomized primality testing algorithm using Fermat's Little Theorem. It can have false positives (Carmichael numbers) but is very fast.

**Usage:**
```cpp
if (is_prime_fermat(N, 10)) { ... }
```

**Time Complexity:** O(iter * log(N))
**Space Complexity:** O(1)

```cpp
#include "../../../core.h"

namespace FermatPrimality {
            
    // Needs a binpower implementation, assuming fast_power or similar exists
    int binpower(int base, int e, int mod) {
        int result = 1;
        base %= mod;
        while (e) {
            if (e & 1) result = (int)((__int128)result * base % mod);
            base = (int)((__int128)base * base % mod);
            e >>= 1;
        }
        return result;
    }

    bool check(int n, int iter = 100) {
        if (n < 4)
            return n == 2 || n == 3;

        for (int i = 0; i < iter; i++) {
            int a = 2 + rand() % (n - 3);
            if (binpower(a, n - 1, n) != 1)
                return false;
        }
        return true;
    }
}

```

## ApplyPermutation

**Description:** Topic: Apply Permutation Computes the K-th power or K-th application of a permutation array efficiently using binary exponentiation in O(N log K).

**Usage:**
```cpp
vector<int> perm = {2, 0, 1};
vector<int> res = apply_permutation(perm, K);
```

**Time Complexity:** O(N log(K))
**Space Complexity:** O(N)

```cpp
#include "../../../core.h"

namespace ApplyPermutation {
            vector<int> power(vector<int>& per, vector<int>& seq, int k) {
        auto apply = [&](vector<int>& a, vector<int>& b) {
            vector<int> na(sz(a));
            for (int j = 0; j < sz(b); j++) {
                na[j] = a[b[j] - 1];
            }
            return na;
        };
        auto res = per;
        while (k) {
            if (k & 1) {
                res = apply(res, seq);
            }
            seq = apply(seq, seq);
            k >>= 1;
        }
        return res;
    }
}

```

## ExtendedEuclidInverse

**Description:** Topic: Extended Euclid Inverse Finds the modular inverse of a modulo m using the Extended Euclidean Algorithm. Works as long as gcd(a, m) == 1 (m does not need to be prime).

**Usage:**
```cpp
int inv = mod_inverse_ext(a, m);
```

**Time Complexity:** O(log(min(a, m)))
**Space Complexity:** O(log(min(a, m))) due to recursion

```cpp
#include "../../../core.h"

namespace ModInverseExtendedEuclid {
            int extended_euclidean(int a, int b, int &x, int &y) {
        if (a < 0) {
            int r = extended_euclidean(-a, b, x, y);
            x *= -1;
            return r;
        }
        if (b < 0) {
            int r = extended_euclidean(a, -b, x, y);
            y *= -1;
            return r;
        }
        if (b == 0) {
            x = 1;
            y = 0;
            return a;
        }
        int sol = extended_euclidean(b, a % b, y, x);
        y -= (a / b) * x;
        return sol;
    }

    const int inf = 2e9; // or some constant representing failure
    int compute(int a, int m) {
        int x, y;
        int g = extended_euclidean(a, m, x, y);
        return (g == 1 ? (x % m + m) % m : -inf);
    }
}
```

## ModularArithmetic

**Description:** Topic: Modular Arithmetic Basic modular arithmetic structures or utility functions to safely handle addition, subtraction, multiplication, and exponentiation without overflow.

**Usage:**
```cpp
ModInt a(10), b(20);
ModInt c = a + b * fast_pow(a, 3);
```

**Time Complexity:** O(1)
**Space Complexity:** O(1)

```cpp
//
// Description:
// Provides safe functions for arithmetic operations over a prime modulo, handling 
// negative numbers correctly. Also contains modular exponentiation and modular inverse.
//
// How to use it:
// int a = 1e9, b = 1e9;
// int res = ModularArithmetic::mul_mod(a, b, 1000000007);
// int mod_inv = ModularArithmetic::mod_inv(a, 1000000007);
#include "../../../core.h"

namespace ModularArithmetic {
    const int MOD = 1e9 + 7;

            int add_mod(int a, int b, int m) {
        return (((a % m) + (b % m)) % m);
    }

            int sub_mod(int a, int b, int m) {
        return (((a % m) - (b % m) + m) % m);
    }

            int mul_mod(int a, int b, int m) {
        return (((a % m) * (b % m)) % m);
    }

    // Time Complexity: O(length of s)
        int Big_Mod(string s, int mod) {
        int res = 0;
        for (auto &c : s) res = (res * 10 + (c - '0')) % mod;
        return res;
    }

    // Time Complexity: O(log(b))
        int mod_pow(int a, int b, int m = MOD) {
        int res = 1;
        while (b) {
            if (b & 1) res = res * a % m;
            a = a * a % m;
            b >>= 1;
        }
        return res;
    }

    // Time Complexity: O(log(m))
        int mod_inv(int a, int m = MOD) {
        return mod_pow(a, m - 2, m); // Fermat's Little Theorem
    }
}
```

## SubsetsWithCertainGCD

**Description:** Topic: Subsets With Certain GCD Dynamic programming or inclusion-exclusion approach to count the number of subsets or pairs in an array that have exactly a given GCD.

**Usage:**
```cpp
int subsets = count_subsets_gcd(arr, target_gcd);
```

**Time Complexity:** O(N log(N))
**Space Complexity:** O(N)

```cpp
//
// Description:
// Given an array of numbers, this algorithm uses a DP sieve-like approach to find 
// the total number of pairs (or subsets) that share a specific GCD value.
//
// How to use it:
// vector<int> a = {2, 4, 6, 8};
// int total_coprime_pairs = SubsetsWithCertainGCD::solve(4, a);
#include "../../../core.h"

namespace SubsetsWithCertainGCD {
    const int N = 1e6 + 10;
    int freq[N], cnt[N];
    int dp[N];

            int solve(int n, vector<int>& a) {
        memset(freq, 0, sizeof(freq));
        memset(cnt, 0, sizeof(cnt));
        memset(dp, 0, sizeof(dp));

        for (int i = 0; i < n; i++) {
            freq[a[i]]++;
        }
        for (int i = 1; i < N; i++) {
            for (int j = i; j < N; j += i) {
                cnt[i] += freq[j];
            }
        }
        for (int i = N - 1; i > 0; i--) {
            // number of pairs with gcd i 
            // if subsets you can make it 2 ^ cnt[i] - 1
            int total = 1LL * cnt[i] * (cnt[i] - 1) / 2;
            for (int j = 2 * i; j < N; j += i) {
                total -= dp[j];
            }
            dp[i] = total;
        }

        return dp[1]; // pairs with gcd 1
    }
}
```

## Phi

**Description:** Topic: Euler Totient (Phi) Calculates the Euler Totient Function phi(n), which counts the number of integers up to n that are relatively prime to n.

**Usage:**
```cpp
int p = phi(N);
```

**Time Complexity:** O(sqrt(n))
**Space Complexity:** O(1)

```cpp
//
// Description:
// Provides an O(sqrt(N)) function to compute phi(N) for a single integer,
// and an O(N log log N) sieve to precompute phi(N) up to MAX.
//
// How to use it:
// int res = EulerTotient::phi(100);
// vector<int> phi_arr = EulerTotient::phi_sieve(100000);
#include "../../../core.h"

namespace EulerTotient {
            int phi(int n) {
        int result = n;
        for (int i = 2; i * i <= n; i++) {
            if (n % i == 0) {
                while (n % i == 0) {
                    n /= i;
                }
                result -= result / i;
            }
        }
        if (n > 1) {
            result -= result / n;
        }
        return result;
    }

    // Time Complexity: O(MAX log(log(MAX)))
    // Space Complexity: O(MAX)
    vector<int> phi_sieve(int MAX) {
        vector<int> phi(MAX + 1);
        iota(all(phi), 0);
        for (int i = 2; i <= MAX; i++) {
            if (phi[i] == i) {
                for (int j = i; j <= MAX; j += i) {
                    phi[j] -= phi[j] / i;
                }
            }
        }
        return phi;
    }
}
```

## Primes Sieve

**Description:** Topic: Primes Sieve Sieve of Eratosthenes implementation to find all primes up to N efficiently. Also includes basic primality testing functions.

**Usage:**
```cpp
sieve(MAXN);
if (is_prime[x]) cout << x << " is prime" << endl;
```

**Time Complexity:** O(sqrt(x))
**Space Complexity:** O(1)

```cpp
//
// Description:
// Provides a fast O(sqrt(N)) single-number prime check and an O(N log log N) 
// Sieve of Eratosthenes to precompute primes up to MAXV. Also stores the 
// smallest prime factor (spf) for rapid factorization.
//
// How to use it:
// Primes::sieve(); // Precompute primes up to 1e7
// bool is_p = Primes::isPrimeSieve(97);
#include "../../../core.h"

namespace Primes {
            bool isprime(int x) {
        if (x <= 1) return false;
        if (x == 2 || x == 3) return true;
        if (x % 2 == 0 || x % 3 == 0) return false;
        for (int i = 5; i * i <= x; i += 6)
            if (x % i == 0 || x % (i + 2) == 0) return false;
        return true;
    }

    const int MAXV = 1e7 + 5;
    vector<int> spf(MAXV), primes;

    // Time Complexity: O(MAXV log(log(MAXV)))
    // Space Complexity: O(MAXV)
    void sieve() {
        for (int i = 1; i < MAXV; i++) spf[i] = i;

        for (int i = 4; i < MAXV; i += 2) spf[i] = 2;

        for (int i = 3; i * i < MAXV; i += 2) 
            if (spf[i] == i)  
                for (int j = i * i; j < MAXV; j += i) 
                    if (spf[j] == j) spf[j] = i;

        for (int i = 2; i < MAXV; i++)
            if (spf[i] == i)
                primes.push_back(i);
    }

    // Time Complexity: O(log(number of primes up to MAXV))
        bool isPrimeSieve(const int &n) {
        return binary_search(all(primes), n);
    } 
}
```

## MillerRabinRandomized

**Description:** * Topic: Miller Rabin Randomized * Description: Randomized Miller-Rabin primality test. Fast and robust probabilistic method with an extremely low chance of false positives. * Usage: *    if (is_prime_randomized(N, iterations)) { ... }

**Time Complexity:** O(iter * log(N))
**Space Complexity:** O(1)

```cpp
#include "../../../core.h"

namespace MillerRabinRandom {
            
    int fast_power(int base, int e, int mod) {
        int result = 1;
        base %= mod;
        while (e) {
            if (e & 1) result = (int)((__int128)result * base % mod);
            base = (int)((__int128)base * base % mod);
            e >>= 1;
        }
        return result;
    }

    bool check_composite(int n, int a, int d, int s) {
        int x = fast_power(a, d, n);
        if (x == 1 || x == n - 1)
            return false;
        for (int r = 1; r < s; r++) {
            x = (int)((__int128)x * x % n);
            if (x == n - 1)
                return false;
        }
        return true;
    }

    bool check(int n, int iter = 100) { 
        if (n < 4)
            return n == 2 || n == 3;

        int s = 0;
        int d = n - 1;
        while ((d & 1ll) == 0) {
            d >>= 1ll;
            s++;
        }

        for (int i = 0; i < iter; i++) {
            int a = 2 + rand() % (n - 3);
            if (check_composite(n, a, d, s))
                return false;
        }
        return true;
    }
}
```

## MillerRabinDeterministic

**Description:** * Topic: Miller Rabin Deterministic * Description: Deterministic Miller-Rabin primality test for 64-bit integers using a known set of fixed bases, providing 100% accuracy up to 2^64. * Usage: *    if (is_prime_mr(N)) { cout << "Prime\n"; }

**Time Complexity:** O(log(N))
**Space Complexity:** O(1)

```cpp
#include "../../../core.h"

namespace MillerRabinDeterministic {
            using u64 = uint64_t;
    using u128 = __uint128_t;

    u64 binpower(u64 base, u64 e, u64 mod) {
        u64 result = 1;
        base %= mod;
        while (e) {
            if (e & 1)
                result = (u128)result * base % mod;
            base = (u128)base * base % mod;
            e >>= 1;
        }
        return result;
    }

    bool check_composite(u64 n, u64 a, u64 d, int s) {
        u64 x = binpower(a, d, n);
        if (x == 1 || x == n - 1)
            return false;
        for (int r = 1; r < s; r++) {
            x = (u128)x * x % n;
            if (x == n - 1)
                return false;
        }
        return true;
    }
    
    bool check(u64 n) {
        if (n < 2)
            return false;

        int r = 0;
        u64 d = n - 1;
        while ((d & 1) == 0) {
            d >>= 1;
            r++;
        }

        for (int a : {2, 3, 5, 7, 11, 13, 17, 19, 23, 29, 31, 37}) {
            if (n == a)
                return true;
            if (check_composite(n, a, d, r))
                return false;
        }
        return true;
    }
}
```

## PhiSum

**Description:** * Topic: Phi Sum * Description: Uses Dirichlet convolution (Mertens trick / Du Sieve) to efficiently compute the prefix sum of the Euler totient function: sum(phi(i)) for i=1..N. * Usage: *    int total_phi = get_phi_sum(N);

**Time Complexity:** O(N^(2/3))
**Space Complexity:** O(N^(2/3))

```cpp
#include "../../../core.h"

namespace PhiSum {
            const int mod = 1e9 + 7, inv = (1e9 + 8) / 2;
    const int N = 1e7;
    int phi[N + 1], pref[N + 1];
    
    void buildPhi(int n) {
        pref[1] = 1;
        for (int i = 0; i <= n; i++) phi[i] = i;
        for (int i = 2; i <= n; i++) {
            if (phi[i] == i) {
                for (int j = i; j <= n; j += i)
                    phi[j] -= phi[j] / i;
            }
            pref[i] = pref[i - 1] + phi[i];
            if (pref[i] >= mod) pref[i] -= mod;
        }
    }
    
    unordered_map<int, int> dp;
    
    int sum(int n) {
        if (n < N) return pref[n];
        if (dp.count(n)) return dp[n];
        int at = 1, value = n;
        int ans = (n % mod) * ((n + 1) % mod) % mod * inv % mod;
        while (at <= value) {
            int t = value / at;
            int nxt = value / t + 1;
            int cnt = nxt - at;
            cnt %= mod;
            if (t != n) ans -= sum(t) * cnt % mod;
            if (ans < 0) ans += mod;
            at = nxt;
        }
        return dp[n] = ans;
    }
}
```

## ArrayInverse

**Description:** * Topic: Array Inverse * Description: Computes the modular inverse for a sequence of N elements in O(N) total time by computing prefix and suffix products. * Usage: *    vector<int> invs = array_inverse(arr, mod);

**Time Complexity:** O(N + log(M))
**Space Complexity:** O(N)

```cpp
#include "../../../core.h"

namespace ArrayInverse {
            int extended_euclidean(int a, int b, int &x, int &y) {
        if (a < 0) {
            int r = extended_euclidean(-a, b, x, y);
            x *= -1;
            return r;
        }
        if (b < 0) {
            int r = extended_euclidean(a, -b, x, y);
            y *= -1;
            return r;
        }
        if (b == 0) {
            x = 1, y = 0;
            return a;
        }
        int sol = extended_euclidean(b, a % b, y, x);
        y -= (a / b) * x;
        return sol;
    }

    vector<int> compute(const vector<int> &a, int m) {
        int n = sz(a);
        if (n == 0) return {};
        vector<int> b(n);
        int v = 1;
        for (int i = 0; i != n; ++i) {
            b[i] = v;
            v = static_cast<int>(v) * a[i] % m;
        }
        int x, y;
        extended_euclidean(v, m, x, y);
        x = (x % m + m) % m;
        for (int i = n - 1; i >= 0; --i) {
            b[i] = static_cast<int>(x) * b[i] % m;
            x = static_cast<int>(x) * a[i] % m;
        }
        return b;
    }
}
```

