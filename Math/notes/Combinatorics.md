# From Combinatorics_Snippets.md
# Combinatorics Snippets

This document consolidates several common combinatorics algorithms and formulas used in competitive programming.

## Catalan Numbers (Combinations Formula)

#

## Description & Math

Catalan numbers are a sequence of natural numbers that occur in various counting problems, often involving recursively defined objects (e.g., valid parenthesis sequences, binary search trees, triangulations of a polygon).
The $n$-th Catalan number is given by the formula:
$$C_n = \frac{1}{n+1} \binom{2n}{n} = \frac{(2n)!}{(n+1)!n!}$$

#

## Complexity

- **Time Complexity:** $O(1)$ per query after an $O(N)$ precomputation for factorials and their modular inverses.
- **Space Complexity:** $O(N)$ for storing factorials and inverses.

#

## When to Use

Use this when you need to answer multiple queries for Catalan numbers modulo a prime (like $10^9+7$), and $N$ is up to a few millions.

#

## Code

```cpp
const int mod = 1e9 + 7;

// Assuming Ncr and binpow are available from a combinatorics base template
int get_catalan(int n) {
    int ans = Ncr(2 * n, n);
    return (1LL * ans * binpow(n + 1, mod - 2)) % mod;
}
```

## Catalan Numbers (Recursive / DP)

#

## Description & Math

When the combinations formula cannot be used (e.g., modulo is not prime, or no modular arithmetic is needed and numbers fit in arbitrary precision / small bounds), Catalan numbers can be defined recursively:
$$C_0 = 1, \quad C_{n} = \sum_{i=0}^{n-1} C_i C_{n-1-i} \text{ for } n > 0$$
This translates nicely into a DP or memoized recursion.

#

## Complexity

- **Time Complexity:** $O(n^2)$ with memoization (or $O(3^n)$ unmemoized).
- **Space Complexity:** $O(n)$ for the recursion stack / memoization table.

#

## When to Use

Use when $n$ is very small, or when extending the Catalan recurrence to non-standard variations where the simple combinatorial formula no longer applies.

#

## Code

```cpp
int get_catalan_recursive(int n) {
    if (n <= 1) return 1;
    int res = 0;
    for (int i = 0; i < n; i++) {
        res += get_catalan_recursive(i) * get_catalan_recursive(n - i - 1);
    }
    return res;
}
```

## Derangements

#

## Description & Math

A derangement is a permutation of the elements of a set such that no element appears in its original position.
The number of derangements of $n$ items, denoted as $D_n$, follows the recurrence relation:
$$D_n = (n-1)(D_{n-1} + D_{n-2})$$
with base cases $D_1 = 0$ and $D_2 = 1$.

#

## Complexity

- **Time Complexity:** $O(n)$ with memoization or tabulation.
- **Space Complexity:** $O(n)$.

#

## When to Use

Use when counting permutations where specific elements (or all elements) must not fall into their initial slots.

#

## Code

```cpp
int get_derangement(int n) {
    if (n <= 2) {
        return n == 2 ? 1 : 0;
    }
    return (n - 1) * (get_derangement(n - 1) + get_derangement(n - 2));
}
```

## Factorial Power Prime (Legendre's Formula)

#

## Description & Math

Legendre's formula gives the exponent of the highest power of a prime $k$ that divides $n!$.
The exponent is calculated by summing the floor of $n$ divided by successive powers of $k$:
$$v_k(n!) = \sum_{i=1}^{\infty} \left\lfloor \frac{n}{k^i} \right\rfloor$$

#

## Complexity

- **Time Complexity:** $O(\log_k n)$.
- **Space Complexity:** $O(1)$.

#

## When to Use

Extremely useful in combinatorics to calculate trailing zeros in factorials, or when computing combinations $\binom{n}{r} \pmod m$ for a composite modulus using prime factorization.

#

## Code

```cpp
int factorial_power(int n, int k) {
    int res = 0;
    while (n) {
        n /= k;
        res += n;
    }
    return res;
}
```

## Lucas Theorem Mod 2 (Parity of nCr)

#

## Description & Math

Lucas' Theorem helps compute $\binom{n}{k} \pmod p$ for a prime $p$. When $p=2$, the theorem implies that $\binom{n}{k}$ is odd if and only if the binary representation of $k$ is a submask of the binary representation of $n$.
In bitwise terms, $\binom{n}{k}$ is odd $\iff (k \& \sim n) == 0$.

#

## Complexity

- **Time Complexity:** $O(1)$.
- **Space Complexity:** $O(1)$.

#

## When to Use

Whenever you need the parity (odd/even) of $\binom{n}{k}$ without computing its actual value, especially for huge $n$ and $k$.

#

## Code

```cpp
bool is_ncr_odd(int n, int i) {
    // Returns true if nCr(n, i) is odd
    return (i & ~n) == 0;
}
```

## Stirling Numbers of the First Kind

#

## Description & Math

Stirling numbers of the first kind (unsigned), $\left[\begin{matrix}n\\k\end{matrix}\right]$, count the number of permutations of $n$ elements with $k$ disjoint cycles.
The recurrence relation is:
$$\left[\begin{matrix}n\\k\end{matrix}\right] = \left[\begin{matrix}n-1\\k-1\end{matrix}\right] + (n-1) \left[\begin{matrix}n-1\\k\end{matrix}\right]$$
with base cases $\left[\begin{matrix}n\\0\end{matrix}\right] = 0$ (for $n>0$) and $\left[\begin{matrix}0\\0\end{matrix}\right] = 1$.

#

## Complexity

- **Time Complexity:** $O(n \cdot k)$ if memoized/tabulated.
- **Space Complexity:** $O(n \cdot k)$ or $O(k)$ with space optimization.

#

## When to Use

Use for permutation cycle counting.

#

## Code

```cpp
// Unsigned Stirling numbers of the first kind
int stirling_number_first(int n, int k) {
    if (k == 0) return n == 0;
    if (n == 0) return 0;
    return stirling_number_first(n - 1, k - 1) + (n - 1) * stirling_number_first(n - 1, k);
}
```

## Number of Sorted Arrays (Stars and Bars)

#

## Description & Math

To find the number of non-decreasing arrays of length $n$ with elements in the range $[L, R]$, we can map this to choosing $n$ elements from $m = R - L + 1$ available numbers with repetition allowed.
Using the "Stars and Bars" combinatorics method, this is equivalent to $\binom{n + m - 1}{n}$.

#

## Complexity

- **Time Complexity:** $O(1)$ queries after $O(N)$ combinatorics precomputation.
- **Space Complexity:** $O(1)$ auxiliary.

#

## When to Use

Counting monotonic sequences bounded by a range.

#

## Code

```cpp
const int mod = 1e9 + 7;

int count_sorted_arrays(int n, int l, int r) {
    int m = r - l + 1;
    // Assuming Ncr is accessible and precomputed
    return Ncr(n + m - 1, n);
}
```

## Sub-Rectangles in a Grid

#

## Description & Math

1. **Total sub-rectangles**: An $N \times M$ grid has $N+1$ horizontal lines and $M+1$ vertical lines. A rectangle is formed by choosing 2 horizontal and 2 vertical lines: $\binom{N+1}{2} \times \binom{M+1}{2} = \frac{N(N+1)}{2} \times \frac{M(M+1)}{2}$.
2. **Sub-rectangles containing a bounding box**: If we have a bounding box spanning $[X_{min}, X_{max}] \times [Y_{min}, Y_{max}]$ (1-indexed), the sub-rectangle must start at or before $X_{min}$, end at or after $X_{max}$, and similarly for $Y$. The number of ways to pick boundaries is $X_{min} \times (N - X_{max} + 1) \times Y_{min} \times (M - Y_{max} + 1)$.

#

## Complexity

- **Time Complexity:** $O(1)$.
- **Space Complexity:** $O(1)$.

#

## When to Use

For combinatorics problems on grids where you need to count structures or find probabilities involving rectangles.

#

## Code

```cpp
struct SubRectanglesInGrid {
    int n, m; // grid dimensions
    
    long long all_sub_rectangles() {
        return 1LL * (n * (n + 1) / 2) * (m * (m + 1) / 2);
    }
    
    long long contain(int min_x, int max_x, int min_y, int max_y) {
        return 1LL * min_x * (n - max_x + 1) * min_y * (m - max_y + 1);
    }
};
```

## CountingGrids

**Description:** Topic: Counting Grids  Uses Burnside's Lemma to count unique n x n grids with 2 colors up to rotation (0, 90, 180, 270 degrees). How to use it: int ans = BurnsideLemma::count_grids(n);

**Time Complexity:** O(log(n))
**Space Complexity:** O(1)

```cpp
#include "../../../core.h"

namespace BurnsideLemma {
    const int mod = 1e9 + 7;

    int binpow(int a, int b) {
        int res = 1;
        while (b) {
            if (b & 1) res = res * a % mod;
            a = a * a % mod;
            b >>= 1;
        }
        return res;
    }

    int count_grids(int n) {
        int res;
        if (n & 1) {
            res = binpow(2, n * n) + 2 * (binpow(2, (n - 1) / 2 * (n + 1) / 2) * 2 + binpow(2, n * n / 2));
        } else {
            res = binpow(2, n * n) + binpow(2, n * n / 4) * 2 + binpow(2, n * n / 2);
        }
        res %= mod;
        return res * binpow(4, mod - 2) % mod;
    }
}

```

## Necklaces

**Description:** Topic: Necklaces  Uses Polya Enumeration Theorem to count unique necklaces with n elements and m colors, considering rotational symmetries. How to use it: int ans = PolyaEnumeration::count_necklaces(n, m);

**Time Complexity:** O(n * log(n))
**Space Complexity:** O(1)

```cpp
#include "../../../core.h"

namespace PolyaEnumeration {
    const int mod = 1e9 + 7;

    int binpow(int a, int b) {
        int res = 1;
        while (b) {
            if (b & 1) res = res * a % mod;
            a = a * a % mod;
            b >>= 1;
        }
        return res;
    }

    int count_necklaces(int n, int m) {
        int res = 0;
        for (int i = 0; i < n; i++) {
            res += binpow(m, gcd(i, n));
            res %= mod;
        }
        return res * binpow(n, mod - 2) % mod;
    }
}

```

## CatalanIterative

**Description:** Topic: Catalan Iterative  Computes Catalan numbers iteratively using DP in O(N^2) time. Useful for smaller N or when combinations are not available. How to use it: CatalanIterative cat; cat.init(); int ans = cat.get(n);

**Time Complexity:** O(N^2)
**Space Complexity:** O(N)

```cpp
#include "../../../core.h"

struct CatalanIterative {
    static const int MAX = 1001;
    static const int mod = 1e9 + 7;
    int catalan[MAX];

    void init() {
        catalan[0] = catalan[1] = 1;
        for (int i = 2; i < MAX; i++) {
            catalan[i] = 0;
            for (int j = 0; j < i; j++) {
                catalan[i] = (catalan[i] + (1LL * catalan[j] * catalan[i - j - 1]) % mod) % mod;
            }
        }
    }
    
    int get(int n) {
        return catalan[n];
    }
};

```

## nCrPascalTriangle

**Description:** Topic: nCr Pascal Triangle  Computes nCr using Pascal's triangle dynamic programming in O(M^2). Useful for small M without modulo operations. How to use it: nCrPascalTriangle pt; pt.PreCalculation(); int ans = pt.Ncr(n, r);

**Time Complexity:** O(M^2)
**Space Complexity:** O(M^2)

```cpp
#include "../../../core.h"

struct nCrPascalTriangle {
    static const int M = 1001;
    int C[M + 1][M + 1];

    void PreCalculation() {
        C[0][0] = 1;
        for (int n = 1; n <= M; ++n) {
            C[n][0] = C[n][n] = 1;
            for (int k = 1; k < n; ++k)
                C[n][k] = C[n - 1][k - 1] + C[n - 1][k];
        }
    }

    int Ncr(int n, int r) {
        if (n < 0 || r < 0 || r > n) return 0;
        return C[n][r];
    }
};

```

## SummationOfValues

**Description:** Topic: Summation Of Values  Computes the number of ways to represent an integer n as an ordered sum of positive integers (compositions), which is 2^(n-1). How to use it: int ans = SummationOfValues::sum(n, mod);

**Time Complexity:** O(log(n))
**Space Complexity:** O(1)

```cpp
#include "../../../core.h"

namespace SummationOfValues {
    int fast_power(int a, int b, int m) {
        int res = 1 % m;
        a %= m;
        while (b) {
            if (b & 1) res = res * a % m;
            a = a * a % m;
            b >>= 1;
        }
        return res;
    }

    int sum(int n, int m = 0) {
        if (!m) return (1LL << (n - 1));
        return fast_power(2LL, n - 1, m);
    }
}

```

## Combinatorics

**Topic:** Combinatorics Basics - [Basic nCr, factorials, inverse factorials]

**Description:** Basic nCr, factorials, inverse factorials. NOTE: This template uses standard integers with manual modulo arithmetic for performance and simplicity. If the problem involves extremely complex chaining of operations or you want to avoid manual modulo bugs, you can easily swap the `int` types back to a `modint` template.

**Usage:**
```cpp
CombinatoricsBasics_Algorithm solver;
auto result = solver.modint(...);
```

**Time Complexity:** O(N) precalc, O(1) query
**Space Complexity:** O(N)

```cpp
#include "../../../core.h"


struct Combinatorics_Algorithm {
    static const int N = 3e5 + 9, mod = 1e9 + 7;

    struct combi{
      int n; vector<int> facts, finvs, invs;
      combi(int _n): n(_n), facts(_n), finvs(_n), invs(_n){
        facts[0] = finvs[0] = 1;
        invs[1] = 1;
        for (int i = 2; i < n; i++) invs[i] = invs[mod % i] * (mod - mod / i) % mod;
        for(int i = 1; i < n; i++){
          facts[i] = facts[i - 1] * i % mod;
          finvs[i] = finvs[i - 1] * invs[i] % mod;
        }
      }
      inline int fact(int n) { return facts[n]; }
      inline int finv(int n) { return finvs[n]; }
      inline int inv(int n) { return invs[n]; }
      inline int ncr(int n, int k) { return n < k or k < 0 ? 0 : facts[n] * finvs[k] % mod * finvs[n-k] % mod; }
    };
    combi C;
    Combinatorics_Algorithm() : C(N) {}
};

```

## PrefixSumQueries

**Topic:** Prefix Sum Queries of nCi - [Mo's algorithm on Pascal's triangle]

**Description:** Mo's algorithm on Pascal's triangle. NOTE: This template uses standard integers with manual modulo arithmetic for performance and simplicity. If the problem involves extremely complex chaining of operations or you want to avoid manual modulo bugs, you can easily swap the `int` types back to a `modint` template. NOTE: This template requires basic combinatorics. You must also include Math/Combinatorics/Basics/Combinatorics.cpp.

**Usage:**
```cpp
PrefixSumQueriesofnCi_Algorithm solver;
auto result = solver.C.ncr(...);
```

**Time Complexity:** O(Q sqrt(N))
**Space Complexity:** O(N)

```cpp
#include "../../../core.h"


struct PrefixSumQueriesofnCi_Algorithm {

    static const int N = 1e5 + 9, B = 330, mod = 1e9 + 7;

    PrefixSumQueriesofnCi_Algorithm() {}


    struct query {
      int l, r, id;
      bool operator < (const query &x) const {
        if(l / B == x.l / B) return ((l / B) & 1)? r > x.r : r < x.r;
        return l / B < x.l / B;
      }
    } Q[N];

    int ans[N];
    // compute f(n, k) = C(n, 0) + C(n, 1) + ... + C(n, k)
    // use the identity: f(n + 1, k) = 2 * f(n, k) - C(n, k)
};

```

## FactorialPowerComposite

**Description:** Topic: Factorial Power Composite  Computes the highest power of a composite number k that divides n! by factorizing k into primes and finding the minimum factorial power. How to use it: int max_power = FactorialPowerComposite::factorial_power(n, k);

**Time Complexity:** O(sqrt(k) + log_k(n))
**Space Complexity:** O(1)

```cpp
#include "../../../core.h"

namespace FactorialPowerComposite {
    int cal(int n, int k) {
        int res = 0;
        while (n) {
            n /= k;
            res += n;
        }
        return res;
    }
    
    int factorial_power(int n, int k) {
        int res = 1e9; // inf
        for (int j = 2; j * j <= k; j++) {
            if (k % j == 0) {
                int count = 0;
                while (k % j == 0) {
                    count++;
                    k /= j;
                }
                res = min(res, cal(n, j) / count);
            }
        }
        if (k != 1) {
            res = min(res, cal(n, k));
        }
        return res;
    }
}
```

## CountUniqueMultiples

**Description:** Topic: Count Unique Multiples  Counts unique multiples of subsets of length m from a given set of numbers up to an upper bound y, utilizing inclusion-exclusion. How to use it: CountUniqueMultiples cum; cum.init(); int ans = cum.calc(n, m, y, a);

**Time Complexity:** O(2^N * N)
**Space Complexity:** O(C^2)

```cpp
#include "../../../core.h"

struct CountUniqueMultiples {
    static const int C = 21;
    unsigned int binom[C][C];

    void init() {
        for (int i = 0; i < C; i++)
            binom[i][0] = binom[i][i] = 1;
        for (int i = 2; i < C; i++)
            for (int j = 1; j < i; j++)
                binom[i][j] = binom[i - 1][j] + binom[i - 1][j - 1];
    }

    // calc(elements count, subset size, upper bound, elements)
    int calc(unsigned int n, unsigned int m, unsigned int y, vector<unsigned int>& a) {
        unsigned int ans = 0;
        for (unsigned int msk = 0; msk < (1ULL << n); msk++) {
            if (popcnt(msk) < m) continue;
            unsigned int L = 1;
            for (int i = 0; i < n; i++) {
                if (~msk >> i & 1) continue;
                unsigned int g = gcd(L, a[i]);
                if ((__int128)(L) * a[i] / g > y) L = y + 1;
                else L *= a[i] / g;
            }
            if (popcnt(msk) % 2 == m % 2)
                ans += (y / L) * binom[popcnt(msk)][m];
            else
                ans -= (y / L) * binom[popcnt(msk)][m];
        }
        return ans;
    }
};
```

## LucasTheorem

**Topic:** Lucas Theorem - [Computes nCr modulo a small prime P]

**Description:** Computes nCr modulo a small prime P. This mathematical template provides an efficient implementation for Lucas Theorem. It is commonly used in Advanced Number Theory problems. NOTE: This template uses standard integers with manual modulo arithmetic for performance and simplicity. If the problem involves extremely complex chaining of operations or you want to avoid manual modulo bugs, you can easily swap the `int` types back to a `modint` template.

**Usage:**
```cpp
LucasTheorem_Algorithm solver;
auto result = solver.modint(...);
```

**Time Complexity:** O(P) precalc, O(log_P N) query
**Space Complexity:** O(P)

```cpp
#include "../../../core.h"


struct LucasTheorem_Algorithm {

    static const int N = 1e6 + 3, mod = 1e6 + 3;
    using ll = int;

    struct combi{
      int n; vector<int> facts, finvs, invs;
      combi(int _n): n(_n), facts(_n), finvs(_n), invs(_n){
        facts[0] = finvs[0] = 1;
        invs[1] = 1;
        for (int i = 2; i < n; i++) invs[i] =  (1LL * invs[mod % i] * (mod - mod / i)) % mod;
        for(int i = 1; i < n; i++){
          facts[i] = (1LL * facts[i - 1] * i) % mod;
          finvs[i] = (1LL * finvs[i - 1] * invs[i]) % mod;
        }
      }
      inline int fact(int n) { return facts[n]; }
      inline int finv(int n) { return finvs[n]; }
      inline int inv(int n) { return invs[n]; }
      inline int ncr(int n, int k) { return n < k or k < 0 ? 0 : 1LL * facts[n] * finvs[k] % mod * finvs[n-k] % mod; }
    };
    combi C = combi(N);

    // returns nCr modulo mod where mod is a prime
    // Complexity: log(n)
    int lucas(ll n, ll r) {
      if (r > n) return 0;
      if (n < mod) return C.ncr(n, r);
      return (1LL * lucas(n / mod, r / mod) * lucas(n % mod, r % mod)) % mod;
    }
};
```

## SumofC ai k

**Topic:** Sum of C(a_i, k) - [Computes sum of combinations for an array]

**Description:** Computes sum of combinations for an array. This mathematical template provides an efficient implementation for Sum of C(a_i, k). It is commonly used in Advanced Number Theory problems. NOTE: This template requires FFT/NTT. You must also include the FFT template from Math/Polynomials_and_FFT/FFT/FastFourierTransform.cpp (or NumberTheoreticTransform.cpp). NOTE: This template requires basic combinatorics. You must also include Math/Combinatorics/Basics/Combinatorics.cpp.

**Usage:**
```cpp
SumofC(a(i),k)foreachkfrom1ton_Algorithm solver;
auto result = solver.base(...);
```

**Time Complexity:** O(N log N)
**Space Complexity:** O(N)

```cpp
#include "../../../../core.h"


struct SumofC(a(i),k)foreachkfrom1ton_Algorithm {

    const int N = 2e5 + 9, mod = 924844033;

    vector<int> g[N];
    int sz[N];
    void dfs(int u, int p = 0) {
      sz[u] = 1;
      for (auto v: g[u]) {
        if (v == p) continue;
        dfs(v, u);
        sz[u] += sz[v];
      }
    }
    int f[N], ifac[N];
    // ans_k = sum_(C(a_i, k))
    vector<int> yo(int n, vector<int> a) {
      vector<int> cnt(N, 0);
      for (auto x: a) cnt[x]++;
      for (int i = 0; i < N; i++) cnt[i] = 1LL * cnt[i] * f[i] % mod;
      vector<int> b(N, 0);
      for (int i = 0; i < N ; i++) b[i] = ifac[i];
      reverse(all(b));
      auto ans = multiply(cnt, b);
      ans.erase(ans.begin(), ans.begin() + N - 1);
      ans.resize(n + 1);
      for (int i = 0; i <= n; i++) ans[i] = 1LL * ans[i] * power(f[i], mod - 2) % mod;
      return ans;
    }

};
```

## PartitionFunction

**Topic:** Partition Function - [Computes the number of integer partitions of N]

**Description:** Computes the number of integer partitions of N. NOTE: This template uses standard integers with manual modulo arithmetic for performance and simplicity. If the problem involves extremely complex chaining of operations or you want to avoid manual modulo bugs, you can easily swap the `int` types back to a `modint` template.

**Usage:**
```cpp
PartitionFunction_Algorithm solver;
auto result = solver.yo(...);
```

**Time Complexity:** O(N * sqrt(N))
**Space Complexity:** O(N)

```cpp
#include "../../../core.h"


struct PartitionFunction_Algorithm {

    static const int mod = 1e9 + 7;

    // ans[i] = number of partitions of i
    // O(n sqrt(n))
    // takes 1.5s for n = 5e5
    vector<int> yo(int n) {
      vector<int> ans(n + 1);
      vector<pair<int, int>> gp; // (sign, generalized pentagonal numbers)
      gp.emplace_back(0, 0);
      for (int i = 1; gp.back().second <= n; i++) {
        gp.emplace_back(i % 2 ? 1 : -1, i * (3 * i - 1) / 2);
        gp.emplace_back(i % 2 ? 1 : -1, i * (3 * i + 1) / 2);
      }
      ans[0] = 1;
      for (int i = 1; i <= n; i++) {
        for (auto it : gp) {
          if (i >= it.second) {
            ans[i] = (ans[i] + ans[i - it.second] * it.first) % mod;
            if (ans[i] < 0) ans[i] += mod;
          }
          else break;
        }
      }
      return ans;
    }
};
```

## BellNumber

**Topic:** Bell Number - [Computes the nth Bell number using FFT]

**Description:** Computes the nth Bell number using FFT. NOTE: This template uses standard integers with manual modulo arithmetic for performance and simplicity. If the problem involves extremely complex chaining of operations or you want to avoid manual modulo bugs, you can easily swap the `int` types back to a `modint` template.

**Note:** You must implement the FFT and polynomial templates from the `Polynomials_and_FFT` folder (which includes `struct poly` and polynomial `exp()` operations) for this to work.

**Usage:**
```cpp
BellNumber_Algorithm solver;
auto result = solver.bell(...);
```

**Time Complexity:** O(N log N)
**Space Complexity:** O(N)

```cpp
struct BellNumber_Algorithm {
    static const int N = 3e5 + 9, mod = 1e9 + 7;

    vector<int> bell(int n) {
      poly p(n + 1);
      long long f = 1;
      for (int i = 0; i <= n; i++) {
        p.a[i] = modInverse(f);
        f = (f * (i + 1)) % mod;
      }
      p.a[0] = (p.a[0] - 1 + mod) % mod;
      p = p.exp(n + 1);
      vector<int> ans(n + 1);
      f = 1;
      for (int i = 0; i <= n; i++) {
        ans[i] = (1LL * p[i] * f) % mod;
        f = (f * (i + 1)) % mod;
      }
      return ans;
    }
};
```

## Permutation

**Topic:** Permutation - Utilities for permutations, nth permutation, and permutation exponentiation

**Description:** Helper methods to apply permutations, compute permutations raised to a power, print all permutations, and map permutations to indices (and vice-versa).  How to use it: vector<int> a = {2, 0, 1};  sequence vector<int> p = {1, 2, 0};  permutation vector<int> result = Permutation::permute(a, p, 10); int idx = Permutation::PermToIndex(p);

```cpp
//
#include "../../../core.h"

namespace Permutation {
    // Time Complexity: O(N! * N)
    // Space Complexity: O(1)
    void Print_Permutation(vector<int> &nums) {
        sort(all(nums));
        do {
            for (auto &i : nums) cout << i << " ";
            cout << "\n";
        } while (next_permutation(all(nums)));
    }

    // Time Complexity: O(N! * N)
    // Space Complexity: O(1)
    void Print_Permutation(string s) {
        sort(all(s));
        do {
            cout << s << "\n";
        } while (next_permutation(all(s)));
    }

    // Time Complexity: O(N)
    // Space Complexity: O(N)
    vector<int> applyPermutation(vector<int> sequence, vector<int> permutation) {
        vector<int> newSequence(sz(sequence));
        for (int i = 0; i < sz(sequence); i++) {
            newSequence[i] = sequence[permutation[i]];
        }
        return newSequence;
    }

    // Time Complexity: O(N log(K))
    // Space Complexity: O(N)
    vector<int> permute(vector<int> sequence, vector<int> permutation, int k) {
        while (k > 0) {
            if (k & 1) {
                sequence = applyPermutation(sequence, permutation);
            }
            permutation = applyPermutation(permutation, permutation);
            k >>= 1;
        }
        return sequence;
    }

    const int N = 21;
    int Fact[N]; // Should be precomputed before calling

    // Time Complexity: O(len)
    // Space Complexity: O(len)
    vector<int> nthPerm(int len, int nth) {
        vector<int> identity(len), perm(len);
        for (int i = 0; i < len; i++) identity[i] = i;

        for (int i = len - 1; i >= 0; --i) {
            int p = nth / Fact[i];
            perm[len - 1 - i] = identity[p];
            identity.erase(identity.begin() + p);
            nth %= Fact[i];
        }
        return perm;
    }

    // Time Complexity: O(len^2)
    // Space Complexity: O(1)
    int PermToIndex(vector<int> perm) {
        int idx = 0;
        int n = sz(perm);
        for (int i = 0; i < n; ++i) {
            idx += Fact[n - i - 1] * perm[i];
            for(int j = i + 1; j < n; j++)
                perm[j] -= perm[j] > perm[i];
        }
        return idx;
    }
}
```

## PermutationIndexing

**Topic:** Permutation Indexing

**Description:** Converts between a permutation and its lexicographical index in order from smallest to largest.

**Usage:**
```cpp
vector<int> p = PermutationIndexing::get_nth_permutation(len, nth);
int idx = PermutationIndexing::permutation_in_order(p);
```

```cpp
#include "../../../core.h"

// Time Complexity: O(N * N!) naive binary search
// Space Complexity: O(N)
namespace PermutationIndexing {
    vector<int> factorial(int n) {
        vector<int> v(n + 1, 1);
        for (int i = 2; i <= n; i++)
            v[i] = i * v[i - 1]; // Assume no overflow up to 20!
        return v;
    }

    // this function take length and index return the permutation that in order of small to large it's index is nth
    // Time Complexity: O(N log N) with ordered_set
    vector<int> get_nth_permutation(int len, int nth) {
        // Needs __gnu_pbds::tree from core.h (pbds)
        // For standard set this is O(N^2)
        // using ordered_set as per original code
        __gnu_pbds::tree<int, __gnu_pbds::null_type, less<int>, __gnu_pbds::rb_tree_tag, __gnu_pbds::tree_order_statistics_node_update> v;
        for (int i = 0; i < len; i++)
            v.insert(i + 1);
        vector<int> res;
        nth--;
        vector<int> fact = factorial(len);
        for (int i = len - 1; i >= 0; i--) {
            int p = nth / fact[i];
            auto it = v.find_by_order(p);
            res.emplace_back(*it);
            v.erase(it);
            nth %= fact[i];
        }
        return res;
    }

    // this function uses the old function it solve the following given a permutation what is it's index in order of small to large
    // Time Complexity: O(N * N!) using binary search over factorials - can be optimized significantly
    int permutation_in_order(vector<int> &v) {
        int len = sz(v);
        vector<int> fact = factorial(len);
        int left = 1, right = fact[len];
        while (left <= right) {
            int mid = left + (right - left) / 2;
            vector<int> temp = get_nth_permutation(len, mid);
            if (temp == v) return mid;
            else if (temp > v) right = mid - 1;
            else left = mid + 1;
        }
        return -1;
    }
}
```

## StirlingFirstKindFixedN

**Topic:** Stirling Number 1st Kind Fixed n - [Computes Stirling numbers of the 1st kind for fixed n]

**Description:** * Description: Computes Stirling numbers of the 1st kind for fixed n. * This mathematical template provides an efficient implementation for Stirling Number 1st Kind Fixed n. * It is commonly used in Advanced Number Theory problems. * * Usage: * StirlingNumberoftheFirstKindforFixedn_Algorithm solver; * auto result = solver.reduce(...);

**Note:** You must implement the FFT/NTT polynomial templates from the `Polynomials_and_FFT` folder for this to work.

**Time Complexity:** O(N log N)
**Space Complexity:** O(N)

```cpp
#include "../../../core.h"


struct StirlingNumberoftheFirstKindforFixedn_Algorithm {

    const int N = 1 << 18;
    const int mod = 998244353;
    
    int fact[N], ifact[N];
    vector<int> shift(vector<int> &f, int c) { //f(x + c)
      int n=sz(f);
      precompute(n + n - 1);
      vector<int> a = f; a.resize(lim);
      for (int i = 0; i < n; ++i) a[i] = (int) a[i] * fact[i] % mod;
      reverse(a.begin(), a.begin()+n);
      vector<int> b; b.resize(lim); b[0] = 1;
      for (int i = 1; i < n; ++i) b[i] = (int) b[i - 1] * c % mod;
      for (int i = 0; i < n; ++i) b[i] = (int) b[i] * ifact[i] % mod;
      ntt(a, 1), ntt(b, 1);
      for (int i = 0; i < lim; ++i) a[i] = (int) a[i] * b[i] % mod;
      ntt(a, 0), reverse(a.begin(), a.begin() + n);
      vector<int> g; g.resize(n);
      for (int i = 0; i < n; ++i) g[i] = (int) a[i] * ifact[i] % mod;
      return g;
    }
    // (x+1)*(x+2)*(x+3)...(x+n)
    // O(n log n) only for ntt friendly primes
    // otherwise use divide and conquer in O(n log^2 n)
    vector<int> range_mul(int n) {
      if (n == 0) return vector<int>({1});
      if (n & 1) {
        vector<int> f = range_mul(n - 1);
        f.push_back(0);
        for (int i = sz(f)-1; i; --i) f[i] = (f[i - 1] + (int) n * f[i]) % mod;
        f[0] = (int) f[0] * n % mod;
        return f;
      }
      else {
        int n_ = n >> 1;
        vector<int> f = range_mul(n_);
        vector<int> tmp = shift(f, n_);
        f.resize(n_ + 1);
        tmp.resize(n_ + 1);
        return multiply(f, tmp);
      }
    }
    // returns stirling1st(n, i) for 0 <= i <= n
    vector<int> stirling(int n) {
      if (n == 0) return {1};
      vector<int> ans = range_mul(n - 1);
      ans.resize(n + 1);
      for (int i = n - 1; i >= 0; i--) {
        ans[i + 1] = ans[i];
      }
      ans[0] = 0;
      return ans;
    }
};
```

## StirlingFirstKindFixedK

**Topic:** Stirling Number 1st Kind Fixed k - [Computes Stirling numbers of the 1st kind for fixed k]

**Description:** * Description: Computes Stirling numbers of the 1st kind for fixed k. * This mathematical template provides an efficient implementation for Stirling Number 1st Kind Fixed k. * It is commonly used in Advanced Number Theory problems. * * Usage: * StirlingNumberoftheFirstKindforFixedk_Algorithm solver; * auto result = solver.reduce(...);

**Note:** You must implement the FFT/NTT polynomial templates from the `Polynomials_and_FFT` folder for this to work.

**Time Complexity:** O(N log N)
**Space Complexity:** O(N)

```cpp
#include "../../../core.h"


struct StirlingNumberoftheFirstKindforFixedk_Algorithm {

    const int N = 1 << 18;
    const int mod = 998244353;
    
    // a^k % x^n
    vector<int> power(vector<int> a, int k, int n) {
      assert(sz(a) <= n);
      vector<int> ans({1});
      while (k) {
        if (k & 1) {
          ans = multiply(ans, a);
          ans.resize(n);
        }
        a = multiply(a, a);
        a.resize(n);
        k >>= 1;
      }
      return ans;
    }
    // returns stirling1st(i, k) for 0 <= i < n
    // O(n log^2 n)
    vector<int> stirling(int n, int k) {
      assert(k <= n);
      vector<int> a(n); // sum c(i) / i! x^i, c(i) = (i - 1)!
      a[0] = 0;
      for (int i = 1; i < n; i++) {
        a[i] = POW(i, mod - 2);
      }
      auto ans = power(a, k, n);
      ans.resize(n);
      int f = 1;
      for (int i = 1; i <= k; i++) {
        f = 1LL * f * i % mod;
      }
      int cur = 1;
      // sum ans(i) / i! = 1 / k! * a^k
      for (int i = 0; i < n; i++) {
        ans[i] = 1LL * ans[i] * cur % mod;
        cur = 1LL * cur * (i + 1) % mod;
        ans[i] = 1LL * ans[i] * POW(f, mod - 2) % mod;
      }
      return ans;
    }
};
```

## StirlingSecondKindFixedN

**Topic:** Stirling Number 2nd Kind Fixed n - [Computes Stirling numbers of the 2nd kind for fixed n]

**Description:** * Description: Computes Stirling numbers of the 2nd kind for fixed n. * This mathematical template provides an efficient implementation for Stirling Number 2nd Kind Fixed n. * It is commonly used in Advanced Number Theory problems. * * NOTE: This template uses standard integers with manual modulo arithmetic for performance and simplicity. If the problem involves extremely complex chaining of operations or you want to avoid manual modulo bugs, you can easily swap the `int` types back to a `modint` template. * * Usage: * StirlingNumberoftheSecondKindforFixedn_Algorithm solver; * auto result = solver.reduce(...);

**Note:** You must implement the FFT/NTT polynomial templates from the `Polynomials_and_FFT` folder for this to work.

**Time Complexity:** O(N log N)
**Space Complexity:** O(N)

```cpp
#include "../../../core.h"


struct StirlingNumberoftheSecondKindforFixedn_Algorithm {

    static const int N = 1 << 18;
    static const int mod = 998244353;
    

    int f[N];
};
```

## StirlingSecondKindFixedK

**Topic:** Stirling Number 2nd Kind Fixed k - [Computes Stirling numbers of the 2nd kind for fixed k]

**Description:** * Description: Computes Stirling numbers of the 2nd kind for fixed k. * This mathematical template provides an efficient implementation for Stirling Number 2nd Kind Fixed k. * It is commonly used in Advanced Number Theory problems. * * NOTE: This template uses standard integers with manual modulo arithmetic for performance and simplicity. If the problem involves extremely complex chaining of operations or you want to avoid manual modulo bugs, you can easily swap the `int` types back to a `modint` template. * Usage: * StirlingNumberoftheSecondKindforFixedk_Algorithm solver; * auto result = solver.base(...);

**Note:** You must implement the FFT/NTT polynomial templates from the `Polynomials_and_FFT` folder for this to work.

**Time Complexity:** O(N log N)
**Space Complexity:** O(N)

```cpp
#include "../../../core.h"
#include <cassert>


struct StirlingNumberoftheSecondKindforFixedk_Algorithm {

    static const int N = 3e5 + 9, mod = 1e9 + 7;
    // NOTE: This template requires FFT and Combinatorics templates to be included in your file.
    // Ensure you have included Math/Polynomials_and_FFT/FFT/FastFourierTransform.cpp
    // Ensure you have included Math/Combinatorics/Basics/Combinatorics.cpp


    ;
    // Uses global Combi logic (assuming it's available)
    // Initialize Combi C = Combi(N) in your main code if necessary, or pass fact/inv arrays.

    // mul (1 - ix)
    void yo(int l, int r, poly &ans){
      if (l == r) {
        ans = poly({1, mod - l});
        return;
      }
      int mid = (l + r) >> 1;
      poly a, b;
      yo(l ,mid, a);
      yo(mid + 1, r, b);
      ans = a * b;
    }
    // stirling2nd(i, k) for 0 <= i <= n
    // O(n log^2 n)
    vector<int> stirling(int n, int k) {
      poly p;
      yo(1, k, p);
      p = p.inverse(n + 1);
      auto ans = p.a;
      ans.insert(ans.begin(), k, 0);
      ans.resize(n + 1);
      return ans;
    }
};

// Implement missing operator / (const int &x) const { 
    int base = x, exp = 1000000007 - 2, res = 1;
    while(exp > 0) { if(exp % 2 == 1) res = (1LL * res * base) % 1000000007; base = (1LL * base * base) % 1000000007; exp /= 2; }
    return (*this) * res; 
}
integrate() const {
    int n = size(); vector<int> ans(n + 1);
    for(int i = 0; i < size(); i++) {
        int base = i + 1, exp = 1000000007 - 2, res = 1;
        while(exp > 0) { if(exp % 2 == 1) res = (1LL * res * base) % 1000000007; base = (1LL * base * base) % 1000000007; exp /= 2; }
        ans[i + 1] = (1LL * coef(i) * res) % 1000000007;
    }
    return ans;
}
```

## SumofnCiCongruenceClass

**Topic:** Sum of nCi Congruence Class - [Computes sum of combinations over congruence class]

**Description:** * Description: Computes sum of combinations over congruence class. * This mathematical template provides an efficient implementation for Sum of nCi Congruence Class. * It is commonly used in Advanced Number Theory problems. * * Usage: * SumofnCioveraFixedCongruenceClass_Algorithm solver; * auto result = solver.reduce(...);

**Note:** You must implement the FFT/NTT polynomial templates from the `Polynomials_and_FFT` folder for this to work.

**Time Complexity:** O(K^3 log N)
**Space Complexity:** O(K^2)

```cpp
#include "../../../../core.h"


struct SumofnCioveraFixedCongruenceClass_Algorithm {

    
    int m;
    void fix(vector<int> &ans) {
      int k = sz(ans);
      if (k < m) return;
      for (int i = k - 1; i >= m; i--) {
        ans[i % m] += ans[i];
        ans[i % m] %= mod;
      }
      ans.resize(m);
    }
};
```

## nCrWithMod

**Description:** * Topic: nCr With Mod * * Description: * Computes nCr modulo a prime number with precomputed factorials and their modular inverses. O(1) query time. * * How to use it: * nCrWithMod ncr; ncr.init(); int ans = ncr.Ncr(n, r);

**Time Complexity:** O(1) after O(N) prep
**Space Complexity:** O(N)

```cpp
#include "../../../core.h"

struct nCrWithMod {
    static const int N = 200005;
    static const int mod = 1e9 + 7;
    int inv[N], FInv[N], fact[N];

    void Inverse() {
        inv[0] = inv[1] = 1;
        for (int i = 2; i < N; i++)
            inv[i] = inv[mod % i] * (mod - mod / i) % mod;
    }

    void FactorialInverse() {
        FInv[0] = FInv[1] = 1;
        for (int i = 2; i < N; i++)
            FInv[i] = (inv[i] * FInv[i - 1]) % mod;
    }

    void factorial() {
        fact[0] = 1;
        for (int i = 1; i < N; i++) {
            fact[i] = (fact[i - 1] * i) % mod;
        }
    }

    void init() {
        Inverse();
        FactorialInverse();
        factorial();
    }

    int Ncr(int n, int r) {
        if (r > n || r < 0) return 0;
        int ans = ((fact[n] * FInv[r]) % mod * FInv[n - r]) % mod;
        return ans;
    }
};
```

