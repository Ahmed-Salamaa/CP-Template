// Topic: Floor Sum With Powers - Calculates sum_{x=0}^{n} x^k1 * floor((a*x + b) / c)^k2
//
// Description:
// Generalization of floor sum capable of handling polynomial sums with powers up to 10.
// Must call init() before making queries.
//
// How to use it:
// FloorSumWithPowers::init();
// FloorSumWithPowers::node ans = FloorSumWithPowers::simgcd(a, b, c, n);
// int result = ans[k1][k2];
#include "../../../core.h"
const int mod = 1e9 + 7;
const int N = 20;

// Time Complexity: O(1)
// Space Complexity: O(1)
int Mod(int x) { return x >= mod ? x - mod : (x < 0 ? x + mod : x); }

// Time Complexity: O(log(b))
// Space Complexity: O(1)
int qpow(int a, int b) {
    int res = 1;
    while (b) {
        if (b & 1ll) res = res * a % mod;
        b >>= 1ll, a = a * a % mod;
    }
    return res;
}

struct node {
    int a[N][N];

    node(int p = 0) {
        for (int i = 0; i <= p; i++)
            for (int j = 0; j <= p; j++) a[i][j] = 0;
    }

    int* operator[](const int& i) { return a[i]; }
};

int C[N][N], S[N][N];

// Time Complexity: O(k^3 * log(max(a, c)))
// Space Complexity: O(k^2 * log(max(a, c)))
node recurse(int a, int b, int c, int n) {
    if (a == 0) {
        node ans = node(10);
        for (int k1 = 0; k1 <= 10; k1++) {
            int sum = 0, power = n + 1;
            for (int i = 1; i <= k1 + 1; i++, power = power * (n + 1) % mod) sum = (sum + power * S[k1][i]) % mod;
            for (int k2 = 0; k1 + k2 <= 10; k2++, sum = sum * (b / c) % mod) ans[k1][k2] = sum;
        }
        return ans;
    }
    if (a >= c) {
        node now = recurse(a % c, b, c, n), ans = node(10);
        for (int k1 = 0; k1 <= 10; k1++) {
            for (int k2 = 0; k1 + k2 <= 10; k2++) {
                int power = 1;
                for (int i = 0; i <= k2; i++, power = power * (a / c) % mod)
                    ans[k1][k2] = (ans[k1][k2] + C[k2][i] * power % mod * now[k1 + i][k2 - i]) % mod;
            }
        }
        return ans;
    }
    if (b >= c) {
        node now = recurse(a, b % c, c, n), ans = node(10);
        for (int k1 = 0; k1 <= 10; k1++) {
            for (int k2 = 0; k1 + k2 <= 10; k2++) {
                int power = 1;
                for (int i = 0; i <= k2; i++, power = power * (b / c) % mod)
                    ans[k1][k2] = (ans[k1][k2] + C[k2][i] * power % mod * now[k1][k2 - i]) % mod;
            }
        }
        return ans;
    }
    int m = (a * n + b) / c;
    node now = recurse(c, a + c - b - 1, a, m - 1), ans = node(10);
    for (int k1 = 0; k1 <= 10; k1++) {
        int sum = 0, power = n + 1;
        for (int i = 1; i <= k1 + 1; i++, power = power * (n + 1) % mod) sum = (sum + power * S[k1][i]) % mod;
        for (int k2 = 0; k1 + k2 <= 10; k2++, sum = sum * m % mod) {
            ans[k1][k2] = sum;
            for (int i = 0; i < k2; i++)
                for (int j = 1; j <= k1 + 1; j++)
                    ans[k1][k2] = (ans[k1][k2] - C[k2][i] * S[k1][j] % mod * now[i][j]) % mod;
            ans[k1][k2] = Mod(ans[k1][k2]);
        }
    }
    return ans;
}

// Time Complexity: O(k^3 * log(max(a, c)))
// Space Complexity: O(k^2 * log(max(a, c)))
node simgcd(int a, int b, int c, int n) { return recurse(a, b, c, n); }

// Time Complexity: O(k^3)
// Space Complexity: O(k^2)
void init() {
    C[0][0] = 1;
    for (int i = 1; i <= 10; i++) {
        C[i][0] = 1;
        for (int j = 1; j <= i; j++) C[i][j] = Mod(C[i - 1][j] + C[i - 1][j - 1]);
    }
    for (int i = 0; i <= 10; i++) {
        S[i][i + 1] = 1;
        for (int j = 0; j < i; j++)
            for (int k = 1; k <= j + 1; k++) S[i][k] = Mod(S[i][k] - C[i + 1][j] * S[j][k] % mod);
        int val = qpow(i + 1, mod - 2);
        for (int j = 1; j <= i + 1; j++) S[i][j] = S[i][j] * val % mod;
    }
}
