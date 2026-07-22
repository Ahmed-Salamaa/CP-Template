## Exponentiation

**Topic:** Exponentiation - Binary exponentiation and modular arithmetic exponentiation

**Time Complexity:** O(log(n))
**Space Complexity:** O(1)

```cpp
//
// Description:
// Computes base^power either directly or under a given modulo.
// Uses fast binary exponentiation algorithm that runs in O(log(power)) time.
//
// How to use it:
// int ans1 = Exponentiation::bin_pow(2, 10);
// int ans2 = Exponentiation::pow_mod(2, 10, 1000000007);
// int inv = Exponentiation::mod_inv(2, 1000000007);
#include "../../../core.h"

namespace Exponentiation {
            int bin_pow(int x, int n) {
        int res = 1;
        while (n) {
            if (n & 1) res = res * x;
            x = x * x;
            n >>= 1;
        }
        return res;
    }

            int pow_mod(int x, int n, int mod) {
        int res = 1;
        while (n) {
            if (n & 1) res = res * x % mod;
            x = x * x % mod;
            n >>= 1;
        }
        return res;
    }

    // Time Complexity: O(log(mod))
        int mod_inv(int x, int mod) {
        return pow_mod(x, mod - 2, mod);
    }
}
```

