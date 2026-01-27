#include <bits/stdc++.h>
using namespace std;

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

