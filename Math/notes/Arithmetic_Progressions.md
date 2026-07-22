# From Arithmetic_Progressions_Snippets.md
## FloorSumofArithmeticProgressions

**Topic:** Floor Sum - [Computes sum of floor((A*i+B)/M)]

**Description:** Computes sum of floor((A*i+B)/M). This mathematical template provides an efficient implementation for Floor Sum. It is commonly used in Advanced Number Theory problems.

**Usage:**
```cpp
FloorSumofArithmeticProgressions_Algorithm solver;
auto result = solver.sumsq(...);
```

**Time Complexity:** O(log A)
**Space Complexity:** O(1)

```cpp
#include "../../../core.h"


struct FloorSumofArithmeticProgressions_Algorithm {

    using ll = int;
    ll sumsq(ll n) {
      return n / 2 * ((n - 1) | 1);
    }
    // \sum_{i = 0}^{n - 1}{(a + d * i) / m}, O(log m)
    ll floor_sum(ll a, ll d, ll m, ll n) {
      ll res = d / m * sumsq(n) + a / m * n;
      d %= m; a %= m;
      if (!d) return res;
      ll to = (n * d + a) / m;
      return res + (n - 1) * to - floor_sum(m - 1 - a, m, d, to);
    }
    // \sum_{i = 0}^{n - 1}{(a + d * i) % m}
    ll mod_sum(ll a, ll d, ll m, ll n) {
      a = ((a % m) + m) % m;
      d = ((d % m) + m) % m;
      return n * a + d * sumsq(n) - m * floor_sum(a, d, m, n);
    }
};

```

## SumofFloors

**Topic:** Sum of Floors - [Computes sum of floor(N/i)]

**Description:** Computes sum of floor(N/i). This mathematical template provides an efficient implementation for Sum of Floors. It is commonly used in Advanced Number Theory problems.

**Usage:**
```cpp
SumofFloors_Algorithm solver;
auto result = solver.floor_sum(...);
```

**Time Complexity:** O(sqrt(N))
**Space Complexity:** O(1)

```cpp
#include "../../../core.h"


struct SumofFloors_Algorithm {

    using ll = int;

    // \sum{k=1}^{n}{floor(n/k)}
    // count of numbers such that n/i = k -> n/k - n/(k+1)
    ll floor_sum(ll n) {
      ll sum = 0;
      for (ll i = 1, last; i <= n; i = last + 1) {
        last = n / (n / i);
        sum += (n / i) * (last - i + 1);
        // n / x yields the same value for i <= x <= last.
      }
      return sum;
    }
};

```

## SumofArithmeticProgressionModularandDivided

**Topic:** Sum of AP Modular - [Computes sums of AP under modulo and division]

**Description:** Computes sums of AP under modulo and division. This mathematical template provides an efficient implementation for Sum of AP Modular. It is commonly used in Advanced Number Theory problems.

**Usage:**
```cpp
SumofArithmeticProgressionModularandDivided_Algorithm solver;
auto result = solver.mod_sum(...);
```

**Time Complexity:** O(log A)
**Space Complexity:** O(1)

```cpp
#include "../../../core.h"


struct SumofArithmeticProgressionModularandDivided_Algorithm {

    const int N = 3e5 + 9;
    /*
    Sums of arithmetic progressions.
    mod_sum(n, a, d, m) = \sum_{i = 0}^{n - 1}{(a + d * i) % m}.
    floor_sum(n, a, d, m) = \sum_{i = 0}^{n - 1}{(a + d * i) / m}.
    log(m), with a large constant.
    */
    int sumsq(int n) {
        return n / 2 * ((n - 1) | 1);
    }
    int floor_sum(int a, int d, int m, int n) {
        int res = d / m * sumsq(n) + a / m * n;
        d %= m; a %= m;
        if (!d) return res;
        int to = (n * d + a) / m;
        return res + (n - 1) * to - floor_sum(m - 1 - a, m, d, to);
    }
    int mod_sum(int a, int d, int m, int n) {
        a = ((a % m) + m) % m;
        d = ((d % m) + m) % m;
        return n * a + d * sumsq(n) - m * floor_sum(a, d, m, n);
    }
};
```

## IntersectionofArithmeticProgressions

**Topic:** Intersection of APs - [Finds intersection of multiple arithmetic progressions]

**Description:** * Description: Finds intersection of multiple arithmetic progressions. * This mathematical template provides an efficient implementation for Intersection of APs. * It is commonly used in Advanced Number Theory problems. * * Usage: * IntersectionofArithmeticProgressions_Algorithm solver; * auto result = solver.extended_euclid(...);

**Time Complexity:** O(log A)
**Space Complexity:** O(1)

```cpp
#include "../../../core.h"


struct IntersectionofArithmeticProgressions_Algorithm {

    using T = __int128;
    using ll = int;
    // ax + by = __gcd(a, b)
    // returns __gcd(a, b)
    T extended_euclid(T a, T b, T &x, T &y) {
      T xx = y = 0;
      T yy = x = 1;
      while (b) {
        T q = a / b;
        T t = b; b = a % b; a = t;
        t = xx; xx = x - q * xx; x = t;
        t = yy; yy = y - q * yy; y = t;
      }
      return a;
    }
    // finds x such that x % m1 = a1, x % m2 = a2. m1 and m2 may not be coprime
    // here, x is unique modulo m = lcm(m1, m2). returns (x, m). on failure, m = -1.
    pair<T, T> CRT(T a1, T m1, T a2, T m2) {
      T p, q;
      T g = extended_euclid(m1, m2, p, q);
      if (a1 % g != a2 % g) return make_pair(0, -1);
      T m = m1 / g * m2;
      p = (p % m + m) % m;
      q = (q % m + m) % m;
      return make_pair((p * a2 % m * (m1 / g) % m + q * a1 % m * (m2 / g) % m) %  m, m);
    }
    // intersecting AP of two APs: (a1 + d1x) and (a2 + d2x)
    pair<ll, ll> intersect(ll a1, ll d1, ll a2, ll d2) {
      auto x = CRT(a1 % d1, d1, a2 % d2, d2);
      ll a = x.first, d = x.second;
      if (d == -1) return {0, 0}; // empty
      ll st = max(a1, a2);
      a = st % d == a ? st : st - st % d + a;
      return {a, d};
    }
};
```

