#include <bits/stdc++.h>
using namespace std;

using ll = long long;

static inline int msb_index_u64(ll x) {
    return 63 - __builtin_clzll(x);
}

static ll powmod_u64(ll base, ll exp, ll mod) {
    if (mod == 1) return 0;
    ll res = 1 % mod;
    base %= mod;
    while (exp > 0) {
        if (exp & 1) res = (ll)((__uint128_t)res * base % mod);
        base = (ll)((__uint128_t)base * base % mod);
        exp >>= 1;
    }
    return res;
}

// Timestamped XOR basis for prefix/range queries.
// Store vectors with their insertion index "pos" and allow querying [L, R]
// from the prefix basis at R.
template <int LOG = 64>
struct XorBasisPrefix {

    array<ll, LOG> basis{};
    array<int, LOG> pos{};
    int last = 0; // max inserted index

    void clear() {
        basis.fill(0);
        pos.fill(0);
        last = 0;
    }

    // Insert value at index idx (1-based). If idx==0, auto-append.
    // Complexity: O(rank) <= O(LOG)
    void insert(ll value, int idx = 0) {
        if (idx == 0) idx = ++last;
        else last = max(last, idx);

        ll x = value;
        int p = idx;
        while (x) {
            int i = msb_index_u64(x);
            if (i >= LOG) {
                x &= ((LOG == 64) ? ~0ULL : ((1ULL << LOG) - 1));
                if (!x) break;
                i = msb_index_u64(x);
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

    // Merge another timestamped basis (useful for segment trees).
    // Complexity: O(LOG * rank)
    void merge(const XorBasisPrefix& other) {
        last = max(last, other.last);
        for (int i = 0; i < LOG; i++) {
            if (other.basis[i]) insert(other.basis[i], other.pos[i]);
        }
    }

    ll max_xor(ll x = 0, int L = 1, int R = 0) const {
        if (R == 0) R = last;
        ll ans = x;
        for (int i = LOG - 1; i >= 0; i--) {
            if (basis[i] && pos[i] >= L && pos[i] <= R) {
                ll cand = ans ^ basis[i];
                if (cand > ans) ans = cand;
            }
        }
        return ans;
    }

    ll min_xor(ll x = 0, int L = 1, int R = 0) const {
        if (R == 0) R = last;
        ll ans = x;
        for (int i = LOG - 1; i >= 0; i--) {
            if (basis[i] && pos[i] >= L && pos[i] <= R) {
                ll cand = ans ^ basis[i];
                if (cand < ans) ans = cand;
            }
        }
        return ans;
    }

    // Canonical basis (RREF) for vectors coming from indices in [L, R].
    // Returned vectors are ordered by increasing MSB, enabling k-th queries.
    vector<ll> canonical(int L = 1, int R = 0) const {
        if (R == 0) R = last;

        array<ll, LOG> tmp{};
        for (int i = LOG - 1; i >= 0; i--) {
            if (!basis[i] || pos[i] < L || pos[i] > R) continue;
            ll x = basis[i];
            if constexpr (LOG < 64) x &= ((1ULL << LOG) - 1);
            while (x) {
                int b = msb_index_u64(x);
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

        vector<ll> out;
        out.reserve(LOG);
        for (int i = 0; i < LOG; i++) if (tmp[i]) out.push_back(tmp[i]);
        return out;
    }

    static ll kth_smallest_distinct_from_canonical(const vector<ll>& can, ll k) {
        if (k == 0) return (ll)-1;
        int rank = (int)can.size();
        if (rank < 64) {
            ll total = 1ULL << rank;
            if (k > total) return (ll)-1;
        }
        ll mask = k - 1;
        ll ans = 0;
        for (int i = 0; i < rank; i++) {
            if ((mask >> i) & 1ULL) ans ^= can[i];
        }
        return ans;
    }

    static ll kth_greatest_distinct_from_canonical(const vector<ll>& can, ll k) {
        if (k == 0) return (ll)-1;
        int rank = (int)can.size();
        if (rank < 64) {
            ll total = 1ULL << rank;
            if (k > total) return (ll)-1;
            return kth_smallest_distinct_from_canonical(can, total - k + 1);
        }
        ll mask = ~ll(0) - (k - 1);
        ll ans = 0;
        for (int i = 0; i < rank; i++) {
            if ((mask >> i) & 1ULL) ans ^= can[i];
        }
        return ans;
    }

    // Number of subsets mapping to any representable XOR value in [L, R], modulo mod.
    // Each distinct XOR value has exactly 2^(len-rank) preimages.
    ll ways_per_value_mod(ll mod, int L = 1, int R = 0) const {
        if (R == 0) R = last;
        int len = R - L + 1;
        if (len < 0) return 0;
        int rank = (int)canonical(L, R).size();
        return powmod_u64(2, (ll)(len - rank), mod);
    }

    // k is 1-indexed: k=1 -> 0.
    ll kth_smallest_distinct(ll k = 1, int L = 1, int R = 0) const {
        if (R == 0) R = last;
        vector<ll> can = canonical(L, R);
        return kth_smallest_distinct_from_canonical(can, k);
    }

    ll kth_greatest_distinct(ll k = 1, int L = 1, int R = 0) const {
        if (R == 0) R = last;
        vector<ll> can = canonical(L, R);
        return kth_greatest_distinct_from_canonical(can, k);
    }

    // Non-unique k-th: each distinct XOR appears 2^(len-rank) times.
    // k is 1-indexed.
    ll kth_smallest(ll k = 1, int L = 1, int R = 0) const {
        if (R == 0) R = last;
        if (k == 0) return (ll)-1;

        int len = R - L + 1;
        if (len < 0) return (ll)-1;
        vector<ll> can = canonical(L, R);
        int rank = (int)can.size();
        int free_bits = len - rank;

        ll block = (free_bits >= 63) ? numeric_limits<ll>::max() : (1ULL << free_bits);
        ll distinct_index = (block == 0) ? (k - 1) : ((k - 1) / block);
        return kth_smallest_distinct_from_canonical(can, distinct_index + 1);
    }

    ll kth_greatest(ll k = 1, int L = 1, int R = 0) const {
        if (R == 0) R = last;
        if (k == 0) return (ll)-1;

        vector<ll> can = canonical(L, R);
        int rank = (int)can.size();
        int len = R - L + 1;
        if (len < 0) return (ll)-1;
        int free_bits = len - rank;
        ll block = (free_bits >= 63) ? numeric_limits<ll>::max() : (1ULL << free_bits);

        if (rank < 64) {
            ll total_distinct = 1ULL << rank;
            ll total = (block == numeric_limits<ll>::max()) ? numeric_limits<ll>::max()
                                                                         : (ll)((__uint128_t)total_distinct * block > numeric_limits<ll>::max()
                                                                                          ? numeric_limits<ll>::max()
                                                                                          : (ll)((__uint128_t)total_distinct * block));
            if (k > total) return (ll)-1;
            ll distinct_from_top = (block == 0) ? k : ((k + block - 1) / block);
            return kth_greatest_distinct_from_canonical(can, distinct_from_top);
        }

        // rank==64: distinct space is enormous; for u64 k, mapping via blocks is enough.
        ll distinct_from_top = (block == 0) ? k : ((k + block - 1) / block);
        return kth_greatest_distinct_from_canonical(can, distinct_from_top);
    }
};

int main() {
    ios::sync_with_stdio(false);
    cin.tie(nullptr);

    int n, q;
    if (!(cin >> n >> q)) return 0;

    vector<XorBasisPrefix<64>> pref(n + 1);
    for (int i = 1; i <= n; i++) {
        ll x;
        cin >> x;
        pref[i] = pref[i - 1];
        pref[i].insert(x, i);
    }

    while (q--) {
        int type, L, R;
        cin >> type >> L >> R;
        if (type == 1) {
            cout << pref[R].max_xor(0, L, R) << "\n";
        } else if (type == 2) {
            ll k;
            cin >> k;
            ll ans = pref[R].kth_smallest_distinct(k, L, R);
            if (ans == (ll)-1) cout << -1 << "\n";
            else cout << ans << "\n";
        } else if (type == 3) {
            ll k;
            cin >> k;
            ll ans = pref[R].kth_greatest(k, L, R);
            if (ans == (ll)-1) cout << -1 << "\n";
            else cout << ans << "\n";
        }
    }

    return 0;
}