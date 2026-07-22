## LinearSieveforMultiplicativeFunctions

**Topic:** Linear Sieve - [Computes multiplicative functions in linear time]

**Description:** Computes multiplicative functions in linear time. This mathematical template provides an efficient implementation for Linear Sieve. It is commonly used in Advanced Number Theory problems.

**Usage:**
```cpp
LinearSieveforMultiplicativeFunctions_Algorithm solver;
auto result = solver.sieve(...);
```

**Time Complexity:** O(N)
**Space Complexity:** O(N)

```cpp
#include "../../../core.h"


struct LinearSieveforMultiplicativeFunctions_Algorithm {

    const int N = 1e7 + 9;

    // f is multiplicative with f(p^k) = k
    int spf[N], f[N], cnt[N]; // cnt[i] = power of spf[i] in i
    vector<int> primes;
    void sieve() {
      f[1] = 1;
      for(int i = 2; i < N; i++) {
        if (spf[i] == 0) { // i is prime
          spf[i] = i, primes.push_back(i);
          f[i] = 1; cnt[i] = 1;
        }
        int sz = sz(primes);
        for (int j = 0; j < sz && i * primes[j] < N && primes[j] <= spf[i]; j++) {
          int p = i * primes[j];
          spf[p] = primes[j];
          if (primes[j] == spf[i]) { // primes[j] divides i
            f[p] = f[i] / cnt[i] * (cnt[i] + 1); // f(i * primes[j]) = f(i / (primes[j]^cnt[i])) * f(primes[j]^(cnt[i] + 1))
            cnt[p] = cnt[i] + 1;
          }
          else { // primes[j] does not divide i
            f[p] = f[i] * f[primes[j]];
            cnt[p] = 1;
          }
        }
      }
    }
};
```

## MobiusFunction

**Topic:** Mobius Function - [Computes Mobius function using linear sieve]

**Description:** Computes Mobius function using linear sieve. This mathematical template provides an efficient implementation for Mobius Function. It is commonly used in Advanced Number Theory problems.

**Usage:**
```cpp
MobiusFunction_Algorithm solver;
auto result = solver.mobius(...);
```

**Time Complexity:** O(N)
**Space Complexity:** O(N)

```cpp
#include "../../../core.h"


struct MobiusFunction_Algorithm {

    const int N = 5e5 + 9;

    int mob[N];
    void mobius() {
      mob[1] = 1;
      for (int i = 2; i < N; i++){
        mob[i]--;
        for (int j = i + i; j < N; j += i) {
          mob[j] -= mob[i];
        }
      }
    }
    bool vis[N];
    vector<int> d[N];
    int mul[N];
    void add(int x, int k) {
      for (auto y: d[x]) {
        mul[y] += k;
      }
    }
    int query(int x) {
      int ans = 0;
      for (auto y: d[x]) {
        ans += mul[y] * mob[y];
      }
      return ans;
    }
    int a[N];
};
```

## DirichletConvolution

**Topic:** Dirichlet Convolution - [Computes Dirichlet convolution of multiplicative functions]

**Description:** * Description: Computes Dirichlet convolution of multiplicative functions. * NOTE: This template uses standard integers with manual modulo arithmetic for performance and simplicity. If the problem involves extremely complex chaining of operations or you want to avoid manual modulo bugs, you can easily swap the `int` types back to a `modint` template. * This mathematical template provides an efficient implementation for Dirichlet Convolution. * It is commonly used in Advanced Number Theory problems. * * Usage: * DirichletConvolution_Algorithm solver; * auto result = solver.Dirichlet::solve(...);

**Time Complexity:** O(N log N)
**Space Complexity:** O(N)

```cpp
#include <ext/pb_ds/assoc_container.hpp>
#include "../../../core.h"
using namespace __gnu_pbds;


struct DirichletConvolution_Algorithm {
    static const int N = 3e5 + 9, mod = 998244353;

    /**
    Prefix sum of multiplicative functions :
      p_f : the prefix sum of f(x) (1 <= x <= T).
      p_g : the prefix sum of g(x) (0 <= x <= N).
      p_c : the prefix sum of f * g(x) (0 <= x <= N).
      T : the threshold, generally should be n ^ (2 / 3). for n = 1e9, T = 1e6
    Magic Functions for different f(x)
    For f(x) = phi(x): Id(x) = phi * I(x) i.e. p_c = prefix sum of Id(x), p_g = prefix sum of I(x). Here Id(n) = n, I(n) = 1
    For f(x) = mu(x): e(x) = mu * I(x). Here e(x) = x == 1 ? 1 : 0
    Complexity: O(n^(2/3))
    **/
    struct Dirichlet {
      //solution for f(x) = phi(x)
      static const int T = 1e7 + 9;
      long long phi[T];
      gp_hash_table<long long, int> mp;
      int dp[T], inv;
      int sz, spf[T], prime[T];
      void init() {
        memset(spf, 0, sizeof spf);
        phi[1] = 1; sz = 0;
        for (int i = 2; i < T; i++) {
          if (spf[i] == 0) phi[i] = i - 1, spf[i] = i, prime[sz++] = i;
          for (int j = 0; j < sz && i * prime[j] < T && prime[j] <= spf[i]; j++) {
            spf[i * prime[j]] = prime[j];
            if (i % prime[j] == 0) phi[i * prime[j]] = phi[i] * prime[j];
            else phi[i * prime[j]] = phi[i] * (prime[j] - 1);
          }
        }
        dp[0] = 0;
        for(int i = 1; i < T; i++) dp[i] = (dp[i - 1] + phi[i]) % mod;
        inv = 1; // g(1)
      }
      int p_c(long long n) {
        if (n % 2 == 0) return (n / 2) % mod * ((n + 1) % mod) % mod;
        return ((n + 1) / 2) % mod * (n % mod) % mod;
      }
      int p_g(long long n) {
        return n % mod;
      }
      int solve (long long x) {
        if (x < T) return dp[x];
        if (mp.find(x) != mp.end()) return mp[x];
        int ans = 0;
        for (long long i = 2, last; i <= x; i = last + 1) {
          last = x / (x / i);
          ans = (ans + 1LL * solve(x / i) % mod * ((p_g(last) - p_g(i - 1) + mod) % mod)) % mod;
        }
        ans = (p_c(x) - ans % mod + mod) % mod;
        return mp[x] = ans;
      }
    };
};
```

