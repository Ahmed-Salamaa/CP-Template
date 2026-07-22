/*
 * Topic: Prefix XOR Basis
 * Description: A linear algebra data structure maintaining a dynamic basis of vector spaces
 *   over GF(2) with prefix ranges. It enables rapid answering of maximum XOR queries
 *   on arbitrary subsegments or prefix elements.
 * prefixes. Features MSB optimization for performance and handles k-th smallest/greatest queries correctly. Usage:
 * XorBasisPrefix<64> pref;
 * pref.insert(5, 1); // Insert 5 at index 1
 * int mx = pref.max_xor(0, 1, 1); // Max XOR in range [1, 1]
 */
#include "../../core.h"

// Time Complexity: O(LOG) per insertion, O(LOG) per max/min query, O(LOG^2) per kth query
// Space Complexity: O(LOG) per prefix state
template <int LOG = 64>
struct XorBasisPrefix {
    array<int, LOG> basis{};
    array<int, LOG> pos{};
    int last = 0;

    int msb_index(int x) const { return 63 - __builtin_clzll(x); }

    int powmod(int base, int exp) const {
        int res = 1 % MOD;
        base %= MOD;
        while (exp > 0) {
            if (exp & 1) res = (int)((__uint128_t)res * base % MOD);
            base = (int)((__uint128_t)base * base % MOD);
            exp >>= 1;
        }
        return res;
    }

    void clear() {
        basis.fill(0);
        pos.fill(0);
        last = 0;
    }

    void insert(int value, int idx = 0) {
        if (idx == 0)
            idx = ++last;
        else
            last = max(last, idx);
        int x = value;
        int p = idx;
        while (x) {
            int i = msb_index(x);
            if (i >= LOG) {
                x &= ((LOG == 64) ? ~0ULL : ((1ULL << LOG) - 1));
                if (!x) break;
                i = msb_index(x);
            }
            if (!basis[i]) {
                basis[i] = x;
                pos[i] = p;
                return;
            }
            if (pos[i] < p) {
                swap(pos[i], p);
                swap(basis[i], x);
            }
            x ^= basis[i];
        }
    }

    void merge(const XorBasisPrefix& other) {
        last = max(last, other.last);
        for (int i = 0; i < LOG; i++) {
            if (other.basis[i]) insert(other.basis[i], other.pos[i]);
        }
    }

    int max_xor(int x = 0, int L = 1, int R = 0) const {
        if (R == 0) R = last;
        int ans = x;
        for (int i = LOG - 1; i >= 0; i--) {
            if (basis[i] && pos[i] >= L && pos[i] <= R) {
                int cand = ans ^ basis[i];
                if (cand > ans) ans = cand;
            }
        }
        return ans;
    }

    int min_xor(int x = 0, int L = 1, int R = 0) const {
        if (R == 0) R = last;
        int ans = x;
        for (int i = LOG - 1; i >= 0; i--) {
            if (basis[i] && pos[i] >= L && pos[i] <= R) {
                int cand = ans ^ basis[i];
                if (cand < ans) ans = cand;
            }
        }
        return ans;
    }

    vector<int> canonical(int L = 1, int R = 0) const {
        if (R == 0) R = last;
        array<int, LOG> tmp{};
        for (int i = LOG - 1; i >= 0; i--) {
            if (!basis[i] || pos[i] < L || pos[i] > R) continue;
            int x = basis[i];
            if constexpr (LOG < 64) x &= ((1ULL << LOG) - 1);
            while (x) {
                int b = msb_index(x);
                if (b >= LOG) break;
                if (!tmp[b]) {
                    tmp[b] = x;
                    break;
                }
                x ^= tmp[b];
            }
        }
        for (int i = 0; i < LOG; i++) {
            if (!tmp[i]) continue;
            for (int j = i + 1; j < LOG; j++) {
                if (tmp[j] && ((tmp[j] >> i) & 1ULL)) tmp[j] ^= tmp[i];
            }
        }
        vector<int> out;
        out.reserve(LOG);
        for (int i = 0; i < LOG; i++)
            if (tmp[i]) out.push_back(tmp[i]);
        return out;
    }

    int kth_smallest_distinct_from_canonical(const vector<int>& can, int k) const {
        if (k == 0) return -1;
        int rank = sz(can);
        if (rank < 64) {
            int total = 1ULL << rank;
            if (k > total) return -1;
        }
        int mask = k - 1;
        int ans = 0;
        for (int i = 0; i < rank; i++) {
            if ((mask >> i) & 1ULL) ans ^= can[i];
        }
        return ans;
    }

    int kth_greatest_distinct_from_canonical(const vector<int>& can, int k) const {
        if (k == 0) return -1;
        int rank = sz(can);
        if (rank < 64) {
            int total = 1ULL << rank;
            if (k > total) return -1;
            return kth_smallest_distinct_from_canonical(can, total - k + 1);
        }
        int mask = ~0ULL - (k - 1);
        int ans = 0;
        for (int i = 0; i < rank; i++) {
            if ((mask >> i) & 1ULL) ans ^= can[i];
        }
        return ans;
    }

    int ways_per_value_mod(int L = 1, int R = 0) const {
        if (R == 0) R = last;
        int len = R - L + 1;
        if (len < 0) return 0;
        int rank = (int)canonical(L, R).size();
        return powmod(2, (int)(len - rank));
    }

    int kth_smallest_distinct(int k = 1, int L = 1, int R = 0) const {
        if (R == 0) R = last;
        vector<int> can = canonical(L, R);
        return kth_smallest_distinct_from_canonical(can, k);
    }

    int kth_greatest_distinct(int k = 1, int L = 1, int R = 0) const {
        if (R == 0) R = last;
        vector<int> can = canonical(L, R);
        return kth_greatest_distinct_from_canonical(can, k);
    }

    int kth_smallest(int k = 1, int L = 1, int R = 0) const {
        if (R == 0) R = last;
        if (k == 0) return -1;
        int len = R - L + 1;
        if (len < 0) return -1;
        vector<int> can = canonical(L, R);
        int rank = sz(can);
        int free_bits = len - rank;
        int block = (free_bits >= 63) ? ULLONG_MAX : (1ULL << free_bits);
        int distinct_index = (block == 0) ? (k - 1) : ((k - 1) / block);
        return kth_smallest_distinct_from_canonical(can, distinct_index + 1);
    }

    int kth_greatest(int k = 1, int L = 1, int R = 0) const {
        if (R == 0) R = last;
        if (k == 0) return -1;
        vector<int> can = canonical(L, R);
        int rank = sz(can);
        int len = R - L + 1;
        if (len < 0) return -1;
        int free_bits = len - rank;
        int block = (free_bits >= 63) ? ULLONG_MAX : (1ULL << free_bits);
        if (rank < 64) {
            int total_distinct = 1ULL << rank;
            int total = (block == ULLONG_MAX) ? ULLONG_MAX
                                              : (int)((__uint128_t)total_distinct * block > ULLONG_MAX
                                                          ? ULLONG_MAX
                                                          : (int)((__uint128_t)total_distinct * block));
            if (k > total) return -1;
            int distinct_from_top = (block == 0) ? k : ((k + block - 1) / block);
            return kth_greatest_distinct_from_canonical(can, distinct_from_top);
        }
        int distinct_from_top = (block == 0) ? k : ((k + block - 1) / block);
        return kth_greatest_distinct_from_canonical(can, distinct_from_top);
    }
};
