# From Fractions_Snippets.md
## RationalApproximation

**Topic:** Rational Approximation Py - [Python script for rational approximation]

**Time Complexity:** O(log N)
**Space Complexity:** O(1)

```cpp
#include "../../../core.h"

from fractions import Fraction

class RationalApproximation_Algorithm:
    def solve(self, x, max_denominator=2323):
        # gets the closest rational number to x where denominator is limited
        p = Fraction(x).limit_denominator(max_denominator)
        return p
```

## ContinuedFractions

**Topic:** Continued Fractions - [Generates continued fraction representation]

**Description:** Generates continued fraction representation. This mathematical template provides an efficient implementation for Continued Fractions. It is commonly used in Advanced Number Theory problems.

**Usage:**
```cpp
ContinuedFractions_Algorithm solver;
auto result = solver.get_frac(...);
```

**Time Complexity:** O(log N)
**Space Complexity:** O(log N)

```cpp
#include "../../../core.h"


struct ContinuedFractions_Algorithm {

    using ll = int;
    // returns the continued fraction of p / q
    // [a[0]; a[1], ..., a[n]] = a[0] + 1 / (a[1] + 1 / (a[2] + ...) + 1 / (a[n - 1] + 1 / a[n]))))
    // a[0] is integer number and a[1],a[2],…,a[n] are positive integer numbers and either n = 0 or a[n] != 1
    // its unique
    vector<ll> get_frac(ll p, ll q) {
      vector<ll> a;
      while (q) {
        a.push_back(p / q);
        p %= q; swap(p, q);
      }
      return a;
    }
    // ans[k] = [a[0]; a[1], ..., a[k]]
    vector<pair<ll, ll>> convergents(vector<ll> a) {
      ll lp = 1, lq = 0;
      vector<pair<ll, ll>> ans({{a[0], 1}});
      for (int i = 1; i < sz(a); i++) {
        ll p = a[i] * ans.back().first + lp;
        ll q = a[i] * ans.back().second + lq;
        // assert(__gcd(p, q) == 1)
        lp = ans.back().first;
        lq = ans.back().second;
        ans.push_back({p, q});
      }
      return ans;
    }
};
```

## RationalApproximation

**Topic:** Rational Approximation - [Finds best rational approximation with bounded denominator]

**Description:** Finds best rational approximation with bounded denominator. This mathematical template provides an efficient implementation for Rational Approximation. It is commonly used in Advanced Number Theory problems.

**Usage:**
```cpp
RationalApproximation_Algorithm solver;
auto result = solver.O(...);
```

**Time Complexity:** O(log N)
**Space Complexity:** O(1)

```cpp
#include "../../../core.h"


struct RationalApproximation_Algorithm {

    using ll = int;
    /**
    Given n and a real number x >= 0, returns the closest rational approximation p/q s.t. p, q <= n.
    It will obey that |p/q - x| is minimum for p, q <= n
    Time: O(log n)
    **/
    using ld = ld;
    pair<ll, ll> approximate(ld x, ll n) {
      ll LP = 0, LQ = 1, P = 1, Q = 0, inf = LLONG_MAX; ld y = x;
      while (1) {
        ll lim = min(P ? (n - LP) / P : inf, Q ? (n - LQ) / Q : inf),
        a = (ll)floor(y), b = min(a, lim),
        NP = b * P + LP, NQ = b * Q + LQ;
        if (a > b) {
          // If b > a/2, we have a semi-convergent that gives us a
          // better approximation; if b = a/2, we *may* have one.
          // Return {P, Q} here for a more canonical approximation.
          return (abs(x - (ld)NP / (ld)NQ) < abs(x - (ld)P / (ld)Q)) ?
            make_pair(NP, NQ) : make_pair(P, Q);
        }
        if (abs(y = 1 / (y - (ld)a)) > 3 * n) {
          return {NP, NQ};
        }
        LP = P; P = NP;
        LQ = Q; Q = NQ;
      }
    }
};
```

