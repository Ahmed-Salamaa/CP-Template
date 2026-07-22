# From Fibonacci_Snippets.md
## Fibonacci

**Topic:** Fast Fibonacci - [Computes Nth Fibonacci number in O(log N)]

**Description:** Computes Nth Fibonacci number in O(log N). This mathematical template provides an efficient implementation for Fast Fibonacci. It is commonly used in Advanced Number Theory problems.

**Usage:**
```cpp
Fibonacci_Algorithm solver;
auto result = solver.fib(...);
```

**Time Complexity:** O(log N)
**Space Complexity:** O(1)

```cpp
#include "../../../core.h"


struct Fibonacci_Algorithm {

    int fib(int n, int mod) {
      if (n <= 1) return n;
      int a = 0, b = 1;
      int i = 1ll << (63 - __builtin_clzll(n) - 1);
      for (; i; i >>= 1) {
        int na = (a *(int) a + b *(int) b) % mod;
        int nb = (2ll * a + b) * b % mod;
        a = na; b = nb;
        if (n & i) {
          int c = a + b; if (c >= mod) c -= mod;
          a = b; b = c;
        }
      }
      return b;
    }
};

```

## LCMofFibonacciNumbers

**Topic:** LCM of Fibonacci - [Computes LCM of a set of Fibonacci numbers]

**Description:** * Description: Computes LCM of a set of Fibonacci numbers. * This mathematical template provides an efficient implementation for LCM of Fibonacci. * It is commonly used in Advanced Number Theory problems. * * Usage: * LCMofFibonacciNumbers_Algorithm solver; * auto result = solver.power(...);

**Time Complexity:** O(N log N)
**Space Complexity:** O(N)

```cpp
#include "../../../core.h"


struct LCMofFibonacciNumbers_Algorithm {

    const int N = 3e5 + 9, mod= 1e9 + 7;

    int power(int n, int k) {
      int ans = 1 % mod; n %= mod; if (n < 0) n += mod;
      while (k) {
        if (k & 1) ans = (int) ans * n % mod;
        n = (int) n * n % mod;
        k >>= 1;
      }
      return ans;
    }
    int fib(int n) {
      assert (n >= 0);
      if (n <= 1) return n;
      int a = 0;
      int b = 1;
      int i = 1ll << (63 - __builtin_clzll(n) - 1);
      for (; i; i >>= 1) {
        int na = (a *(int) a + b *(int) b) % mod;
        int nb = (2ll * a + b) * b % mod;
        a = na;
        b = nb;
        if (n & i) {
          int c = a + b; if (c >= mod) c -= mod;
          a = b;
          b = c;
        }
      }
      return b;
    }
    map<vector<int>, int> dp;
    /**
    O((max number of divisors of a[i]) * n * log(max a[i])) but faster in practice
    lcm(a1, a2, ... an)
    = lcm(lcm(a1, ..., a[n-1]), an)
    = lcm(a1, ..., a[n-1]) * an / gcd(lcm(a1, ..., a[n-1]), an)
    = lcm(a1, ..., a[n-1]) * an / lcm(gcd(a1, an), ... gcd(a[n-1], an))
    **/
    int yo(vector<int> a) {
      sort(a.rbegin(), a.rend());
      while (!a.empty() && a.back() <= 2) a.pop_back();
      a.resize(unique(all(a)) - a.begin());
      if (a.empty()) return 1;
      if (sz(a) == 1) return fib(a[0]);
      if (dp.count(a)) return dp[a];
      vector<int> b(all(a) - 1);
      int res = yo(b);
      for (int i = 0; i < sz(b); ++i) b[i] = __gcd(b[i], a.back());
      res = res * fib(a.back()) % mod * power(yo(b), mod - 2) % mod;
      dp[a] = res;
      return res;
    }
};
```

