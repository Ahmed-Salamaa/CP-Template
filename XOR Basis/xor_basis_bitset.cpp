#include <bits/stdc++.h>
using namespace std;

typedef long long ll;
const ll MOD = 1e9 + 7;

static ll powmod(ll base, ll exp, ll mod = MOD) {
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

// ---------------------------------------------------------
// Standard Xor Basis (Supports up to N bits)
// ---------------------------------------------------------
template <size_t N>
struct XorBasis {
    bitset<N> b[N];
    int rk;
    ll inserted;

    XorBasis() : rk(0), inserted(0) {}

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

    void merge(const XorBasis<N>& other) {
        inserted += other.inserted;
        for (int i = 0; i < (int)N; i++) {
            if (other.b[i][i]) {
                _insert(other.b[i]);
            }
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

    ll ways_to_generate(bitset<N> x) const {
        if (!contains(x)) return 0;
        ll free_bits = inserted - rk;
        if (free_bits < 0) free_bits = 0;
        return powmod(2, free_bits, MOD);
    }
};

// ---------------------------------------------------------
// Prefix Xor Basis for Range Queries [L, R]
// ---------------------------------------------------------
template <size_t N>
struct XorBasisPrefix {
    bitset<N> b[N];
    int pos[N];
    int rk;
    int last;

    XorBasisPrefix() : rk(0), last(0) {
        memset(pos, 0, sizeof(pos));
    }

    bool insert(bitset<N> x, int idx = 0) {
        if (idx == 0) idx = ++last;
        else last = max(last, idx);

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

    ll ways_to_generate(bitset<N> x, int L = 1, int R = 0) const {
        if (R == 0) R = last;
        if (!contains(x, L, R)) return 0;
        int len = R - L + 1;
        
        int m = 0;
        for (int i = 0; i < (int)N; i++) {
            if (b[i][i] && pos[i] >= L && pos[i] <= R) m++;
        }
        
        return powmod(2, (ll)(len - m), MOD);
    }
};


