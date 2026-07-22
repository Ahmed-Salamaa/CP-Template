# From Congruences_Snippets.md
## ExtendedEuclid

**Topic:** Extended Euclidean Algorithm - [Finds x, y such that ax + by = gcd(a, b)]

**Description:** Finds x, y such that ax + by = gcd(a, b). This mathematical template provides an efficient implementation for Extended Euclidean Algorithm. It is commonly used in Advanced Number Theory problems.

**Usage:**
```cpp
ExtendedEuclid_Algorithm solver;
auto result = solver.extended_euclid(...);
```

**Time Complexity:** O(log(min(a, b)))
**Space Complexity:** O(1)

```cpp
#include "../../../core.h"


struct ExtendedEuclid_Algorithm {

    using ll = int;
    ll extended_euclid(ll a, ll b, ll &x, ll &y) {
      if (b == 0) {
        x = 1; y = 0;
        return a;
      }
      ll x1, y1;
      ll d = extended_euclid(b, a % b, x1, y1);
      x = y1;
      y = x1 - y1 * (a / b);
      return d;
    }
    ll inverse(ll a, ll m) {
      ll x, y;
      ll g = extended_euclid(a, m, x, y);
      if (g != 1) return -1;
      return (x % m + m) % m;
    }
};

```

## CRT

**Topic:** Chinese Remainder Theorem - [Solves system of linear congruences]

**Description:** Solves system of linear congruences. This mathematical template provides an efficient implementation for Chinese Remainder Theorem. It is commonly used in Advanced Number Theory problems.

**Usage:**
```cpp
CRT_Algorithm solver;
auto result = solver.extended_euclid(...);
```

**Time Complexity:** O(log(min(m1, m2)))
**Space Complexity:** O(1)

```cpp
#include "../../../core.h"


struct CRT_Algorithm {

    using T = __int128;
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
};
```

## LinearCongruenceEquation

**Topic:** Linear Congruence Equation - [Solves ax = b mod m]

**Description:** * Description: Solves ax = b mod m. * This mathematical template provides an efficient implementation for Linear Congruence Equation. * It is commonly used in Advanced Number Theory problems. * * Usage: * LinearCongruenceEquation_Algorithm solver; * auto result = solver.extended_euclid(...);

**Time Complexity:** O(log m)
**Space Complexity:** O(1)

```cpp
#include "../../../core.h"


struct LinearCongruenceEquation_Algorithm {

    using ll = int;

    ll extended_euclid(ll a, ll b, ll &x, ll &y) {
      if (b == 0) {
        x = 1; y = 0;
        return a;
      }
      ll x1, y1;
      ll d = extended_euclid(b, a % b, x1, y1);
      x = y1;
      y = x1 - y1 * (a / b);
      return d;
    }
    ll inverse(ll a, ll m) {
      ll x, y;
      ll g = extended_euclid(a, m, x, y);
      if (g != 1) return -1;
      return (x % m + m) % m;
    }
    // ax = b (mod m)
    vector<ll> congruence_equation(ll a, ll b, ll m) {
      vector<ll> ret;
      ll g = gcd(a, m), x;
      if (b % g != 0) return ret;
      a /= g, b /= g;
      x = inverse(a, m / g);
      for (int k = 0; k < g; ++k) { // exactly g solutions
        ret.push_back((x * b + m / g * k) % m);
      }
      return ret;
    }
};
```

## MultiplicativeOrder

**Topic:** Multiplicative Order - [Finds smallest k such that a^k = 1 mod m]

**Description:** * Description: Finds smallest k such that a^k = 1 mod m. * This mathematical template provides an efficient implementation for Multiplicative Order. * It is commonly used in Advanced Number Theory problems. * * Usage: * MultiplicativeOrder_Algorithm solver; * auto result = solver.power(...);

**Time Complexity:** O(sqrt(M))
**Space Complexity:** O(1)

```cpp
#include "../../../core.h"


struct MultiplicativeOrder_Algorithm {

    using ll = int;
    ll power(ll n, ll k, const ll mod) {
      ll res = 1;
      while (k) {
        if (k & 1) res = __int128(res) * n % mod;
        n = __int128(n) * n % mod;
        k >>= 1;
      }
      return res;
    }
    ll totient(ll n) {
      ll ans = n;
      for (ll i = 2; i * i <= n; i++) {
        if (n % i == 0) {
          while (n % i == 0) n /= i;
          ans = ans / i * (i - 1);
        }
      }
      if (n > 1) ans = ans / n * (n - 1);
      return ans;
    }
    // returns the minimum positive k s.t. a^ k = 1 modulo mod. On failure, returns -1
    // we just have to check the divisors of phi(mod) as candidates of k (Lagranges Theorem)
    // which can still be optimized further. Check: https://cp-algorithms.com/algebra/primitive-root.html#toc-tgt-3
    // it always exists if a and mod are coprime
    // O((log(mod)^2)) + sqrt(mod) for calculating totient
    // it can still be optimized. Check: https://brilliant.org/wiki/carmichaels-lambda-function/
    ll multiplicative_order(ll a, ll mod) {
      if (__gcd(a, mod) != 1) return -1;
      ll m = totient(mod), p = m;
      ll ans = 2e18;
      if (power(a, p, mod) == 1) ans = p;
      vector<ll> fac;
      for (ll i = 2 ; i * i <= m; i++) {
        if (m % i == 0) {
          while(m % i == 0) m /= i, fac.push_back(i);
        }
      }
      if (m > 1) fac.push_back(m);
      for (auto x: fac) {
        if (power(a, p / x, mod) == 1) p /= x, ans = p;
      }
      assert(ans != 2e18);
      return ans;
    }
};
```

## NumberofDistinctKthPowersModulon

**Topic:** Distinct Kth Powers - [Counts distinct x^k mod n]

**Description:** * Description: Counts distinct x^k mod n. * This mathematical template provides an efficient implementation for Distinct Kth Powers. * It is commonly used in Advanced Number Theory problems. * * Usage: * NumberofDistinctKthPowersModulon_Algorithm solver; * auto result = solver.power(...);

**Time Complexity:** O(log N)
**Space Complexity:** O(1)

```cpp
#include "../../../core.h"


struct NumberofDistinctKthPowersModulon_Algorithm {

    using ll = int;
    ll power(ll n, ll k) {
      ll ans = 1;
      while (k--) {
        ans *= n;
      }
      return ans;
    }
    // returns the number of distinct values of (a^k % p^cnt) over all integers a (p is prime)
    // can be optimized by precalculating powers
    // current complexity: O(cnt * cnt)
    ll f(ll p, ll cnt, ll k) {
      if (cnt <= 0 or k == 0) return 1;
      if (p == 2) {
        if (cnt == 1) return 2;
        ll u = power(2, cnt - 2) / __gcd(k, power(2, cnt - 2));
        if (k % 2) u *= 2;
        return u + f(2, cnt - k, k);
      }
      ll phi = power(p, cnt) - power(p, cnt - 1);
      ll u = phi / __gcd(k, phi);
      return u + f(p, cnt - k, k);
    }
    // returns the number of distinct values of (a^k % n) over all integers a
    ll yo(ll k, ll n) {
      ll ans = 1;
      for (ll i = 2; i * i <= n; i++) {
        if (n % i == 0) {
          int cnt = 0;
          while (n % i == 0) {
            cnt++;
            n /= i;
          }
          ans *= f(i, cnt, k);
        }
      }
      if (n > 1) ans *= f(n, 1, k);
      return ans;
    }
};
```

## Numberofax%pinaRange

**Topic:** Count ax%p in Range - [Counts x such that ax mod p in [L, R]]

**Description:** * Description: Counts x such that ax mod p in [L, R. * This mathematical template provides an efficient implementation for Count ax%p in Range. * It is commonly used in Advanced Number Theory problems. * * Usage: * Numberofax%pinaRange_Algorithm solver; * auto result = solver.lattice_cnt(...);

**Time Complexity:** O(log p)
**Space Complexity:** O(1)

```cpp
#include "../../../core.h"


struct Numberofax%pinaRange_Algorithm {

    using ll = int;

    // number of integer solutions to ax + by <= c s.t. x, y >= 0
    ll lattice_cnt(ll a, ll b, ll c) {
      assert(a >= 0 && b >= 0);
      if (c < 0) return 0;
      if (a == 0 or b == 0) {
        // infinite solutions
        assert(0);
        return -1;
      }
      assert(a > 0 && b > 0);
      if (a > b) swap(a, b);
      ll ans = 0;
      while (c >= 0) {
        ll k = b / a;
        ll l = b % a;
        ll f = c / b;
        ll e = c % b / a;
        ll g = c % b % a;
        ans += (f + 1) * (e + 1) + (f + 1) * f / 2 * k;
        c = f * l - a + g;
        b = a;
        a = l;
      }
      return ans;
    }
    // returns the number of 0 <= (a * x % m) <= c s.t. 0 <= x <= n
    ll mod_count(ll a, ll m, ll c, ll n) {
      ++c; ++n;
      assert(m > 0);
      if (n == 0) return 0;
      a %= m; if (a < 0) a += m;
      ll extra_c = c / m; c %= m;
      if (c < 0) extra_c--, c += m;
      assert(0 <= c && c < m);
      ll ans = extra_c * n;
      ll extra_n = n / m; n %= m;
      if (n < 0) extra_n--, n += m;
      assert(0 <= n && n < m);
      if (extra_n) {
        ans += extra_n * (lattice_cnt(m, a + m, (a + m) * (m - 1)) - lattice_cnt(m, a + m, (a + m) * (m - 1) - c));
      }
      if (n) {
        ans += lattice_cnt(m, a + m, (a + m) * (n - 1)) - lattice_cnt(m, a + m, (a + m) * (n - 1) - c);
      }
      return ans;
    }
    // returns the count of nlo <= x <= nhi s.t. clo <= (a * x % m) <= chi
    ll mod_count_range(ll a, ll m, ll clo, ll chi, ll nlo, ll nhi) {
      return mod_count(a, m, chi, nhi) - mod_count(a, m, chi, nlo - 1) - mod_count(a, m, clo - 1, nhi) + mod_count(a, m, clo - 1, nlo - 1);
    }
};
```

## PowerTower

**Topic:** Power Tower - [Computes a^b^c... mod m using generalized Euler theorem]

**Description:** * Description: Computes a^b^c... mod m using generalized Euler theorem. * This mathematical template provides an efficient implementation for Power Tower. * It is commonly used in Advanced Number Theory problems. * * Usage: * PowerTower_Algorithm solver; * auto result = solver.phi(...);

**Time Complexity:** O(log M)
**Space Complexity:** O(1)

```cpp
#include "../../../core.h"


struct PowerTower_Algorithm {

    const int N = 1e5 + 9;
    using ll = int;

    map<ll, ll> mp;
    ll phi(ll n) {
      if (mp.count(n)) return mp[n];
      ll ans = n, m = n;
      for (ll i = 2; i * i <= m; i++) {
        if (m % i == 0) {
          while (m % i == 0) m /= i;
          ans = ans / i * (i - 1);
        }
      }
      if (m > 1) ans = ans / m * (m - 1);
      return mp[n] = ans;
    }
    inline ll MOD(ll x, ll m) {
      if (x < m) return x;
      return x % m + m;
    }
    ll power(ll n, ll k, ll mod) {
      ll ans = MOD(1, mod);
      while (k) {
        if (k & 1) ans = MOD(ans * n, mod);
        n = MOD(n * n, mod);
        k >>= 1;
      }
      return ans;
    }
    int a[N];
    // if x >= log2(m), then a^x = a^(MOD(x, phi(m))) % m
    ll yo(ll l, ll r, ll m) {
      if (l == r) return MOD(a[l], m);
      if (m == 1) return 1;
      return power(a[l], yo(l + 1, r, phi(m)), m);
    }
};
```

## PrimitiveRoot

**Topic:** Primitive Root - [Finds generator of multiplicative group modulo n]

**Description:** * Description: Finds generator of multiplicative group modulo n. * This mathematical template provides an efficient implementation for Primitive Root. * It is commonly used in Advanced Number Theory problems. * * Usage: * PrimitiveRoot_Algorithm solver; * auto result = solver.totient(...);

**Time Complexity:** O(sqrt(P))
**Space Complexity:** O(1)

```cpp
#include "../../../core.h"


struct PrimitiveRoot_Algorithm {

    int totient(int n) {
      int ans = n;
      for (int i = 2; i * i <= n; i++) {
        if (n % i == 0) {
          while (n % i == 0) n /= i;
          ans = ans / i * (i - 1);
        }
      }
      if (n > 1) ans = ans / n * (n - 1);
      return ans;
    }
    int power(int a, int b, int m) {
      int res = 1;
      while (b > 0) {
        if (b & 1) res = 1LL * res * a % m;
        a = 1LL * a * a % m;
        b >>= 1;
      }
      return res;
    }
    // g is a primitive root modulo p if and only if for any integer a such that
    // gcd(a, p) = 1, there exists an integer k such that: g^k = a(mod p).
    // primitive root modulo n exists iff n = 1, 2, 4 or n = p^k or 2 * p^k for some odd prime p
    int primitive_root(int p) {
      // first check if primitive root exists or not. I have omitted this part here
      vector<int> fact;
      int phi = totient(p),  n = phi;
      for (int i = 2; i * i <= n; ++i) {
        if (n % i == 0) {
          fact.push_back(i);
          while (n % i == 0) n /= i;
        }
      }
      if (n > 1) fact.push_back(n);
      for (int res = 2; res <= p; ++res) { // this loop will run at most (logp ^ 6) times i.e. until a root is found
        bool ok = true;
        // check if this is a primitive root modulo p
        for (size_t i = 0; i < sz(fact) && ok; ++i)
          ok &= power(res, phi / fact[i], p) != 1;
        if (ok)  return res;
      }
      return -1;
    }
};
```

## SmallestNonnegativeIntegerxs.t.l_=ax%p_=r

**Topic:** Smallest x for ax%p in [L, R] - [Finds smallest x satisfying bounding box]

**Description:** * Description: Finds smallest x satisfying bounding box. * This mathematical template provides an efficient implementation for Smallest x for ax%p in [L, R]. * It is commonly used in Advanced Number Theory problems. * * Usage: * SmallestNonnegativeIntegerxs.t.l_=ax%p_=r_Algorithm solver; * auto result = solver.extended_euclid(...);

**Time Complexity:** O(log P)
**Space Complexity:** O(1)

```cpp
#include "../../../core.h"


struct SmallestNonnegativeIntegerxs.t.l_=ax%p_=r_Algorithm {

    const int N = 1e9, mod = 1e9 + 7;

    using T = __int128;
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

    // find z such that z % x = a, z % y = b.
    // here, z is unique modulo M = lcm(x,y).
    // returns (z, M). on failure, M = -1.
    pair<T, T> CRT(T x, T a, T y, T b) {
      T s, t;
      T d = extended_euclid(x, y, s, t);
      if (a % d != b % d) return make_pair(0, -1);
      T m = x * y;
      s %= m; s = (s + m) % m;
      t %= m; t = (t + m) % m;
      return make_pair((s * b % m * x % m + t * a % m * y % m) %  m / d, m / d);
    }

    T cdiv(T x, T y) { return (x + y - 1) / y; }
    // returns the smallest non-negative integer x s.t l <= a * x mod p <= r
    // IMPORTANT : 0 <= a < p, 0 <= l <= r < p, p is not necessarily prime
    // Complexity: O(log (p))
    int f(int p, int a, int l, int r) {
      if (a == 0) return l == 0 ? 0 : -1;
      int c = cdiv((T)l, (T)a);
      if (a * c <= r) return c;
      int b = p % a; // p = k * a + b, l <= a(x - k * y) - b * y <= r
      // => -r <= b * y % a <= -l
      auto y = f(a, b, a - r % a, a - l % a);
      return y == -1 ? y : cdiv(l + (T)b * y, a) + p / a * y;
    }

    const int M = 1LL * mod * (mod + 2);
};
```

## TonelliShanksAlgorithm

**Topic:** Tonelli-Shanks - [Finds square roots modulo a prime]

**Description:** * Description: Finds square roots modulo a prime. * This mathematical template provides an efficient implementation for Tonelli-Shanks. * It is commonly used in Advanced Number Theory problems. * * Usage: * TonelliShanksAlgorithm_Algorithm solver; * auto result = solver.power(...);

**Time Complexity:** O(log^2 P)
**Space Complexity:** O(1)

```cpp
#include "../../../core.h"


struct TonelliShanksAlgorithm_Algorithm {

    const int N = 3e5 + 9;

    int power(int n, int k, const int mod) {
      int ans = 1 % mod; n %= mod; if (n < 0) n += mod;
      while (k) {
        if (k & 1) ans = (int) ans * n % mod;
        n = (int) n * n % mod;
        k >>= 1;
      }
      return ans;
    }
    // find sqrt(a) % p, i.e. find any x such that x^2 = a (mod p)
    // if a solution exist, then if a == 0 or p == 2, there are 1 solution, otherwise, there are exactly 2 solutions (x and p - x)
    // p is prime
    // complexity: O(log^2 p) worst case, O(log p) on average
    int SQRT(int a, int p) {
      a %= p; if (a < 0) a += p;
      if (a == 0) return 0;
      if (power(a, (p - 1) / 2, p) != 1) return -1; // solution does not exist
      if (p % 4 == 3) return power(a, (p + 1) / 4, p);
      int s = p - 1, n = 2;
      int r = 0, m;
      while (s % 2 == 0) ++r, s /= 2;
      // find a non-square mod p
      while (power(n, (p - 1) / 2, p) != p - 1) ++n;
      int x = power(a, (s + 1) / 2, p);
      int b = power(a, s, p), g = power(n, s, p);
      for (;; r = m) {
        int t = b;
        for (m = 0; m < r && t != 1; ++m) t = 1LL * t * t % p;
        if (m == 0) return x;
        int gs = power(g, 1LL << (r - m - 1), p);
        g = 1LL * gs * gs % p;
        x = 1LL * x * gs % p;
        b = 1LL * b * g % p;
      }
    }
};
```

