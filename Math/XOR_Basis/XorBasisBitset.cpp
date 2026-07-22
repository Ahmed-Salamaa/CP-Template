/*
 * Topic: Bitset XOR Basis
 * Description: An XOR basis implementation optimized with bitsets to handle vectors of very
 *   high dimensions (e.g., up to 1000 bits) over GF(2). This allows solving linear
 *   dependence and subset span problems efficiently for large binary strings.
 * both standard insertion and prefix-based range queries [L, R]. Usage: XorBasisBitset<100> basis; bitset<100>
 * b("10101"); basis.insert(b); bool has = basis.contains(b);
 */
#include "../../core.h"

// Time Complexity: O(N^2 / 64) per insertion/query
// Space Complexity: O(N^2 / 8)
template <size_t N>
struct XorBasisBitset {
    bitset<N> b[N];
    int rk;
    int inserted;

    int powmod(int base, int exp) const {
        if (MOD == 1) return 0;
        int res = 1 % MOD;
        base %= MOD;
        while (exp > 0) {
            if (exp & 1ULL) res = (int)((__uint128_t)res * base % MOD);
            base = (int)((__uint128_t)base * base % MOD);
            exp >>= 1;
        }
        return res;
    }

    XorBasisBitset() : rk(0), inserted(0) {}

    bool insert(bitset<N> curr) {
        for (int i = (int)N - 1; i >= 0; i--) {
            if (!curr[i]) continue;
            if (!b[i][i]) {
                b[i] = curr;
                rk++;
                return true;
            }
            curr ^= b[i];
        }
        return false;
    }

    void merge(const XorBasisBitset<N>& other) {
        inserted += other.inserted;
        for (int i = 0; i < (int)N; i++) {
            if (other.b[i][i]) { insert(other.b[i]); }
        }
    }

    bool contains(bitset<N> curr) const {
        for (int i = (int)N - 1; i >= 0; i--) {
            if (!curr[i]) continue;
            if (!b[i][i]) return false;
            curr ^= b[i];
        }
        return curr.none();
    }

    int ways_to_generate(bitset<N> x) const {
        if (!contains(x)) return 0;
        int free_bits = inserted - rk;
        if (free_bits < 0) free_bits = 0;
        return powmod(2, free_bits);
    }
};
