/*
 * Topic: FFT
 * Description: Fast Fourier Transform for polynomial multiplication without mod
 */
#include "../../core.h"
// FFT
// Fast Fourier Transform for polynomial multiplication without mod
// O(n log n) complexity
// if wrong try use precompute_w with fft function from FFT_With_MOD.cpp
#define complex complex<long double>

void fft(vector<complex>& a, bool invert) {
    int n = a.size();
    for (int i = 1, j = 0; i < n; i++) {
        int bit = n >> 1;
        for (; j >= bit; bit >>= 1) j -= bit;
        j += bit;
        if (i < j) swap(a[i], a[j]);
    }
    for (int len = 2; len <= n; len <<= 1) {
        double angle = 2 * PI / len * (invert ? -1 : 1);
        complex wlen(cos(angle), sin(angle));
        for (int i = 0; i < n; i += len) {
            complex w(1);
            for (int j = 0; j < len / 2; j++) {
                complex u = a[i + j];
                complex v = a[i + j + len / 2] * w;
                a[i + j] = u + v;
                a[i + j + len / 2] = u - v;
                w *= wlen;
            }
        }
    }
    if (invert) {
        for (auto& x : a) x /= n;
    }
}

vector<int> multiply(const vector<int>& a, const vector<int>& b) {
    int n = 1;
    while (n < sz(a) + sz(b) - 1) n <<= 1;
    vector<complex> fa(a.begin(), a.end()), fb(b.begin(), b.end());
    fa.resize(n);
    fb.resize(n);
    fft(fa, false);
    fft(fb, false);
    for (int i = 0; i < n; i++) fa[i] *= fb[i];
    fft(fa, true);
    vector<int> result(n);
    for (int i = 0; i < n; i++) result[i] = llround(fa[i].real());

    return result;
}

// Function to compute the power of a polynomial using FFT
//! use multiply with limit argument
//! and clip the result to the limit before return in multiply
/*
    vector<int> multiply(const vector<int> &a, const vector<int> &b, int limit = 1e5)
    {
        int n = 1;
        while (n < sz(a) + sz(b) - 1)
            n <<= 1;
        vector<complex> fa(a.begin(), a.end()), fb(b.begin(), b.end());
        fa.resize(n);
        fb.resize(n);
        fft(fa, false);
        fft(fb, false);
        for (int i = 0; i < n; i++)
            fa[i] *= fb[i];
        fft(fa, true);
        vector<int> result(min(n, limit));
        for (int i = 0; i < min(n, limit); i++)
            result[i] = round(fa[i].real());
        return result;
    }
*/
static vector<int> poly_pow(const vector<int>& a, int exp) {
    int n = a.size();
    vector<int> result(n, 0);
    result[0] = 1;
    vector<int> base = a;

    while (exp) {
        if (exp & 1) result = multiply(result, base);
        base = multiply(base, base);
        exp >>= 1;
    }
    return result;
}

// Function to multiply two large integers represented as strings in O(n log n) time using FFT
string mul_big_int(const string& a, const string& b) {
    int n = a.size(), m = b.size();
    vector<int> num1(n), num2(m);
    for (int i = 0; i < n; i++) { num1[i] = a[n - 1 - i] - '0'; }
    for (int i = 0; i < m; i++) { num2[i] = b[m - 1 - i] - '0'; }
    vector<int> result = multiply(num1, num2);
    string res;
    int carry = 0;
    for (auto& val : result) {
        val += carry;
        carry = val / 10;
        res.push_back((val % 10) + '0');
    }
    while (carry) {
        res.push_back((carry % 10) + '0');
        carry /= 10;
    }
    reverse(res.begin(), res.end());
    for (int i = 0; i < sz(res); i++) {
        if (res[i] != '0') { return res.substr(i); }
    }
    return "0";
}
