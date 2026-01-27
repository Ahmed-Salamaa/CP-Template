#include <bits/stdc++.h>
using namespace std;

template <typename t>
struct line
{
    point<t> a, b;  // line passing through points a and b
    t A_, B_, C_;   // line equation: Ax + By + C = 0

    // ---------- Constructors ----------

    // Default
    line() {}

    // Construct from two points
    line(const point<t>& p1, const point<t>& p2) : a(p1), b(p2)
    {
        compute_equation();
    }

    // Construct from slope-intercept: y = s*x + c
    line(t slope, t intercept)
    {
        a = point<t>(0, intercept);
        b = point<t>(1, slope + intercept);
        compute_equation();
    }

    line(t A, t B, t C)
    {
        A_ = A; B_ = B; C_ = C;

        // Pick two points on the line
        if (B != 0) {
            // y = (-A*x - C)/B
            a = point<t>(0, -C / B);          // x = 0
            b = point<t>(1, -(A + C) / B);    // x = 1
        } else if (A != 0) {
            // vertical line x = -C/A
            a = point<t>(-C / A, 0);          // y = 0
            b = point<t>(-C / A, 1);          // y = 1
        } else {
            // degenerate line, A = B = 0 → invalid
            a = b = point<t>(0, 0);
            cerr << "Warning: Invalid line equation A=B=0\n";
        }
    }

    // Compute A, B, C from points a, b
    void compute_equation()
    {
        A_ = b.y - a.y;
        B_ = a.x - b.x;
        C_ = -(A_ * a.x + B_ * a.y);
    }

public:
    // ---------- Access equation coefficients ----------
    t A() const { return A_; }
    t B() const { return B_; }
    t C() const { return C_; }

    // ---------- Utility helpers added ----------
    // cross product of vectors (p1->p2) x (p1->p3)
    static t cross(const point<t>& p1, const point<t>& p2, const point<t>& p3)
    {
        return (p2.x - p1.x) * (p3.y - p1.y) - (p2.y - p1.y) * (p3.x - p1.x);
    }

    // ---------- Distance functions ----------

    // extend = 0 -> Distance to segment [a,b], optional extension
    // extend = 1 -> Distance to infinite line
    pair<t, point<t>> distance (const point<t>& p, bool extend = false) const
    {
        point<t> ab = b - a;
        point<t> ap = p - a;
        point<t> bp = p - b;

        t ab2 = ab.norm2(); // squared length of segment
        if (ab2 == 0) 
            return { point<t>::dist(a, p), point<t>((t)a.x, (t)a.y) }; // segment is a point

        t t_ = ap.dot(ab) / (t)ab2; // projection parameter

        point<t> closest;

        if (!extend)
        {
            if (t_ < 0) closest = point<t>((t)a.x, (t)a.y);
            else if (t_ > 1) closest = point<t>((t)b.x, (t)b.y);
            else closest = point<t>((t)(a.x + ab.x * t_), (t)(a.y + ab.y * t_));
        }
        else
        {
            closest = point<t>((t)(a.x + ab.x * t_), (t)(a.y + ab.y * t_));
        }

        t dist = point<t>::dist(p, closest);
        return { dist, closest };
    }

    // Length of the line segment (distance between a and b)
    t length() const 
    {
        return (b - a).len();
    }

    // Project point onto infinite line
    point<t> project(const point<t>& p) const
    {
        point<t> ab = b - a;
        t t_ = (p - a).dot(ab) / (t)ab.norm2();
        return a + ab * t_;
    }

    // in_segment = 1 -> Check if point is on segment [ a , b ]
    // in_segment = 0 -> Check if point is on line
    bool contains(const point<t>& p , bool in_segment = true ) const
    {
        if ( in_segment ) return line<t>::cross(a, b, p) == 0 && distance(p).first < 1e-9 ;
        else return line<t>::cross(a, b, p) == 0  ;
        
    }

    // Intersection of infinite lines
    // bool = true if lines intersect, false if parallel
    static pair<bool, point<t>> intersect(const line<t>& l1, const line<t>& l2)
    {
        t det = l1.A_ * l2.B_ - l2.A_ * l1.B_;
        if (abs(det) < 1e-9) return {false, point<t>(0,0)}; // parallel

        t x = (l1.B_ * l2.C_ - l2.B_ * l1.C_) / det;
        t y = (l2.A_ * l1.C_ - l1.A_ * l2.C_) / det;
        return {true, point<t>(x, y)};
    }

    // angle between two lines
    // returns {true, angle} where angle is in radians by default (0..pi).
    // If in_degrees == true, angle is returned in degrees.
    // returns {false, 0.0} if either line is degenerate (zero-length).
    static pair<bool, t> angle_between(const line<t>& l1, const line<t>& l2, bool in_degrees = false)
    {
        point<t> v1 = l1.b - l1.a;
        point<t> v2 = l2.b - l2.a;

        t n1 = sqrt((t)v1.norm2());
        t n2 = sqrt((t)v2.norm2());
        if (n1 < 1e-18 || n2 < 1e-18) return {false, 0.0}; // degenerate line

        t dot = (t)v1.dot(v2);
        t cross = (t)v1.cross(v2);

        // atan2(|cross|, dot) gives the smaller angle between vectors in [0, pi]
        t ang = atan2(fabs(cross), dot);

        if (in_degrees) ang = ang * 180.0 / PI;
        return {true, ang};
    }

    // returns {true, {s, c}} for y = s*x + c
    // returns {false, {0, 0}} if vertical line
    pair<bool, pair<t, t>> slope_intercept() const
    {
        if (abs(B_) < 1e-18)
            return {false, {0, 0}}; // vertical

        t s = -A_ / B_;
        t c = -C_ / B_;
        return {true, {s, c}};
    }

    // returns { c , { slope_y , slope_x } }
    // vertical line encoded as: x = c  -> { c , { 1 , 0 } }
    pair<int, pair<int, int>> slope_normalized() const
    {
        // direction vector
        t dx = b.x - a.x;
        t dy = b.y - a.y;

        // -------- Vertical line: x = c --------
        if (abs(dx) < 1e-18)
        {
            int c = (int)a.x;          // x = c
            return { c, { 1, 0 } };    // slope marker for vertical
        }

        // -------- Normalize slope dy/dx --------
        int x = (int)dx;
        int y = (int)dy;

        int sign = 1;
        if (x < 0) sign *= -1, x *= -1;
        if (y < 0) sign *= -1, y *= -1;

        int g = __gcd(x, y);
        x /= g;
        y /= g;

        int slope_x = sign * x;
        int slope_y = y;

        // -------- Compute intercept c --------
        // y = (slope_y / slope_x) * x + c
        // c = y0 - (slope_y / slope_x) * x0
        // => c = (y0*slope_x - slope_y*x0) / slope_x
        int c = (int)(a.y * slope_x - slope_y * a.x) / slope_x;

        return { c, { slope_y, slope_x } };
    }
};
