/*
 * Topic: FFT
 * Description: Fast Fourier Transform for polynomial multiplication without mod
 */
#include "../../core.h"

// FFT
// Fast Fourier Transform for polynomial multiplication without mod
// O(n log n) complexity
// if wrong try use precompute_w with fft function from FFT_With_MOD.cpp

struct FFT {
    using ftype = double;

    struct cd {
        ftype x, y;

        cd(ftype x = 0, ftype y = 0) : x(x), y(y) {}

        inline cd operator+(const cd& o) const { return {x + o.x, y + o.y}; }

        inline cd operator-(const cd& o) const { return {x - o.x, y - o.y}; }

        inline cd operator*(const cd& o) const { return {x * o.x - y * o.y, x * o.y + y * o.x}; }

        inline void operator*=(const cd& o) {
            ftype nx = x * o.x - y * o.y, ny = x * o.y + y * o.x;
            x = nx;
            y = ny;
        }

        inline void operator/=(const ftype& d) {
            x /= d;
            y /= d;
        }

        inline ftype real() const { return x; }
    };

    static vector<cd> w[2];
    static int max_len;

    static void precompute_w(int n) {
        if (max_len >= n) return;
        max_len = n;
        w[0].assign(n, cd());
        w[1].assign(n, cd());
        for (int len = 2; len <= n; len <<= 1) {
            double angle = 2 * PI / len;
            cd wlen(cos(angle), sin(angle));
            cd wlen_inv(cos(-angle), sin(-angle));
            cd curr(1), curr_inv(1);
            for (int i = 0; i < len / 2; i++) {
                w[0][len / 2 + i] = curr;
                w[1][len / 2 + i] = curr_inv;
                curr *= wlen;
                curr_inv *= wlen_inv;
            }
        }
    }

    static void fft(vector<cd>& a, bool invert) {
        int n = a.size();
        precompute_w(n);
        for (int i = 1, j = 0; i < n; i++) {
            int bit = n >> 1;
            for (; j & bit; bit >>= 1) j ^= bit;
            j ^= bit;
            if (i < j) swap(a[i], a[j]);
        }
        for (int len = 2; len <= n; len <<= 1) {
            int half = len / 2;
            for (int i = 0; i < n; i += len) {
                for (int j = 0; j < half; j++) {
                    cd u = a[i + j];
                    cd v = a[i + j + half] * w[invert][half + j];
                    a[i + j] = u + v;
                    a[i + j + half] = u - v;
                }
            }
        }
        if (invert) {
            for (auto& x : a) x /= n;
        }
    }

    static vector<int> multiply(const vector<int>& a, const vector<int>& b) {
        int n = 1;
        while (n < sz(a) + sz(b) - 1) n <<= 1;
        vector<cd> fa(a.begin(), a.end()), fb(b.begin(), b.end());
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
    static string mul_big_int(const string& a, const string& b) {
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
};

vector<FFT::cd> FFT::w[2];
int FFT::max_len = 0;
