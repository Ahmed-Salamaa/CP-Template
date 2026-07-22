# From DigitSum/DigitSum_Snippets.md
## DigitSum

**Topic:** Digit Sum - Computes the sum of all digits from 1 to N

**Time Complexity:** O(log10(n))
**Space Complexity:** O(log10(n))

```cpp
// 
// Description:
// This utility computes the total sum of digits of all numbers from 1 up to N.
// It leverages digit DP concepts but is implemented recursively. 
// It precomputes the sum of digits up to 10^d - 1 to fast-forward the sum calculation.
//
// How to use it:
// cout << DigitSum::sumOfDigitsFrom1ToN(328) << endl;
//
#include "../../../core.h"

namespace DigitSum {
            int sumOfDigitsFrom1ToN(int n) {
        if (n < 10)
            return n * (n + 1) / 2;

        int d = log10(n);

        int *a = new int[d + 1];
        a[0] = 0;
        a[1] = 45;
        for (int i = 2; i <= d; i++)
            a[i] = a[i - 1] * 10 + 45 * ceil(pow(10, i - 1));

        int p = ceil(pow(10, d));
        int msd = n / p;

        int result = msd * a[d] + (msd * (msd - 1) / 2) * p +
                     msd * (1 + n % p) + sumOfDigitsFrom1ToN(n % p);
                     
        delete[] a;
        return result;
    }
}

```

## Misc

**Topic:** Misc Math - Simple math utilities like sums and log bounds

**Time Complexity:** O(length of string)
**Space Complexity:** O(1)

```cpp
//
// Description:
// Contains utilities like checking substrings and computing fast sums 
// of sequences (all numbers, odd numbers, even numbers).
//
// How to use it:
// cout << MiscMath::sumodd(5) << endl; // outputs 9 (1+3+5)
#include "../../../core.h"

namespace MiscMath {
            bool issubstring(string s1, string s2) {
        if (s2.find(s1) != string::npos) {
            return 1;
        }
        return 0;
    }

    // Time Complexity: O(1)
        int sumall(int x) {
        int res = (x * (x + 1)) / 2;
        return res;
    }

    // Time Complexity: O(1)
        int sumodd(int x) {
        int res = (x + 1) / 2;
        return res * res;
    }

    // Time Complexity: O(1)
        int sumeven(int x) { 
        return x * (x + 1); 
    }

    // Time Complexity: O(1)
        double get_log(int a, int b) { 
        return log(a) / log(b); 
    }
}
```

## FloorSum

**Topic:** Floor Sum - Calculates sum_{i=0..n-1} floor((a*i + b) / c)

**Time Complexity:** O(1)
**Space Complexity:** O(1)

```cpp
//
// Description:
// Computes the sum of floor((a*i + b) / c) for i from 0 to n-1 in O(log(a + c)) time.
// This is analogous to the Euclidean algorithm, counting points under a line segment.
//
// How to use it:
// int a = 3, b = 2, c = 5, n = 10;
// int ans = FloorSum::floorSum(a, b, c, n);
#include "../../../core.h"

namespace FloorSum {
            static inline int floor_div(int x, int y) {
        if (y <= 0) throw;
        if (x >= 0) return x / y;
        return -((-x + y - 1) / y);
    }

    // Time Complexity: O(log(a + c))
    // Space Complexity: O(log(a + c))
    int floorSum(int a, int b, int c, int n) {
        if (n <= 0) return 0;
        int qa = floor_div(a, c);
        int qb = floor_div(b, c);
        int res = qa * (n * (n - 1) / 2) + qb * n;

        a -= qa * c;
        b -= qb * c;

        if (a < 0) a += c;
        if (b < 0) b += c;

        int m = (a * n + b) / c;
        if (m == 0) return res;

        int new_b = (a * n + b) % c;
        return res + floorSum(c, new_b, a, m);
    }
}
```

## GrayCode

**Topic:** Gray Code - Utilities to convert between decimal and Gray code and generate Gray code sequences

**Time Complexity:** O(1)
**Space Complexity:** O(1)

```cpp
//
// Description:
// Provides functionality to convert an integer to its Gray code counterpart and vice versa.
// Also generates the complete sequence of Gray codes of length N as binary strings.
//
// How to use it:
// int gray = GrayCode::g(5);
// int dec = GrayCode::rev_g(gray);
// vector<string> seq = GrayCode::GenerateGrayCodeKnownLength(3);
#include "../../../core.h"

namespace GrayCode {
            // convert from decimal to gray
    int g(int n) {
        return n ^ (n >> 1);
    }

    // Time Complexity: O(log(n))
        // convert from gray to decimal
    int rev_g(int g) {
        int n = 0;
        for (; g; g >>= 1)
            n ^= g;
        return n;
    }

    // Time Complexity: O(2^n * n)
    // Space Complexity: O(2^n * n)
    vector<string> GenerateGrayCodeKnownLength(int n) {
        vector<string> ans;
        for (int i = 0; i < (1 << n); i++) {
            bitset<16> b = g(i);
            string temp;
            for (int j = n - 1; j >= 0; j--) {
                temp += to_string(b[j]);
            }
            ans.emplace_back(temp);
        }
        return ans;
    }
}
```

## FermatsTheoremonSumofTwoSquares

**Topic:** Sum of Two Squares - [Expresses a prime p = 1 mod 4 as sum of two squares]

**Description:** * Description: Expresses a prime p = 1 mod 4 as sum of two squares. * This mathematical template provides an efficient implementation for Sum of Two Squares. * It is commonly used in Advanced Number Theory problems. * * Usage: * FermatsTheoremonSumofTwoSquares_Algorithm solver; * auto result = solver.mult(...);

**Time Complexity:** O(log P)
**Space Complexity:** O(1)

```cpp
#include "../../../core.h"


struct FermatsTheoremonSumofTwoSquares_Algorithm {

    #define FR FermatRepresent
    namespace FermatRepresent {
        template<class num_t>
        inline num_t mult(num_t a, num_t b, num_t p) {
            num_t q = (num_t) ((ld) a * b / p);
            num_t r = a * b - q * p;
            while (r < 0) r += p;
            while (r >= p) r -= p;
            return r;
        }
        template<class num_t>
        inline num_t fpow(num_t n, num_t k, num_t p) {
            num_t r = 1;
            for (; k; k >>= 1) {
                if (k & 1) r = mult(r, n, p);
                n = mult(n, n, p);
            }
            return r;
        }
        template<class num_t>
        inline num_t isqrt(num_t k) {
            num_t r = sqrt(k) + 1;
            while (r * r > k) r--;
            return r;
        }
        int func(int p) {
            srand(2311);
            while (1) {
                int u = (int) rand() * rand() % p;
                if (fpow(u, (p - 1) / 2, p) == p - 1) {
                    int res = fpow(u, (p - 1) / 4, p);
                    return max(res, p - res);
                }
            }
        }
        // given an odd prime p
        // returns (a, b) s.t. a^2 + b^2 = p
        // p % 4 = 1, otherwise no solution exists
        pair<int, int> calc(int p) {
            int a = p, b = func(p);
            int ip = isqrt(p);
            while (b > ip) {
                a %= b;
                swap(a, b);
            }
            return make_pair(b, isqrt(p - b * b));
        }
    }
};
```

## StringExponentiation

**Topic:** String Exponentiation - Calculates (Base ^ Exponent_String) in any numeral base for generic types

**Time Complexity:** O(1)
**Space Complexity:** O(1)

```cpp
//
// Description:
// Generic string exponentiation. Works when the exponent is too large to fit in a standard integer type.
// It computes the answer by using the numeral representation base of the exponent string.
//
// How to use it:
// int base = 2;
// string huge_exp = "10000000000000000000000";
// int res = StringExponentiation::string_pow(base, huge_exp, 10, 1LL);
#include "../../../core.h"

namespace StringExponentiation {
            int char_to_val(char c) {
        if (c >= '0' && c <= '9') return c - '0';
        if (c >= 'A' && c <= 'Z') return c - 'A' + 10;
        if (c >= 'a' && c <= 'z') return c - 'a' + 10;
        return 0;
    }

    // Time Complexity: O(log(exp) * O(mult))
        template <typename T>
    T binpow(T base, int exp, T identity) {
        T res = identity;
        while (exp > 0) {
            if (exp & 1) res = res * base;
            base = base * base;
            exp >>= 1;
        }
        return res;
    }

    // Time Complexity: O(L * B * O(mult))
    // Space Complexity: O(B * size(T))
    template <typename T>
    T string_pow(T a, const string& exp_str, int numeral_base, T identity) {
        vector<T> p_base(numeral_base, identity);
        for (int i = 1; i < numeral_base; ++i) {
            p_base[i] = p_base[i - 1] * a;
        }

        T res = identity;
        for (char c : exp_str) {
            int d = char_to_val(c);
            res = binpow(res, (int)numeral_base, identity) * p_base[d];
        }
        
        return res;
    }
}
```

## Fraction

**Topic:** Fraction - Represents a mathematical fraction (numerator / denominator)

**Time Complexity:** O(log(min(a, b)))
**Space Complexity:** O(1)

```cpp
//
// Description:
// Struct for fractions that supports basic arithmetic operations (addition, subtraction,
// and comparisons). It automatically simplifies the fraction using GCD whenever
// instantiated or modified.
//
// How to use it:
// Fraction::frac f1(1, 2);
// Fraction::frac f2(1, 3);
// Fraction::frac f3 = f1 + f2;
// cout << f3.n << "/" << f3.d << endl; // Prints 5/6
//
#include "../../../core.h"

namespace Fraction {
            int gcd(int a, int b) {
        while (b) {
            a %= b;
            swap(a, b);
        }
        return a;
    }

    struct frac {
        int n, d;
        
        // Time Complexity: O(1)
                frac() { n = 0; d = 1; }
        
        // Time Complexity: O(log(min(n, d)))
                frac(int n, int d) : n(n), d(d) { simplify(); }
        
        // Time Complexity: O(1)
                bool operator<(const frac &other) const {
            return n * other.d < d * other.n;
        }
        
        // Time Complexity: O(log(min(n, d)))
                frac operator+(const frac &f) const {
            frac ans{n * f.d + f.n * d, d * f.d};
            ans.simplify();
            return ans;
        }
        
        // Time Complexity: O(log(min(n, d)))
                frac operator-(const frac &f) const {
            frac ans{n * f.d - f.n * d, d * f.d};
            ans.simplify();
            return ans;
        }
        
        // Time Complexity: O(1)
                void Abs() { 
            n = abs(n); 
            d = abs(d); 
        }
        
        // Time Complexity: O(log(min(n, d)))
                void simplify() {
            int g = gcd(abs(n), abs(d));
            n /= g; 
            d /= g;
        }
    };
}
```

