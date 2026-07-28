/*
 * Topic: FFT
 * Description: Short and compact Fast Fourier Transform implementation
 */
#include "../../core.h"

struct FFT_Short {
    using cd = complex<double>;

    static void fft(vector<cd>& a, bool inv) {
        int n = a.size();
        for (int i = 1, j = 0; i < n; i++) {
            int bit = n >> 1;
            for (; j & bit; bit >>= 1) j ^= bit;
            j ^= bit;
            if (i < j) swap(a[i], a[j]);
        }
        for (int len = 2; len <= n; len <<= 1) {
            double ang = 2 * PI / len * (inv ? -1 : 1);
            cd wlen(cos(ang), sin(ang));
            for (int i = 0; i < n; i += len) {
                cd w(1);
                for (int j = 0; j < len / 2; j++, w *= wlen) {
                    cd u = a[i + j], v = a[i + j + len / 2] * w;
                    a[i + j] = u + v;
                    a[i + j + len / 2] = u - v;
                }
            }
        }
        if (inv) {
            for (cd& x : a) x /= n;
        }
    }

    static vector<int> multiply(const vector<int>& a, const vector<int>& b) {
        vector<cd> fa(a.begin(), a.end()), fb(b.begin(), b.end());
        int n = 1;
        while (n < sz(a) + sz(b)) n <<= 1;
        fa.resize(n);
        fb.resize(n);
        fft(fa, false);
        fft(fb, false);
        for (int i = 0; i < n; i++) fa[i] *= fb[i];
        fft(fa, true);
        vector<int> res(n);
        for (int i = 0; i < n; i++) res[i] = round(fa[i].real());
        return res;
    }
};
