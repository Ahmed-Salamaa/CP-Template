// Topic: Prime Number System - [Converts numbers to and from prime basis representation]
#include "../../../core.h"

/*
 * Description: Prime Number System represents numbers uniquely via prime factorization or a
 *   positional system based on primes. It is useful for exploiting specific
 *   structural properties of integers.
 * This mathematical template provides an efficient implementation for Prime Number System.
 * It is commonly used in Advanced Number Theory problems.
 *
 * Usage:
 * PrimeNumberSystem_Algorithm solver;
 * auto result = solver.linear_sieve(...);
 */
// Time Complexity: O(log N)
// Space Complexity: O(1)
struct PrimeNumberSystem_Algorithm {
    const int N = 1e5 + 9, mod = 1e9 + 7;
    int spf[N];
    vector<int> primes;

    void linear_sieve() {
        for (int i = 2; i < N; i++) {
            if (spf[i] == 0) spf[i] = i, primes.push_back(i);
            int sz = sz(primes);
            for (int j = 0; j < sz && i * primes[j] < N && primes[j] <= spf[i]; j++) { spf[i * primes[j]] = primes[j]; }
        }
    }

    int power(int n, int k) {
        int ans = 1 % mod;
        n %= mod;
        if (n < 0) n += mod;
        while (k) {
            if (k & 1) ans = (int)ans * n % mod;
            n = (int)n * n % mod;
            k >>= 1;
        }
        return ans;
    }

    struct base {  // doesn't support zero
        map<int, int> mp;

        base() {}

        base(int n) {
            while (n > 1) {
                int k = spf[n], cnt = 0;
                while (n % k == 0) {
                    n /= k;
                    cnt++;
                }
                mp[k] = cnt;
            }
        }

        base(map<int, int> z) { mp = z; }

        int val() {
            int ans = 1;
            for (auto x : mp) {
                assert(x.second >= 0);
                ans = 1LL * ans * power(x.first, x.second) % mod;
            }
            return ans;
        }

        base pow(int k) {
            map<int, int> ans = mp;
            for (auto& x : ans) { x.second *= k; }
            return base(ans);
        }

        base sqrt() {
            map<int, int> ans = mp;
            for (auto& x : ans) {
                assert(x.second % 2 == 0);
                x.second /= 2;
            }
            return base(ans);
        }

        base operator*(base p) {
            map<int, int> ans = mp;
            for (auto x : p.mp) { ans[x.first] += x.second; }
            return base(ans);
        }

        base operator/(base p) {
            map<int, int> ans = mp;
            for (auto x : p.mp) { ans[x.first] -= x.second; }
            return base(ans);
        }

        base& operator*=(base p) { return *this = (*this) * p; }

        base& operator/=(base p) { return *this = (*this) / p; }

        void print() {
            bool f = 0;
            for (auto x : mp) {
                if (x.second) {
                    f = 1;
                    cout << "(" << x.first << ' ' << x.second << ") ";
                }
            }
            if (!f) { cout << 1; }
            cout << '\n';
        }
    };
};
