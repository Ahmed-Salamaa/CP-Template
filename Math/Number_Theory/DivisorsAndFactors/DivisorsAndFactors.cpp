/*
 * Topic: Divisors And Factors
 * NOTE: This template requires basic combinatorics. You must also include Math/Combinatorics/Basics/Combinatorics.cpp.
 * Description: A comprehensive collection of routines for finding all divisors and prime
 *   factors of numbers, often precalculating the smallest prime factor (SPF) via
 *   sieves for fast O(log N) multiple queries.
 * the sum of divisors (sigma(n)), and computing sums over ranges. Usage: vector<int> factors = get_factors(N); int d =
 * count_divisors(N); int s = sum_divisors(N);
 */
//
// Description:
// A collection of highly optimized math utilities related to divisor and factor calculations.
// It includes factorization, obtaining all divisors, sum of divisors, counting divisors,
// product of divisors, and range sums of divisors.
//
// How to use it:
// vector<int> factors = DivisorsAndFactors::prime_factorization(100);
// int total_divisors = DivisorsAndFactors::count_divisors(100);
#include "../../../core.h"

// Time Complexity: O(sqrt(n))
// Space Complexity: O(log(n))
vector<int> prime_factorization(int n) {
    vector<int> factorization;
    for (int d : {2, 3, 5}) {
        while (n % d == 0) {
            factorization.push_back(d);
            n /= d;
        }
    }
    static array<int, 8> increments = {4, 2, 4, 2, 4, 6, 2, 6};
    int i = 0;
    for (int d = 7; d * d <= n; d += increments[i++]) {
        while (n % d == 0) {
            factorization.push_back(d);
            n /= d;
        }
        if (i == 8) i = 0;
    }
    if (n > 1) factorization.push_back(n);
    return factorization;
}

// Time Complexity: O(sqrt(n))
// Space Complexity: O(log(n))
vector<pair<int, int>> get_factors(int n) {
    vector<pair<int, int>> factors;
    int d = 1;
    for (int i = 2; i * i <= n; i += d, d = 2) {
        if (n % i == 0) {
            factors.push_back({i, 0});
            while (n % i == 0) {
                n /= i;
                factors.back().second++;
            }
        }
    }
    if (n != 1) factors.push_back({n, 1});
    return factors;
}

// Time Complexity: O(sqrt(n))
// Space Complexity: O(d(n)) (where d(n) is number of divisors)
vector<int> get_divisors(int n) {
    vector<int> divs;
    for (int i = 1; i * i <= n; i++) {
        if (n % i == 0) {
            divs.push_back(i);
            if (i != n / i) divs.push_back(n / i);
        }
    }
    sort(all(divs));
    return divs;
}

// Time Complexity: O(sqrt(n))
// Space Complexity: O(1)
int count_divisors(int num) {
    int total = 1;
    for (int i = 2; (int)i * i <= num; i++) {
        if (num % i == 0) {
            int e = 0;
            do {
                e++;
                num /= i;
            } while (num % i == 0);
            total *= (e + 1);
        }
    }
    if (num > 1) { total *= 2; }
    return total;
}

// Time Complexity: O(sqrt(n))
// Space Complexity: O(1)
int SumOfDivisors(int num) {
    int total = 1;
    for (int i = 2; (int)i * i <= num; i++) {
        if (num % i == 0) {
            int e = 0;
            do {
                e++;
                num /= i;
            } while (num % i == 0);
            int sum = 0, pow = 1;
            do {
                sum += pow;
                pow *= i;
            } while (e-- > 0);
            total *= sum;
        }
    }
    if (num > 1) { total *= (1 + num); }
    return total;
}

// Time Complexity: O(log_val(n))
// Space Complexity: O(1)
int CountPrimeDivisors(int n, int val) {
    int res = 0;
    while (n >= val) {
        res += n / val;
        n /= val;
    }
    return res;
}

const int MOD = 1000000007LL;
const int MOD1 = 1000000006LL;

// Time Complexity: O(number of prime factors)
// Space Complexity: O(1)
int sumDivisors(const vector<pair<int, int>>& factors) {
    int ans = 1;
    for (auto& p : factors) {
        int x = p.first % MOD;
        int k = p.second;
        if (x == 1) {
            ans = (ans * ((k + 1) % MOD)) % MOD;
        } else {
            int numerator = (fast_power(x, k + 1, MOD) - 1 + MOD) % MOD;
            int denom_inv = fast_power((x - 1 + MOD) % MOD, MOD - 2, MOD);
            int term = ((unsigned __int128)numerator * denom_inv) % MOD;
            ans = (ans * term) % MOD;
        }
    }
    return ans;
}

// Time Complexity: O(number of prime factors)
// Space Complexity: O(number of prime factors)
int productDivisors(const vector<pair<int, int>>& orig_factors) {
    vector<pair<int, int>> factors = orig_factors;
    int num1 = 1;
    bool halvedOne = false;
    for (auto& p : factors) {
        int k = p.second;
        if (!halvedOne && (k % 2 == 1)) {
            num1 = (num1 * (((k + 1) / 2) % MOD1)) % MOD1;
            halvedOne = true;
        } else {
            num1 = (num1 * ((k + 1) % MOD1)) % MOD1;
        }
    }
    if (!halvedOne) {
        for (auto& p : factors) p.second /= 2;
    }
    int number = 1;
    for (auto& p : factors) {
        int x = p.first % MOD;
        int k = p.second;
        number = ((unsigned __int128)number * fast_power(x, k, MOD)) % MOD;
    }
    int prod = fast_power(number, num1, MOD);
    return prod;
}

// Time Complexity: O(1)
// Space Complexity: O(1)
unsigned __int128 sum(unsigned __int128 n) { return n * (n + 1) / 2; }

// Time Complexity: O(sqrt(n))
// Space Complexity: O(1)
unsigned __int128 sum_divisors_1_to_n(unsigned __int128 n, unsigned __int128 mod) {
    unsigned __int128 res = 0;
    for (unsigned __int128 i = 1; i * i <= n; i++) {
        unsigned __int128 l = i, r = n / i;
        res = res + i * (r - l + 1) + sum(r) - sum(l);
        res %= mod;
    }
    return res;
}
