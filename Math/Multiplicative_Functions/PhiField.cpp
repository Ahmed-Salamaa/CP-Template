// Topic: Phi Field - [Computes prefix sums of Euler's totient function]
#include <cassert>

#include "../../../core.h"

/*
 * Description: This algorithm computes Euler's totient function or related field properties for
 *   modular arithmetic. It is essential for finding multiplicative inverses and
 *   analyzing the order of elements modulo a number.
 * NOTE: This template uses standard integers with manual modulo arithmetic for performance and simplicity. If the
 * problem involves extremely complex chaining of operations or you want to avoid manual modulo bugs, you can easily
 * swap the `int` types back to a `modint` template. This mathematical template provides an efficient implementation for
 * Phi Field. It is commonly used in Advanced Number Theory problems.
 *
 * Usage:
 * PhiField_Algorithm solver;
 * auto result = solver.fib(...);
 */
// Time Complexity: O(N^(2/3))
// Space Complexity: O(N^(2/3))
struct PhiField_Algorithm {
    static const int mod = 998244353;
    using ll = int;

    static int power(int base, int exp) {
        int res = 1;
        base %= mod;
        while (exp > 0) {
            if (exp % 2 == 1) res = (1LL * res * base) % mod;
            base = (1LL * base * base) % mod;
            exp /= 2;
        }
        return res;
    }

    static int inv(int n) { return power(n, mod - 2); }

    struct field {  // a . 1 + b . sqrt(5)
        int a, b;

        field(int a = 0, int b = 0) : a((a % mod + mod) % mod), b((b % mod + mod) % mod) {}

        bool operator==(const field& oth) const { return a == oth.a and b == oth.b; }

        field operator+(const field& oth) const { return field(a + oth.a, b + oth.b); }

        field operator-(const field& oth) const { return field(a - oth.a, b - oth.b); }

        field operator*(const field& oth) const {
            return field((1LL * a * oth.a % mod + 5LL * b * oth.b % mod) % mod,
                         (1LL * a * oth.b % mod + 1LL * b * oth.a % mod) % mod);
        }

        field operator*(const int& oth) const { return field((1LL * a * oth) % mod, (1LL * b * oth) % mod); }

        field inverse() {
            int d = inv((1LL * a * a % mod - 5LL * b * b % mod + mod) % mod);
            return field((1LL * a * d) % mod, (1LL * -b % mod * d % mod + mod) % mod);
        }

        field operator/(field oth) {
            field f = oth.inverse();
            return (*this) * f;
        }

        field operator/(const int& oth) const {
            int p = inv(oth);
            return field((1LL * a * p) % mod, (1LL * b * p) % mod);
        }

        field pow(ll k) {
            field ans(1, 0), cur = *this;
            while (k) {
                if (k & 1) ans = ans * cur;
                cur = cur * cur;
                k >>= 1;
            }
            return ans;
        }
    } phi;

    PhiField_Algorithm() : phi((mod + 1) / 2, (mod + 1) / 2) {}

    int fib(ll n) {
        field ans = phi.pow(n) - (field(1, 0) - phi).pow(n);
        ans = ans / field(0, 1);
        assert(ans.b == 0);
        return ans.a;
    }
};
