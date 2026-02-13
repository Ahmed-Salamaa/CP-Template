// Point Template
// Dependencies: None (standalone)
// Usage: Include this file first before Line.cpp or Regular_Polygon.cpp

#include <bits/stdc++.h>
using namespace std;

const double PI = acos(-1.0);

template <typename t>
struct point
{
    t x, y;

    point() : x(0), y(0) {}
    point(t _x, t _y) : x(_x), y(_y) {}
    point(const point &p) : x(p.x), y(p.y) {}

    point operator + (const point& o) const { return point(x + o.x, y + o.y); }
    point operator - (const point& o) const { return point(x - o.x, y - o.y); }

    point operator + (t o) const { return point(x + o, y + o); }
    point operator - (t o) const { return point(x - o, y - o); }

    point operator * (t k) const { return point(x * k, y * k); }
    point operator / (t k) const { return point(x / k, y / k); }

    // left scalar multiplication: k * p
    friend point operator * (t k, const point& p) { return p * k; }

    // compound operators call basic ones
    point& operator += (const point& o) { return *this = *this + o; }
    point& operator -= (const point& o) { return *this = *this - o; }

    point& operator += (t k) { return *this = *this + k; }
    point& operator -= (t k) { return *this = *this - k; }

    point& operator *= (t k) { return *this = *this * k; }
    point& operator /= (t k) { return *this = *this / k; }

    bool operator < (const point& o) const
    {
        if (x != o.x) return x < o.x;
        return y < o.y;
    }

    bool operator > (const point& o) const
    {
        if (x != o.x) return x > o.x;
        return y > o.y;
    }

    bool operator <= (const point& o) const
    {
        if (x != o.x) return x <= o.x;
        return y <= o.y;
    }

    bool operator >= (const point& o) const
    {
        if (x != o.x) return x >= o.x;
        return y >= o.y;
    }

    bool operator == (const point& o) const { return x == o.x && y == o.y; }

    point & operator = ( const point &o ) 
    {
        if (this != &o) x = o.x , y = o.y ;
        return *this;
    }

    // ---------- dot & cross ----------
    // dot product (returns t)
    t dot(const point& o) const { return x * o.x + y * o.y; }

    // 2D cross product (scalar) a x b = ax * by - ay * bx  (returns t)
    t cross(const point& o) const { return x * o.y - y * o.x; }

    // cross of (b - a) x (c - a)  (returns t)
    static t cross(const point& a, const point& b, const point& c) { return (b - a).cross(c - a); }

    // ---------- useful helpers ----------
    // squared norm (returns t)
    t norm2() const {  return x * x + y * y; }

    // Euclidean length (returns double)
    double len() const { return sqrt((double)norm2()); }

    // perpendicular vector (90° CCW), returns point
    point perp() const { return point(-y, x); }

    static double dist(const point& a, const point& b)
    {
        point diff = a - b;
        return diff.len();
    }

    // ---------- Rotation & Transformation ----------
    // Rotate point around origin by angle (radians by default)
    point rotate(double angle, bool in_degrees = false) const
    {
        if (in_degrees) angle = angle * PI / 180.0;
        double c = cos(angle);
        double s = sin(angle);
        return point((t)(x * c - y * s), (t)(x * s + y * c));
    }

    // Rotate point around a pivot by angle (radians by default)
    point rotate_around(const point& pivot, double angle, bool in_degrees = false) const
    {
        return (*this - pivot).rotate(angle, in_degrees) + pivot;
    }

    // Reflect point across a line defined by two points
    point reflect(const point& p1, const point& p2) const
    {
        point v = p2 - p1;
        point u = *this - p1;
        double t_val = (double)u.dot(v) / (double)v.norm2();
        point proj = p1 + v * t_val;
        return proj * 2 - *this;
    }

    // Reflect across x-axis
    point reflect_x() const { return point(x, -y); }

    // Reflect across y-axis
    point reflect_y() const { return point(-x, y); }

    // Scale point from origin
    point scale(double factor) const
    {
        return point((t)(x * factor), (t)(y * factor));
    }

    // Scale point from a pivot
    point scale_from(const point& pivot, double factor) const
    {
        return pivot + (*this - pivot).scale(factor);
    }

    // ---------- Angle calculations ----------
    // Angle of vector from origin to this point (in radians by default)
    double angle(bool in_degrees = false) const
    {
        double ang = atan2((double)y, (double)x);
        if (in_degrees) ang = ang * 180.0 / PI;
        return ang;
    }

    // Angle between two vectors (from origin)
    static double angle_between(const point& a, const point& b, bool in_degrees = false)
    {
        double ang = atan2((double)a.cross(b), (double)a.dot(b));
        if (in_degrees) ang = ang * 180.0 / PI;
        return ang;
    }

    // Normalize vector to unit length
    point normalize() const
    {
        double l = len();
        if (l < 1e-18) return point(0, 0);
        return point((t)(x / l), (t)(y / l));
    }

    // ---------- I/O ----------
    friend istream& operator >> (istream& in, point& p)
    {
        in >> p.x >> p.y;
        return in;
    }

    friend ostream& operator << (ostream& out, const point& p)
    {
        out << p.x << " " << p.y;
        return out;
    }
};




