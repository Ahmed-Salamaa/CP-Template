/*
 * Topic: Polygon
 * Description: Provides operations on 2D Polygons, such as computing the area via the Shoelace
 *   formula, checking for convexity, and Point-in-Polygon (Ray-Casting) tests.
 * a point strictly or weakly lies inside a polygon, computing the area, and half-plane intersections. Usage:
 *    vector<Point> pts = {Point(0,0), Point(2,0), Point(0,2)};
 *    double a = polygonArea(pts);
 *    bool in = pointInPolygon(Point(1, 1), pts);
 *    vector<Point> hull = convexHull(pts);
 */
#include "../core.h"
#pragma once
#include <algorithm>
#include <cmath>
#include <vector>

#include "Line.cpp"
#include "Point.cpp"

// Calculates: Represents a polygon using a list of vertices.
// Returns: A polygon object.
// Time Complexity: O(1)
// Space Complexity: O(n)
template <typename T>
struct polygon {
    vector<point<T>> vertices;

    polygon() {}

    polygon(const vector<point<T>>& v) : vertices(v) {}
};

// Calculates: Represents a regular polygon (all sides and angles equal).
// Returns: A regular polygon object with computed vertices.
// Time Complexity: O(1)
// Space Complexity: O(n)
template <typename T>
struct regular_polygon {
    vector<point<T>> vertices;
    int n;
    point<T> center;
    double radius;

    regular_polygon() : n(0), center(), radius(0.0) {}

    // Calculates: Initializes a regular polygon from its center, circumradius, and number of sides.
    // Returns: A constructed regular_polygon.
    // Time Complexity: O(n)
    // Space Complexity: O(n)
    regular_polygon(const point<T>& c, double r, int num_sides, double starting_angle = 0)
        : n(num_sides), center(c), radius(r) {
        if (n < 3) {
            n = 0;
            return;
        }
        if (radius < 0) radius = -radius;
        vertices.resize(n);
        double angle_step = 2.0 * PI / n;
        for (int i = 0; i < n; i++) {
            double ang = starting_angle + i * angle_step;
            vertices[i] = point<T>((T)(center.x + radius * cos(ang)), (T)(center.y + radius * sin(ang)));
        }
    }

    // Calculates: Initializes a regular polygon from its side length and number of sides (centered at the origin).
    // Returns: A constructed regular_polygon.
    // Time Complexity: O(n)
    // Space Complexity: O(n)
    regular_polygon(int num_sides, double side_length) : n(num_sides), center(point<T>(0, 0)) {
        if (n < 3) {
            n = 0;
            return;
        }
        double s = sin(PI / n);
        radius = abs(side_length) / (2.0 * s);
        vertices.resize(n);
        double angle_step = 2.0 * PI / n;
        for (int i = 0; i < n; i++) {
            double ang = i * angle_step;
            vertices[i] = point<T>((T)(radius * cos(ang)), (T)(radius * sin(ang)));
        }
    }

    // Calculates: Initializes a regular polygon from its center, one known vertex, and number of sides.
    // Returns: A constructed regular_polygon.
    // Time Complexity: O(n)
    // Space Complexity: O(n)
    regular_polygon(const point<T>& c, const point<T>& first_vertex, int num_sides) : n(num_sides), center(c) {
        if (n < 3) {
            n = 0;
            return;
        }
        radius = distance(c, first_vertex);
        double starting_angle = atan2((double)(first_vertex.y - c.y), (double)(first_vertex.x - c.x));
        vertices.resize(n);
        double angle_step = 2.0 * PI / n;
        for (int i = 0; i < n; i++) {
            double ang = starting_angle + i * angle_step;
            vertices[i] = point<T>((T)(center.x + radius * cos(ang)), (T)(center.y + radius * sin(ang)));
        }
    }
};

// Calculates: The area of a given polygon.
// Returns: A double representing the area.
// Time Complexity: O(n)
// Space Complexity: O(1)
template <typename T>
double polygon_area(const polygon<T>& poly) {
    if (sz(poly.vertices) < 3) return 0.0;
    ld sum = 0.0L;
    int n = sz(poly.vertices);
    for (int i = 0; i < n; i++) {
        int j = (i + 1) % n;
        sum += (ld)poly.vertices[i].x * poly.vertices[j].y - (ld)poly.vertices[i].y * poly.vertices[j].x;
    }
    return fabs((double)(sum * 0.5L));
}

// Calculates: The perimeter length of a given polygon.
// Returns: A double representing the perimeter.
// Time Complexity: O(n)
// Space Complexity: O(1)
template <typename T>
double polygon_perimeter(const polygon<T>& poly) {
    double p = 0;
    int n = sz(poly.vertices);
    for (int i = 0; i < n; i++) { p += distance(poly.vertices[i], poly.vertices[(i + 1) % n]); }
    return p;
}

// Calculates: The centroid (center of mass) of a given polygon.
// Returns: A point representing the centroid.
// Time Complexity: O(n)
// Space Complexity: O(1)
template <typename T>
point<double> polygon_centroid(const polygon<T>& poly) {
    double cx = 0, cy = 0;
    double area2 = 0;
    int n = sz(poly.vertices);
    for (int i = 0; i < n; i++) {
        int j = (i + 1) % n;
        double cross = cross_product(poly.vertices[i], poly.vertices[j]);
        area2 += cross;
        cx += (poly.vertices[i].x + poly.vertices[j].x) * cross;
        cy += (poly.vertices[i].y + poly.vertices[j].y) * cross;
    }
    area2 *= 3.0;
    if (abs(area2) <= EPS) return point<double>(0, 0);  // fallback
    return point<double>(cx / area2, cy / area2);
}

// Calculates: Whether a given polygon is strictly convex.
// Returns: True if convex, false otherwise.
// Time Complexity: O(n)
// Space Complexity: O(1)
template <typename T>
bool is_convex(const polygon<T>& poly) {
    int n = sz(poly.vertices);
    if (n < 3) return false;
    int sign = 0;
    for (int i = 0; i < n; i++) {
        ld c = cross_val(poly.vertices[i], poly.vertices[(i + 1) % n], poly.vertices[(i + 2) % n]);
        if (abs(c) <= EPS) continue;
        int cur = (c > 0 ? 1 : -1);
        if (sign == 0)
            sign = cur;
        else if (sign != cur)
            return false;
    }
    return true;
}

// Calculates: Whether a point lies inside a general polygon using the winding number algorithm.
// Returns: True if inside (or on the boundary if included), false otherwise.
// Time Complexity: O(n)
// Space Complexity: O(1)
template <typename T>
bool point_in_polygon(const point<T>& p, const polygon<T>& poly, bool include_boundary = true) {
    int n = sz(poly.vertices);
    if (n < 3) return false;
    int winding = 0;
    for (int i = 0; i < n; i++) {
        int j = (i + 1) % n;
        if (include_boundary && on_segment(p, poly.vertices[i], poly.vertices[j])) return true;
        const point<T>& a = poly.vertices[i];
        const point<T>& b = poly.vertices[j];
        if (a.y <= p.y) {
            if (b.y > p.y) {
                if (cross_val(a, b, p) > EPS) winding++;
            }
        } else {
            if (b.y <= p.y) {
                if (cross_val(a, b, p) < -EPS) winding--;
            }
        }
    }
    return winding != 0;
}

// Calculates: The remaining polygon after cutting it with a line (Sutherland-Hodgman clipping).
// Returns: A polygon representing the kept portion.
// Time Complexity: O(n)
// Space Complexity: O(n)
template <typename T>
polygon<T> cut_polygon(const polygon<T>& poly, const line<T>& cutting_line, bool keep_left = true) {
    vector<point<T>> out;
    int n = sz(poly.vertices);
    if (n == 0) return polygon<T>();
    for (int i = 0; i < n; i++) {
        const point<T>& cur = poly.vertices[i];
        const point<T>& nxt = poly.vertices[(i + 1) % n];
        ld sc = cross_val(cutting_line.a, cutting_line.b, cur);
        ld sn = cross_val(cutting_line.a, cutting_line.b, nxt);
        bool in_cur = keep_left ? (sc >= -EPS) : (sc <= EPS);
        bool in_nxt = keep_left ? (sn >= -EPS) : (sn <= EPS);
        if (in_cur && in_nxt) {
            out.push_back(nxt);
        } else if (in_cur && !in_nxt) {
            auto intersection = line_intersection(line<T>(cur, nxt), cutting_line);
            if (intersection.first) out.push_back(intersection.second);
        } else if (!in_cur && in_nxt) {
            auto intersection = line_intersection(line<T>(cur, nxt), cutting_line);
            if (intersection.first) out.push_back(intersection.second);
            out.push_back(nxt);
        }
    }
    return polygon<T>(out);
}

// Calculates: The intersection of two convex polygons.
// Returns: A polygon representing the intersection region.
// Time Complexity: O(n * m)
// Space Complexity: O(n + m)
template <typename T>
polygon<T> polygon_intersection(const polygon<T>& p1, const polygon<T>& p2) {
    polygon<T> result = p1;
    int n2 = sz(p2.vertices);
    if (n2 < 3) return polygon<T>();
    ld sum = 0;
    for (int i = 0; i < n2; ++i) { sum += cross_product(p2.vertices[i], p2.vertices[(i + 1) % n2]); }
    bool ccw = (sum >= 0);
    for (int i = 0; i < n2 && !result.vertices.empty(); i++) {
        line<T> edge(p2.vertices[i], p2.vertices[(i + 1) % n2]);
        result = cut_polygon(result, edge, ccw);
    }
    return result;
}

// Calculates: Whether a point lies inside a convex polygon in logarithmic time.
// Returns: True if inside or on boundary, false otherwise.
// Time Complexity: O(log n)
// Space Complexity: O(1)
template <typename T>
bool point_in_convex_polygon(const point<T>& p, const polygon<T>& poly) {
    int n = sz(poly.vertices);
    if (n < 3) return false;
    const auto& v = poly.vertices;
    if (orientation(v[0], v[1], p) < 0 || orientation(v[0], v[n - 1], p) > 0) return false;
    int l = 1, r = n - 1;
    while (r - l > 1) {
        int mid = l + (r - l) / 2;
        if (orientation(v[0], v[mid], p) >= 0)
            l = mid;
        else
            r = mid;
    }
    return orientation(v[l], v[r], p) >= 0;
}

// Calculates: The greatest common divisor (GCD) of two integers.
// Returns: The GCD.
// Time Complexity: O(log(min(a, b)))
inline int cp_gcd(int a, int b) {
    while (b) {
        a %= b;
        swap(a, b);
    }
    return a;
}

// Calculates: The number of integer lattice points on the boundary of a polygon.
// Returns: An integer representing the count of boundary points.
// Time Complexity: O(n)
// Space Complexity: O(1)
template <typename T>
int boundary_lattice_points(const polygon<T>& poly) {
    int b = 0;
    int n = sz(poly.vertices);
    for (int i = 0; i < n; i++) {
        int j = (i + 1) % n;
        int dx = abs((int)poly.vertices[i].x - (int)poly.vertices[j].x);
        int dy = abs((int)poly.vertices[i].y - (int)poly.vertices[j].y);
        b += cp_gcd(dx, dy);
    }
    return b;
}

// Calculates: The number of strictly interior integer lattice points of a polygon using Pick's Theorem.
// Returns: An integer representing the count of interior points.
// Time Complexity: O(n)
// Space Complexity: O(1)
template <typename T>
int interior_lattice_points(const polygon<T>& poly) {
    int A2 = round(polygon_area(poly) * 2.0);  // 2 * Area
    int b = boundary_lattice_points(poly);
    return (A2 - b + 2) / 2;
}

// Calculates: The Minkowski sum of two convex polygons.
// Returns: A polygon representing the Minkowski sum.
// Time Complexity: O(n + m)
// Space Complexity: O(n + m)
template <typename T>
polygon<T> minkowski_sum(const polygon<T>& P, const polygon<T>& Q) {
    auto reorder = [](vector<point<T>> pts) {
        int pos = 0;
        for (int i = 1; i < sz(pts); i++) {
            if (pts[i].y < pts[pos].y || (pts[i].y == pts[pos].y && pts[i].x < pts[pos].x)) pos = i;
        }
        rotate(pts.begin(), pts.begin() + pos, pts.end());
        return pts;
    };
    vector<point<T>> p1 = reorder(P.vertices);
    vector<point<T>> p2 = reorder(Q.vertices);
    int n = sz(p1), m = sz(p2);
    if (n == 0 || m == 0) return polygon<T>();
    p1.push_back(p1[0]);
    p1.push_back(p1[1]);
    p2.push_back(p2[0]);
    p2.push_back(p2[1]);
    vector<point<T>> res;
    int i = 0, j = 0;
    while (i < n || j < m) {
        res.push_back(p1[i] + p2[j]);
        double cross = cross_val(point<T>(0, 0), p1[i + 1] - p1[i], p2[j + 1] - p2[j]);
        if (cross >= 0 && i < n) ++i;
        if (cross <= 0 && j < m) ++j;
    }
    return polygon<T>(res);
}

// Calculates: Exact inclusion test for a point in a polygon using integer arithmetic.
// Returns: 0 if OUTSIDE, 1 if INSIDE, 2 if BOUNDARY.
// Time Complexity: O(n)
// Space Complexity: O(1)
int point_in_polygon_exact(const vector<pair<int, int>>& poly, const pair<int, int>& p) {
    auto cross128 = [](int ax, int ay, int bx, int by) -> __int128 { return (__int128)ax * by - (__int128)ay * bx; };
    auto orient = [&](const pair<int, int>& a, const pair<int, int>& b, const pair<int, int>& pt) -> __int128 {
        return cross128(b.first - a.first, b.second - a.second, pt.first - a.first, pt.second - a.second);
    };
    auto on_segment_exact = [&](const pair<int, int>& a, const pair<int, int>& b, const pair<int, int>& pt) {
        if (orient(a, b, pt) != 0) return false;
        int minx = min(a.first, b.first), maxx = max(a.first, b.first);
        int miny = min(a.second, b.second), maxy = max(a.second, b.second);
        return (pt.first >= minx && pt.first <= maxx && pt.second >= miny && pt.second <= maxy);
    };
    int n = sz(poly);
    for (int i = 0; i < n; ++i) {
        if (on_segment_exact(poly[i], poly[(i + 1) % n], p)) return 2;
    }
    int wn = 0;
    for (int i = 0; i < n; ++i) {
        int j = (i + 1) % n;
        int yi = poly[i].second, yj = poly[j].second;
        if (yi <= p.second) {
            if (yj > p.second && orient(poly[i], poly[j], p) > 0) ++wn;
        } else {
            if (yj <= p.second && orient(poly[i], poly[j], p) < 0) --wn;
        }
    }
    return wn == 0 ? 0 : 1;
}

// Calculates: Detailed inclusion test for a point in a strictly CCW convex polygon.
// Returns: 0 if outside, 1 if inside, 2 if on the boundary.
// Time Complexity: O(log n)
// Space Complexity: O(1)
template <typename T>
int point_in_convex_polygon_strict(const point<T>& p, const polygon<T>& poly) {
    const auto& h = poly.vertices;
    int n = sz(h);
    if (n == 0) return 0;
    if (n == 1) return (distance(h[0], p) <= EPS) ? 2 : 0;
    if (n == 2) return (abs(cross_val(h[0], h[1], p)) <= EPS && dot_product(p - h[0], p - h[1]) <= EPS) ? 2 : 0;
    ld c1 = cross_val(h[0], h[1], p), c2 = cross_val(h[0], h[n - 1], p);
    if (c1 < -EPS || c2 > EPS) return 0;
    if (abs(c1) <= EPS) return dot_product(p - h[0], p - h[1]) <= EPS ? 2 : 0;
    if (abs(c2) <= EPS) return dot_product(p - h[0], p - h[n - 1]) <= EPS ? 2 : 0;
    int l = 1, r = n - 2, idx = -1;
    while (l <= r) {
        int mid = (l + r) / 2;
        if (cross_val(h[0], h[mid], p) >= -EPS) {
            idx = mid;
            l = mid + 1;
        } else {
            r = mid - 1;
        }
    }
    ld v = cross_val(h[idx], h[idx + 1], p);
    return v < -EPS ? 0 : (v <= EPS ? 2 : 1);
}
