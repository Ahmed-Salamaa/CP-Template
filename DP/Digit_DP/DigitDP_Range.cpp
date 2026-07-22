/*
 * Topic: DP - Digit DP
 * Description: Digit DP (Digit Dynamic Programming) is an algorithmic technique used to count
 *   numbers with specific properties within a large range [L, R].
 */
#include "../../core.h"

/*
 * Digit DP (Range)
 * ----------------
 * Solves problems asking for the number of integers in a range [L, R] that satisfy a property based on their digits.
 * Computes exactly for range [L, R] simultaneously. Faster if constraints on [L, R] must be tied together.
 *
 * Optimization Note: We only memoize states when `!not_smaller && !not_greater`.
 * When both limits no longer restrict us, the answer depends ONLY on the remaining length (`rem`) and current state.
 * This allows reusing the SAME memo table across multiple queries!
 */
struct DigitDP {
    string left_limit, right_limit;
    int target_modulo;
    // memo[rem][current_sum]
    vector<vector<int>> memo;

    // O ( 1 )
    DigitDP(int mod, int max_length = 20) : target_modulo(mod) {
        // Initialize memo once for the max possible number of digits.
        memo.assign(max_length + 5, vector<int>(mod + 5, -1));
    }

    // O ( 1 )
    void set_limits(string left_val, string right_val) {
        left_limit = left_val;
        right_limit = right_val;
        // Pad the left limit with leading zeros to match the right limit's length
        if (left_limit.size() < right_limit.size())
            left_limit = string(right_limit.size() - left_limit.size(), '0') + left_limit;
    }

    // O ( N * MOD * 10 )
    int solve(int index = 0, bool not_smaller = true, bool not_greater = true, int current_sum = 0) {
        // Base case: we successfully built a number, check if it matches target conditions
        if (index == (int)left_limit.size()) return current_sum == 0 ? 1 : 0;
        int rem = (int)left_limit.size() - index;
        // We only use cached results if we are completely free from both limits
        bool is_free = (!not_smaller && !not_greater);
        if (is_free && memo[rem][current_sum] != -1) return memo[rem][current_sum];
        int result = 0;
        // Bounds for the current digit based on our prefix flags
        int min_digit = not_smaller ? left_limit[index] - '0' : 0;
        int max_digit = not_greater ? right_limit[index] - '0' : 9;
        for (int digit = min_digit; digit <= max_digit; digit++) {
            // Update flags for the next recursive step
            bool next_not_smaller = not_smaller && (digit == left_limit[index] - '0');
            bool next_not_greater = not_greater && (digit == right_limit[index] - '0');
            // Example transition logic (modify this part for specific problem rules)
            int next_sum = (current_sum + digit) % target_modulo;
            result += solve(index + 1, next_not_smaller, next_not_greater, next_sum);
        }
        // Only cache the result if we were completely free
        if (is_free) memo[rem][current_sum] = result;
        return result;
    }
};
