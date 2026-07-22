/*
 * Topic: XOR Basis
 * Description: The standard linear algebra technique using Gaussian elimination to maintain a
 *   basis for a set of numbers under the XOR operation. Fundamental for finding the
 *   maximum XOR subset, subset XOR queries, and representing vector spaces in GF(2).
 * can be generated, finding kth smallest/greatest generated number, etc. Usage: XorBasis<60> basis; basis.insert(10);
 * int mx = basis.max_xor();
 */
#include "../../core.h"

// Time Complexity: O(LOG) per insertion/query
// Space Complexity: O(LOG)
template <int LOG = 62>
struct XorBasis {
    static_assert(LOG >= 1 && LOG <= 63);
    int b[LOG];
    int rk;
    int inserted;

    int powmod(int base, int exp) const {
        if (MOD == 1) return 0;
        int res = 1 % MOD;
        base %= MOD;
        while (exp > 0) {
            if (exp & 1) res = (int)((__uint128_t)res * base % MOD);
            base = (int)((__uint128_t)base * base % MOD);
            exp >>= 1;
        }
        return res;
    }

    XorBasis() {
        memset(b, 0, sizeof(b));
        rk = 0;
        inserted = 0;
    }

    bool insert(int x) {
        inserted++;
        int v = x;
        for (int i = LOG - 1; i >= 0; i--) {
            if (((v >> i) & 1) == 0) continue;
            if (!b[i]) {
                b[i] = v;
                rk++;
                return true;
            }
            v ^= b[i];
        }
        return false;
    }

    void merge(const XorBasis& other) {
        inserted += other.inserted;
        for (int i = 0; i < LOG; i++) {
            if (other.b[i]) insert(other.b[i]);
        }
    }

    int max_xor(int start = 0) const {
        int ans = start;
        for (int i = LOG - 1; i >= 0; i--) {
            if (b[i]) ans = max(ans, ans ^ b[i]);
        }
        return ans;
    }

    int min_xor(int start = 0) const {
        int ans = start;
        for (int i = LOG - 1; i >= 0; i--) {
            if (b[i]) ans = min(ans, ans ^ b[i]);
        }
        return ans;
    }

    int max_xor_bounded(int min_val, int max_val, int start = 0) const {
        int tmp[LOG];
        memcpy(tmp, b, sizeof(tmp));
        for (int i = LOG - 1; i >= 0; i--) {
            if (!tmp[i]) continue;
            for (int j = i - 1; j >= 0; j--) {
                if (tmp[j] && ((tmp[i] >> j) & 1)) tmp[i] ^= tmp[j];
            }
        }
        vector<int> c;
        for (int i = LOG - 1; i >= 0; i--)
            if (tmp[i]) c.push_back(tmp[i]);

        int best = -1;
        auto dfs = [&](auto& self, int idx, int current_val) -> void {
            if (idx == sz(c)) {
                if (current_val >= min_val && current_val <= max_val) { best = max(best, current_val); }
                return;
            }
            int max_reach = current_val, min_reach = current_val;
            for (int j = idx; j < sz(c); j++) {
                max_reach = max(max_reach, max_reach ^ c[j]);
                min_reach = min(min_reach, min_reach ^ c[j]);
            }
            if (max_reach < min_val || min_reach > max_val) return;

            int v1 = current_val ^ c[idx];
            int v2 = current_val;
            if (v1 > v2) {
                self(self, idx + 1, v1);
                if (best != -1) return;
                self(self, idx + 1, v2);
            } else {
                self(self, idx + 1, v2);
                if (best != -1) return;
                self(self, idx + 1, v1);
            }
        };
        dfs(dfs, 0, start);
        return best;
    }

    int min_xor_bounded(int min_val, int max_val, int start = 0) const {
        int tmp[LOG];
        memcpy(tmp, b, sizeof(tmp));
        for (int i = LOG - 1; i >= 0; i--) {
            if (!tmp[i]) continue;
            for (int j = i - 1; j >= 0; j--) {
                if (tmp[j] && ((tmp[i] >> j) & 1)) tmp[i] ^= tmp[j];
            }
        }
        vector<int> c;
        for (int i = LOG - 1; i >= 0; i--)
            if (tmp[i]) c.push_back(tmp[i]);

        int best = -1;
        auto dfs = [&](auto& self, int idx, int current_val) -> void {
            if (idx == sz(c)) {
                if (current_val >= min_val && current_val <= max_val) {
                    best = (best == -1) ? current_val : min(best, current_val);
                }
                return;
            }
            int max_reach = current_val, min_reach = current_val;
            for (int j = idx; j < sz(c); j++) {
                max_reach = max(max_reach, max_reach ^ c[j]);
                min_reach = min(min_reach, min_reach ^ c[j]);
            }
            if (max_reach < min_val || min_reach > max_val) return;

            int v1 = current_val ^ c[idx];
            int v2 = current_val;
            if (v1 < v2) {
                self(self, idx + 1, v1);
                if (best != -1) return;
                self(self, idx + 1, v2);
            } else {
                self(self, idx + 1, v2);
                if (best != -1) return;
                self(self, idx + 1, v1);
            }
        };
        dfs(dfs, 0, start);
        return best;
    }

    int reduce(int x) const {
        int ans = x;
        for (int i = LOG - 1; i >= 0; i--) {
            if (b[i]) ans = min(ans, ans ^ b[i]);
        }
        return ans;
    }

    bool contains(int x) const { return reduce(x) == 0; }

    int kth_smallest_unique(int k) const {
        if (k <= 0) return -1;
        int tmp[LOG];
        memcpy(tmp, b, sizeof(tmp));
        for (int i = LOG - 1; i >= 0; i--) {
            if (!tmp[i]) continue;
            for (int j = i - 1; j >= 0; j--) {
                if (tmp[j] && ((tmp[i] >> j) & 1)) tmp[i] ^= tmp[j];
            }
        }
        int rank = 0;
        for (int i = 0; i < LOG; i++) {
            if (tmp[i]) rank++;
        }
        if (rank < 63 && k > (1ULL << rank)) return -1;
        int ans = 0;
        k--;
        for (int i = 0; i < LOG; i++) {
            if (tmp[i] && (k & 1)) ans ^= tmp[i];
            k >>= 1;
        }
        return ans;
    }

    int kth_greatest_unique(int k) const {
        if (k <= 0) return -1;
        int total = (rk < 63) ? (1ULL << rk) : (1ULL << 63);
        if (k > total) return -1;
        return kth_smallest_unique(total - k + 1);
    }

    int kth_smallest(int k) const {
        if (k <= 0) return -1;
        if (inserted < 63 && k > (1ULL << inserted)) return -1;
        int free_bits = inserted - rk;
        int group_size = (free_bits >= 63) ? ULLONG_MAX : (1ULL << free_bits);
        int unique_idx = (group_size == ULLONG_MAX) ? 1 : (k + group_size - 1) / group_size;
        return kth_smallest_unique(unique_idx);
    }

    int kth_greatest(int k) const {
        if (k <= 0) return -1;
        if (inserted < 63 && k > (1ULL << inserted)) return -1;
        int total = (inserted < 63) ? (1ULL << inserted) : (1ULL << 63);
        return kth_smallest(total - k + 1);
    }

    int ways_to_generate(int x) const {
        if (!contains(x)) return 0;
        int free_bits = inserted - rk;
        if (free_bits < 0) free_bits = 0;
        return powmod(2, free_bits);
    }

    int count_distinct_numbers() const { return powmod(2, rk); }

    int count_with_duplicates() const {
        int free_bits = inserted - rk;
        if (free_bits < 0) free_bits = 0;
        return powmod(2, free_bits);
    }
};
