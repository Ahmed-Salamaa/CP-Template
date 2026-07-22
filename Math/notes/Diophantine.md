## NumberofNonnegativeIntegerSolutionstoax+by =c

**Topic:** Count Diophantine Solutions - [Counts non-negative solutions to ax+by <= c]

**Description:** Counts non-negative solutions to ax+by <= c. This mathematical template provides an efficient implementation for Count Diophantine Solutions. It is commonly used in Advanced Number Theory problems.

**Usage:**
```cpp
NumberofNonnegativeIntegerSolutionstoax+by_=c_Algorithm solver;
auto result = solver.lattice_cnt(...);
```

**Time Complexity:** O(log(min(a, b)))
**Space Complexity:** O(1)

```cpp
#include "../../../core.h"


struct NumberofNonnegativeIntegerSolutionstoax+by_=c_Algorithm {

    using ll = int;

    // number of integer solutions to ax + by <= c s.t. x, y >= 0
    // number of nonnegative integer lattice points under or on the line ax + by = c
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
};
```

## LinearDiophantineEquationwithNonnegativeSolutions

**Topic:** Linear Diophantine Non-negative - [Solves ax + by = c with non-negative constraints]

**Description:** * Description: Solves ax + by = c with non-negative constraints. * This mathematical template provides an efficient implementation for Linear Diophantine Non-negative. * It is commonly used in Advanced Number Theory problems. * * Usage: * LinearDiophantineEquationwithNonnegativeSolutions_Algorithm solver; * auto result = solver.LDE(...);

**Time Complexity:** O(log(min(a, b)))
**Space Complexity:** O(1)

```cpp
#include "../../../core.h"


struct LinearDiophantineEquationwithNonnegativeSolutions_Algorithm {

    using ll = int;
    const ll inf = 1e18;

    struct LDE { // sum(a[i] * x[i]) = k, x[i] >= 0
      int n, x;
      vector<ll> d;
      LDE() {}
      LDE(vector<int> a) { // O(min(a[i]) * n * log(min(a[i])))
        n = sz(a);
        priority_queue<pair<ll, int>, vector<pair<ll, int>>, greater<pair<ll, int>>> q;
        x = *min_element(all(a));
        assert(x > 0);
        d.resize(x);
        fill(all(d), inf);
        d[0] = 0; q.push(pair<ll, int>(0, 0));
        while (!q.empty()) {
          pair<ll, int> nw = q.top(); q.pop();
          int u = nw.second;
          ll val = nw.first;
          if (d[u] != val) continue;
          for (int i = 0; i < n; ++ i) {
            ll tmp = val + a[i];
            int v = tmp % x;
            if (d[v] > tmp) {
              d[v] = tmp;
              q.push(pair<ll, int>(tmp, v));
            }
          }
        }
      }
      bool can(ll k) { // if a solution exists
        return d[k % x] <= k;
      }
      ll count(ll l, ll r) { // count of l <= k <= r s.t. solution for k exists
        ll ans = 0;
        for (int i = 0; i < x; i++) { // d[i], d[i] + x, d[i] + 2 * x, ... are achievable
          ans += d[i] <= r ? (r - d[i]) / x + 1 : 0;
          ans -= d[i] <= l - 1 ? (l - 1 - d[i]) / x + 1: 0;
        }
        return ans;
      }
    };
};
```

## LinearDiophantineWithNUnknownsandTwoEquations

**Topic:** Linear Diophantine N Unknowns 2 Equations - [Solves a system of two Diophantine equations]

**Description:** * Description: Solves a system of two Diophantine equations. * This mathematical template provides an efficient implementation for Linear Diophantine N Unknowns 2 Equations. * It is commonly used in Advanced Number Theory problems. * * Usage: * LinearDiophantineWithNUnknownsandTwoEquations_Algorithm solver; * auto result = solver.sum(...);

**Time Complexity:** O(N log(min(a, b)))
**Space Complexity:** O(N)

```cpp
#include "../../../core.h"


struct LinearDiophantineWithNUnknownsandTwoEquations_Algorithm {

    using ll = int;

    /**
    given are a[i], b[i], p and q:
    sum(x[i] * a[i]) = p --- (i)
    sum(x[i] * b[i]) = q --- (ii)
    x[i]s can only be integers.

    does a solution exist?
    **/
    ll extended_euclid(ll a, ll b, ll& x, ll& y) {
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
    ll a, b, coeff; // possible points that can be generated are of the form (a * m, b * m + k * coeff)
    // add (a[i], b[i])
    void add(ll c, ll d) {
      if (c == 0 and d == 0) return;
      if (a == 0 and b == 0) {
        a = c; b = d;
        return;
      }
      ll x0, y0;
      // a * x0 + c * y0 = m * gcd(a, c)
      // all solutions are of the form (x0 + k * (c / g), y0 - k * (a / g))
      ll g = extended_euclid(a, c, x0, y0);
      // replace (x, y) to the equation b * x + d * y = z
      ll tmp_m = b * x0 + d * y0;
      ll tmp_coeff = abs(b * (c / g) - d * (a / g));
      coeff = __gcd(coeff, tmp_coeff);
      a = g;
      b = tmp_m % coeff;
    }
    // check if solution exists for some (p, q)
    bool can(ll x, ll y) {
      if (x == 0 and y == 0) return true;
      if (coeff == 0) {
        if (a == 0 or x % a) return false;
        if (b == 0 or y % b) return false;
        return x / a == y / b;
      }
      if (a == 0 or x % a) return false;
      return (y - (x / a) * b) % coeff == 0;
    }
};
```

## NumberofSolutionstoaBasicLinearAlgebraicEquation

**Topic:** Count Solutions to Linear Equation - [Stars and bars to count solutions]

**Description:** * Description: Stars and bars to count solutions. * This mathematical template provides an efficient implementation for Count Solutions to Linear Equation. * It is commonly used in Advanced Number Theory problems. * * NOTE: This template uses standard integers with manual modulo arithmetic for performance and simplicity. If the problem involves extremely complex chaining of operations or you want to avoid manual modulo bugs, you can easily swap the `int` types back to a `modint` template. * * Usage: * NumberofSolutionstoaBasicLinearAlgebraicEquation_Algorithm solver; * auto result = solver.yo(...);

**Time Complexity:** O(N)
**Space Complexity:** O(1)

```cpp
#include "../../../core.h"


struct NumberofSolutionstoaBasicLinearAlgebraicEquation_Algorithm {

    static const int N = 1e5 + 9, mod = 1e9 + 7;

    struct Combi{
      int n; vector<int> facts, finvs, invs;
      Combi(int _n): n(_n), facts(_n), finvs(_n), invs(_n){
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
      inline int ncr(int n, int k) { return n < k ? 0 : 1LL * facts[n] * finvs[k] % mod * finvs[n-k] % mod; }
    };
    Combi C = Combi(N);

    // returns the number of solutions to the equation
    // x_1 + x_2 + ... + x_n = s and 0 <= l <= x_i <= r
    int yo(int n, int s, int l, int r) {
      if (s < l * n) return 0;
      s -= l * n;
      r -= l;
      int ans = 0;
      for (int k = 0; k <= n; k++) {
        int cur = (1LL * C.ncr(s - k - k * r + n - 1 + 1, n - 1 + 1) * C.ncr(n, k)) % mod;
        if (k & 1) ans = (ans - cur + mod) % mod;
        else ans = (ans + cur) % mod;
      }
      return ans;
    }
};
```

