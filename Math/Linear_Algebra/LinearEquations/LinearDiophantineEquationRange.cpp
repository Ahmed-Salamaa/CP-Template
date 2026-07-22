/*
 * Topic: Linear Diophantine Equation in Range - Counts solutions of ax + by = c in [x1, x2] and [y1, y2]
 * Description: Solves linear Diophantine equations of the form ax + by = c and extends to find
 *   the number of valid solutions bounded within specific given ranges for x and y.
 *   Used in resource allocation and integer partitioning problems.
 * within the specified ranges for x and y. Usage: int count = LinearDiophantineEquationRange::findAll(a, b, c, min_x,
 * max_x, min_y, max_y);
 */
#include "../../../core.h"

// Time Complexity: O(log(min(a, b)))
// Space Complexity: O(log(min(a, b)))
int gcdExtended(int a, int b, int& x, int& y) {
    if (a < 0) {
        int r = gcdExtended(-a, b, x, y);
        x *= -1;
        return r;
    }
    if (b < 0) {
        int r = gcdExtended(a, -b, x, y);
        y *= -1;
        return r;
    }
    if (b == 0) {
        x = 1;
        y = 0;
        return a;
    }
    int sol = gcdExtended(b, a % b, y, x);
    y -= (a / b) * x;
    return sol;
}

// Time Complexity: O(log(min(a, b)))
// Space Complexity: O(log(min(a, b)))
bool findAnySolution(int a, int b, int c, int& x0, int& y0, int& g) {
    g = gcdExtended(abs(a), abs(b), x0, y0);
    if (c % g) return false;
    x0 *= c / g;
    y0 *= c / g;
    if (a < 0) x0 = -x0;
    if (b < 0) y0 = -y0;
    g = abs(g);
    return true;
}

int floorDiv(int a, int b) {
    int q = a / b;
    if ((a ^ b) < 0 && a % b) --q;
    return q;
}

int ceilDiv(int a, int b) {
    int q = a / b;
    if ((a ^ b) >= 0 && a % b) ++q;
    return q;
}

// Time Complexity: O(log(min(a, b)))
// Space Complexity: O(log(min(a, b)))
int findAll(int a, int b, int c, int x1, int x2, int y1, int y2) {
    if (a == 0 && b == 0) { return (c == 0) ? (max(0LL, x2 - x1 + 1) * max(0LL, y2 - y1 + 1)) : 0; }
    if (a == 0) {
        if (c % b != 0) return 0;
        int y0 = c / b;
        if (y0 < y1 || y0 > y2) return 0;
        return max(0LL, x2 - x1 + 1);
    }
    if (b == 0) {
        if (c % a != 0) return 0;
        int x0 = c / a;
        if (x0 < x1 || x0 > x2) return 0;
        return max(0LL, y2 - y1 + 1);
    }
    int X = -1, Y = -1, g = -1;
    if (!findAnySolution(a, b, c, X, Y, g)) {
        return 0;
    } else {
        auto get = [&](int s, int lo, int hi, int delta, int sign) {
            int d = delta * sign;
            int L, R;
            if (d > 0) {
                L = ceilDiv(lo - s, d);
                R = floorDiv(hi - s, d);
            } else {
                L = ceilDiv(hi - s, d);
                R = floorDiv(lo - s, d);
            }
            return make_pair(L, R);
        };
        auto p1 = get(X, x1, x2, b / g, 1);
        auto p2 = get(Y, y1, y2, a / g, -1);
        auto ans = make_pair(max(p1.first, p2.first), min(p2.second, p1.second));
        return max(0LL, ans.second - ans.first + 1);
    }
}
