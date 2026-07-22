/*
 * Topic: BigInt
 * Description: Implements arbitrary-precision arithmetic for numbers that exceed the standard
 *   64-bit integer limits. Used extensively in combinatorics and large scale
 *   computations where built-in types overflow.
 * Usage:
 * BigInt a = "1234567890"; BigInt b = 100; cout << a + b;
 */
#include "../core.h"
const int DIG = 9;
const int BASE = 1e9;

struct BigInt {
    int sgn;
    vector<int> a;

    BigInt() : sgn(1) {}

    BigInt(long long v) { *this = v; }

    BigInt& operator=(long long v) {
        sgn = 1;
        if (v < 0) sgn = -1, v = -v;
        a.clear();
        for (; v > 0; v /= BASE) a.push_back(v % BASE);
        return *this;
    }

    BigInt(const BigInt& other) {
        sgn = other.sgn;
        a = other.a;
    }

    friend void swap(BigInt& a, BigInt& b) {
        swap(a.sgn, b.sgn);
        swap(a.a, b.a);
    }

    BigInt& operator=(BigInt other) {
        swap(*this, other);
        return *this;
    }

    BigInt(const string& s) { read(s); }

    void read(const string& s) {
        sgn = 1;
        a.clear();
        int k = 0;
        for (; k < sz(s) && (s[k] == '-' || s[k] == '+'); k++)
            if (s[k] == '-') sgn = -sgn;
        for (int i = sz(s) - 1; i >= k; i -= DIG) {
            int x = 0;
            for (int j = max(k, i - DIG + 1); j <= i; j++) x = x * 10 + s[j] - '0';
            a.push_back(x);
        }
        trim();
    }

    friend istream& operator>>(istream& in, BigInt& v) {
        string s;
        in >> s;
        v.read(s);
        return in;
    }

    friend ostream& operator<<(ostream& out, const BigInt& v) {
        if (v.sgn == -1 && !v.zero()) out << '-';
        out << (v.a.empty() ? 0 : v.a.back());
        for (int i = sz(v.a) - 2; i >= 0; --i) out << setw(DIG) << setfill('0') << v.a[i];
        return out;
    }

    bool operator<(const BigInt& v) const {
        if (sgn != v.sgn) return sgn < v.sgn;
        if (sz(a) != sz(v.a)) return sz(a) * sgn < sz(v.a) * v.sgn;
        for (int i = sz(a) - 1; i >= 0; i--)
            if (a[i] != v.a[i]) return a[i] * sgn < v.a[i] * sgn;
        return 0;
    }

    bool operator>(const BigInt& v) const { return v < *this; }

    bool operator<=(const BigInt& v) const { return !(v < *this); }

    bool operator>=(const BigInt& v) const { return !(*this < v); }

    bool operator==(const BigInt& v) const { return !(*this < v) && !(v < *this); }

    bool operator!=(const BigInt& v) const { return *this < v || v < *this; }

    friend int __cmp(const BigInt& x, const BigInt& y) {
        if (sz(x.a) != sz(y.a)) return sz(x.a) < sz(y.a) ? -1 : 1;
        for (int i = sz(x.a) - 1; i >= 0; --i)
            if (x.a[i] != y.a[i]) return x.a[i] < y.a[i] ? -1 : 1;
        return 0;
    }

    BigInt operator-() const {
        BigInt res = *this;
        if (zero()) return res;
        res.sgn = -sgn;
        return res;
    }

    void __add(const BigInt& v) {
        if (sz(a) < sz(v.a)) a.resize(sz(v.a), 0);
        for (int i = 0, carry = 0; i < max(sz(a), sz(v.a)) || carry; ++i) {
            if (i == sz(a)) a.push_back(0);
            a[i] += carry + (i < sz(v.a) ? v.a[i] : 0);
            carry = a[i] >= BASE;
            if (carry) a[i] -= BASE;
        }
    }

    void __sub(const BigInt& v) {
        for (int i = 0, carry = 0; i < sz(v.a) || carry; ++i) {
            a[i] -= carry + (i < sz(v.a) ? v.a[i] : 0);
            carry = a[i] < 0;
            if (carry) a[i] += BASE;
        }
        trim();
    }

    BigInt operator+=(const BigInt& v) {
        if (sgn == v.sgn)
            __add(v);
        else if (__cmp(*this, v) >= 0)
            __sub(v);
        else {
            BigInt vv = v;
            swap(*this, vv);
            __sub(vv);
        }
        return *this;
    }

    BigInt operator-=(const BigInt& v) {
        if (sgn == v.sgn) {
            if (__cmp(*this, v) >= 0)
                __sub(v);
            else {
                BigInt vv = v;
                swap(*this, vv);
                __sub(vv);
                sgn = -sgn;
            }
        } else
            __add(v);
        return *this;
    }

    BigInt operator+(const BigInt& v) const {
        BigInt res = *this;
        res += v;
        return res;
    }

    BigInt operator-(const BigInt& v) const {
        BigInt res = *this;
        res -= v;
        return res;
    }

    BigInt operator*(const BigInt& v) const {
        BigInt res;
        res.sgn = sgn * v.sgn;
        res.a.resize(sz(a) + sz(v.a));
        for (int i = 0; i < sz(a); i++) {
            if (a[i]) {
                long long carry = 0;
                for (int j = 0; j < sz(v.a) || carry; j++) {
                    long long cur = res.a[i + j] + (long long)a[i] * (j < sz(v.a) ? v.a[j] : 0) + carry;
                    carry = cur / BASE;
                    res.a[i + j] = cur % BASE;
                }
            }
        }
        res.trim();
        return res;
    }

    void operator*=(const BigInt& v) { *this = *this * v; }

    void operator*=(int v) {
        if (abs(v) >= BASE) {
            *this *= BigInt(v);
            return;
        }
        if (v < 0) sgn = -sgn, v = -v;
        for (int i = 0, carry = 0; i < sz(a) || carry; ++i) {
            if (i == sz(a)) a.push_back(0);
            long long cur = a[i] * (long long)v + carry;
            carry = cur / BASE;
            a[i] = cur % BASE;
        }
        trim();
    }

    BigInt operator*(int v) const {
        BigInt res = *this;
        res *= v;
        return res;
    }

    friend pair<BigInt, BigInt> divmod(const BigInt& a1, const BigInt& b1) {
        long long norm = BASE / (b1.a.back() + 1);
        BigInt a = a1.abs() * norm, b = b1.abs() * norm, q = 0, r = 0;
        q.a.resize(sz(a.a));
        for (int i = sz(a.a) - 1; i >= 0; i--) {
            r *= BASE;
            r += a.a[i];
            long long s1 = sz(r.a) <= sz(b.a) ? 0 : r.a[sz(b.a)];
            long long s2 = sz(r.a) <= sz(b.a) - 1 ? 0 : r.a[sz(b.a) - 1];
            long long d = ((long long)BASE * s1 + s2) / b.a.back();
            r -= b * d;
            while (r < 0) r += b, --d;
            q.a[i] = d;
        }
        q.sgn = a1.sgn * b1.sgn;
        r.sgn = a1.sgn;
        q.trim();
        r.trim();
        auto res = make_pair(q, r / norm);
        if (res.second < 0) res.second += b1;
        return res;
    }

    BigInt operator/(const BigInt& v) const { return divmod(*this, v).first; }

    BigInt operator%(const BigInt& v) const { return divmod(*this, v).second; }

    void operator/=(int v) {
        if (abs(v) >= BASE) {
            *this /= BigInt(v);
            return;
        }
        if (v < 0) sgn = -sgn, v = -v;
        for (int i = sz(a) - 1, rem = 0; i >= 0; --i) {
            long long cur = a[i] + rem * (long long)BASE;
            a[i] = cur / v;
            rem = cur % v;
        }
        trim();
    }

    BigInt operator/(int v) const {
        if (abs(v) >= BASE) return *this / BigInt(v);
        BigInt res = *this;
        res /= v;
        return res;
    }

    void operator/=(const BigInt& v) { *this = *this / v; }

    long long operator%(long long v) const {
        long long m = 0;
        for (int i = sz(a) - 1; i >= 0; --i) m = (a[i] + m * (long long)BASE) % v;
        return m * sgn;
    }

    BigInt abs() const {
        BigInt res = *this;
        res.sgn = 1;
        return res;
    }

    void trim() {
        while (!a.empty() && !a.back()) a.pop_back();
    }

    bool zero() const { return a.empty() || (sz(a) == 1 && !a[0]); }
};