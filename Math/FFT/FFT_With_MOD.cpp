/*
 * Topic: FFT with Modulo
 * Description: Fast Fourier Transform for polynomial multiplication with arbitrary modulo
 */
#include "../../core.h"
// FFT
// Fast Fourier Transform for polynomial multiplication without mod
// O(n log n) complexity

#define complex complex<long double>
vector<vector<complex>> w;

void precompute_w(int lg) {
    w.resize(lg + 1);
    for (int lvl = 1; lvl <= lg; lvl++) {
        int len = 1 << lvl;
        w[lvl].resize(len / 2);
        long double ang = 2 * PI / len;
        for (int j = 0; j < len / 2; j++) { w[lvl][j] = complex(cosl(ang * j), sinl(ang * j)); }
    }
}

void fft(vector<complex>& a, bool invert) {
    int n = a.size();
    for (int i = 1, j = 0; i < n; i++) {
        int bit = n >> 1;
        for (; j >= bit; bit >>= 1) j -= bit;
        j += bit;
        if (i < j) swap(a[i], a[j]);
    }
    for (int len = 2, lvl = 1; len <= n; len <<= 1, lvl++) {
        for (int i = 0; i < n; i += len) {
            for (int j = 0; j < len / 2; j++) {
                complex wj = w[lvl][j];
                complex u = a[i + j];
                complex v = a[i + j + len / 2] * wj;
                a[i + j] = u + v;
                a[i + j + len / 2] = u - v;
            }
        }
    }
    if (invert) {
        reverse(a.begin() + 1, a.end());
        for (auto& x : a) x /= n;
    }
}

// =====================================================================================

const int C = sqrt(MOD);  // C is the base for splitting numbers = sqrt(max value)

vector<int> multiply(const vector<int>& a, const vector<int>& b) {
    int n = 1, lg = 0;
    while (n < sz(a) + sz(b) - 1) n <<= 1, lg++;
    precompute_w(lg);
    vector<complex> A1(n), A2(n), B1(n), B2(n);
    for (int i = 0; i < sz(a); i++) A1[i] = a[i] % C, A2[i] = a[i] / C;
    for (int i = 0; i < sz(b); i++) B1[i] = b[i] % C, B2[i] = b[i] / C;
    fft(A1, false);
    fft(A2, false);
    fft(B1, false);
    fft(B2, false);
    vector<complex> R1(n), R2(n), R3(n);
    for (int i = 0; i < n; i++) {
        R1[i] = A1[i] * B1[i];
        R2[i] = A2[i] * B2[i];
        R3[i] = (A1[i] * B2[i] + A2[i] * B1[i]);
    }
    fft(R1, true);
    fft(R2, true);
    fft(R3, true);
    vector<int> result(n);
    for (int i = 0; i < n; i++) {
        int val = llround(R1[i].real());
        result[i] = (result[i] + val % MOD + MOD) % MOD;
        val = llround(R2[i].real());
        result[i] = (result[i] + (val % MOD * C % MOD * C % MOD) % MOD + MOD) % MOD;
        val = llround(R3[i].real());
        result[i] = (result[i] + (val % MOD * C % MOD) % MOD + MOD) % MOD;
    }
    return result;
}
