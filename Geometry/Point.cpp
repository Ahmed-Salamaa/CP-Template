// Point Template (standalone)
// Dependencies: None

#pragma once

#include <bits/stdc++.h>

using namespace std;

namespace cp_geometry
{
    static const long double PI = acos(-1.0L);
    static const long double EPS = 1e-9L;
}

template <typename T>
struct point
{
    T x, y;

    point() : x(0), y(0) {}
    point(T x_, T y_) : x(x_), y(y_) {}
    point(const point& other) : x(other.x), y(other.y) {}
    
    point operator + (const point& o) const { return point(x + o.x, y + o.y); }
    point operator - (const point& o) const { return point(x - o.x, y - o.y); }
    point operator + (T k) const { return point(x + k, y + k); }
    point operator - (T k) const { return point(x - k, y - k); }
    point operator * (T k) const { return point(x * k, y * k); }
    point operator / (T k) const { return point(x / k, y / k); }
    friend point operator * (T k, const point& p) { return p * k; }

    point& operator += (const point& o) { return *this = *this + o; }
    point& operator -= (const point& o) { return *this = *this - o; }
    point& operator += (T k) { return *this = *this + k; }
    point& operator -= (T k) { return *this = *this - k; }
    point& operator *= (T k) { return *this = *this * k; }
    point& operator /= (T k) { return *this = *this / k; }

    bool operator < (const point& o) const { return (x != o.x) ? x < o.x : y < o.y ; }
    bool operator > (const point& o) const { return (x != o.x) ? x > o.x : y > o.y ; }
    bool operator <= (const point& o) const { return (x != o.x) ? x <= o.x : y <= o.y ; }
    bool operator >= (const point& o) const { return (x != o.x) ? x >= o.x : y >= o.y ; }
    bool operator == (const point& o) const { return x == o.x && y == o.y; }
    point& operator = (const point& o) { if (this != &o) x = o.x, y = o.y; return *this; }

    // Dot product between two vectors.
    T dot(const point& o) const { return x * o.x + y * o.y; }

    // 2D cross product value (signed area * 2 of parallelogram).
    T cross(const point& o) const { return x * o.y - y * o.x; }

    // Cross product of vectors (b - a) and (c - a).
    static T cross(const point& a, const point& b, const point& c)
    {
        return (b - a).cross(c - a);
    }

    // Cross product of (b - a) x (c - a) in long double.
    // Useful for robust orientation tests when T may overflow.
    static long double cross_val(const point& a, const point& b, const point& c)
    {
        return ((long double)b.x - (long double)a.x) * ((long double)c.y - (long double)a.y) -
               ((long double)b.y - (long double)a.y) * ((long double)c.x - (long double)a.x);
    }

    // Sign function with tolerance.
    static int sgn(long double v, long double eps = cp_geometry::EPS)
    {
        if (fabsl(v) <= eps) return 0;
        return (v < 0 ? -1 : 1);
    }

    // Orientation of ordered triple (a, b, c).
    // Returns: 1 (CCW), -1 (CW), 0 (collinear).
    // Depends on: cross_val(), sgn().
    // Time Complexity: O(1).
    static int orientation(const point& a, const point& b, const point& c, long double eps = cp_geometry::EPS)
    {
        return sgn(cross_val(a, b, c), eps);
    }

    // Check if x lies between l and r with tolerance.
    // Time Complexity: O(1).
    static bool between(T x, T l, T r, long double eps = cp_geometry::EPS)
    {
        long double lx = (long double)l;
        long double rx = (long double)r;
        if (lx > rx) swap(lx, rx);
        long double xx = (long double)x;
        return xx + eps >= lx && xx - eps <= rx;
    }

    // Check if p lies on segment [a, b] with tolerance.
    // Depends on: orientation(), between().
    // Time Complexity: O(1).
    static bool on_segment(const point& p, const point& a, const point& b, long double eps = cp_geometry::EPS)
    {
        if (orientation(a, b, p, eps) != 0) return false;
        return between(p.x, a.x, b.x, eps) && between(p.y, a.y, b.y, eps);
    }

    // Near-equality check between two points using squared distance.
    // Time Complexity: O(1).
    static bool same_point(const point& a, const point& b, long double eps = cp_geometry::EPS)
    {
        long double dx = (long double)a.x - (long double)b.x;
        long double dy = (long double)a.y - (long double)b.y;
        return dx * dx + dy * dy <= eps * eps;
    }

    // Squared vector length (avoids square root).
    // Time Complexity: O(1).
    T norm2() const { return x * x + y * y; }

    // Euclidean vector length.
    // Depends on: norm2().
    // Time Complexity: O(1).
    double len() const { return sqrt((double)norm2()); }

    // Rotate vector by +90 degrees (counterclockwise).
    // Time Complexity: O(1).
    point perp() const { return point(-y, x); }

    // Euclidean distance between two points.
    // Depends on: len().
    // Time Complexity: O(1).
    static double dist(const point& a, const point& b)
    {
        return (a - b).len();
    }

    // Rotate around origin by angle (radians by default).
    // Time Complexity: O(1).
    point rotate(double angle, bool in_degrees = false) const
    {
        if (in_degrees) angle = angle * (double)cp_geometry::PI / 180.0;
        double c = cos(angle);
        double s = sin(angle);
        return point((T)(x * c - y * s), (T)(x * s + y * c));
    }

    // Rotate around a custom pivot point.
    // Depends on: rotate().
    // Time Complexity: O(1).
    point rotate_around(const point& pivot, double angle, bool in_degrees = false) const
    {
        return (*this - pivot).rotate(angle, in_degrees) + pivot;
    }

    // Reflect current point across line through p1 and p2.
    // Depends on: norm2(), dot().
    // Time Complexity: O(1).
    point reflect(const point& p1, const point& p2) const
    {
        point v = p2 - p1;
        point u = *this - p1;
        double v2 = (double)v.norm2();
        if (v2 <= (double)cp_geometry::EPS) return *this;
        double t_val = (double)u.dot(v) / v2;
        point proj = p1 + v * (T)t_val;
        return proj * (T)2 - *this;
    }

    // Reflection across x-axis.
    // Time Complexity: O(1).
    point reflect_x() const { return point(x, -y); }

    // Reflection across y-axis.
    // Time Complexity: O(1).
    point reflect_y() const { return point(-x, y); }

    // Scale point from origin by factor.
    // Time Complexity: O(1).
    point scale(double factor) const
    {
        return point((T)(x * factor), (T)(y * factor));
    }

    // Scale point around pivot by factor.
    // Depends on: scale().
    // Time Complexity: O(1).
    point scale_from(const point& pivot, double factor) const
    {
        return pivot + (*this - pivot).scale(factor);
    }

    // Polar angle of vector from origin.
    // Time Complexity: O(1).
    double angle(bool in_degrees = false) const
    {
        double ang = atan2((double)y, (double)x);
        if (in_degrees) ang = ang * 180.0 / (double)cp_geometry::PI;
        return ang;
    }

    // Signed angle from vector a to vector b.
    // Depends on: cross(), dot().
    // Time Complexity: O(1).
    static double angle_between(const point& a, const point& b, bool in_degrees = false)
    {
        double ang = atan2((double)a.cross(b), (double)a.dot(b));
        if (in_degrees) ang = ang * 180.0 / (double)cp_geometry::PI;
        return ang;
    }

    // Unit-length vector in the same direction.
    // Depends on: len().
    // Time Complexity: O(1).
    point normalize() const
    {
        double l = len();
        if (l <= (double)cp_geometry::EPS) return point(0, 0);
        return point((T)(x / l), (T)(y / l));
    }

    // Read point from input stream.
    // Time Complexity: O(1).
    friend istream& operator >> (istream& in, point& p)
    {
        in >> p.x >> p.y;
        return in;
    }

    // Write point to output stream.
    // Time Complexity: O(1).
    friend ostream& operator << (ostream& out, const point& p)
    {
        out << p.x << " " << p.y;
        return out;
    }
};

namespace cp_geometry
{
    // Forward declaration (for include-order friendliness).
    // Defined in Convex_Hull.cpp.
    template <typename T>
    vector<point<T>> convex_hull_points(vector<point<T>> pts, bool keep_collinear = false);
}




