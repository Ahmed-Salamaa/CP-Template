#pragma GCC optimize("O3,unroll-loops")
#pragma GCC target("avx2,bmi,bmi2,lzcnt,popcnt")

#include <bits/stdc++.h>
using namespace std;

const long long MOD = 1e9 + 7; // Change to 998244353 if needed

// O(log exp) Fast Binary Exponentiation
long long power(long long base, long long exp) {
    long long res = 1;
    base %= MOD;
    while (exp > 0) {
        if (exp & 1) res = (res * base) % MOD;
        base = (base * base) % MOD;
        exp >>= 1;
    }
    return res;
}

template<int LOG = 62>
struct XorBasis {
    long long basis[LOG];
    int pos[LOG];
    int cnt = 0; // Tracks the number of elements inserted (acts as default R)

    XorBasis() {
        memset(basis, 0, sizeof(basis));
        memset(pos, 0, sizeof(pos));
    }

    // 1. Swap
    // O(1) swap to easily swap two basis structures
    void swap_basis(XorBasis& other) {
        swap(basis, other.basis);
        swap(pos, other.pos);
        swap(cnt, other.cnt);
    }

    // 2. Insert
    // O(LOG)
    void insert(long long val, int idx = 0) {
        if (idx == 0) idx = ++cnt;
        else cnt = max(cnt, idx);

        for (int i = LOG - 1; i >= 0; i--) {
            if ((val >> i) & 1) {
                if (!basis[i]) {
                    basis[i] = val;
                    pos[i] = idx;
                    return;
                }
                if (pos[i] < idx) {
                    swap(pos[i], idx);
                    swap(basis[i], val);
                }
                val ^= basis[i];
            }
        }
    }

    // 3. Merge
    // O(LOG^2)
    void merge(const XorBasis& other) {
        for (int i = 0; i < LOG; i++) {
            if (other.basis[i]) {
                insert(other.basis[i], other.pos[i]);
            }
        }
    }

    // 4. Get Max XOR (with optional initial value x)
    // O(LOG)
    long long get_max(long long x = 0, int L = 1, int R = 0) const {
        if (R == 0) R = cnt;
        long long ans = x;
        for (int i = LOG - 1; i >= 0; i--) {
            if (pos[i] >= L && pos[i] <= R && (ans ^ basis[i]) > ans) {
                ans ^= basis[i];
            }
        }
        return ans;
    }

    // 5. Get Min XOR (with optional initial value x)
    // O(LOG)
    long long get_min(long long x = 0, int L = 1, int R = 0) const {
        if (R == 0) R = cnt;
        long long ans = x;
        for (int i = LOG - 1; i >= 0; i--) {
            if (pos[i] >= L && pos[i] <= R && (ans ^ basis[i]) < ans) {
                ans ^= basis[i];
            }
        }
        return ans;
    }

    // Helper: Canonical Basis (Reduced Row Echelon Form)
    // O(LOG^2)
    vector<long long> get_canonical(int L = 1, int R = 0) const {
        if (R == 0) R = cnt;
        vector<long long> temp_basis(LOG, 0);
        for (int i = LOG - 1; i >= 0; i--) {
            if (pos[i] >= L && pos[i] <= R && basis[i]) {
                long long val = basis[i];
                for (int j = LOG - 1; j >= 0; j--) {
                    if ((val >> j) & 1) {
                        if (!temp_basis[j]) {
                            temp_basis[j] = val;
                            break;
                        }
                        val ^= temp_basis[j];
                    }
                }
            }
        }

        vector<long long> canonical;
        for (int i = 0; i < LOG; i++) {
            if (!temp_basis[i]) continue;
            for (int j = i + 1; j < LOG; j++) {
                if ((temp_basis[j] >> i) & 1) {
                    temp_basis[j] ^= temp_basis[i];
                }
            }
            canonical.push_back(temp_basis[i]);
        }
        return canonical;
    }

    // 6. Number of ways to form any formable sum
    // O(LOG^2)
    long long get_ways(int L = 1, int R = 0) const {
        if (R == 0) R = cnt;
        int N = R - L + 1;
        vector<long long> can = get_canonical(L, R);
        int sz = can.size();
        return power(2, N - sz); 
    }

    // 7. Get Smallest K-th DISTINCT XOR sum
    // O(LOG^2)
    long long kth_smallest_distinct(long long k = 1, int L = 1, int R = 0) const {
        if (R == 0) R = cnt;
        vector<long long> can = get_canonical(L, R);
        long long sz = can.size();
        
        if (k > (1LL << sz)) return -1; // Out of bounds
        k--; // 0-based

        long long ans = 0;
        for (int i = 0; i < sz; i++) {
            if ((k >> i) & 1) ans ^= can[i];
        }
        return ans;
    }

    // 8. Get Greatest K-th DISTINCT XOR sum
    // O(LOG^2)
    long long kth_greatest_distinct(long long k = 1, int L = 1, int R = 0) const {
        if (R == 0) R = cnt;
        vector<long long> can = get_canonical(L, R);
        long long sz = can.size();
        
        if (k > (1LL << sz)) return -1; // Out of bounds
        
        // Map "greatest K-th" to "smallest K-th"
        long long smallest_k = (1LL << sz) - k + 1;
        return kth_smallest_distinct(smallest_k, L, R);
    }

    // 9. Get Smallest K-th NON-UNIQUE XOR sum
    // O(LOG^2)
    long long kth_smallest_non_unique(long long k = 1, int L = 1, int R = 0) const {
        if (R == 0) R = cnt;
        int N = R - L + 1;
        vector<long long> can = get_canonical(L, R);
        int sz = can.size();
        
        long long zero_ways = 1;
        if (N - sz >= 62) zero_ways = 2e18; // Prevent overflow
        else zero_ways = 1LL << (N - sz);

        if (k <= zero_ways) return 0;
        k -= zero_ways;

        long long distinct_k = (k + zero_ways - 1) / zero_ways;
        return kth_smallest_distinct(distinct_k, L, R);
    }

    // 10. Get Greatest K-th NON-UNIQUE XOR sum
    // O(LOG^2)
    long long kth_greatest_non_unique(long long k = 1, int L = 1, int R = 0) const {
        if (R == 0) R = cnt;
        int N = R - L + 1;
        vector<long long> can = get_canonical(L, R);
        int sz = can.size();
        
        long long zero_ways = 1;
        if (N - sz >= 62) zero_ways = 2e18; // Prevent overflow
        else zero_ways = 1LL << (N - sz);

        // Map non-unique K from the top to distinct K from the top
        long long distinct_k_from_top = (k + zero_ways - 1) / zero_ways;
        return kth_greatest_distinct(distinct_k_from_top, L, R);
    }
};

// Global Array of Prefix Bases
const int MAXN = 200005;
XorBasis <62> pref[MAXN];

int main() {
    ios_base::sync_with_stdio(false);
    cin.tie(NULL);

    int n, q;
    if (!(cin >> n >> q)) return 0;

    for(int i = 1; i <= n; i++) {
        long long x; 
        cin >> x;
        pref[i] = pref[i - 1]; 
        pref[i].insert(x); // Auto-increments internal 'cnt'
    }
    
    // Example queries demonstrating standard L, R passing
    while(q--) {
        int type, L, R;
        cin >> type >> L >> R;

        if (type == 1) {
            // Get max in [L, R] starting with x=0
            cout << pref[R].get_max(0, L, R) << "\n";
        } 
        else if (type == 2) {
            long long k; cin >> k;
            // Get smallest K-th distinct in [L, R]
            cout << pref[R].kth_smallest_distinct(k, L, R) << "\n";
        }
        else if (type == 3) {
            long long k; cin >> k;
            // Get greatest K-th non-unique in [L, R]
            cout << pref[R].kth_greatest_non_unique(k, L, R) << "\n";
        }
    }

    return 0;
}