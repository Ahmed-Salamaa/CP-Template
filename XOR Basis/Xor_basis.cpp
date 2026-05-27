#include <bits/stdc++.h>
using namespace std;

using ll = long long;


template <int LOG = 62>
struct XorBasis {
    static_assert(LOG >= 1 && LOG <= 63);
    ll b[LOG];
    int rk;
    ll inserted;

    const int mod = 1e9 + 7 ; 

    ll powmod(ll base, ll exp, ll mod) {
        if (mod == 1) return 0;
        ll res = 1 % mod;
        base %= mod;
        while (exp > 0) {
            if (exp & 1ULL) res = (ll)((__uint128_t)res * base % mod);
            base = (ll)((__uint128_t)base * base % mod);
            exp >>= 1;
        }
        return res;
    }

    XorBasis () {
        memset(b, 0, sizeof(b));
        rk = 0;
        inserted = 0;
    }

    bool insert(ll x) {
        inserted++;
        ll v = (ll)x;
        for (int i = LOG - 1; i >= 0; i--) {
            if (((v >> i) & 1ULL) == 0) continue;
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
            if (other.b[i]) insert((ll)other.b[i]);
        }
    }

    ll max_xor(ll start = 0) const {
        ll ans = (ll)start;
        for (int i = LOG - 1; i >= 0; i--) {
            if (b[i]) ans = max(ans, ans ^ b[i]);
        }
        return (ll)ans;
    }

    ll min_xor(ll start = 0) const {
        ll ans = (ll)start;
        for (int i = LOG - 1; i >= 0; i--) {
            if (b[i]) ans = min(ans, ans ^ b[i]);
        }
        return (ll)ans;
    }

    ll reduce(ll x) const {
        ll ans = (ll)x;
        for (int i = LOG - 1; i >= 0; i--) {
            if (b[i]) ans = min(ans, ans ^ b[i]);
        }
        return (ll)ans;
    }

    bool contains(ll x) const {
        ll v = (ll)x;
        for (int i = LOG - 1; i >= 0; i--) {
            if (b[i]) v = min(v, v ^ b[i]);
        }
        return v == 0;
    }

    ll kth_smallest_unique(ll k) const {
        if (k <= 0) return -1;
        ll tmp[LOG];
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
        if (rank < 63 && k > (ll)(1ULL << rank)) return -1;
        ll ans = 0;
        k--; // k encodes which basis vectors to XOR
        for (int i = 0; i < LOG; i++) {
            if (tmp[i] && (k & 1)) ans ^= tmp[i];
            k >>= 1;
        }
        return ans;
    }

    ll kth_greatest_unique(ll k) const {
        if (k <= 0) return -1;
        int m = rk;
        ll total = (m < 63) ? (1ULL << m) : (1ULL << 63);
        if ((ll)k > total) return -1;
        return kth_smallest_unique(total - (ll)k + 1);
    }

    ll kth_smallest(ll k) const {
        if (k <= 0) return -1;
        if (inserted < 63 && k > (ll)(1ULL << inserted)) return -1;
        ll tmp[LOG];
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
        ll free_bits = inserted - rank;
        ll group_size = (free_bits >= 63) ? ULLONG_MAX : (1ULL << free_bits);
        ll unique_idx = (group_size == ULLONG_MAX) ? 1 : (k + group_size - 1) / group_size;
        if (rank < 63 && unique_idx > (ll)(1ULL << rank)) return -1;
        ll ans = 0;
        unique_idx--; // map k to the unique XOR index
        for (int i = 0; i < LOG; i++) {
            if (tmp[i] && (unique_idx & 1)) ans ^= tmp[i];
            unique_idx >>= 1;
        }
        return ans;
    }

    ll kth_greatest(ll k) const {
        if (k <= 0) return -1;
        if (inserted < 63 && (ll)k > (1ULL << (int)inserted)) return -1;
        ll total = (inserted < 63) ? (1ULL << (int)inserted) : (1ULL << 63);
        return kth_smallest(total - (ll)k + 1);
    }

    ll ways_to_generate(ll x) const {
        if (!contains(x)) return 0;
        ll free_bits = inserted - rk;
        if (free_bits < 0) free_bits = 0;
        return powmod(2, free_bits, mod);
    }

    // Count distinct numbers generateable: 2^rk (without duplicates)
    ll count_distinct_numbers() const {
        return powmod(2, rk, mod);
    }

    // Count with duplicates considered (2^inserted accounting for redundancy)
    ll count_with_duplicates() const {
        ll free_bits = inserted - rk;
        if (free_bits < 0) free_bits = 0;
        return powmod(2, free_bits, mod);
    }
};




