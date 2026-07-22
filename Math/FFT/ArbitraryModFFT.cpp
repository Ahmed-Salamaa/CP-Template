/*
 * Topic: Arbitrary Modulo FFT - Multiplies two polynomials modulo any number
 * Description: Arbitrary Modulo FFT performs Fast Fourier Transform to multiply polynomials
 *   under an arbitrary modulo by splitting numbers and doing multiple FFTs. Crucial
 *   for combinatorial problems requiring polynomial multiplication without a
 *   specific NTT-friendly modulus.
 * by splitting coefficients into smaller chunks.
 * Usage:
 * vector<int> a = {1, 2, 3};
 * vector<int> b = {4, 5, 6};
 * vector<int> res = ArbitraryModFFT::convMod<1000000007>(a, b);
 */
#include "../../../core.h"
using C = complex<double>;

// Time Complexity: O(N log N)
// Space Complexity: O(N)
void fft(vector<C>& a) {
    int n = sz(a);
    int L = 31 - __builtin_clz(n);
    vector<complex<ld>> R(2, 1.0);
    vector<C> rt(2, C(1, 0));
    for (int k = 2; k < n; k *= 2) {
        R.resize(n);
        rt.resize(n);
        auto x = polar(1.0L, acos(-1.0L) / k);
        for (int i = k; i < 2 * k; i++) {
            R[i] = (i & 1) ? R[i / 2] * x : R[i / 2];
            rt[i] = C(R[i].real(), R[i].imag());
        }
    }
    vector<int> rev(n);
    for (int i = 0; i < n; i++) { rev[i] = (rev[i / 2] | ((i & 1) << L)) / 2; }
    for (int i = 0; i < n; i++) {
        if (i < rev[i]) swap(a[i], a[rev[i]]);
    }
    for (int k = 1; k < n; k *= 2) {
        for (int i = 0; i < n; i += 2 * k) {
            for (int j = 0; j < k; j++) {
                double* x = (double*)&rt[j + k];
                double* y = (double*)&a[i + j + k];
                C z(x[0] * y[0] - x[1] * y[1], x[0] * y[1] + x[1] * y[0]);
                a[i + j + k] = a[i + j] - z;
                a[i + j] += z;
            }
        }
    }
}

// Time Complexity: O(N log N)
// Space Complexity: O(N)
template <int M>
vector<int> convMod(const vector<int>& a, const vector<int>& b) {
    if (a.empty() || b.empty()) return {};
    int resSize = sz(a) + sz(b) - 1;
    vector<int> res(resSize);
    int B = 32 - __builtin_clz(resSize);
    int n = 1 << B;
    int cut = (int)sqrt(M);
    vector<C> L(n), R(n), outs(n), outl(n);
    for (int i = 0; i < sz(a); i++) { L[i] = C(a[i] / cut, a[i] % cut); }
    for (int i = 0; i < sz(b); i++) { R[i] = C(b[i] / cut, b[i] % cut); }
    fft(L);
    fft(R);
    for (int i = 0; i < n; i++) {
        int j = (-i) & (n - 1);
        outl[j] = (L[i] + conj(L[j])) * R[i] / (2.0 * n);
        outs[j] = (L[i] - conj(L[j])) * R[i] / (2.0 * n) / C(0, 1);
    }
    fft(outl);
    fft(outs);
    for (int i = 0; i < resSize; i++) {
        int av = (int)(real(outl[i]) + 0.5);
        int cv = (int)(imag(outs[i]) + 0.5);
        int bv = (int)(imag(outl[i]) + 0.5) + (int)(real(outs[i]) + 0.5);
        res[i] = ((av % M * cut + bv) % M * cut + cv) % M;
    }
    return res;
}
