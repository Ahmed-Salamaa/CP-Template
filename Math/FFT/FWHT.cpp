#include <bits/stdc++.h>
using namespace std;

const int N = 3e5 + 9;
const int mod = 1e9 + 7;
const int inv2 = (mod + 1) >> 1;
const int M = (1 << 20);

#define OR 0
#define AND 1
#define XOR 2

int POW(long long n, long long k) {
    int ans = 1 % mod;
    n = (n % mod + mod) % mod;
    while (k) {
        if (k & 1) ans = (long long)ans * n % mod;
        n = (long long)n * n % mod;
        k >>= 1;
    }
    return ans;
}

struct FWHT {
    vector<int> P1, P2;
    
    FWHT(int max_size) {
        P1.resize(max_size, 0);
        P2.resize(max_size, 0);
    }

    void wt(int *a, int n, int flag = XOR) {
        if (n == 0) return;
        int m = n / 2;
        wt(a, m, flag);
        wt(a + m, m, flag);
        for (int i = 0; i < m; i++) {
            int x = a[i], y = a[i + m];
            if (flag == OR)  { a[i] = x; a[i + m] = (x + y) % mod; }
            if (flag == AND) { a[i] = (x + y) % mod; a[i + m] = y; }
            if (flag == XOR) { a[i] = (x + y) % mod; a[i + m] = (x - y + mod) % mod; }
        }
    }

    void iwt(int *a, int n, int flag = XOR) {
        if (n == 0) return;
        int m = n / 2;
        iwt(a, m, flag);
        iwt(a + m, m, flag);
        for (int i = 0; i < m; i++) {
            int x = a[i], y = a[i + m];
            if (flag == OR)  { a[i] = x; a[i + m] = (y - x + mod) % mod; }
            if (flag == AND) { a[i] = (x - y + mod) % mod; a[i + m] = y; }
            if (flag == XOR) { a[i] = 1LL * (x + y) * inv2 % mod; a[i + m] = 1LL * (x - y + mod) * inv2 % mod; }
        }
    }

    vector<int> multiply(int n, const vector<int>& A, const vector<int>& B, int flag = XOR) {
        assert(__builtin_popcount(n) == 1);
        for (int i = 0; i < n; i++) P1[i] = (i < A.size() ? A[i] : 0);
        for (int i = 0; i < n; i++) P2[i] = (i < B.size() ? B[i] : 0);
        
        wt(P1.data(), n, flag);
        wt(P2.data(), n, flag);
        for (int i = 0; i < n; i++) P1[i] = 1LL * P1[i] * P2[i] % mod;
        iwt(P1.data(), n, flag);
        
        return vector<int>(P1.begin(), P1.begin() + n);
    }

    vector<int> pow(int n, const vector<int>& A, long long k, int flag = XOR) {
        assert(__builtin_popcount(n) == 1);
        for (int i = 0; i < n; i++) P1[i] = (i < A.size() ? A[i] : 0);
        
        wt(P1.data(), n, flag);
        for (int i = 0; i < n; i++) P1[i] = POW(P1[i], k);
        iwt(P1.data(), n, flag);
        
        return vector<int>(P1.begin(), P1.begin() + n);
    }
};

signed main() {
    
    ios_base::sync_with_stdio(false);
    cin.tie(NULL);

    int n; 
    if (!(cin >> n)) return 0;

    vector<int> freq(M, 0);

    for (int i = 0; i < n; i++) {
        int x; 
        cin >> x;
        if (x < M) freq[x]++;
    } 

    FWHT ds(M);

    auto ans = ds.pow(M, freq, n, AND);
    
    int cnt = 1;
    for ( int i = 1 ; i < M ; i ++ ) {
        cnt += ( ans[i] > 0);
    }

    cout << cnt << "\n";

    return 0;
}