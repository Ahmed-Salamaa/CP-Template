# From Primes_and_Divisors_Snippets.md
## MaximumCoprimeProduct

**Topic:** Maximum Coprime Product - [Finds maximum product of coprime integers]

**Description:** Finds maximum product of coprime integers. This mathematical template provides an efficient implementation for Maximum Coprime Product. It is commonly used in Advanced Number Theory problems.

**Usage:**
```cpp
MaximumCoprimeProduct_Algorithm solver;
auto result = solver.coprime(...);
```

**Time Complexity:** O(N log N)
**Space Complexity:** O(N)

```cpp
#include "../../../core.h"


struct MaximumCoprimeProduct_Algorithm {
    using ll = int;
    const int N = 1e5;

    // credit: mango_lassi
    int arr[N + 1];
    int u[N + 1];
    int cnt[N + 1];

    vector<int> d[N + 1];
    bool b[N + 1];

    bool coprime(int x) {
      int ret = 0;
      for (int i : d[x]) ret += cnt[i] * u[i];
      return ret;
    }

    void update(int x, int a) {
      for (int i : d[x]) cnt[i] += a;
    }
};

```

## SmallestNumberHavingExactlyKDivisors

**Topic:** Smallest with K Divisors - [Finds smallest number with exactly K divisors]

**Description:** * Description: Finds smallest number with exactly K divisors. * This mathematical template provides an efficient implementation for Smallest with K Divisors. * It is commonly used in Advanced Number Theory problems. * * Usage: * SmallestNumberHavingExactlyKDivisors_Algorithm solver; * auto result = solver.power(...);

**Time Complexity:** O(K)
**Space Complexity:** O(K)

```cpp
#include "../../../core.h"


struct SmallestNumberHavingExactlyKDivisors_Algorithm {

    const int N = 1e6 + 9, mod = 1e9 + 7;

    int power(int n, int k) {
      int ans = 1 % mod; n %= mod; if (n < 0) n += mod;
      while (k) {
        if (k & 1) ans = (int) ans * n % mod;
        n = (int) n * n % mod;
        k >>= 1;
      }
      return ans;
    }
    int spf[N];
    vector<int> primes;
    void sieve() {
      for(int i = 2; i < N; i++) {
        if (spf[i] == 0) spf[i] = i, primes.push_back(i);
        int sz = sz(primes);
        for (int j = 0; j < sz && i * primes[j] < N && primes[j] <= spf[i]; j++) {
          spf[i * primes[j]] = primes[j];
        }
      }
    }
    double lgp[N];
    vector<int> v;
    unordered_map<int, pair<double, int>> dp[100];
    pair<double, int> yo(int i, int n) { // it solves for odd divisors
      if (n == 1) {
        return {0, 1};
      }
      if (dp[i].find(n) != dp[i].end()) {
        return dp[i][n];
      }
      pair<double, int> ans = {1e50, 0};
      for (auto x: v) {
        if (x > n) break;
        if (n % x != 0) continue;
        auto z = lgp[i + 1] * (x - 1); // i for all divisors
        if (z > ans.first) {
          break;
        }
        auto cur = yo(i + 1, n / x);
        cur.first += z;
        cur.second = 1LL * cur.second * power(primes[i + 1], x - 1) % mod; // i for all divisors
        ans = min(ans, cur);
      }
      return dp[i][n] = ans;
    }
};
```

