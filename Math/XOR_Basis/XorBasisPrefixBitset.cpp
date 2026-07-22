/*
 * Topic: Bitset XOR Basis Prefix
 * Description: Combines prefix XOR basis queries with bitset optimizations for extremely large
 *   bit lengths. Useful for subset queries over large binary numbers bounded by
 *   spatial or temporal constraints in competitive programming.
 * number of bits N exceeds 64. Usage: XorBasisPrefixBitset<100> basis; bitset<100> b("10101"); basis.insert(b, 1); bool
 * has = basis.contains(b, 1, 1);
 */
#include "../../core.h"

// Time Complexity: O(N^2 / 64) per insertion/query
// Space Complexity: O(N^2 / 8)
template <size_t N>
struct XorBasisPrefixBitset {
    bitset<N> b[N];
    int pos[N];
    int rk;
    int last;

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

    XorBasisPrefixBitset() : rk(0), last(0) {
        for (int i = 0; i < N; i++) pos[i] = 0;
    }

    bool insert(bitset<N> x, int idx = 0) {
        if (idx == 0)
            idx = ++last;
        else
            last = max(last, idx);
        int p = idx;
        for (int i = (int)N - 1; i >= 0; i--) {
            if (!x[i]) continue;
            if (!b[i][i]) {
                b[i] = x;
                pos[i] = p;
                rk++;
                return true;
            }
            if (pos[i] < p) {
                swap(pos[i], p);
                swap(b[i], x);
            }
            x ^= b[i];
        }
        return false;
    }

    bool contains(bitset<N> x, int L = 1, int R = 0) const {
        if (R == 0) R = last;
        for (int i = (int)N - 1; i >= 0; i--) {
            if (!x[i]) continue;
            if (!b[i][i] || pos[i] < L || pos[i] > R) return false;
            x ^= b[i];
        }
        return x.none();
    }

    int ways_to_generate(bitset<N> x, int L = 1, int R = 0) const {
        if (R == 0) R = last;
        if (!contains(x, L, R)) return 0;
        int len = R - L + 1;
        int m = 0;
        for (int i = 0; i < (int)N; i++) {
            if (b[i][i] && pos[i] >= L && pos[i] <= R) m++;
        }
        return powmod(2, (int)(len - m));
    }
};
