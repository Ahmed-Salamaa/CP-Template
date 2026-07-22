/* Topic: Double String Hash
 * Description: Double Hash is a string hashing technique that computes two separate polynomial
 *   hashes with different moduli or bases to minimize the probability of collisions
 *   when comparing strings.
 * Usage: DoubleHash dh(s); dh.query(l, r); dh.merge(l1, r1, l2, r2);
 */
#include "../../core.h"

// Time Complexity: O(N) build, O(1) query
// Space Complexity: O(N)
struct DoubleHash {
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

    static int get_B2() {
        static int B2 = get_rand_base();
        return B2;
    }

    static int get_M1() {
        static int M1 = get_rand_prime();
        return M1;
    }

    static int get_M2() {
        static int M2 = get_rand_prime();
        while (M2 == get_M1()) M2 = get_rand_prime();
        return M2;
    }

    int M1, M2;
    vector<int> h1, h2, p1, p2;

    DoubleHash(string s) : h1(sz(s) + 1), h2(sz(s) + 1), p1(sz(s) + 1, 1), p2(sz(s) + 1, 1) {
        int B1 = get_B1(), B2 = get_B2();
        M1 = get_M1();
        M2 = get_M2();
        for (int i = 0; i < sz(s); i++) {
            h1[i + 1] = (h1[i] * B1 + s[i]) % M1;
            h2[i + 1] = (h2[i] * B2 + s[i]) % M2;
            p1[i + 1] = (p1[i] * B1) % M1;
            p2[i + 1] = (p2[i] * B2) % M2;
        }
    }

    pair<int, int> query(int l, int r) {  // 0-indexed, inclusive
        int v1 = (h1[r + 1] - (h1[l] * p1[r - l + 1]) % M1 + M1) % M1;
        int v2 = (h2[r + 1] - (h2[l] * p2[r - l + 1]) % M2 + M2) % M2;
        return {v1, v2};
    }

    pair<int, int> merge(pair<int, int> h_left, pair<int, int> h_right, int len_right) {
        int v1 = (h_left.first * p1[len_right] % M1 + h_right.first) % M1;
        int v2 = (h_left.second * p2[len_right] % M2 + h_right.second) % M2;
        return {v1, v2};
    }

    pair<int, int> merge(int l1, int r1, int l2, int r2) { return merge(query(l1, r1), query(l2, r2), r2 - l2 + 1); }
};
