/* Topic: Convex Hull
 * Description: Computes the Convex Hull of a set of 2D points in O(N log N) time using the
 *   Monotone Chain algorithm, finding the minimal convex polygon enclosing all
 *   points.
 * Usage:
 *    vector<point<T>> hull = convex_hull(pts);
 *    double diam = polygon_diameter(pts);
 */
#include "../core.h"
#include "Point.cpp"

// Calculates: The convex hull of a set of 2D points using the Monotone Chain algorithm.
// Returns: A vector of points representing the convex hull.
// Time Complexity: O(n log n)
// Space Complexity: O(n)
template <typename T>
vector<point<T>> convex_hull(vector<point<T>> pts, bool keep_collinear = false) {
    if (sz(pts) <= 2) return pts;
    sort(all(pts));
    pts.erase(unique(all(pts)), pts.end());
    vector<point<T>> lower, upper;
    for (const auto& p : pts) {
        while (sz(lower) >= 2) {
            ld c = cross_val(lower[sz(lower) - 2], lower.back(), p);
            if (keep_collinear ? (c < -EPS) : (c <= EPS))
                lower.pop_back();
            else
                break;
        }
        lower.push_back(p);
    }
    for (int i = sz(pts) - 1; i >= 0; i--) {
        const auto& p = pts[i];
        while (sz(upper) >= 2) {
            ld c = cross_val(upper[sz(upper) - 2], upper.back(), p);
            if (keep_collinear ? (c < -EPS) : (c <= EPS))
                upper.pop_back();
            else
                break;
        }
        upper.push_back(p);
    }
    lower.pop_back();
    upper.pop_back();
    lower.insert(lower.end(), all(upper));
    return lower;
}

// Calculates: The maximum distance between any two points in the given set (diameter) using Rotating Calipers.
// Returns: A double representing the maximum distance.
// Time Complexity: O(n log n)
// Space Complexity: O(n)
template <typename T>
double polygon_diameter(const vector<point<T>>& pts) {
    vector<point<T>> hull = convex_hull(pts);
    int m = sz(hull);
    if (m <= 1) return 0.0;
    if (m == 2) return distance(hull[0], hull[1]);
    double best = 0.0;
    int j = 1;
    for (int i = 0; i < m; i++) {
        int ni = (i + 1) % m;
        while (true) {
            int nj = (j + 1) % m;
            ld cur = cross_val(hull[i], hull[ni], hull[j]);
            ld nxt = cross_val(hull[i], hull[ni], hull[nj]);
            if (nxt > cur + EPS)
                j = nj;
            else
                break;
        }
        best = max(best, distance(hull[i], hull[j]));
        best = max(best, distance(hull[ni], hull[j]));
    }
    return best;
}
