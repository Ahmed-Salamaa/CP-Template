/* Topic: XOR Hashing
 * Description: XOR Hashing (Zobrist Hashing) assigns random bitstrings to elements and XORs
 *   them to represent sets compactly. Used to efficiently check set equality, multi-
 *   set equivalence, or find zero-sum subsequences.
 * Usage: map<int, int> h; h[x] = rng();
 */
#include "../../core.h"

// Time Complexity: O(N log N)
// Space Complexity: O(N)
struct XORHashing {
    map<int, int> hash_vals;
    mt19937_64 gen;

    XORHashing() : gen(chrono::steady_clock::now().time_since_epoch().count()) {}

    inline int get(int x) {
        if (!hash_vals.count(x)) hash_vals[x] = gen();
        return hash_vals[x];
    }
};
