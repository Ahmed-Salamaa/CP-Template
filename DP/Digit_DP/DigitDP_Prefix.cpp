/*
 * Topic: DP - Digit DP
 * Description: Digit DP (Digit Dynamic Programming) is an algorithmic technique used to count
 *   numbers with specific properties within a range [0, X] by building the number
 *   digit by digit.
 */
#include "../../core.h"

/*
 * Digit DP (Prefix)
 * -----------------
 * Solves problems asking for the number of integers in a range [0, X] that satisfy a property based on their digits.
 * Query range [L, R] by solving for R, then solving for L-1, and subtracting.
 *
 * Optimization Note: We only memoize states when `!is_tight` (meaning the prefix no longer restricts us).
 * When `!is_tight`, the answer depends ONLY on the remaining length (`rem`) and current state.
 * This allows reusing the SAME memoization table across MULTIPLE queries without clearing it!
 */
struct DigitDP {
    string limit_string;
    int target_modulo;
    // memo[rem][current_sum]
    vector<vector<int>> memo;

    // O ( 1 )
    DigitDP(int mod, int max_length = 20) : target_modulo(mod) {
        // Initialize memo once for the max possible number of digits.
        memo.assign(max_length + 5, vector<int>(mod + 5, -1));
    }

    // O ( 1 )
    void set_limit(string val) { limit_string = val; }

    // O ( N )
    // Helper to compute L-1 for massive string numbers when querying [L, R]
    static string subtract_one(string s) {
        for (int i = (int)s.size() - 1; i >= 0; i--) {
            if (s[i] > '0') {
                s[i]--;
                break;
            }
            s[i] = '9';
        }
        if (s[0] == '0' && s.size() > 1) s.erase(s.begin());
        return s;
    }

    // O ( N * MOD * 10 )
    int solve(int index = 0, bool is_tight = true, int current_sum = 0) {
        // Base case: we successfully built a number, check if it matches target conditions
        if (index == (int)limit_string.size()) return current_sum == 0 ? 1 : 0;
        int rem = (int)limit_string.size() - index;
        // We only use cached results if we are NOT tightly bound by the limit prefix
        if (!is_tight && memo[rem][current_sum] != -1) return memo[rem][current_sum];
        int result = 0;
        // Upper bound for the current digit
        int max_digit = is_tight ? limit_string[index] - '0' : 9;
        for (int digit = 0; digit <= max_digit; digit++) {
            // Check if we are still strictly following the limit prefix
            bool next_tight = is_tight && (digit == limit_string[index] - '0');
            // Example transition logic (modify this part for specific problem rules)
            int next_sum = (current_sum + digit) % target_modulo;
            result += solve(index + 1, next_tight, next_sum);
        }
        // Only cache the result if we were not tightly bound
        if (!is_tight) memo[rem][current_sum] = result;
        return result;
    }
};
