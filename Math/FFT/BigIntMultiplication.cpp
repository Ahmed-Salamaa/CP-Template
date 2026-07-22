/*
 * Topic: Big Integer Multiplication (FFT)
 * Description: Multiplies exceptionally large numbers in O(N log N) time by treating their
 *   digits as coefficients of polynomials and using Fast Fourier Transform. Useful
 *   when standard multiplication is too slow for thousands or millions of digits.
 * Usage:
 * string a = "123456789";
 * string b = "987654321";
 * string res = BigIntMultiplication::mulBigInt(a, b);
 */
#include "../../../core.h"
using cd = complex<double>;
const double PI = acos(-1);

// Time Complexity: O(N log N)
// Space Complexity: O(N)
void fft(vector<cd>& a, bool invert) {
    int n = sz(a);
    for (int i = 1, j = 0; i < n; i++) {
        int bit = n >> 1;
        for (; j & bit; bit >>= 1) j ^= bit;
        j ^= bit;
        if (i < j) swap(a[i], a[j]);
    }
    for (int len = 2; len <= n; len <<= 1) {
        double ang = 2 * PI / len * (invert ? -1 : 1);
        cd wlen(cos(ang), sin(ang));
        for (int i = 0; i < n; i += len) {
            cd w(1, 0);
            for (int j = 0; j < len / 2; j++) {
                cd u = a[i + j], v = a[i + j + len / 2] * w;
                a[i + j] = u + v;
                a[i + j + len / 2] = u - v;
                w *= wlen;
            }
        }
    }
    if (invert) {
        for (cd& x : a) x /= n;
    }
}

// Time Complexity: O(N log N)
// Space Complexity: O(N)
vector<int> multiply(const vector<int>& a, const vector<int>& b) {
    if (a.empty() || b.empty()) return {};
    vector<cd> fa(all(a)), fb(all(b));
    int n = 1;
    while (n < sz(a) + sz(b)) n <<= 1;
    fa.resize(n);
    fb.resize(n);
    fft(fa, false);
    fft(fb, false);
    for (int i = 0; i < n; i++) fa[i] *= fb[i];
    fft(fa, true);
    vector<int> result(n);
    for (int i = 0; i < n; i++) result[i] = round(fa[i].real());
    while (sz(result) > 1 && result.back() == 0 && sz(result) != sz(a) + sz(b) - 1) { result.pop_back(); }
    return result;
}

// Time Complexity: O(N log N)
// Space Complexity: O(N)
string mulBigInt(const string& a, const string& b) {
    int n = sz(a), m = sz(b);
    if (n == 0 || m == 0 || a == "0" || b == "0") return "0";
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
    reverse(all(res));
    for (int i = 0; i < sz(res); i++) {
        if (res[i] != '0') { return res.substr(i); }
    }
    return "0";
}
