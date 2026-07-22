/*
 * Topic: DP - Line Optimizations
 * Description: Convex Hull Trick (CHT) optimizes DP transitions of the form dp[i] = min(m[j] *
 *   x[i] + c[j]) down to O(1) or O(log N) by maintaining the lower/upper envelope of
 *   lines.
 */
#include "../../core.h"

/*
 * Convex Hull Trick (CHT)
 * -----------------------
 * Optimizes DP transitions of the form: dp[i] = min_{j < i} (m[j] * x[i] + c[j])
 * where slopes m[j] are monotonic (either always increasing or decreasing).
 * Queries can be linear (if x[i] is monotonic) or binary search (if x[i] is arbitrary).
 */
struct ConvexHullTrick {
    vector<int> slopes, intercepts;
    int pointer = 1;
    bool is_max_mode;

    // O ( 1 )
    ConvexHullTrick(bool is_max_mode = false) : is_max_mode(is_max_mode) {
        // 1-based indexing initialization
        slopes.push_back(0);
        intercepts.push_back(0);
    }

    // O ( 1 )
    double get_intersection(int i, int j) { return (double)(intercepts[j] - intercepts[i]) / (slopes[i] - slopes[j]); }

    // O ( 1 )
    bool is_bad(int l1, int l2, int l3) { return get_intersection(l1, l3) <= get_intersection(l1, l2); }

    // O ( 1 )
    void add_line(int slope, int intercept) {
        // Invert slopes and intercepts to simulate MAX queries using a MIN hull
        if (is_max_mode) {
            slope = -slope;
            intercept = -intercept;
        }
        // Handle identical slopes by keeping only the optimal (lowest) intercept
        if (slopes.size() > 1 && slopes.back() == slope) {
            if (intercepts.back() <= intercept) return;
            slopes.pop_back();
            intercepts.pop_back();
        }
        slopes.push_back(slope);
        intercepts.push_back(intercept);
        int current_size = (int)slopes.size() - 1;
        // Remove lines that are no longer part of the lower convex envelope
        while (current_size >= 3 && is_bad(current_size - 2, current_size - 1, current_size)) {
            slopes.erase(slopes.end() - 2);
            intercepts.erase(intercepts.end() - 2);
            current_size--;
        }
    }

    // O ( 1 )
    int evaluate(int index, int x) { return slopes[index] * x + intercepts[index]; }

    // O ( 1 )
    int query_monotonic(int x) {
        if (slopes.size() <= 1) return 0;
        if (pointer >= (int)slopes.size()) pointer = (int)slopes.size() - 1;
        // Advance the pointer as long as the next line yields a better (lower) result
        while (pointer < (int)slopes.size() - 1 && evaluate(pointer + 1, x) <= evaluate(pointer, x)) pointer++;
        int answer = evaluate(pointer, x);
        return is_max_mode ? -answer : answer;  // Revert inversion if MAX mode
    }

    // O ( log(N) )
    int query_arbitrary(int x) {
        if (slopes.size() <= 1) return 0;
        int left = 1, right = (int)slopes.size() - 1;
        while (left < right) {
            int mid = left + (right - left) / 2;
            // If the next line evaluates lower, the true minimum lies to the right
            if (evaluate(mid + 1, x) <= evaluate(mid, x))
                left = mid + 1;
            else
                right = mid;
        }
        int answer = evaluate(left, x);
        return is_max_mode ? -answer : answer;  // Revert inversion if MAX mode
    }
};
