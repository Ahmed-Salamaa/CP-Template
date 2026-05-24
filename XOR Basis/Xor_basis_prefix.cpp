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
struct XorBasisPrefix {
    static_assert(LOG >= 1 && LOG <= 63);
    ull b[LOG];
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
        ull v = (ull)x;
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
        ull ans = (ull)start;
        for (int i = LOG - 1; i >= 0; i--) {
            if (b[i] && pos[i] >= L && pos[i] <= R) {
                ans = max(ans, ans ^ b[i]);
            }
        }
        return (ll)ans;
    }

    ll min_xor(ll start = 0, int L = 1, int R = 0) const {
        if (R == 0) R = last;
        ull ans = (ull)start;
        for (int i = LOG - 1; i >= 0; i--) {
            if (b[i] && pos[i] >= L && pos[i] <= R) {
                ans = min(ans, ans ^ b[i]);
            }
        }
        return (ll)ans;
    }

    ll reduce(ll x, int L = 1, int R = 0) const {
        if (R == 0) R = last;
        ull ans = (ull)x;
        for (int i = LOG - 1; i >= 0; i--) {
            if (b[i] && pos[i] >= L && pos[i] <= R) {
                ans = min(ans, ans ^ b[i]);
            }
        }
        return (ll)ans;
    }

    bool contains(ll x, int L = 1, int R = 0) const {
        if (R == 0) R = last;
        ull v = (ull)x;
        for (int i = LOG - 1; i >= 0; i--) {
            if (b[i] && pos[i] >= L && pos[i] <= R) {
                v = min(v, v ^ b[i]);
            }
        }
        return v == 0;
    }

    void canonical(ull tmp[LOG], int& m, int L = 1, int R = 0) const {
        if (R == 0) R = last;
        memset(tmp, 0, sizeof(ull) * LOG);
        for (int i = LOG - 1; i >= 0; i--) {
            if (!b[i] || pos[i] < L || pos[i] > R) continue;
            ull x = b[i];
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
        ull tmp[LOG];
        int m = 0;
        canonical(tmp, m, L, R);
        if (m < 63 && (ull)k > (1ULL << m)) return -1;
        ull ans = 0;
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
        ull tmp[LOG];
        int m = 0;
        canonical(tmp, m, L, R);
        ull total = (m < 63) ? (1ULL << m) : (1ULL << 63);
        if ((ull)k > total) return -1;
        return kth_smallest_unique(total - (ull)k + 1, L, R);
    }

    ll kth_smallest(ll k, int L = 1, int R = 0) const {
        if (k <= 0) return -1;
        if (R == 0) R = last;
        int len = R - L + 1;
        if (len <= 0) return -1;
        ull tmp[LOG];
        int m = 0;
        canonical(tmp, m, L, R);
        ll free_bits = len - m;
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

    ll kth_greatest(ll k, int L = 1, int R = 0) const {
        if (k <= 0) return -1;
        if (R == 0) R = last;
        int len = R - L + 1;
        if (len <= 0) return -1;
        ull tmp[LOG];
        int m = 0;
        canonical(tmp, m, L, R);
        ll free_bits = len - m;
        ull block = (free_bits >= 63) ? ULLONG_MAX : (1ULL << (int)free_bits);
        ull total = (m < 63) ? (1ULL << m) : (1ULL << 63);
        if (block != ULLONG_MAX && total > ULLONG_MAX / block) {
            return kth_smallest(k, L, R);
        }
        ull total_elem = (block == ULLONG_MAX) ? ULLONG_MAX : total * block;
        if (k > (ll)total_elem) return -1;
        ull distinct_from_top = (block == ULLONG_MAX) ? k : (k + block - 1) / block;
        return kth_greatest_unique(distinct_from_top, L, R);
    }

    ll multiplicity(ll mod, int L = 1, int R = 0) const {
        if (R == 0) R = last;
        int len = R - L + 1;
        if (len <= 0 || mod <= 0) return 0;
        ull tmp[LOG];
        int m = 0;
        canonical(tmp, m, L, R);
        if (m >= len) return 0;
        return (ll)powmod(2, (ull)(len - m), (ull)mod);
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







