/*
 * Topic: Point
 * Description: Provides a robust 2D Point structure using vectors or complex numbers,
 *   supporting cross products, dot products, rotation, and distance calculations.
 * and cross products, and includes utilities for precision constants (EPS) and polar sorting. Usage: Point
 * p1(1.0, 2.0), p2(3.0, 4.0); Point p3 = p1 + p2; double cp = cross(p1, p2); double dp = dot(p1, p2);
 */
#include "../core.h"
#pragma once

// Calculates: The sign of a floating point value considering a given epsilon.
// Returns: An integer representing the sign: -1 for negative, 1 for positive, 0 for zero (within epsilon).
// Time Complexity: O(1)
// Space Complexity: O(1)
int sgn(ld v, ld eps = EPS) {
    if (fabsl(v) <= eps) return 0;
    return (v < 0 ? -1 : 1);
}

// Point structure for 2D coordinates
// Time Complexity: O(1)
// Space Complexity: O(1)
template <typename T>
struct point {
    T x, y;

    point() : x(0), y(0) {}

    point(T x_, T y_) : x(x_), y(y_) {}

    point operator+(const point& o) const { return point(x + o.x, y + o.y); }

    point operator-(const point& o) const { return point(x - o.x, y - o.y); }

    point operator*(T k) const { return point(x * k, y * k); }

    point operator/(T k) const { return point(x / k, y / k); }

    bool operator<(const point& o) const { return (x != o.x) ? x < o.x : y < o.y; }

    bool operator==(const point& o) const { return x == o.x && y == o.y; }
};

// Calculates: The dot product of two 2D vectors.
// Returns: A scalar representing the dot product.
// Time Complexity: O(1)
// Space Complexity: O(1)
template <typename T>
T dot_product(const point<T>& a, const point<T>& b) {
    return a.x * b.x + a.y * b.y;
}

// Calculates: The cross product of two 2D vectors (z-component in 3D).
// Returns: A scalar representing the cross product.
// Time Complexity: O(1)
// Space Complexity: O(1)
template <typename T>
T cross_product(const point<T>& a, const point<T>& b) {
    return a.x * b.y - a.y * b.x;
}

// Calculates: The cross product of vectors AB and AC.
// Returns: A scalar representing the cross product.
// Time Complexity: O(1)
// Space Complexity: O(1)
template <typename T>
T cross_product(const point<T>& a, const point<T>& b, const point<T>& c) {
    return cross_product(b - a, c - a);
}

// Calculates: The cross product of vectors AB and AC, cast to long double for higher precision.
// Returns: A long double representing the precise cross product.
// Time Complexity: O(1)
// Space Complexity: O(1)
template <typename T>
ld cross_val(const point<T>& a, const point<T>& b, const point<T>& c) {
    return ((ld)b.x - a.x) * ((ld)c.y - a.y) - ((ld)b.y - a.y) * ((ld)c.x - a.x);
}

// Calculates: The squared magnitude (norm) of a 2D vector.
// Returns: A scalar representing the squared magnitude.
// Time Complexity: O(1)
// Space Complexity: O(1)
template <typename T>
T norm2(const point<T>& a) {
    return a.x * a.x + a.y * a.y;
}

// Calculates: The length (magnitude) of a 2D vector.
// Returns: A double representing the vector's length.
// Time Complexity: O(1)
// Space Complexity: O(1)
template <typename T>
double length(const point<T>& a) {
    return sqrt((double)norm2(a));
}

// Calculates: The orientation of the ordered triplet (a, b, c).
// Returns: 1 (CCW), -1 (CW), 0 (collinear).
// Time Complexity: O(1)
// Space Complexity: O(1)
template <typename T>
int orientation(const point<T>& a, const point<T>& b, const point<T>& c) {
    return sgn(cross_val(a, b, c));
}

// Calculates: Whether point p lies on the line segment defined by a and b.
// Returns: True if p is on the segment, false otherwise.
// Time Complexity: O(1)
// Space Complexity: O(1)
template <typename T>
bool on_segment(const point<T>& p, const point<T>& a, const point<T>& b) {
    if (orientation(a, b, p) != 0) return false;
    return p.x >= min(a.x, b.x) && p.x <= max(a.x, b.x) && p.y >= min(a.y, b.y) && p.y <= max(a.y, b.y);
}

// Calculates: The Euclidean distance between two points.
// Returns: A double representing the distance.
// Time Complexity: O(1)
// Space Complexity: O(1)
template <typename T>
double distance(const point<T>& a, const point<T>& b) {
    return length(a - b);
}

// Calculates: A point rotated counter-clockwise around the origin by the given angle.
// Returns: The rotated point.
// Time Complexity: O(1)
// Space Complexity: O(1)
template <typename T>
point<T> rotate(const point<T>& p, double angle) {
    double c = cos(angle), s = sin(angle);
    return point<T>((T)(p.x * c - p.y * s), (T)(p.x * s + p.y * c));
}

// Calculates: A point rotated counter-clockwise around a specified pivot by the given angle.
// Returns: The rotated point.
// Time Complexity: O(1)
// Space Complexity: O(1)
template <typename T>
point<T> rotate_around(const point<T>& p, const point<T>& pivot, double angle) {
    return rotate(p - pivot, angle) + pivot;
}

// Calculates: The projection of one vector onto another.
// Returns: A point representing the projected vector.
// Time Complexity: O(1)
// Space Complexity: O(1)
template <typename T>
point<T> project_vector(const point<T>& v, const point<T>& onto) {
    double d = dot_product(v, onto) / (double)norm2(onto);
    return point<T>((T)(onto.x * d), (T)(onto.y * d));
}

// Calculates: The reflection of point p across the line passing through a and b.
// Returns: The reflected point.
// Time Complexity: O(1)
// Space Complexity: O(1)
template <typename T>
point<T> reflect(const point<T>& p, const point<T>& a, const point<T>& b) {
    point<T> ab = b - a;
    point<T> ap = p - a;
    double t = dot_product(ap, ab) / (double)norm2(ab);
    point<T> proj(a.x + ab.x * t, a.y + ab.y * t);
    return point<T>(2 * proj.x - p.x, 2 * proj.y - p.y);
}

// Calculates: Sorts a list of points by their polar angle around a given pivot.
// Returns: Nothing (sorts in-place).
// Time Complexity: O(n log n)
// Space Complexity: O(1)
template <typename T>
void polar_sort(vector<point<T>>& pts, const point<T>& pivot) {
    auto half = [&](const point<T>& p) { return p.y > pivot.y || (p.y == pivot.y && p.x > pivot.x); };
    sort(all(pts), [&](const point<T>& a, const point<T>& b) {
        int ha = half(a), hb = half(b);
        if (ha != hb) return ha < hb;
        ld cross = cross_val(pivot, a, b);
        if (abs(cross) > EPS) return cross > 0;
        return norm2(a - pivot) < norm2(b - pivot);
    });
}

// Calculates: The minimum Euclidean distance between any two points in the given list.
// Returns: A double representing the closest pair distance.
// Time Complexity: O(n log n)
// Space Complexity: O(n)
template <typename T>
double closest_pair(vector<point<T>> pts) {
    if (sz(pts) < 2) return 0.0;
    sort(all(pts), [](const point<T>& a, const point<T>& b) { return a.x < b.x || (a.x == b.x && a.y < b.y); });
    vector<point<T>> t(sz(pts));
    auto solve = [&](auto& self, int l, int r) -> double {
        if (r - l <= 3) {
            double best = 1e18;
            for (int i = l; i < r; ++i)
                for (int j = i + 1; j < r; ++j) best = min(best, distance(pts[i], pts[j]));
            sort(pts.begin() + l, pts.begin() + r, [](const point<T>& a, const point<T>& b) { return a.y < b.y; });
            return best;
        }
        int mid = (l + r) / 2;
        T midx = pts[mid].x;
        double best = min(self(self, l, mid), self(self, mid, r));
        merge(pts.begin() + l, pts.begin() + mid, pts.begin() + mid, pts.begin() + r, t.begin(),
              [](const point<T>& a, const point<T>& b) { return a.y < b.y; });
        copy(t.begin(), t.begin() + (r - l), pts.begin() + l);
        int tsz = 0;
        for (int i = l; i < r; ++i) {
            if (abs(pts[i].x - midx) < best) {
                for (int j = tsz - 1; j >= 0 && pts[i].y - t[j].y < best; --j) {
                    best = min(best, distance(pts[i], t[j]));
                }
                t[tsz++] = pts[i];
            }
        }
        return best;
    };
    return solve(solve, 0, sz(pts));
}

// Calculates: The directed angle between two vectors.
// Returns: A double representing the angle in radians.
// Time Complexity: O(1)
// Space Complexity: O(1)
template <typename T>
double angle_between(const point<T>& a, const point<T>& b) {
    double dot = dot_product(a, b);
    double det = cross_product(a, b);
    return atan2(det, dot);
}

// Calculates: A vector representing the angle bisector of angle ABC.
// Returns: A point (vector) in the direction of the bisector.
// Time Complexity: O(1)
// Space Complexity: O(1)
template <typename T>
point<double> angle_bisector(const point<T>& a, const point<T>& b, const point<T>& c) {
    point<double> ba(a.x - b.x, a.y - b.y);
    point<double> bc(c.x - b.x, c.y - b.y);
    double len_ba = length(ba);
    double len_bc = length(bc);
    if (len_ba <= EPS || len_bc <= EPS) return point<double>(0, 0);
    return point<double>(ba.x / len_ba + bc.x / len_bc, ba.y / len_ba + bc.y / len_bc);
}
