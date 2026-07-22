// Topic: Prime Basis - [Maintains prime basis of a set of numbers]
#include "../../../core.h"

/*
 * Description: Prime Basis decomposes a set of numbers into a set of prime factors or a
 *   multiplicative basis. It is useful when analyzing the multiplicative properties
 *   or GCD/LCM dependencies of an array.
 * This mathematical template provides an efficient implementation for Prime Basis.
 * It is commonly used in Advanced Number Theory problems.
 *
 * Usage:
 * PrimeBasis_Algorithm solver;
 * auto result = solver.gcd(...);
 */
// Time Complexity: O(N)
// Space Complexity: O(N)
struct PrimeBasis_Algorithm {
    // https://codeforces.com/blog/entry/108053
    // for a set of n elements S, computes the basis of the set S
    // any element of S will be representable using the elements of the basis.
    // gcd(basis[i], basis[j]) == 1 for  all i < j
    // all x * y, x / y, gcd(x, y) and lcm(x, y) for x in S, y in S will also be representable
    // this also holds for all subsets of S
    // basis size = n * log_log(A) where A = max(S)
    // takes O(n^2 log_A) time to build the whole basis
    template <typename T>
    T gcd(T a, T b) {
        return b == 0 ? a : gcd(b, a % b);
    }

    template <typename T>
    struct PrimeBasis {
        void reduce_pair(T& x, T& y) {
            bool to_swap = 0;
            if (x > y) {
                to_swap ^= 1;
                swap(x, y);
            }
            while (x > 1 && y % x == 0) {
                y /= x;
                if (x > y) {
                    to_swap ^= 1;
                    swap(x, y);
                }
            }
            if (to_swap) swap(x, y);
        }

        vector<T> basis;

        void solve_inner(int pos, T& val) {
            while (basis[pos] % val == 0) basis[pos] /= val;
            vector<T> curr_basis = {basis[pos], val};
            int ptr = 1;
            while (ptr < sz(curr_basis)) {
                for (int i = 0; i < ptr; i++) {
                    reduce_pair(curr_basis[i], curr_basis[ptr]);
                    if (curr_basis[ptr] == 1) break;
                    if (curr_basis[i] == 1) break;
                    T g = gcd(curr_basis[ptr], curr_basis[i]);
                    if (g > 1) {
                        curr_basis[ptr] /= g;
                        curr_basis[i] /= g;
                        curr_basis.push_back(g);
                    }
                }
                ++ptr;
            }
            basis[pos] = curr_basis[0];
            val = curr_basis[1];
            for (int i = 2; i < sz(curr_basis); i++)
                if (curr_basis[i] > 1) basis.push_back(curr_basis[i]);
            if (basis[pos] == 1) {
                swap(basis[pos], basis.back());
                basis.pop_back();
            }
        }

        void add_element(T val) {
            for (int i = 0; i < sz(basis); i++) {
                reduce_pair(val, basis[i]);
                if (basis[i] == 1) {
                    swap(basis[i], basis.back());
                    basis.pop_back();
                    break;
                }
                if (val == 1) return;
                if (gcd(basis[i], val) > 1) solve_inner(i, val);
            }
            if (val > 1) basis.push_back(val);
        }

        void verify_basis() {
            for (int i = 0; i < sz(basis); i++) {
                for (int j = i + 1; j < sz(basis); j++) { assert(gcd(basis[i], basis[j]) == 1); }
            }
        }

        void verify_element(T x) {
            for (auto& k : basis) {
                while (x % k == 0) x /= k;
            }
            assert(x == 1);
        }

        vector<int> factorize(T x) {
            vector<int> factors(sz(basis));
            for (int i = 0; i < sz(basis); i++) {
                while (x % basis[i] == 0) {
                    factors[i]++;
                    x /= basis[i];
                }
            }
            return factors;
        }
    };

    vector<int> lcm(vector<int> a, vector<int> b) {
        for (int i = 0; i < sz(a); i++) { a[i] = max(a[i], b[i]); }
        return a;
    }

    using ll = int;
};
