// Line Template
// Dependencies: Point.cpp (include Point.cpp before this file)

#pragma once

#include <bits/stdc++.h>

using namespace std;

template <typename T>
struct line
{
    point<T> a, b;
    T Acoef, Bcoef, Ccoef;

    // Build degenerate line at origin.
    // Time Complexity: O(1).
    line() : a(), b(), Acoef(0), Bcoef(0), Ccoef(0) {}

    // Build line passing through points p1 and p2.
    // Time Complexity: O(1).
    line(const point<T>& p1, const point<T>& p2) : a(p1), b(p2)
    {
        compute_equation();
    }

    // Build line from slope-intercept form y = slope*x + intercept.
    // Time Complexity: O(1).
    line(T slope, T intercept)
    {
        a = point<T>(0, intercept);
        b = point<T>(1, slope + intercept);
        compute_equation();
    }

    // Build line from equation A*x + B*y + C = 0.
    // Time Complexity: O(1).
    line(T A, T B, T C)
    {
        Acoef = A;
        Bcoef = B;
        Ccoef = C;

        if (fabsl((long double)B) > cp_geometry::EPS)
        {
            a = point<T>(0, -C / B);
            b = point<T>(1, -(A + C) / B);
        }
        else if (fabsl((long double)A) > cp_geometry::EPS)
        {
            a = point<T>(-C / A, 0);
            b = point<T>(-C / A, 1);
        }
        else
        {
            a = point<T>(0, 0);
            b = point<T>(0, 0);
        }
    }

    // Compute A, B, C coefficients from endpoints a and b.
    // Time Complexity: O(1).
    void compute_equation()
    {
        Acoef = b.y - a.y;
        Bcoef = a.x - b.x;
        Ccoef = -(Acoef * a.x + Bcoef * a.y);
    }

    // Return A coefficient in A*x + B*y + C = 0.
    // Time Complexity: O(1).
    T A() const { return Acoef; }

    // Return B coefficient in A*x + B*y + C = 0.
    // Time Complexity: O(1).
    T B() const { return Bcoef; }

    // Return C coefficient in A*x + B*y + C = 0.
    // Time Complexity: O(1).
    T C() const { return Ccoef; }

    // Cross product (p1->p2) x (p1->p3).
    // Depends on: point<T>::cross() (wrapper).
    // Time Complexity: O(1).
    static T cross(const point<T>& p1, const point<T>& p2, const point<T>& p3)
    {
        return point<T>::cross(p1, p2, p3);
    }

    // Orientation test: 1 counterclockwise, -1 clockwise, 0 collinear.
    // Depends on: point<T>::orientation() (wrapper).
    // Time Complexity: O(1).
    static int orientation(const point<T>& p1, const point<T>& p2, const point<T>& p3)
    {
        return point<T>::orientation(p1, p2, p3);
    }

    // Check if x lies between l and r (with epsilon tolerance).
    // Depends on: point<T>::between() (wrapper).
    // Time Complexity: O(1).
    static bool between(T x, T l, T r)
    {
        return point<T>::between(x, l, r);
    }

    // Check whether point p lies on segment [s1, s2].
    // Depends on: point<T>::on_segment() (wrapper).
    // Time Complexity: O(1).
    static bool on_segment(const point<T>& p, const point<T>& s1, const point<T>& s2)
    {
        return point<T>::on_segment(p, s1, s2);
    }

    // Distance from point p to segment [a,b] (or infinite line if extend=true), and nearest point.
    // Depends on: point<T>::dist().
    // Time Complexity: O(1).
    pair<double, point<T>> distance(const point<T>& p, bool extend = false) const
    {
        point<T> ab = b - a;
        long double ab2 = (long double)ab.norm2();
        if (fabsl(ab2) <= cp_geometry::EPS)
            return {point<T>::dist(a, p), a};

        long double t = (long double)(p - a).dot(ab) / ab2;
        if (!extend)
        {
            if (t < 0.0L) t = 0.0L;
            if (t > 1.0L) t = 1.0L;
        }

        point<T> closest(
            (T)((long double)a.x + (long double)ab.x * t),
            (T)((long double)a.y + (long double)ab.y * t)
        );

        return {point<T>::dist(p, closest), closest};
    }

    // Segment length |b-a|.
    // Depends on: point<T>::len().
    // Time Complexity: O(1).
    double length() const
    {
        return (b - a).len();
    }

    // Orthogonal projection of point p onto infinite line.
    // Depends on: point<T>::dot(), point<T>::norm2().
    // Time Complexity: O(1).
    point<T> project(const point<T>& p) const
    {
        point<T> ab = b - a;
        long double ab2 = (long double)ab.norm2();
        if (fabsl(ab2) <= cp_geometry::EPS) return a;

        long double t = (long double)(p - a).dot(ab) / ab2;
        return point<T>(
            (T)((long double)a.x + (long double)ab.x * t),
            (T)((long double)a.y + (long double)ab.y * t)
        );
    }

    // Check whether p lies on the line, or on segment [a,b] when in_segment=true.
    // Depends on: point<T>::on_segment(), point<T>::orientation().
    // Time Complexity: O(1).
    bool contains(const point<T>& p, bool in_segment = true) const
    {
        if (in_segment) return point<T>::on_segment(p, a, b);
        return point<T>::orientation(a, b, p) == 0;
    }

    // Intersection point of two infinite lines; returns false for parallel/coincident lines.
    // Time Complexity: O(1).
    static pair<bool, point<T>> intersect(const line<T>& l1, const line<T>& l2)
    {
        long double det = (long double)l1.Acoef * l2.Bcoef - (long double)l2.Acoef * l1.Bcoef;
        if (fabsl(det) <= cp_geometry::EPS) return {false, point<T>(0, 0)};

        long double x = ((long double)l1.Bcoef * l2.Ccoef - (long double)l2.Bcoef * l1.Ccoef) / det;
        long double y = ((long double)l2.Acoef * l1.Ccoef - (long double)l1.Acoef * l2.Ccoef) / det;
        return {true, point<T>((T)x, (T)y)};
    }

    // Smaller angle between line directions in radians (or degrees when requested).
    // Depends on: point<T>::len(), point<T>::dot(), point<T>::cross().
    // Time Complexity: O(1).
    static pair<bool, double> angle_between(const line<T>& l1, const line<T>& l2, bool in_degrees = false)
    {
        point<T> v1 = l1.b - l1.a;
        point<T> v2 = l2.b - l2.a;

        double n1 = v1.len();
        double n2 = v2.len();
        if (n1 <= (double)cp_geometry::EPS || n2 <= (double)cp_geometry::EPS) return {false, 0.0};

        long double dot_val = (long double)v1.dot(v2);
        long double cross_val = (long double)v1.cross(v2);
        double ang = atan2(fabsl(cross_val), dot_val);

        if (in_degrees) ang = ang * 180.0 / (double)cp_geometry::PI;
        return {true, ang};
    }

    // Convert line to y = s*x + c form; returns false for vertical line.
    // Time Complexity: O(1).
    pair<bool, pair<T, T>> slope_intercept() const
    {
        if (fabsl((long double)Bcoef) <= cp_geometry::EPS)
            return {false, {0, 0}};

        T s = -Acoef / Bcoef;
        T c = -Ccoef / Bcoef;
        return {true, {s, c}};
    }

    // Integer-friendly normalization, best used when points are integer valued.
    // Returns {c, {dy, dx}} to represent slope dy/dx and intercept-like value c.
    // Time Complexity: O(log V), where V is coordinate magnitude (due to gcd).
    pair<long long, pair<long long, long long>> slope_normalized() const
    {
        long long dx = llround((long double)b.x - (long double)a.x);
        long long dy = llround((long double)b.y - (long double)a.y);

        if (dx == 0)
        {
            long long c = llround((long double)a.x);
            return {c, {1, 0}};
        }

        long long g = gcd(llabs(dx), llabs(dy));
        if (g == 0) return {0, {0, 0}};
        dx /= g;
        dy /= g;

        if (dx < 0)
        {
            dx = -dx;
            dy = -dy;
        }

        long long c_num = llround((long double)a.y * dx - (long double)dy * a.x);
        long long c_den = dx;
        if (c_den != 0) c_num /= c_den;

        return {c_num, {dy, dx}};
    }

    // Check if two infinite lines are parallel.
    // Time Complexity: O(1).
    static bool are_parallel(const line<T>& l1, const line<T>& l2)
    {
        long double det = (long double)l1.Acoef * l2.Bcoef - (long double)l2.Acoef * l1.Bcoef;
        return fabsl(det) <= cp_geometry::EPS;
    }

    // Check if two infinite lines are perpendicular.
    // Time Complexity: O(1).
    static bool are_perpendicular(const line<T>& l1, const line<T>& l2)
    {
        long double dot_val = (long double)l1.Acoef * l2.Acoef + (long double)l1.Bcoef * l2.Bcoef;
        return fabsl(dot_val) <= cp_geometry::EPS;
    }

    // Build a line through p that is perpendicular to current line.
    // Depends on: point<T>::perp().
    // Time Complexity: O(1).
    line<T> perpendicular(const point<T>& p) const
    {
        point<T> dir = b - a;
        return line<T>(p, p + dir.perp());
    }

    // Build a line through p that is parallel to current line.
    // Time Complexity: O(1).
    line<T> parallel(const point<T>& p) const
    {
        point<T> dir = b - a;
        return line<T>(p, p + dir);
    }

    // Segment intersection check with returned intersection/end-point when available.
    // Depends on: intersect(), point<T>::orientation(), point<T>::on_segment().
    // Time Complexity: O(1).
    static pair<bool, point<T>> segment_intersect(const line<T>& s1, const line<T>& s2)
    {
        int o1 = point<T>::orientation(s1.a, s1.b, s2.a);
        int o2 = point<T>::orientation(s1.a, s1.b, s2.b);
        int o3 = point<T>::orientation(s2.a, s2.b, s1.a);
        int o4 = point<T>::orientation(s2.a, s2.b, s1.b);

        if (o1 == 0 && point<T>::on_segment(s2.a, s1.a, s1.b)) return {true, s2.a};
        if (o2 == 0 && point<T>::on_segment(s2.b, s1.a, s1.b)) return {true, s2.b};
        if (o3 == 0 && point<T>::on_segment(s1.a, s2.a, s2.b)) return {true, s1.a};
        if (o4 == 0 && point<T>::on_segment(s1.b, s2.a, s2.b)) return {true, s1.b};

        if (o1 != o2 && o3 != o4)
        {
            auto [ok, p] = intersect(s1, s2);
            if (!ok) return {false, point<T>(0, 0)};
            return {true, p};
        }

        return {false, point<T>(0, 0)};
    }

    // Bool-only version of segment intersection.
    // Depends on: segment_intersect() (wrapper).
    // Time Complexity: O(1).
    static bool segments_intersect(const line<T>& s1, const line<T>& s2)
    {
        return segment_intersect(s1, s2).first;
    }

    // Midpoint of segment [a,b].
    // Time Complexity: O(1).
    point<T> midpoint() const
    {
        return point<T>((a.x + b.x) / (T)2, (a.y + b.y) / (T)2);
    }

    // Direction vector (b - a).
    // Time Complexity: O(1).
    point<T> direction() const
    {
        return b - a;
    }

    // Unit direction vector.
    // Depends on: point<T>::len().
    // Time Complexity: O(1).
    point<T> direction_normalized() const
    {
        point<T> dir = b - a;
        double len = dir.len();
        if (len <= (double)cp_geometry::EPS) return point<T>(0, 0);
        return point<T>((T)(dir.x / len), (T)(dir.y / len));
    }

    enum object_type
    {
        LINE = 0,
        RAY = 1,
        SEGMENT = 2
    };

private:

    // Does: Validate line parameter against object type constraints.
    // Takes: t as long double, type as object_type.
    // Returns: bool.
    // Time Complexity: O(1).
    static bool valid_parameter(long double t, object_type type)
    {
        if (type == LINE) return true;
        if (type == RAY) return t >= -cp_geometry::EPS;
        return t >= -cp_geometry::EPS && t <= 1.0L + cp_geometry::EPS;
    }

    // Does: Compute distance from point to line/ray/segment object.
    // Takes: p as point<T>, L as line<T>, type as object_type.
    // Returns: long double.
    // Depends on: point<T>::dist(), point<T>::dot(), point<T>::norm2().
    // Time Complexity: O(1).
    static long double point_object_distance(
        const point<T>& p,
        const line<T>& L,
        object_type type)
    {
        point<T> ab = L.b - L.a;
        long double ab2 = (long double)ab.norm2();

        if (fabsl(ab2) <= cp_geometry::EPS)
            return point<T>::dist(p, L.a);

        long double t =
            (long double)(p - L.a).dot(ab) / ab2;

        if (type == RAY) t = max(0.0L, t);
        else if (type == SEGMENT) t = min(1.0L, max(0.0L, t));

        point<T> proj(
            (T)((long double)L.a.x + (long double)ab.x * t),
            (T)((long double)L.a.y + (long double)ab.y * t)
        );
        return point<T>::dist(p, proj);
    }

    // Does: Check whether two line/ray/segment objects intersect.
    // Takes: L1 as line<T>, t1 as object_type, L2 as line<T>, t2 as object_type.
    // Returns: bool.
    // Depends on: valid_parameter(), point_object_distance(), point<T>::cross_val().
    // Time Complexity: O(1).
    static bool objects_intersect(
        const line<T>& L1, object_type t1,
        const line<T>& L2, object_type t2)
    {
        point<T> d1 = L1.b - L1.a;
        point<T> d2 = L2.b - L2.a;
        point<T> s = L2.a - L1.a;

        long double det = (long double)d1.cross(d2);

        // Proper (non-parallel) case: solve by parameters and validate ranges.
        if (fabsl(det) > cp_geometry::EPS)
        {
            long double u = (long double)s.cross(d1) / det;
            long double t = (long double)s.cross(d2) / det;
            return valid_parameter(t, t1) && valid_parameter(u, t2);
        }

        // Parallel but not collinear => no intersection.
        if (fabsl(point<T>::cross_val(L1.a, L1.b, L2.a)) > cp_geometry::EPS)
            return false;

        // Collinear: quick acceptance for any collinear line involvement.
        if (t1 == LINE || t2 == LINE)
            return true;

        // For ray/segment combos, endpoint-on-object checks are sufficient on collinear supports.
        if (point_object_distance(L1.a, L2, t2) <= cp_geometry::EPS) return true;
        if (point_object_distance(L1.b, L2, t2) <= cp_geometry::EPS) return true;
        if (point_object_distance(L2.a, L1, t1) <= cp_geometry::EPS) return true;
        if (point_object_distance(L2.b, L1, t1) <= cp_geometry::EPS) return true;

        return false;
    }

public:

    // Does: Compute minimum distance between two line/ray/segment objects.
    // Takes: L1 as line<T>, t1 as object_type, L2 as line<T>, t2 as object_type.
    // Returns: long double.
    // Depends on: objects_intersect(), point_object_distance().
    // Time Complexity: O(1).
    static long double minimum_distance(
        const line<T>& L1, object_type t1,
        const line<T>& L2, object_type t2)
    {
        if (objects_intersect(L1, t1, L2, t2))
            return 0.0L;

        long double ans = 1e100;

        ans = min(ans, point_object_distance(L1.a, L2, t2));
        ans = min(ans, point_object_distance(L1.b, L2, t2));
        ans = min(ans, point_object_distance(L2.a, L1, t1));
        ans = min(ans, point_object_distance(L2.b, L1, t1));

        return ans;
    }
};




