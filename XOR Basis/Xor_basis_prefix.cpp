#include <bits/stdc++.h>
using namespace std;


static ll powmod(ll base, ll exp, ll mod) {
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

template <int LOG = 62>
struct XorBasisPrefix {
    static_assert(LOG >= 1 && LOG <= 63);
    ll b[LOG];
    int pos[LOG];
    int rk;
    int last;

    XorBasisPrefix() {
        memset(b, 0, sizeof(b));
        memset(pos, 0, sizeof(pos));
        rk = 0;
        last = 0;
    }

    bool insert(ll x, int idx = 0) {
        if (idx == 0) idx = ++last;
        else last = max(last, idx);
        ll v = (ll)x;
        int p = idx;
        for (int i = LOG - 1; i >= 0; i--) {
            if (((v >> i) & 1) == 0) continue;
            if (!b[i]) {
                b[i] = v;
                pos[i] = p;
                rk++;
                return true;
            }
            if (pos[i] < p) {
                swap(pos[i], p);
                swap(b[i], v);
            }
            v ^= b[i];
        }
        return false;
    }

    void merge(const XorBasisPrefix& other) {
        for (int i = 0; i < LOG; i++) {
            if (other.b[i]) insert((ll)other.b[i], other.pos[i]);
        }
    }

    ll max_xor(ll start = 0, int L = 1, int R = 0) const {
        if (R == 0) R = last;
        ll ans = (ll)start;
        for (int i = LOG - 1; i >= 0; i--) {
            if (b[i] && pos[i] >= L && pos[i] <= R) {
                ans = max(ans, ans ^ b[i]);
            }
        }
        return (ll)ans;
    }

    ll min_xor(ll start = 0, int L = 1, int R = 0) const {
        if (R == 0) R = last;
        ll ans = (ll)start;
        for (int i = LOG - 1; i >= 0; i--) {
            if (b[i] && pos[i] >= L && pos[i] <= R) {
                ans = min(ans, ans ^ b[i]);
            }
        }
        return (ll)ans;
    }

    ll reduce(ll x, int L = 1, int R = 0) const {
        if (R == 0) R = last;
        ll ans = (ll)x;
        for (int i = LOG - 1; i >= 0; i--) {
            if (b[i] && pos[i] >= L && pos[i] <= R) {
                ans = min(ans, ans ^ b[i]);
            }
        }
        return (ll)ans;
    }

    bool contains(ll x, int L = 1, int R = 0) const {
        if (R == 0) R = last;
        ll v = (ll)x;
        for (int i = LOG - 1; i >= 0; i--) {
            if (b[i] && pos[i] >= L && pos[i] <= R) {
                v = min(v, v ^ b[i]);
            }
        }
        return v == 0;
    }

    void canonical(ll tmp[LOG], int& m, int L = 1, int R = 0) const {
        if (R == 0) R = last;
        memset(tmp, 0, sizeof(ll) * LOG);
        for (int i = LOG - 1; i >= 0; i--) {
            if (!b[i] || pos[i] < L || pos[i] > R) continue;
            ll x = b[i];
            for (int j = i - 1; j >= 0; j--) {
                if (tmp[j] && ((x >> j) & 1)) x ^= tmp[j];
            }
            tmp[i] = x;
        }
        for (int i = 0; i < LOG; i++) {
            if (tmp[i]) {
                for (int j = i + 1; j < LOG; j++) {
                    if (tmp[j] && ((tmp[j] >> i) & 1)) tmp[j] ^= tmp[i];
                }
            }
        }
        m = 0;
        for (int i = 0; i < LOG; i++) {
            if (tmp[i]) m++;
        }
    }

    ll kth_smallest_unique(ll k, int L = 1, int R = 0) const {
        if (k <= 0) return -1;
        if (R == 0) R = last;
        ll tmp[LOG];
        int m = 0;
        canonical(tmp, m, L, R);
        if (m < 63 && (ll)k > (1ULL << m)) return -1;
        ll ans = 0;
        k--;
        for (int i = 0; i < LOG; i++) {
            if (tmp[i] && (k & 1)) ans ^= tmp[i];
            k >>= 1;
        }
        return (ll)ans;
    }

    ll kth_greatest_unique(ll k, int L = 1, int R = 0) const {
        if (k <= 0) return -1;
        if (R == 0) R = last;
        ll tmp[LOG];
        int m = 0;
        canonical(tmp, m, L, R);
        ll total = (m < 63) ? (1ULL << m) : (1ULL << 63);
        if ((ll)k > total) return -1;
        return kth_smallest_unique(total - (ll)k + 1, L, R);
    }

    ll kth_smallest(ll k, int L = 1, int R = 0) const {
        if (k <= 0) return -1;
        if (R == 0) R = last;
        int len = R - L + 1;
        if (len <= 0) return -1;
        ll tmp[LOG];
        int m = 0;
        canonical(tmp, m, L, R);
        ll free_bits = len - m;
        ll block = (free_bits >= 63) ? ULLONG_MAX : (1ULL << (int)free_bits);
        ll distinct_k = (block == ULLONG_MAX) ? 1 : (k + block - 1) / block;
        if (m < 63 && distinct_k > (1ULL << m)) return -1;
        ll ans = 0;
        distinct_k--;
        for (int i = 0; i < LOG; i++) {
            if (tmp[i] && (distinct_k & 1)) ans ^= tmp[i];
            distinct_k >>= 1;
        }
        return (ll)ans;
    }

    ll kth_greatest(ll k, int L = 1, int R = 0) const {
        if (k <= 0) return -1;
        if (R == 0) R = last;
        int len = R - L + 1;
        if (len <= 0) return -1;
        ll tmp[LOG];
        int m = 0;
        canonical(tmp, m, L, R);
        ll free_bits = len - m;
        ll block = (free_bits >= 63) ? ULLONG_MAX : (1ULL << (int)free_bits);
        ll total = (m < 63) ? (1ULL << m) : (1ULL << 63);
        if (block != ULLONG_MAX && total > ULLONG_MAX / block) {
            return kth_smallest(k, L, R);
        }
        ll total_elem = (block == ULLONG_MAX) ? ULLONG_MAX : total * block;
        if (k > (ll)total_elem) return -1;
        ll distinct_from_top = (block == ULLONG_MAX) ? k : (k + block - 1) / block;
        return kth_greatest_unique(distinct_from_top, L, R);
    }

    ll multiplicity(ll mod, int L = 1, int R = 0) const {
        if (R == 0) R = last;
        int len = R - L + 1;
        if (len <= 0 || mod <= 0) return 0;
        ll tmp[LOG];
        int m = 0;
        canonical(tmp, m, L, R);
        if (m >= len) return 0;
        return (ll)powmod(2, (ll)(len - m), (ll)mod);
    }

    ll ways_to_generate(ll x, int L = 1, int R = 0) const {
        if (R == 0) R = last;
        if (!contains(x, L, R)) return 0;
        int len = R - L + 1;
        ll tmp[LOG];
        int m = 0;
        canonical(tmp, m, L, R);
        return powmod(2, (ll)(len - m), MOD);
    }

    // Count distinct numbers generateable: 2^rk (without duplicates)
    ll count_distinct_numbers(int L = 1, int R = 0) const {
        if (R == 0) R = last;
        int rank = 0;
        ll tmp[LOG];
        canonical(tmp, rank, L, R);
        return powmod(2, (ll)rank, (ll)1e9 + 7);
    }

    // Count with duplicates considered (2^(len - rank) accounting for redundancy)
    ll count_with_duplicates(int L = 1, int R = 0) const {
        if (R == 0) R = last;
        int len = R - L + 1;
        if (len <= 0) return 1;
        int rank = 0;
        ll tmp[LOG];
        canonical(tmp, rank, L, R);
        ll free_bits = len - rank;
        if (free_bits < 0) free_bits = 0;
        return powmod(2, free_bits, (ll)1e9 + 7);
    }
};







