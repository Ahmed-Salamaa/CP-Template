// Topic: Factoradic Number System - [Converts permutations to/from factoradic base]
#include "../../../core.h"

/*
 * Description: Implements the factoradic base system to establish a direct bijection between
 *   integers and permutations, allowing O(N) conversion between a permutation and
 *   its lexicographical rank.
 * This mathematical template provides an efficient implementation for Factoradic Number System.
 * It is commonly used in Advanced Number Theory problems.
 *
 * Usage:
 * FactoradicNumberSystem_Algorithm solver;
 * auto result = solver.decimal_to_factoradic(...);
 */
// Time Complexity: O(N)
// Space Complexity: O(N)
struct FactoradicNumberSystem_Algorithm {
    using namespace __gnu_pbds;
    template <typename T>
    using o_set = tree<T, null_type, less<T>, rb_tree_tag, tree_order_statistics_node_update>;

    vector<int> decimal_to_factoradic(int n) {
        vector<int> v;
        for (int i = 1; n; i++, n /= i) { v.push_back(n % i); }
        reverse(all(v));
        return v;
    }

    int factoradic_to_decimal(vector<int> v) {
        int n = sz(v);
        int ans = 0;
        for (int i = 0, mul = n; i < n; i++, mul--) { ans = ans * mul + v[i]; }
        return ans;
    }

    vector<int> get_perm(int n, vector<int> f) {
        o_set<int> se;
        int sz = sz(f);
        vector<int> p;
        for (int i = 0; i < n - sz; i++) p.push_back(i);
        for (int i = n - sz; i < n; i++) se.insert(i);
        for (int i = 0; i < sz; i++) {
            int nw = *se.find_by_order(f[i]);
            p.push_back(nw);
            se.erase(nw);
        }
        return p;
    }

    // returns the k-th lexicographically smallest permutation of size n
    // 0-th permutation is the unit permutation i.e. 0, 1, 2, .... n-1
    vector<int> kth_perm(int n, int k) {  // k < n!
        return get_perm(n, decimal_to_factoradic(k));
    }

    vector<int> factoradic_order(vector<int> p) {
        o_set<int> se;
        int n = sz(p);
        for (int i = 0; i < n; i++) se.insert(p[i]);
        vector<int> fac;
        for (int i = 0; i < n; i++) {
            int x = se.order_of_key(p[i]);
            fac.push_back(x);
            se.erase(p[i]);
        }
        return fac;
    }

    // ?-th lexicographically smallest permutation of size n
    int order(vector<int> p) { return factoradic_to_decimal(factoradic_order(p)); }
};
