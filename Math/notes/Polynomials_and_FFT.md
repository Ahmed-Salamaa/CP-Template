# From FFT/FFT_Snippets.md
## PolynomialPower

**Description:** Topic: Polynomial Power - Fast exponentiation of a polynomial using FFT Computes (P(x))^k using binary exponentiation and FFT.

**Usage:**
```cpp
vector<int> base = {1, 1}; // e.g. 1 + x
vector<int> ans = {1};     // Identity polynomial
PolynomialPower::polyPower(ans, base, 10, 100);
```

**Time Complexity:** O(K * N log N) where K is exponent bits, N is max polynomial degree
**Space Complexity:** O(N)

```cpp
#include "../../../core.h"
#include "FastFourierTransform.cpp"

namespace PolynomialPower {

            void polyPower(vector<int>& ans, vector<int>& base, int k, int limitN) {
        while (k) {
            if (k % 2) {
                ans = FastFourierTransform::multiply(ans, base);
                while (sz(ans) > limitN) ans.pop_back();
                for (auto& x : ans) if (x > 1) x = 1; // Assuming we want boolean coefficients, remove if not needed
            }
            k /= 2;
            if (k) {
                base = FastFourierTransform::multiply(base, base);
                while (sz(base) > limitN) base.pop_back();
                for (auto& x : base) if (x > 1) x = 1; // Assuming we want boolean coefficients
            }
        }
    }
}

```

## BooleanConvolution

**Description:** Topic: Boolean Convolution (Bitset) Multiplies two bitsets efficiently in O(N^2 / 64) worst-case. Computes polynomial multiplication and powers where coefficients are boolean (modulo 2).

**Usage:**
```cpp
bitset<100000> a, b;
a.set(1); b.set(2);
bitset<100000> res = BooleanConvolution::multiply<100000>(a, b);
bitset<100000> res2 = BooleanConvolution::power<100000>(a, 5);
```

**Time Complexity:** Worst-case O(N^2 / 64)
**Space Complexity:** O(N / 64)

```cpp
#include "../../../core.h"

namespace BooleanConvolution {

            template <size_t N>
    bitset<N> multiply(const bitset<N>& A, const bitset<N>& B) {
        bitset<N> res;
        for (int i = B._Find_first(); i < N; i = B._Find_next(i)) {
            res |= (A << i);
        }
        return res;
    }

    // Time Complexity: Worst-case O((N^2 / 64) * log(K))
        template <size_t N>
    bitset<N> power(bitset<N> base, int k) {
        bitset<N> res;
        res.set(0); 
        
        while (k > 0) {
            if (k % 2 == 1) {
                res = multiply<N>(res, base);
            }
            if (k > 1) {
                base = multiply<N>(base, base);
            }
            k /= 2;
        }
        return res;
    }
}
```

## FastFourierTransform

**Description:** * Topic: Fast Fourier Transform (FFT) - Multiplies two polynomials in O(N log N) * Description: Computes the discrete Fourier transform and its inverse. * Used to multiply two polynomials or large integers efficiently. * Usage: * vector<int> a = {1, 2, 3}; * vector<int> b = {4, 5, 6}; * vector<int> res = FastFourierTransform::multiply(a, b);

**Time Complexity:** O(N log N)
**Space Complexity:** O(N)

```cpp
#include "../../../core.h"

namespace FastFourierTransform {
    using cd = complex<double>;
    const double PI = acos(-1);

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

            vector<int> multiply(const vector<int>& a, const vector<int>& b) {
        if (a.empty() || b.empty()) return {};
        vector<cd> fa(all(a)), fb(all(b));
        int n = 1;
        while (n < sz(a) + sz(b)) n <<= 1;
        fa.resize(n); fb.resize(n);

        fft(fa, false);
        fft(fb, false);
        for (int i = 0; i < n; i++) fa[i] *= fb[i];
        fft(fa, true);

        vector<int> result(n);
        for (int i = 0; i < n; i++) result[i] = round(fa[i].real());
        while (sz(result) > 1 && result.back() == 0 && sz(result) != sz(a) + sz(b) - 1) {
            result.pop_back();
        }
        if (sz(result) != sz(a) + sz(b) - 1) {
            result.resize(sz(a) + sz(b) - 1);
        }
        return result;
    }
}
```

## FastWalshHadamardTransform

**Description:** * Topic: Fast Walsh-Hadamard Transform - Convolutions with AND, OR, XOR * Description: Computes bitwise convolutions of two arrays. * Usage: * vector<int> a = {1, 2}, b = {3, 4}; * vector<int> res = FastWalshHadamardTransform::multiply(a, b, 2); // 0:AND, 1:OR, 2:XOR

**Time Complexity:** O(N log N)
**Space Complexity:** O(N)

```cpp
#include "../../../core.h"

namespace FastWalshHadamardTransform {

    inline int add(int a, int b) { return a + b; }
    inline int sub(int a, int b) { return a - b; }

    // f: 0 for AND, 1 for OR, 2 for XOR
            void fwht(vector<int>& a, bool inv, int f) {
        int sz = sz(a);
        for (int len = 1; 2 * len <= sz; len <<= 1) {
            for (int i = 0; i < sz; i += 2 * len) {
                for (int j = 0; j < len; j++) {
                    int x = a[i + j];
                    int y = a[i + j + len];

                    if (f == 0) { // AND
                        if (!inv) { a[i + j] = y; a[i + j + len] = add(x, y); }
                        else { a[i + j] = sub(y, x); a[i + j + len] = x; }
                    } else if (f == 1) { // OR
                        if (!inv) { a[i + j + len] = add(x, y); }
                        else { a[i + j + len] = sub(y, x); }
                    } else { // XOR
                        a[i + j] = add(x, y);
                        a[i + j + len] = sub(x, y);
                    }
                }
            }
        }
    }

            vector<int> multiply(vector<int> a, vector<int> b, int f) {
        int sz = sz(a);
        fwht(a, false, f);
        fwht(b, false, f);
        vector<int> c(sz);
        for (int i = 0; i < sz; ++i) {
            c[i] = 1LL * a[i] * b[i]; // May need modulo if counting paths
        }
        fwht(c, true, f);
        if (f == 2) {
            for (int i = 0; i < sz; ++i) {
                c[i] /= sz;
            }
        }
        return c;
    }
}
```

## NumberTheoreticTransform

**Description:** * Topic: Number Theoretic Transform (NTT) - Multiplies two polynomials modulo prime * Description: Similar to FFT but works under a specific prime modulo (e.g. 998244353). * Usage: * vector<int> a = {1, 2, 3}, b = {4, 5, 6}; * vector<int> res = NumberTheoreticTransform::conv(a, b);

**Time Complexity:** O(N log N)
**Space Complexity:** O(N)

```cpp
#include "../../../core.h"

namespace NumberTheoreticTransform {
    const int mod = 998244353;
    const int root = 62; 
    // For p < 2^30 there is also e.g. 5 << 25, 7 << 26, 479 << 21, 483 << 21
    
    inline int modpow(int b, int e, int m) {
        int ans = 1;
        for (; e; b = 1LL * b * b % m, e /= 2)
            if (e & 1) ans = 1LL * ans * b % m;
        return ans;
    }

            void ntt(vector<int>& a) {
        int n = sz(a), L = 31 - __builtin_clz(n);
        vector<int> rt(2, 1);
        for (int k = 2, s = 2; k < n; k *= 2, s++) {
            rt.resize(n);
            int z[] = {1, modpow(root, mod >> s, mod)};
            for (int i = k; i < 2 * k; ++i) rt[i] = 1LL * rt[i / 2] * z[i & 1] % mod;
        }
        vector<int> rev(n);
        for (int i = 0; i < n; ++i) rev[i] = (rev[i / 2] | (i & 1) << L) / 2;
        for (int i = 0; i < n; ++i) if (i < rev[i]) swap(a[i], a[rev[i]]);
        for (int k = 1; k < n; k *= 2) {
            for (int i = 0; i < n; i += 2 * k) {
                for (int j = 0; j < k; ++j) {
                    int z = 1LL * rt[j + k] * a[i + j + k] % mod, &ai = a[i + j];
                    a[i + j + k] = ai - z + (z > ai ? mod : 0);
                    ai += (ai + z >= mod ? z - mod : z);
                }
            }
        }
    }

            vector<int> conv(const vector<int>& a, const vector<int>& b) {
        if (a.empty() || b.empty()) return {};
        int s = sz(a) + sz(b) - 1, B = 32 - __builtin_clz(s), n = 1 << B;
        int inv = modpow(n, mod - 2, mod);
        vector<int> L(a), R(b), out(n);
        L.resize(n); R.resize(n);
        ntt(L); ntt(R);
        for (int i = 0; i < n; ++i) out[-i & (n - 1)] = 1LL * L[i] * R[i] % mod * inv % mod;
        ntt(out);
        return {out.begin(), out.begin() + s};
    }
}
```

