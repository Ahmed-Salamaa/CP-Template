#pragma once
#include <bits/stdc++.h>
using namespace std;

namespace cp_geometry {
    static const long double PI = acos(-1.0L);
    static const long double EPS = 1e-9L;
    
    int sgn(long double v, long double eps = EPS) {
        if (fabsl(v) <= eps) return 0;
        return (v < 0 ? -1 : 1);
    }
}

template <typename T>
struct point {
    T x, y;
    point() : x(0), y(0) {}
    point(T x_, T y_) : x(x_), y(y_) {}
    
    point operator + (const point& o) const { return point(x + o.x, y + o.y); }
    point operator - (const point& o) const { return point(x - o.x, y - o.y); }
    point operator * (T k) const { return point(x * k, y * k); }
    point operator / (T k) const { return point(x / k, y / k); }
    
    bool operator < (const point& o) const { return (x != o.x) ? x < o.x : y < o.y; }
    bool operator == (const point& o) const { return x == o.x && y == o.y; }
};

// Time Complexity: O(1)
// Space Complexity: O(1)
template <typename T>
T dot_product(const point<T>& a, const point<T>& b) {
    return a.x * b.x + a.y * b.y;
}

// Time Complexity: O(1)
// Space Complexity: O(1)
template <typename T>
T cross_product(const point<T>& a, const point<T>& b) {
    return a.x * b.y - a.y * b.x;
}

// Time Complexity: O(1)
// Space Complexity: O(1)
template <typename T>
T cross_product(const point<T>& a, const point<T>& b, const point<T>& c) {
    return cross_product(b - a, c - a);
}

// Time Complexity: O(1)
// Space Complexity: O(1)
template <typename T>
long double cross_val(const point<T>& a, const point<T>& b, const point<T>& c) {
    return ((long double)b.x - a.x) * ((long double)c.y - a.y) - ((long double)b.y - a.y) * ((long double)c.x - a.x);
}

// Time Complexity: O(1)
// Space Complexity: O(1)
template <typename T>
T norm2(const point<T>& a) {
    return a.x * a.x + a.y * a.y;
}

// Time Complexity: O(1)
// Space Complexity: O(1)
template <typename T>
double length(const point<T>& a) {
    return sqrt((double)norm2(a));
}

// Returns: 1 (CCW), -1 (CW), 0 (collinear)
// Time Complexity: O(1)
// Space Complexity: O(1)
template <typename T>
int orientation(const point<T>& a, const point<T>& b, const point<T>& c) {
    return cp_geometry::sgn(cross_val(a, b, c));
}

// Time Complexity: O(1)
// Space Complexity: O(1)
template <typename T>
bool on_segment(const point<T>& p, const point<T>& a, const point<T>& b) {
    if (orientation(a, b, p) != 0) return false;
    return p.x >= std::min(a.x, b.x) && p.x <= std::max(a.x, b.x) &&
           p.y >= std::min(a.y, b.y) && p.y <= std::max(a.y, b.y);
}

// Time Complexity: O(1)
// Space Complexity: O(1)
template <typename T>
double distance(const point<T>& a, const point<T>& b) {
    return length(a - b);
}

// Rotate around origin
// Time Complexity: O(1)
// Space Complexity: O(1)
template <typename T>
point<T> rotate(const point<T>& p, double angle) {
    double c = cos(angle), s = sin(angle);
    return point<T>((T)(p.x * c - p.y * s), (T)(p.x * s + p.y * c));
}

// Rotate around pivot
// Time Complexity: O(1)
// Space Complexity: O(1)
template <typename T>
point<T> rotate_around(const point<T>& p, const point<T>& pivot, double angle) {
    return rotate(p - pivot, angle) + pivot;
}

// Time Complexity: O(1)
// Space Complexity: O(1)
template <typename T>
point<T> project_vector(const point<T>& v, const point<T>& onto) {
    double d = dot_product(v, onto) / (double)norm2(onto);
    return point<T>((T)(onto.x * d), (T)(onto.y * d));
}

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

// Sort points by polar angle around a pivot
// Time Complexity: O(n log n)
// Space Complexity: O(1)
template <typename T>
void polar_sort(std::vector<point<T>>& pts, const point<T>& pivot) {
    auto half = [&](const point<T>& p) {
        return p.y > pivot.y || (p.y == pivot.y && p.x > pivot.x);
    };
    std::sort(pts.begin(), pts.end(), [&](const point<T>& a, const point<T>& b) {
        int ha = half(a), hb = half(b);
        if (ha != hb) return ha < hb;
        long double cross = cross_val(pivot, a, b);
        if (std::abs(cross) > cp_geometry::EPS) return cross > 0;
        return norm2(a - pivot) < norm2(b - pivot);
    });
}

// Closest pair of points using Divide and Conquer
// Time Complexity: O(n log n)
// Space Complexity: O(n)
template <typename T>
double closest_pair(std::vector<point<T>> pts) {
    if (pts.size() < 2) return 0.0;
    std::sort(pts.begin(), pts.end(), [](const point<T>& a, const point<T>& b) {
        return a.x < b.x || (a.x == b.x && a.y < b.y);
    });
    
    std::vector<point<T>> t(pts.size());
    
    auto solve = [&](auto& self, int l, int r) -> double {
        if (r - l <= 3) {
            double best = 1e18;
            for (int i = l; i < r; ++i)
                for (int j = i + 1; j < r; ++j)
                    best = std::min(best, distance(pts[i], pts[j]));
            std::sort(pts.begin() + l, pts.begin() + r, [](const point<T>& a, const point<T>& b) {
                return a.y < b.y;
            });
            return best;
        }
        
        int mid = (l + r) / 2;
        T midx = pts[mid].x;
        double best = std::min(self(self, l, mid), self(self, mid, r));
        
        std::merge(pts.begin() + l, pts.begin() + mid, pts.begin() + mid, pts.begin() + r, t.begin(), 
            [](const point<T>& a, const point<T>& b) { return a.y < b.y; });
        std::copy(t.begin(), t.begin() + (r - l), pts.begin() + l);
        
        int tsz = 0;
        for (int i = l; i < r; ++i) {
            if (std::abs(pts[i].x - midx) < best) {
                for (int j = tsz - 1; j >= 0 && pts[i].y - t[j].y < best; --j) {
                    best = std::min(best, distance(pts[i], t[j]));
                }
                t[tsz++] = pts[i];
            }
        }
        return best;
    };
    return solve(solve, 0, pts.size());
}

// Angle between two vectors
// Time Complexity: O(1)
// Space Complexity: O(1)
template <typename T>
double angle_between(const point<T>& a, const point<T>& b) {
    double dot = dot_product(a, b);
    double det = cross_product(a, b);
    return std::atan2(det, dot);
}

// Angle bisector of a, b, c (ray from b)
// Time Complexity: O(1)
// Space Complexity: O(1)
template <typename T>
point<double> angle_bisector(const point<T>& a, const point<T>& b, const point<T>& c) {
    point<double> ba(a.x - b.x, a.y - b.y);
    point<double> bc(c.x - b.x, c.y - b.y);
    double len_ba = length(ba);
    double len_bc = length(bc);
    if (len_ba <= cp_geometry::EPS || len_bc <= cp_geometry::EPS) return point<double>(0, 0);
    return point<double>(ba.x / len_ba + bc.x / len_bc, ba.y / len_ba + bc.y / len_bc);
}
