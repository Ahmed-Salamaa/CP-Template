#pragma once
#include "Point.cpp"
#include "Line.cpp"
#include <vector>
#include <cmath>
#include <algorithm>
#include <random>

template <typename T>
struct circle {
    point<T> c;
    double r;
    circle() : c(), r(0) {}
    circle(point<T> c, double r) : c(c), r(r) {}
};

// Time Complexity: O(1)
// Space Complexity: O(1)
template <typename T>
double circle_area(const circle<T>& circ) {
    return cp_geometry::PI * circ.r * circ.r;
}

// Time Complexity: O(1)
// Space Complexity: O(1)
template <typename T>
double circle_circumference(const circle<T>& circ) {
    return 2.0 * cp_geometry::PI * circ.r;
}

// Time Complexity: O(1)
// Space Complexity: O(1)
template <typename T>
bool is_point_in_circle(const point<T>& p, const circle<T>& circ) {
    return distance(p, circ.c) <= circ.r + cp_geometry::EPS;
}

// Time Complexity: O(1)
// Space Complexity: O(1)
template <typename T>
std::vector<point<double>> circle_line_intersection(const circle<T>& circ, const line<T>& l) {
    point<double> ab(l.b.x - l.a.x, l.b.y - l.a.y);
    point<double> ca(l.a.x - circ.c.x, l.a.y - circ.c.y);
    
    double a = dot_product(ab, ab);
    double b = 2.0 * dot_product(ca, ab);
    double c = dot_product(ca, ca) - circ.r * circ.r;
    
    double delta = b * b - 4 * a * c;
    if (delta < -cp_geometry::EPS) return {};
    if (delta <= cp_geometry::EPS) {
        double t = -b / (2 * a);
        return {point<double>(l.a.x + ab.x * t, l.a.y + ab.y * t)};
    }
    
    double t1 = (-b - sqrt(delta)) / (2 * a);
    double t2 = (-b + sqrt(delta)) / (2 * a);
    return {
        point<double>(l.a.x + ab.x * t1, l.a.y + ab.y * t1),
        point<double>(l.a.x + ab.x * t2, l.a.y + ab.y * t2)
    };
}

// Helper to get circumcircle of 3 points
// Time Complexity: O(1)
// Space Complexity: O(1)
template <typename T>
circle<double> circumcircle(const point<T>& a, const point<T>& b, const point<T>& c) {
    double D = 2.0 * (a.x * (b.y - c.y) + b.x * (c.y - a.y) + c.x * (a.y - b.y));
    double a2 = a.x*a.x + a.y*a.y;
    double b2 = b.x*b.x + b.y*b.y;
    double c2 = c.x*c.x + c.y*c.y;
    double cx = (a2 * (b.y - c.y) + b2 * (c.y - a.y) + c2 * (a.y - b.y)) / D;
    double cy = (a2 * (c.x - b.x) + b2 * (a.x - c.x) + c2 * (b.x - a.x)) / D;
    point<double> center(cx, cy);
    return circle<double>(center, distance(point<double>(a.x, a.y), center));
}

// Welzl's algorithm for Minimum Enclosing Circle
// Time Complexity: Expected O(n)
// Space Complexity: O(n)
template <typename T>
circle<double> min_enclosing_circle(std::vector<point<T>> pts) {
    std::mt19937 rng(1337);
    std::shuffle(pts.begin(), pts.end(), rng);
    
    circle<double> c(point<double>(0, 0), 0);
    int n = pts.size();
    for (int i = 0; i < n; i++) {
        point<double> pi(pts[i].x, pts[i].y);
        if (!is_point_in_circle(pi, c)) {
            c = circle<double>(pi, 0);
            for (int j = 0; j < i; j++) {
                point<double> pj(pts[j].x, pts[j].y);
                if (!is_point_in_circle(pj, c)) {
                    point<double> mid((pi.x + pj.x) / 2.0, (pi.y + pj.y) / 2.0);
                    c = circle<double>(mid, distance(pi, mid));
                    for (int k = 0; k < j; k++) {
                        point<double> pk(pts[k].x, pts[k].y);
                        if (!is_point_in_circle(pk, c)) {
                            c = circumcircle(pi, pj, pk);
                        }
                    }
                }
            }
        }
    }
    return c;
}

// Time Complexity: O(1)
// Space Complexity: O(1)
template <typename T>
std::vector<point<double>> circle_circle_intersection(const circle<T>& c1, const circle<T>& c2) {
    double d = distance(c1.c, c2.c);
    if (d > c1.r + c2.r + cp_geometry::EPS || d < std::abs(c1.r - c2.r) - cp_geometry::EPS || d < cp_geometry::EPS)
        return {}; // No intersection, or one is inside the other, or identical
    
    double a = (c1.r * c1.r - c2.r * c2.r + d * d) / (2 * d);
    double h = std::sqrt(std::max(0.0, c1.r * c1.r - a * a));
    
    point<double> p2(c2.c.x - c1.c.x, c2.c.y - c1.c.y);
    point<double> p3(c1.c.x + p2.x * a / d, c1.c.y + p2.y * a / d);
    
    if (std::abs(h) <= cp_geometry::EPS) return {p3};
    return {
        point<double>(p3.x + p2.y * h / d, p3.y - p2.x * h / d),
        point<double>(p3.x - p2.y * h / d, p3.y + p2.x * h / d)
    };
}

// Time Complexity: O(1)
// Space Complexity: O(1)
template <typename T>
std::vector<point<double>> tangents_point_to_circle(const point<T>& p, const circle<T>& c) {
    double d = distance(p, c.c);
    if (d < c.r - cp_geometry::EPS) return {};
    if (std::abs(d - c.r) <= cp_geometry::EPS) return {point<double>(p.x, p.y)};
    
    double a = c.r * c.r / d;
    double h = std::sqrt(std::max(0.0, c.r * c.r - a * a));
    
    point<double> p2(p.x - c.c.x, p.y - c.c.y);
    point<double> p3(c.c.x + p2.x * a / d, c.c.y + p2.y * a / d);
    
    return {
        point<double>(p3.x + p2.y * h / d, p3.y - p2.x * h / d),
        point<double>(p3.x - p2.y * h / d, p3.y + p2.x * h / d)
    };
}

// Intersection area of two circles
// Time Complexity: O(1)
// Space Complexity: O(1)
template <typename T>
double circle_circle_intersection_area(const circle<T>& c1, const circle<T>& c2) {
    double d = distance(c1.c, c2.c);
    if (d >= c1.r + c2.r - cp_geometry::EPS) return 0.0;
    if (d <= std::abs(c1.r - c2.r) + cp_geometry::EPS) {
        double r = std::min(c1.r, c2.r);
        return cp_geometry::PI * r * r;
    }
    double alpha = 2 * std::acos((c1.r * c1.r + d * d - c2.r * c2.r) / (2 * c1.r * d));
    double beta = 2 * std::acos((c2.r * c2.r + d * d - c1.r * c1.r) / (2 * c2.r * d));
    double a1 = 0.5 * c1.r * c1.r * (alpha - std::sin(alpha));
    double a2 = 0.5 * c2.r * c2.r * (beta - std::sin(beta));
    return a1 + a2;
}

// Common outer and inner tangents of two circles
// Returns pairs of points (one on c1, one on c2) representing the tangent lines
// Time Complexity: O(1)
// Space Complexity: O(1)
template <typename T>
std::vector<std::pair<point<double>, point<double>>> common_tangents(const circle<T>& c1, const circle<T>& c2, bool inner) {
    std::vector<std::pair<point<double>, point<double>>> res;
    point<double> d(c2.c.x - c1.c.x, c2.c.y - c1.c.y);
    double dr = inner ? (c1.r + c2.r) : (c1.r - c2.r);
    double d2 = norm2(d);
    if (d2 < dr * dr - cp_geometry::EPS) return res;
    if (std::abs(d2) <= cp_geometry::EPS) return res; // concentric
    double base_angle = std::atan2(d.y, d.x);
    double angle_offset = std::acos(std::max(-1.0, std::min(1.0, dr / std::sqrt(d2))));
    for (int sign : {-1, 1}) {
        double angle = base_angle + sign * angle_offset;
        point<double> p1(c1.c.x + c1.r * std::cos(angle), c1.c.y + c1.r * std::sin(angle));
        point<double> p2(c2.c.x + (inner ? -1 : 1) * c2.r * std::cos(angle), c2.c.y + (inner ? -1 : 1) * c2.r * std::sin(angle));
        res.push_back({p1, p2});
        if (std::abs(dr * dr - d2) <= cp_geometry::EPS) break; // touch externally/internally at one point
    }
    return res;
}
