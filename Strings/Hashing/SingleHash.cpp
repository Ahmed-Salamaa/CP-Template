/* Topic: Single String Hash
 * Description: Single String Hash uses a standard polynomial rolling hash for quickly checking
 *   string equivalence. Often implemented modulo a large prime to achieve high
 *   reliability in substring comparison tasks.
 * Usage: SingleHash sh(s); sh.query(l, r); sh.merge(l1, r1, l2, r2);
 */
#include "../../core.h"

// Time Complexity: O(N) build, O(1) query
// Space Complexity: O(N)
struct SingleHash {
    static int get_rand_base(int min_val = 313, int max_val = 1e9) {
        static mt19937 rng(chrono::steady_clock::now().time_since_epoch().count());
        return uniform_int_distribution<int>(min_val, max_val)(rng);
    }

    static int get_rand_prime() {
        static const int primes[] = {1000000007, 1000000009, 1000000021, 1000000033, 1000000087,
                                     1000000093, 1000000097, 1000000103, 1000000123, 1000000181};
        static mt19937 rng(chrono::steady_clock::now().time_since_epoch().count());
        return primes[uniform_int_distribution<int>(0, 9)(rng)];
    }

    static int get_B1() {
        static int B1 = get_rand_base();
        return B1;
    }

    static int get_M1() {
        static int M1 = get_rand_prime();
        return M1;
    }

    int M1;
    vector<int> h1, p1;

    SingleHash(string s) : h1(sz(s) + 1), p1(sz(s) + 1, 1) {
        int B1 = get_B1();
        M1 = get_M1();
        for (int i = 0; i < sz(s); i++) {
            h1[i + 1] = (h1[i] * B1 + s[i]) % M1;
            p1[i + 1] = (p1[i] * B1) % M1;
        }
    }

    int query(int l, int r) {  // 0-indexed, inclusive
        return (h1[r + 1] - (h1[l] * p1[r - l + 1]) % M1 + M1) % M1;
    }

    int merge(int h_left, int h_right, int len_right) { return (h_left * p1[len_right] % M1 + h_right) % M1; }

    int merge(int l1, int r1, int l2, int r2) { return merge(query(l1, r1), query(l2, r2), r2 - l2 + 1); }
};
