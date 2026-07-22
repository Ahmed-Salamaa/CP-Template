/*
 * Topic: DP - Divide and Conquer Optimization
 * Description: Divide and Conquer Optimization reduces the time complexity of DP transitions of
 *   the form dp[i][j] = min(dp[i-1][k] + cost(k, j)) from O(K N^2) to O(K N log N)
 *   when the cost function satisfies the quadrangle inequality.
 */
#include "../../core.h"

/*
 * Divide and Conquer DP Optimization
 * ----------------------------------
 * Optimizes DP of the form: dp_curr[i] = min_{k <= i} (dp_before[k-1] + cost(k, i))
 * Condition: cost function must satisfy the quadrangle inequality (wider is worse).
 * C(a, c) + C(b, d) <= C(a, d) + C(b, c) for a <= b <= c <= d
 */
struct DivideAndConquerDP {
    int total_elements;
    vector<int> dp_before, dp_curr;

    // O ( N )
    DivideAndConquerDP(int total_elements)
        : total_elements(total_elements), dp_before(total_elements + 5, 0), dp_curr(total_elements + 5, 0) {}

    // O ( 1 )
    int get_cost(int left, int right) { return 0; }  // Replace with actual cost logic

    // O ( N log N )
    void compute(int left, int right, int opt_left, int opt_right) {
        if (left > right) return;
        int mid = left + (right - left) / 2;
        pair<int, int> best = {2e9, -1};  // Change to {-2e9, -1} if maximizing
        for (int k = opt_left; k <= min(mid, opt_right); k++) {
            // k represents the split point. dp_before[k - 1] is the optimal cost before placing k
            int current_cost = (k > 1 ? dp_before[k - 1] : 0) + get_cost(k, mid);
            if (current_cost < best.first) best = {current_cost, k};  // Change < to > if maximizing
        }
        dp_curr[mid] = best.first;
        int optimal_k = best.second;
        // The optimal split point for the left half must be <= optimal_k
        compute(left, mid - 1, opt_left, optimal_k);
        // The optimal split point for the right half must be >= optimal_k
        compute(mid + 1, right, optimal_k, opt_right);
    }

    // O ( K * N log N )
    int solve(int total_layers) {
        // 1-based indexing for DP layers and elements
        for (int i = 1; i <= total_elements; i++) dp_before[i] = get_cost(1, i);
        // i represents the current layer/partition we are computing
        for (int i = 2; i <= total_layers; i++) {
            compute(1, total_elements, 1, total_elements);
            vector<int> temp = dp_before;
            dp_before = dp_curr;
            dp_curr = temp;
        }
        return dp_before[total_elements];
    }
};
