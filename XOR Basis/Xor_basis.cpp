#include <bits/stdc++.h>
using namespace std;

using ll = long long;
using ull = unsigned long long;

static ull powmod(ull base, ull exp, ull mod) {
    if (mod == 1) return 0;
    ull res = 1 % mod;
    base %= mod;
    while (exp > 0) {
        if (exp & 1ULL) res = (ull)((__uint128_t)res * base % mod);
        base = (ull)((__uint128_t)base * base % mod);
        exp >>= 1;
    }
    return res;
}

template <int LOG = 62>
struct XorBasis {
    static_assert(LOG >= 1 && LOG <= 63);
    ull b[LOG];
    int rk;
    ll inserted;

    XorBasis () {
        memset(b, 0, sizeof(b));
        rk = 0;
        inserted = 0;
    }

    bool insert(ll x) {
        inserted++;
        ull v = (ull)x;
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
        ull ans = (ull)start;
        for (int i = LOG - 1; i >= 0; i--) {
            if (b[i]) ans = max(ans, ans ^ b[i]);
        }
        return (ll)ans;
    }

    ll min_xor(ll start = 0) const {
        ull ans = (ull)start;
        for (int i = LOG - 1; i >= 0; i--) {
            if (b[i]) ans = min(ans, ans ^ b[i]);
        }
        return (ll)ans;
    }

    ll reduce(ll x) const {
        ull ans = (ull)x;
        for (int i = LOG - 1; i >= 0; i--) {
            if (b[i]) ans = min(ans, ans ^ b[i]);
        }
        return (ll)ans;
    }

    bool contains(ll x) const {
        ull v = (ull)x;
        for (int i = LOG - 1; i >= 0; i--) {
            if (b[i]) v = min(v, v ^ b[i]);
        }
        return v == 0;
    }

    ll kth_smallest_unique(ll k) const {
        if (k <= 0) return -1;
        ull tmp[LOG];
        memcpy(tmp, b, sizeof(tmp));
        for (int i = LOG - 1; i >= 0; i--) {
            if (!tmp[i]) continue;
            for (int j = i - 1; j >= 0; j--) {
                if (tmp[j] && ((tmp[i] >> j) & 1)) tmp[i] ^= tmp[j];
            }
        }
        int m = 0;
        for (int i = 0; i < LOG; i++) {
            if (tmp[i]) m++;
        }
        if (m < 63 && (ull)k > (1ULL << m)) return -1;
        ull ans = 0;
        k--;
        for (int i = 0; i < LOG; i++) {
            if (tmp[i] && (k & 1)) ans ^= tmp[i];
            k >>= 1;
        }
        return (ll)ans;
    }

    ll kth_greatest_unique(ll k) const {
        if (k <= 0) return -1;
        int m = rk;
        ull total = (m < 63) ? (1ULL << m) : (1ULL << 63);
        if ((ull)k > total) return -1;
        return kth_smallest_unique(total - (ull)k + 1);
    }

    ll kth_smallest(ll k) const {
        if (k <= 0) return -1;
        if (inserted < 63 && (ull)k > (1ULL << (int)inserted)) return -1;
        ull tmp[LOG];
        memcpy(tmp, b, sizeof(tmp));
        for (int i = LOG - 1; i >= 0; i--) {
            if (!tmp[i]) continue;
            for (int j = i - 1; j >= 0; j--) {
                if (tmp[j] && ((tmp[i] >> j) & 1)) tmp[i] ^= tmp[j];
            }
        }
        int m = 0;
        for (int i = 0; i < LOG; i++) {
            if (tmp[i]) m++;
        }
        ll free_bits = inserted - m;
        ull block = (free_bits >= 63) ? ULLONG_MAX : (1ULL << (int)free_bits);
        ull distinct_k = (block == ULLONG_MAX) ? 1 : (k + block - 1) / block;
        if (m < 63 && distinct_k > (1ULL << m)) return -1;
        ull ans = 0;
        distinct_k--;
        for (int i = 0; i < LOG; i++) {
            if (tmp[i] && (distinct_k & 1)) ans ^= tmp[i];
            distinct_k >>= 1;
        }
        return (ll)ans;
    }

    ll kth_greatest(ll k) const {
        if (k <= 0) return -1;
        if (inserted < 63 && (ull)k > (1ULL << (int)inserted)) return -1;
        ull total = (inserted < 63) ? (1ULL << (int)inserted) : (1ULL << 63);
        return kth_smallest(total - (ull)k + 1);
    }

    ll multiplicity(ll mod) const {
        if (mod <= 0 || inserted < rk) return 0;
        return (ll)powmod(2, (ull)(inserted - rk), (ull)mod);
    }

    ll ways_to_generate(ll x, int L = 1, int R = 0) const {
        if (R == 0) R = last;
        if (!contains(x, L, R)) return 0;
        int len = R - L + 1;
        ull tmp[LOG];
        int m = 0;
        canonical(tmp, m, L, R);
        return powmod(2, (ull)(len - m), MOD);
    }
};
