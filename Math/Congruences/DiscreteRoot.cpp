// Topic: Discrete Root - [Finds x such that x^k = a mod m]
#include "../../../core.h"

/*
 * Description: Finds solutions for x in x^k ≡ a (mod m) using primitive roots and discrete
 *   logarithms. Essential for modular arithmetic equations resembling k-th roots.
 * This mathematical template provides an efficient implementation for Discrete Root.
 * It is commonly used in Advanced Number Theory problems.
 *
 * Usage:
 * DiscreteRoot_Algorithm solver;
 * auto result = solver.power(...);
 */
// Time Complexity: O(sqrt(M))
// Space Complexity: O(sqrt(M))
struct DiscreteRoot_Algorithm {
    int power(int a, int b, int m) {
        int res = 1;
        while (b > 0) {
            if (b & 1) res = 1LL * res * a % m;
            a = 1LL * a * a % m;
            b >>= 1;
        }
        return res;
    }

    // p is prime
    int primitive_root(int p) {
        vector<int> fact;
        int phi = p - 1, n = phi;
        for (int i = 2; i * i <= n; ++i) {
            if (n % i == 0) {
                fact.push_back(i);
                while (n % i == 0) n /= i;
            }
        }
        if (n > 1) fact.push_back(n);
        for (int res = 2; res <= p; ++res) {  // this loop will run at most (logp ^ 6) times i.e. until a root is found
            bool ok = true;
            // check if this is a primitive root modulo p
            for (size_t i = 0; i < sz(fact) && ok; ++i) ok &= power(res, phi / fact[i], p) != 1;
            if (ok) return res;
        }
        return -1;
    }

    // returns any or all numbers x such that x ^ k = a (mod m)
    // existence: a = 0 is trivial, and if a > 0: a ^ (phi(m) / gcd(k, phi(m))) == 1 mod m
    // if solution exists, then number of solutions = gcd(k, phi(m)).
    // here m is prime, but it will work for any integer which has a primitive root
    int discrete_root(int k, int a, int m) {
        if (a == 0) return 1;
        int g = primitive_root(m);
        int phi = m - 1;  // m is prime
        // run baby step-giant step
        int sq = (int)sqrt(m + .0) + 1;
        vector<pair<int, int>> dec(sq);
        for (int i = 1; i <= sq; ++i) dec[i - 1] = make_pair(power(g, 1LL * i * sq % phi * k % phi, m), i);
        sort(all(dec));
        int any_ans = -1;
        for (int i = 0; i < sq; ++i) {
            int my = power(g, 1LL * i * k % phi, m) * 1LL * a % m;
            auto it = lower_bound(all(dec), make_pair(my, 0));
            if (it != dec.end() && it->first == my) {
                any_ans = it->second * sq - i;
                break;
            }
        }
        if (any_ans == -1) return -1;  // no solution
        // for any answer
        int delta = (m - 1) / __gcd(k, m - 1);
        return power(g, any_ans % delta, m);
        // // for all possible answers
        // int delta = (m - 1) / __gcd(k, m - 1);
        // vector<int> ans;
        // for (int cur = any_ans % delta; cur < m-1; cur += delta) ans.push_back(power(g, cur, m));
        // sort(all(ans));
        // // assert(sz(ans) == __gcd(k, m - 1))
        // return ans;
    }
};
