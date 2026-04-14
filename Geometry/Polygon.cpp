// Polygon Template (generic)
// Dependencies: Point.cpp, Line.cpp (include them before this file)
// Optional: Convex_Hull.cpp (required if you use unite() or diameter())

// Function Dependency Index (alphabetical, direct, template-only)
// Format:
//   - polygon::name(args)
//       Use: ...
//       Depends on: ...
//       Time: ...
//
// - polygon::add_point(p)
//     Use: append vertex.
//     Depends on: (none).
//     Time: amortized O(1).
//
// - polygon::area()
//     Use: absolute area.
//     Depends on: signed_area() (wrapper).
//     Time: O(n).
//
// - polygon::bounding_box()
//     Use: axis-aligned bounding box.
//     Depends on: (none).
//     Time: O(n).
//
// - polygon::centroid()
//     Use: polygon centroid (area-weighted; fallback to average).
//     Depends on: (none).
//     Time: O(n).
//
// - polygon::closest_point(p)
//     Use: closest boundary point to p.
//     Depends on: contains(), line<t>::distance().
//     Time: O(n).
//
// - polygon::contains(p)
//     Use: point-in-polygon (winding number).
//     Depends on: on_boundary(), point<t>::cross_val().
//     Time: O(n).
//
// - polygon::cut(line)
//     Use: clip by a directed line (wrapper overload).
//     Depends on: cut_by_line() (wrapper).
//     Time: O(n).
//
// - polygon::cut(polygon)
//     Use: clip by a polygon (wrapper overload).
//     Depends on: cut_by_polygon() (wrapper).
//     Time: O(n*m).
//
// - polygon::cut_by_line(line)
//     Use: half-plane clipping by directed line.
//     Depends on: side_value(), segment_line_intersection(), point<t>::same_point().
//     Time: O(n).
//
// - polygon::cut_by_polygon(clipper)
//     Use: clip by convex polygon (Sutherland–Hodgman).
//     Depends on: signed_area(), cut_by_line(), side_value(), set_vertices().
//     Time: O(n*m).
//
// - polygon::decrease_at_middle(factor)
//     Use: shrink around centroid.
//     Depends on: scale() (wrapper).
//     Time: O(n).
//
// - polygon::delete_point(p)
//     Use: delete first vertex equal to p within eps.
//     Depends on: point<t>::same_point().
//     Time: O(n).
//
// - polygon::delete_point_at(idx)
//     Use: delete vertex at index.
//     Depends on: (none).
//     Time: O(n).
//
// - polygon::diameter()
//     Use: max distance between any two vertices (via hull).
//     Depends on: diameter2() (wrapper).
//     Time: O(n log n).
//
// - polygon::diameter2()
//     Use: max squared distance between any two vertices (via hull).
//     Depends on: cp_geometry::convex_hull_points(), point<t>::cross_val().
//     Time: O(n log n).
//
// - polygon::distance_to(p)
//     Use: min distance from p to polygon (0 if inside).
//     Depends on: contains(), line<t>::distance().
//     Time: O(n).
//
// - polygon::edges()
//     Use: polygon edges as segments.
//     Depends on: (none).
//     Time: O(n).
//
// - polygon::get_vertices()
//     Use: return copy of vertices.
//     Depends on: (none).
//     Time: O(n).
//
// - polygon::increase_at_middle(factor)
//     Use: grow around centroid.
//     Depends on: scale() (wrapper).
//     Time: O(n).
//
// - polygon::intersect(other)
//     Use: intersection with another convex polygon (wrapper).
//     Depends on: cut_by_polygon() (wrapper).
//     Time: O(n*m).
//
// - polygon::is_convex(strict)
//     Use: convexity check.
//     Depends on: point<t>::cross_val().
//     Time: O(n).
//
// - polygon::on_boundary(p)
//     Use: boundary check.
//     Depends on: point<t>::on_segment().
//     Time: O(n).
//
// - polygon::perimeter()
//     Use: polygon perimeter.
//     Depends on: point<t>::dist().
//     Time: O(n).
//
// - polygon::polygon()
//     Use: create empty polygon.
//     Depends on: (none).
//     Time: O(1).
//
// - polygon::polygon(vertices)
//     Use: create polygon from vertices.
//     Depends on: (none).
//     Time: O(n).
//
// - polygon::reflect(axis)
//     Use: reflect polygon around line.
//     Depends on: point<t>::reflect().
//     Time: O(n).
//
// - polygon::reflect_x()
//     Use: reflect polygon across x-axis.
//     Depends on: point<t>::reflect_x().
//     Time: O(n).
//
// - polygon::reflect_y()
//     Use: reflect polygon across y-axis.
//     Depends on: point<t>::reflect_y().
//     Time: O(n).
//
// - polygon::rotate(angle)
//     Use: rotate around centroid.
//     Depends on: centroid(), rotate_around() (wrapper).
//     Time: O(n).
//
// - polygon::rotate_around(pivot)
//     Use: rotate around pivot.
//     Depends on: point<t>::rotate_around().
//     Time: O(n).
//
// - polygon::scale(factor)
//     Use: scale around centroid.
//     Depends on: centroid(), scale_from() (wrapper).
//     Time: O(n).
//
// - polygon::scale_at_middle(factor)
//     Use: scale around centroid (wrapper).
//     Depends on: scale() (wrapper).
//     Time: O(n).
//
// - polygon::scale_from(pivot)
//     Use: scale around pivot.
//     Depends on: point<t>::scale_from().
//     Time: O(n).
//
// - polygon::segment_line_intersection(p1,p2,a,b)
//     Use: helper for clipping.
//     Depends on: (none).
//     Time: O(1).
//
// - polygon::set_vertices(vertices)
//     Use: replace vertices and update n.
//     Depends on: (none).
//     Time: O(n).
//
// - polygon::side_value(a,b,p)
//     Use: helper signed-side test.
//     Depends on: point<t>::cross_val().
//     Time: O(1).
//
// - polygon::signed_area()
//     Use: signed area (shoelace).
//     Depends on: (none).
//     Time: O(n).
//
// - polygon::transform(Func)
//     Use: map each vertex with a callback.
//     Depends on: (none).
//     Time: O(n).
//
// - polygon::translate(delta)
//     Use: translate polygon.
//     Depends on: (none).
//     Time: O(n).
//
// - polygon::unite(other)
//     Use: convex-hull union of vertices.
//     Depends on: cp_geometry::convex_hull_points().
//     Time: O((n+m) log(n+m)).
//
// - polygon::vertex(idx)
//     Use: vertex by circular index.
//     Depends on: (none).
//     Time: O(1).

#pragma once

#include <bits/stdc++.h>

using namespace std;

template <typename t>
struct polygon
{
    vector<point<t>> vertices;
    int n;

private:
    static long double side_value(const point<t>& a, const point<t>& b, const point<t>& p);
    static point<t> segment_line_intersection(const point<t>& p1, const point<t>& p2, const point<t>& a, const point<t>& b);

public:
    polygon() : n(0) {}
    polygon(const vector<point<t>>& verts) : vertices(verts), n((int)verts.size()) {}
    void set_vertices(const vector<point<t>>& verts)
    {
        vertices = verts;
        n = (int)vertices.size();
    }
    vector<point<t>> get_vertices() const
    {
        return vertices;
    }
    point<t> vertex(int index) const
    {
        if (n == 0) return point<t>(0, 0);
        index = ((index % n) + n) % n;
        return vertices[index];
    }
    void add_point(const point<t>& p)
    {
        vertices.push_back(p);
        n = (int)vertices.size();
    }
    bool delete_point_at(int idx)
    {
        if (idx < 0 || idx >= n) return false;
        vertices.erase(vertices.begin() + idx);
        n = (int)vertices.size();
        return true;
    }
    bool delete_point(const point<t>& p, long double eps = cp_geometry::EPS)
    {
        for (int i = 0; i < n; i++)
        {
            if (point<t>::same_point(vertices[i], p, eps))
            {
                vertices.erase(vertices.begin() + i);
                n = (int)vertices.size();
                return true;
            }
        }
        return false;
    }
    vector<line<t>> edges() const
    {
        vector<line<t>> result;
        if (n < 2) return result;

        result.reserve(n);
        for (int i = 0; i < n; i++)
        {
            int j = (i + 1) % n;
            result.push_back(line<t>(vertices[i], vertices[j]));
        }
        return result;
    }
    double signed_area() const;
    double area() const;
    double perimeter() const;
    long double diameter2(bool keep_collinear = false) const;
    double diameter(bool keep_collinear = false) const;
    pair<point<t>, point<t>> bounding_box() const;
    point<t> centroid() const;
    bool is_convex(bool strict = false) const;
    bool on_boundary(const point<t>& p, double tolerance = (double)cp_geometry::EPS) const;
    bool contains(const point<t>& p, bool include_boundary = true) const;
    double distance_to(const point<t>& p) const;
    point<t> closest_point(const point<t>& p) const;
    polygon<t> translate(const point<t>& delta) const;
    polygon<t> rotate_around(const point<t>& pivot, double angle, bool in_degrees = false) const;
    polygon<t> rotate(double angle, bool in_degrees = false) const;
    polygon<t> scale_from(const point<t>& pivot, double factor) const;
    polygon<t> scale(double factor) const;
    polygon<t> scale_at_middle(double factor) const;
    polygon<t> increase_at_middle(double factor) const;
    polygon<t> decrease_at_middle(double factor) const;
    polygon<t> cut_by_line(const line<t>& cutting_line, bool keep_left = true) const;
    polygon<t> cut_by_polygon(const polygon<t>& clipper, bool keep_boundary = true) const;
    polygon<t> cut(const line<t>& cutting_line, bool keep_left = true) const;
    polygon<t> cut(const polygon<t>& clipper, bool keep_boundary = true) const;
    polygon<t> intersect(const polygon<t>& other, bool keep_boundary = true) const;
    polygon<t> unite(const polygon<t>& other, bool keep_collinear = false) const;
    polygon<t> reflect(const line<t>& axis) const;
    polygon<t> reflect_x() const;
    polygon<t> reflect_y() const;
    template <typename Func>
    polygon<t> transform(Func func) const;
};

// -------------------- Implementations --------------------

template <typename t>
long double polygon<t>::side_value(const point<t>& a, const point<t>& b, const point<t>& p)
{
    return point<t>::cross_val(a, b, p);
}

template <typename t>
point<t> polygon<t>::segment_line_intersection(const point<t>& p1, const point<t>& p2, const point<t>& a, const point<t>& b)
{
    point<t> r = p2 - p1;
    point<t> s = b - a;

    long double den = (long double)r.x * (long double)s.y - (long double)r.y * (long double)s.x;
    if (fabsl(den) <= cp_geometry::EPS) return p2;

    point<t> ap = a - p1;
    long double num = (long double)ap.x * (long double)s.y - (long double)ap.y * (long double)s.x;
    long double tval = num / den;
    return point<t>(
        (t)((long double)p1.x + (long double)r.x * tval),
        (t)((long double)p1.y + (long double)r.y * tval)
    );
}

template <typename t>
double polygon<t>::signed_area() const
{
    if (n < 3) return 0.0;

    long double sum = 0.0L;
    for (int i = 0; i < n; i++)
    {
        int j = (i + 1) % n;
        sum += (long double)vertices[i].x * vertices[j].y - (long double)vertices[i].y * vertices[j].x;
    }
    return (double)(sum * 0.5L);
}

template <typename t>
double polygon<t>::area() const
{
    return fabs(signed_area());
}

template <typename t>
double polygon<t>::perimeter() const
{
    if (n < 2) return 0.0;

    double p = 0.0;
    for (int i = 0; i < n; i++)
    {
        int j = (i + 1) % n;
        p += point<t>::dist(vertices[i], vertices[j]);
    }
    return p;
}

template <typename t>
long double polygon<t>::diameter2(bool keep_collinear) const
{
    if (n <= 1) return 0.0L;

    vector<point<t>> hull = cp_geometry::convex_hull_points(vertices, keep_collinear);
    int m = (int)hull.size();

    auto dist2 = [&](const point<t>& a, const point<t>& b) -> long double
    {
        long double dx = (long double)a.x - (long double)b.x;
        long double dy = (long double)a.y - (long double)b.y;
        return dx * dx + dy * dy;
    };

    if (m <= 1) return 0.0L;
    if (m == 2) return dist2(hull[0], hull[1]);

    long double best = 0.0L;
    int j = 1;

    for (int i = 0; i < m; i++)
    {
        int ni = (i + 1) % m;

        while (true)
        {
            int nj = (j + 1) % m;
            long double cur = point<t>::cross_val(hull[i], hull[ni], hull[j]);
            long double nxt = point<t>::cross_val(hull[i], hull[ni], hull[nj]);

            // For CCW convex hull, cross_val is non-negative up to eps.
            if (nxt > cur + cp_geometry::EPS) j = nj;
            else break;
        }

        best = max(best, dist2(hull[i], hull[j]));
        best = max(best, dist2(hull[ni], hull[j]));
    }

    return best;
}

template <typename t>
double polygon<t>::diameter(bool keep_collinear) const
{
    return sqrt((double)diameter2(keep_collinear));
}

template <typename t>
pair<point<t>, point<t>> polygon<t>::bounding_box() const
{
    if (n == 0) return {point<t>(0, 0), point<t>(0, 0)};

    t min_x = vertices[0].x, max_x = vertices[0].x;
    t min_y = vertices[0].y, max_y = vertices[0].y;

    for (const auto& v : vertices)
    {
        min_x = min(min_x, v.x);
        max_x = max(max_x, v.x);
        min_y = min(min_y, v.y);
        max_y = max(max_y, v.y);
    }

    return {point<t>(min_x, min_y), point<t>(max_x, max_y)};
}

template <typename t>
point<t> polygon<t>::centroid() const
{
    if (n == 0) return point<t>(0, 0);

    long double a2 = 0.0L;
    long double cx = 0.0L;
    long double cy = 0.0L;

    for (int i = 0; i < n; i++)
    {
        int j = (i + 1) % n;
        long double cross = (long double)vertices[i].x * vertices[j].y - (long double)vertices[j].x * vertices[i].y;
        a2 += cross;
        cx += ((long double)vertices[i].x + vertices[j].x) * cross;
        cy += ((long double)vertices[i].y + vertices[j].y) * cross;
    }

    if (fabsl(a2) <= cp_geometry::EPS)
    {
        long double sx = 0.0L, sy = 0.0L;
        for (const auto& v : vertices)
        {
            sx += v.x;
            sy += v.y;
        }
        return point<t>((t)(sx / n), (t)(sy / n));
    }

    cx /= (3.0L * a2);
    cy /= (3.0L * a2);
    return point<t>((t)cx, (t)cy);
}

template <typename t>
bool polygon<t>::is_convex(bool strict) const
{
    if (n < 3) return false;

    int sign = 0;
    for (int i = 0; i < n; i++)
    {
        int j = (i + 1) % n;
        int k = (i + 2) % n;
        long double c = point<t>::cross_val(vertices[i], vertices[j], vertices[k]);

        if (fabsl(c) <= cp_geometry::EPS)
        {
            if (strict) return false;
            continue;
        }

        int cur = (c > 0 ? 1 : -1);
        if (sign == 0) sign = cur;
        else if (cur != sign) return false;
    }

    return sign != 0 || !strict;
}

template <typename t>
bool polygon<t>::on_boundary(const point<t>& p, double tolerance) const
{
    if (n < 2) return false;

    for (int i = 0; i < n; i++)
    {
        int j = (i + 1) % n;
        if (point<t>::on_segment(p, vertices[i], vertices[j], (long double)tolerance)) return true;
    }

    return false;
}

template <typename t>
bool polygon<t>::contains(const point<t>& p, bool include_boundary) const
{
    if (n < 3) return false;
    if (include_boundary && on_boundary(p)) return true;

    int winding = 0;
    for (int i = 0; i < n; i++)
    {
        int j = (i + 1) % n;
        const point<t>& a = vertices[i];
        const point<t>& b = vertices[j];

        if ((long double)a.y <= (long double)p.y)
        {
            if ((long double)b.y > (long double)p.y)
            {
                long double c = point<t>::cross_val(a, b, p);
                if (c > cp_geometry::EPS) winding++;
            }
        }
        else
        {
            if ((long double)b.y <= (long double)p.y)
            {
                long double c = point<t>::cross_val(a, b, p);
                if (c < -cp_geometry::EPS) winding--;
            }
        }
    }

    return winding != 0;
}

template <typename t>
double polygon<t>::distance_to(const point<t>& p) const
{
    if (contains(p, true)) return 0.0;
    if (n == 0) return 0.0;

    double best = 1e100;
    for (int i = 0; i < n; i++)
    {
        int j = (i + 1) % n;
        line<t> e(vertices[i], vertices[j]);
        best = min(best, e.distance(p).first);
    }

    return best;
}

template <typename t>
point<t> polygon<t>::closest_point(const point<t>& p) const
{
    if (n == 0) return point<t>(0, 0);
    if (contains(p, true)) return p;

    double best = 1e100;
    point<t> ans = vertices[0];

    for (int i = 0; i < n; i++)
    {
        int j = (i + 1) % n;
        line<t> e(vertices[i], vertices[j]);
        auto [d, q] = e.distance(p);
        if (d < best)
        {
            best = d;
            ans = q;
        }
    }

    return ans;
}

template <typename t>
polygon<t> polygon<t>::translate(const point<t>& delta) const
{
    vector<point<t>> out;
    out.reserve(n);
    for (const auto& v : vertices) out.push_back(v + delta);
    return polygon<t>(out);
}

template <typename t>
polygon<t> polygon<t>::rotate_around(const point<t>& pivot, double angle, bool in_degrees) const
{
    vector<point<t>> out;
    out.reserve(n);
    for (const auto& v : vertices) out.push_back(v.rotate_around(pivot, angle, in_degrees));
    return polygon<t>(out);
}

template <typename t>
polygon<t> polygon<t>::rotate(double angle, bool in_degrees) const
{
    return rotate_around(centroid(), angle, in_degrees);
}

template <typename t>
polygon<t> polygon<t>::scale_from(const point<t>& pivot, double factor) const
{
    vector<point<t>> out;
    out.reserve(n);
    for (const auto& v : vertices) out.push_back(v.scale_from(pivot, factor));
    return polygon<t>(out);
}

template <typename t>
polygon<t> polygon<t>::scale(double factor) const
{
    return scale_from(centroid(), factor);
}

template <typename t>
polygon<t> polygon<t>::scale_at_middle(double factor) const
{
    return scale(factor);
}

template <typename t>
polygon<t> polygon<t>::increase_at_middle(double factor) const
{
    if (factor < 0) factor = -factor;
    return scale(factor);
}

template <typename t>
polygon<t> polygon<t>::decrease_at_middle(double factor) const
{
    if (factor <= (double)cp_geometry::EPS) return *this;
    return scale(1.0 / factor);
}

template <typename t>
polygon<t> polygon<t>::cut_by_line(const line<t>& cutting_line, bool keep_left) const
{
    if (n == 0) return polygon<t>();

    vector<point<t>> out;
    out.reserve(n + 2);

    for (int i = 0; i < n; i++)
    {
        const point<t>& cur = vertices[i];
        const point<t>& nxt = vertices[(i + 1) % n];

        long double sc = side_value(cutting_line.a, cutting_line.b, cur);
        long double sn = side_value(cutting_line.a, cutting_line.b, nxt);

        bool in_cur = keep_left ? (sc >= -cp_geometry::EPS) : (sc <= cp_geometry::EPS);
        bool in_nxt = keep_left ? (sn >= -cp_geometry::EPS) : (sn <= cp_geometry::EPS);

        if (in_cur && in_nxt)
        {
            out.push_back(nxt);
        }
        else if (in_cur && !in_nxt)
        {
            out.push_back(segment_line_intersection(cur, nxt, cutting_line.a, cutting_line.b));
        }
        else if (!in_cur && in_nxt)
        {
            out.push_back(segment_line_intersection(cur, nxt, cutting_line.a, cutting_line.b));
            out.push_back(nxt);
        }
    }

    if (out.empty()) return polygon<t>();

    vector<point<t>> clean;
    clean.reserve(out.size());
    for (const auto& p : out)
    {
        if (clean.empty() || !point<t>::same_point(clean.back(), p)) clean.push_back(p);
    }

    if ((int)clean.size() >= 2 && point<t>::same_point(clean.front(), clean.back())) clean.pop_back();

    return polygon<t>(clean);
}

template <typename t>
polygon<t> polygon<t>::cut_by_polygon(const polygon<t>& clipper, bool keep_boundary) const
{
    if (n == 0 || clipper.n < 3) return polygon<t>();

    polygon<t> result = *this;
    bool ccw = clipper.signed_area() >= 0.0;

    for (int i = 0; i < clipper.n && result.n > 0; i++)
    {
        int j = (i + 1) % clipper.n;
        line<t> edge(clipper.vertices[i], clipper.vertices[j]);

        bool keep_left = ccw;
        if (!keep_boundary)
        {
            result = result.cut_by_line(edge, keep_left);

            vector<point<t>> strict;
            strict.reserve(result.n);
            for (const auto& p : result.vertices)
            {
                long double s = side_value(edge.a, edge.b, p);
                bool inside = keep_left ? (s > cp_geometry::EPS) : (s < -cp_geometry::EPS);
                if (inside) strict.push_back(p);
            }
            result.set_vertices(strict);
        }
        else
        {
            result = result.cut_by_line(edge, keep_left);
        }
    }

    return result;
}

template <typename t>
polygon<t> polygon<t>::cut(const line<t>& cutting_line, bool keep_left) const
{
    return cut_by_line(cutting_line, keep_left);
}

template <typename t>
polygon<t> polygon<t>::cut(const polygon<t>& clipper, bool keep_boundary) const
{
    return cut_by_polygon(clipper, keep_boundary);
}

template <typename t>
polygon<t> polygon<t>::intersect(const polygon<t>& other, bool keep_boundary) const
{
    return cut_by_polygon(other, keep_boundary);
}

template <typename t>
polygon<t> polygon<t>::unite(const polygon<t>& other, bool keep_collinear) const
{
    vector<point<t>> all = vertices;
    all.insert(all.end(), other.vertices.begin(), other.vertices.end());
    return polygon<t>(cp_geometry::convex_hull_points(all, keep_collinear));
}

template <typename t>
polygon<t> polygon<t>::reflect(const line<t>& axis) const
{
    vector<point<t>> out;
    out.reserve(n);
    for (const auto& v : vertices) out.push_back(v.reflect(axis.a, axis.b));
    return polygon<t>(out);
}

template <typename t>
polygon<t> polygon<t>::reflect_x() const
{
    vector<point<t>> out;
    out.reserve(n);
    for (const auto& v : vertices) out.push_back(v.reflect_x());
    return polygon<t>(out);
}

template <typename t>
polygon<t> polygon<t>::reflect_y() const
{
    vector<point<t>> out;
    out.reserve(n);
    for (const auto& v : vertices) out.push_back(v.reflect_y());
    return polygon<t>(out);
}

template <typename t>
template <typename Func>
polygon<t> polygon<t>::transform(Func func) const
{
    vector<point<t>> out;
    out.reserve(n);
    for (const auto& v : vertices) out.push_back(func(v));
    return polygon<t>(out);
}
