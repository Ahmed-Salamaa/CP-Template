/*
 * Topic: Line
 * Description: Provides a mathematical representation of a 2D line (Ax + By + C = 0),
 *   supporting operations like intersection, distance to a point, and
 *   parallel/perpendicular checks.
 * projection of a point onto a line, and calculating distances from points to lines or segments. Usage: Line
 * l1(Point(0, 0), Point(10, 10)); Line l2(Point(0, 10), Point(10, 0)); Point p = l1.intersect(l2); double d =
 * l1.distToPoint(Point(5, 0));
 */
#include "../core.h"
#pragma once
#include "Point.cpp"

// Line structure for 2D geometry (represented by two points)
// Time Complexity: O(1)
// Space Complexity: O(1)
template <typename T>
struct line {
    point<T> a, b;

    line() {}

    line(point<T> a, point<T> b) : a(a), b(b) {}
};

// Calculates: The projection of a point onto an infinite line.
// Returns: A point representing the projected location.
// Time Complexity: O(1)
// Space Complexity: O(1)
template <typename T>
point<T> project_point_on_line(const point<T>& p, const line<T>& l) {
    point<T> ab = l.b - l.a;
    ld ab2 = norm2(ab);
    if (fabsl(ab2) <= EPS) return l.a;
    ld t = dot_product(p - l.a, ab) / ab2;
    return point<T>(l.a.x + ab.x * t, l.a.y + ab.y * t);
}

// Calculates: The shortest distance from a point to an infinite line.
// Returns: A double representing the distance.
// Time Complexity: O(1)
// Space Complexity: O(1)
template <typename T>
double distance_point_to_line(const point<T>& p, const line<T>& l) {
    return distance(p, project_point_on_line(p, l));
}

// Calculates: The projection of a point onto a finite line segment.
// Returns: A point representing the projected location clamped to the segment.
// Time Complexity: O(1)
// Space Complexity: O(1)
template <typename T>
point<T> project_point_on_segment(const point<T>& p, const line<T>& seg) {
    point<T> ab = seg.b - seg.a;
    ld ab2 = norm2(ab);
    if (fabsl(ab2) <= EPS) return seg.a;
    ld t = dot_product(p - seg.a, ab) / ab2;
    t = max(0.0L, min(1.0L, t));
    return point<T>(seg.a.x + ab.x * t, seg.a.y + ab.y * t);
}

// Calculates: The shortest distance from a point to a line segment.
// Returns: A double representing the distance.
// Time Complexity: O(1)
// Space Complexity: O(1)
template <typename T>
double distance_point_to_segment(const point<T>& p, const line<T>& seg) {
    return distance(p, project_point_on_segment(p, seg));
}

// Calculates: The intersection point of two infinite lines.
// Returns: A pair of a boolean (true if they intersect) and the intersection point.
// Time Complexity: O(1)
// Space Complexity: O(1)
template <typename T>
pair<bool, point<T>> line_intersection(const line<T>& l1, const line<T>& l2) {
    T a1 = l1.b.y - l1.a.y, b1 = l1.a.x - l1.b.x, c1 = -(a1 * l1.a.x + b1 * l1.a.y);
    T a2 = l2.b.y - l2.a.y, b2 = l2.a.x - l2.b.x, c2 = -(a2 * l2.a.x + b2 * l2.a.y);
    ld det = (ld)a1 * b2 - (ld)a2 * b1;
    if (fabsl(det) <= EPS) return {false, point<T>(0, 0)};
    ld x = ((ld)b1 * c2 - (ld)b2 * c1) / det;
    ld y = ((ld)a2 * c1 - (ld)a1 * c2) / det;
    return {true, point<T>((T)x, (T)y)};
}

// Calculates: Whether two line segments intersect.
// Returns: True if they intersect, false otherwise.
// Time Complexity: O(1)
// Space Complexity: O(1)
template <typename T>
bool do_segments_intersect(const line<T>& s1, const line<T>& s2) {
    int o1 = orientation(s1.a, s1.b, s2.a);
    int o2 = orientation(s1.a, s1.b, s2.b);
    int o3 = orientation(s2.a, s2.b, s1.a);
    int o4 = orientation(s2.a, s2.b, s1.b);
    if (o1 != o2 && o3 != o4) return true;
    if (o1 == 0 && on_segment(s2.a, s1.a, s1.b)) return true;
    if (o2 == 0 && on_segment(s2.b, s1.a, s1.b)) return true;
    if (o3 == 0 && on_segment(s1.a, s2.a, s2.b)) return true;
    if (o4 == 0 && on_segment(s1.b, s2.a, s2.b)) return true;
    return false;
}

// Calculates: The intersection point of two line segments.
// Returns: A pair of a boolean (true if they intersect) and the intersection point.
// Time Complexity: O(1)
// Space Complexity: O(1)
template <typename T>
pair<bool, point<T>> segment_intersection(const line<T>& s1, const line<T>& s2) {
    int o1 = orientation(s1.a, s1.b, s2.a);
    int o2 = orientation(s1.a, s1.b, s2.b);
    int o3 = orientation(s2.a, s2.b, s1.a);
    int o4 = orientation(s2.a, s2.b, s1.b);
    if (o1 == 0 && on_segment(s2.a, s1.a, s1.b)) return {true, s2.a};
    if (o2 == 0 && on_segment(s2.b, s1.a, s1.b)) return {true, s2.b};
    if (o3 == 0 && on_segment(s1.a, s2.a, s2.b)) return {true, s1.a};
    if (o4 == 0 && on_segment(s1.b, s2.a, s2.b)) return {true, s1.b};
    if (o1 != o2 && o3 != o4) { return line_intersection(s1, s2); }
    return {false, point<T>(0, 0)};
}

// Calculates: The shortest distance between two line segments.
// Returns: A double representing the distance.
// Time Complexity: O(1)
// Space Complexity: O(1)
template <typename T>
double distance_segment_to_segment(const line<T>& s1, const line<T>& s2) {
    if (do_segments_intersect(s1, s2)) return 0.0;
    double ans = distance_point_to_segment(s1.a, s2);
    ans = min(ans, distance_point_to_segment(s1.b, s2));
    ans = min(ans, distance_point_to_segment(s2.a, s1));
    ans = min(ans, distance_point_to_segment(s2.b, s1));
    return ans;
}
