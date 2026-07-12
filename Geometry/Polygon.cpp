#pragma once
#include "Point.cpp"
#include "Line.cpp"
#include <vector>
#include <cmath>
#include <algorithm>

template <typename T>
struct polygon {
    std::vector<point<T>> vertices;
    polygon() {}
    polygon(const std::vector<point<T>>& v) : vertices(v) {}
};

template <typename T>
struct regular_polygon {
    std::vector<point<T>> vertices;
    int n;
    point<T> center;
    double radius;

    regular_polygon() : n(0), center(), radius(0.0) {}

    // Time Complexity: O(n)
    // Space Complexity: O(n)
    regular_polygon(const point<T>& c, double r, int num_sides, double starting_angle = 0)
        : n(num_sides), center(c), radius(r) {
        if (n < 3) { n = 0; return; }
        if (radius < 0) radius = -radius;
        vertices.resize(n);
        double angle_step = 2.0 * cp_geometry::PI / n;
        for (int i = 0; i < n; i++) {
            double ang = starting_angle + i * angle_step;
            vertices[i] = point<T>((T)(center.x + radius * cos(ang)), (T)(center.y + radius * sin(ang)));
        }
    }

    // From side length and number of sides (centered at origin)
    // Time Complexity: O(n)
    // Space Complexity: O(n)
    regular_polygon(int num_sides, double side_length) : n(num_sides), center(point<T>(0, 0)) {
        if (n < 3) { n = 0; return; }
        double s = std::sin(cp_geometry::PI / n);
        radius = std::abs(side_length) / (2.0 * s);
        vertices.resize(n);
        double angle_step = 2.0 * cp_geometry::PI / n;
        for (int i = 0; i < n; i++) {
            double ang = i * angle_step;
            vertices[i] = point<T>((T)(radius * std::cos(ang)), (T)(radius * std::sin(ang)));
        }
    }

    // From center, a known first vertex, and side count
    // Time Complexity: O(n)
    // Space Complexity: O(n)
    regular_polygon(const point<T>& c, const point<T>& first_vertex, int num_sides) : n(num_sides), center(c) {
        if (n < 3) { n = 0; return; }
        radius = distance(c, first_vertex);
        double starting_angle = std::atan2((double)(first_vertex.y - c.y), (double)(first_vertex.x - c.x));
        vertices.resize(n);
        double angle_step = 2.0 * cp_geometry::PI / n;
        for (int i = 0; i < n; i++) {
            double ang = starting_angle + i * angle_step;
            vertices[i] = point<T>((T)(center.x + radius * std::cos(ang)), (T)(center.y + radius * std::sin(ang)));
        }
    }
};

// Time Complexity: O(n)
// Space Complexity: O(1)
template <typename T>
double polygon_area(const polygon<T>& poly) {
    if (poly.vertices.size() < 3) return 0.0;
    long double sum = 0.0L;
    int n = poly.vertices.size();
    for (int i = 0; i < n; i++) {
        int j = (i + 1) % n;
        sum += (long double)poly.vertices[i].x * poly.vertices[j].y - (long double)poly.vertices[i].y * poly.vertices[j].x;
    }
    return std::fabs((double)(sum * 0.5L));
}

// Time Complexity: O(n)
// Space Complexity: O(1)
template <typename T>
double polygon_perimeter(const polygon<T>& poly) {
    double p = 0;
    int n = poly.vertices.size();
    for (int i = 0; i < n; i++) {
        p += distance(poly.vertices[i], poly.vertices[(i + 1) % n]);
    }
    return p;
}

// Time Complexity: O(n)
// Space Complexity: O(1)
template <typename T>
point<double> polygon_centroid(const polygon<T>& poly) {
    double cx = 0, cy = 0;
    double area2 = 0;
    int n = poly.vertices.size();
    for (int i = 0; i < n; i++) {
        int j = (i + 1) % n;
        double cross = cross_product(poly.vertices[i], poly.vertices[j]);
        area2 += cross;
        cx += (poly.vertices[i].x + poly.vertices[j].x) * cross;
        cy += (poly.vertices[i].y + poly.vertices[j].y) * cross;
    }
    area2 *= 3.0;
    if (std::abs(area2) <= cp_geometry::EPS) return point<double>(0, 0); // fallback
    return point<double>(cx / area2, cy / area2);
}

// Time Complexity: O(n)
// Space Complexity: O(1)
template <typename T>
bool is_convex(const polygon<T>& poly) {
    int n = poly.vertices.size();
    if (n < 3) return false;
    int sign = 0;
    for (int i = 0; i < n; i++) {
        long double c = cross_val(poly.vertices[i], poly.vertices[(i + 1) % n], poly.vertices[(i + 2) % n]);
        if (std::abs(c) <= cp_geometry::EPS) continue;
        int cur = (c > 0 ? 1 : -1);
        if (sign == 0) sign = cur;
        else if (sign != cur) return false;
    }
    return true;
}

// Winding number algorithm
// Time Complexity: O(n)
// Space Complexity: O(1)
template <typename T>
bool point_in_polygon(const point<T>& p, const polygon<T>& poly, bool include_boundary = true) {
    int n = poly.vertices.size();
    if (n < 3) return false;
    
    int winding = 0;
    for (int i = 0; i < n; i++) {
        int j = (i + 1) % n;
        if (include_boundary && on_segment(p, poly.vertices[i], poly.vertices[j])) return true;
        
        const point<T>& a = poly.vertices[i];
        const point<T>& b = poly.vertices[j];
        
        if (a.y <= p.y) {
            if (b.y > p.y) {
                if (cross_val(a, b, p) > cp_geometry::EPS) winding++;
            }
        } else {
            if (b.y <= p.y) {
                if (cross_val(a, b, p) < -cp_geometry::EPS) winding--;
            }
        }
    }
    return winding != 0;
}

// Sutherland-Hodgman clipping against a line
// Time Complexity: O(n)
// Space Complexity: O(n)
template <typename T>
polygon<T> cut_polygon(const polygon<T>& poly, const line<T>& cutting_line, bool keep_left = true) {
    std::vector<point<T>> out;
    int n = poly.vertices.size();
    if (n == 0) return polygon<T>();

    for (int i = 0; i < n; i++) {
        const point<T>& cur = poly.vertices[i];
        const point<T>& nxt = poly.vertices[(i + 1) % n];

        long double sc = cross_val(cutting_line.a, cutting_line.b, cur);
        long double sn = cross_val(cutting_line.a, cutting_line.b, nxt);

        bool in_cur = keep_left ? (sc >= -cp_geometry::EPS) : (sc <= cp_geometry::EPS);
        bool in_nxt = keep_left ? (sn >= -cp_geometry::EPS) : (sn <= cp_geometry::EPS);

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

// Convex Polygon intersection using Sutherland-Hodgman
// Time Complexity: O(n * m)
// Space Complexity: O(n + m)
template <typename T>
polygon<T> polygon_intersection(const polygon<T>& p1, const polygon<T>& p2) {
    polygon<T> result = p1;
    int n2 = p2.vertices.size();
    if (n2 < 3) return polygon<T>();
    
    long double sum = 0;
    for(int i=0; i<n2; ++i) {
        sum += cross_product(p2.vertices[i], p2.vertices[(i+1)%n2]);
    }
    bool ccw = (sum >= 0);

    for (int i = 0; i < n2 && !result.vertices.empty(); i++) {
        line<T> edge(p2.vertices[i], p2.vertices[(i + 1) % n2]);
        result = cut_polygon(result, edge, ccw);
    }
    return result;
}

// Monotone Chain algorithm
// Time Complexity: O(n log n)
// Space Complexity: O(n)
template <typename T>
std::vector<point<T>> convex_hull(std::vector<point<T>> pts, bool keep_collinear = false) {
    if (pts.size() <= 2) return pts;
    std::sort(pts.begin(), pts.end());
    pts.erase(std::unique(pts.begin(), pts.end()), pts.end());
    
    std::vector<point<T>> lower, upper;
    for (const auto& p : pts) {
        while (lower.size() >= 2) {
            long double c = cross_val(lower[lower.size() - 2], lower.back(), p);
            if (keep_collinear ? (c < -cp_geometry::EPS) : (c <= cp_geometry::EPS)) lower.pop_back();
            else break;
        }
        lower.push_back(p);
    }
    for (int i = (int)pts.size() - 1; i >= 0; i--) {
        const auto& p = pts[i];
        while (upper.size() >= 2) {
            long double c = cross_val(upper[upper.size() - 2], upper.back(), p);
            if (keep_collinear ? (c < -cp_geometry::EPS) : (c <= cp_geometry::EPS)) upper.pop_back();
            else break;
        }
        upper.push_back(p);
    }
    lower.pop_back();
    upper.pop_back();
    lower.insert(lower.end(), upper.begin(), upper.end());
    return lower;
}

// Rotating Calipers algorithm
// Time Complexity: O(n log n)
// Space Complexity: O(n)
template <typename T>
double polygon_diameter(const std::vector<point<T>>& pts) {
    std::vector<point<T>> hull = convex_hull(pts);
    int m = hull.size();
    if (m <= 1) return 0.0;
    if (m == 2) return distance(hull[0], hull[1]);
    
    double best = 0.0;
    int j = 1;
    for (int i = 0; i < m; i++) {
        int ni = (i + 1) % m;
        while (true) {
            int nj = (j + 1) % m;
            long double cur = cross_val(hull[i], hull[ni], hull[j]);
            long double nxt = cross_val(hull[i], hull[ni], hull[nj]);
            if (nxt > cur + cp_geometry::EPS) j = nj;
            else break;
        }
        best = std::max(best, distance(hull[i], hull[j]));
        best = std::max(best, distance(hull[ni], hull[j]));
    }
    return best;
}

// O(log n) inclusion test for convex polygon (vertices must be strictly CCW or CW, usually CCW)
// Time Complexity: O(log n)
// Space Complexity: O(1)
template <typename T>
bool point_in_convex_polygon(const point<T>& p, const polygon<T>& poly) {
    int n = poly.vertices.size();
    if (n < 3) return false;
    
    const auto& v = poly.vertices;
    if (orientation(v[0], v[1], p) < 0 || orientation(v[0], v[n - 1], p) > 0)
        return false;
        
    int l = 1, r = n - 1;
    while (r - l > 1) {
        int mid = l + (r - l) / 2;
        if (orientation(v[0], v[mid], p) >= 0) l = mid;
        else r = mid;
    }
    return orientation(v[l], v[r], p) >= 0;
}

// Helper GCD for Pick's Theorem
// Time Complexity: O(log(min(a, b)))
inline long long cp_gcd(long long a, long long b) {
    while (b) {
        a %= b;
        std::swap(a, b);
    }
    return a;
}

// Time Complexity: O(n)
// Space Complexity: O(1)
template <typename T>
long long boundary_lattice_points(const polygon<T>& poly) {
    long long b = 0;
    int n = poly.vertices.size();
    for (int i = 0; i < n; i++) {
        int j = (i + 1) % n;
        long long dx = std::abs((long long)poly.vertices[i].x - (long long)poly.vertices[j].x);
        long long dy = std::abs((long long)poly.vertices[i].y - (long long)poly.vertices[j].y);
        b += cp_gcd(dx, dy);
    }
    return b;
}

// Pick's Theorem: Area = Interior + Boundary/2 - 1
// Time Complexity: O(n)
// Space Complexity: O(1)
template <typename T>
long long interior_lattice_points(const polygon<T>& poly) {
    long long A2 = std::round(polygon_area(poly) * 2.0); // 2 * Area
    long long b = boundary_lattice_points(poly);
    return (A2 - b + 2) / 2;
}

// Minkowski sum of two convex polygons
// Time Complexity: O(n + m)
// Space Complexity: O(n + m)
template <typename T>
polygon<T> minkowski_sum(const polygon<T>& P, const polygon<T>& Q) {
    auto reorder = [](std::vector<point<T>> pts) {
        int pos = 0;
        for (int i = 1; i < (int)pts.size(); i++) {
            if (pts[i].y < pts[pos].y || (pts[i].y == pts[pos].y && pts[i].x < pts[pos].x))
                pos = i;
        }
        std::rotate(pts.begin(), pts.begin() + pos, pts.end());
        return pts;
    };
    std::vector<point<T>> p1 = reorder(P.vertices);
    std::vector<point<T>> p2 = reorder(Q.vertices);
    int n = p1.size(), m = p2.size();
    if (n == 0 || m == 0) return polygon<T>();
    p1.push_back(p1[0]); p1.push_back(p1[1]);
    p2.push_back(p2[0]); p2.push_back(p2[1]);
    std::vector<point<T>> res;
    int i = 0, j = 0;
    while (i < n || j < m) {
        res.push_back(p1[i] + p2[j]);
        double cross = cross_val(point<T>(0,0), p1[i + 1] - p1[i], p2[j + 1] - p2[j]);
        if (cross >= 0 && i < n) ++i;
        if (cross <= 0 && j < m) ++j;
    }
    return polygon<T>(res);
}
