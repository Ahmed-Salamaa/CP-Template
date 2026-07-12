#pragma once
#include "Point.cpp"

template <typename T>
struct line {
    point<T> a, b;
    line() {}
    line(point<T> a, point<T> b) : a(a), b(b) {}
};

// Time Complexity: O(1)
// Space Complexity: O(1)
template <typename T>
point<T> project_point_on_line(const point<T>& p, const line<T>& l) {
    point<T> ab = l.b - l.a;
    long double ab2 = norm2(ab);
    if (fabsl(ab2) <= cp_geometry::EPS) return l.a;
    long double t = dot_product(p - l.a, ab) / ab2;
    return point<T>(l.a.x + ab.x * t, l.a.y + ab.y * t);
}

// Time Complexity: O(1)
// Space Complexity: O(1)
template <typename T>
double distance_point_to_line(const point<T>& p, const line<T>& l) {
    return distance(p, project_point_on_line(p, l));
}

// Time Complexity: O(1)
// Space Complexity: O(1)
template <typename T>
point<T> project_point_on_segment(const point<T>& p, const line<T>& seg) {
    point<T> ab = seg.b - seg.a;
    long double ab2 = norm2(ab);
    if (fabsl(ab2) <= cp_geometry::EPS) return seg.a;
    long double t = dot_product(p - seg.a, ab) / ab2;
    t = std::max(0.0L, std::min(1.0L, t));
    return point<T>(seg.a.x + ab.x * t, seg.a.y + ab.y * t);
}

// Time Complexity: O(1)
// Space Complexity: O(1)
template <typename T>
double distance_point_to_segment(const point<T>& p, const line<T>& seg) {
    return distance(p, project_point_on_segment(p, seg));
}

// Time Complexity: O(1)
// Space Complexity: O(1)
template <typename T>
std::pair<bool, point<T>> line_intersection(const line<T>& l1, const line<T>& l2) {
    T a1 = l1.b.y - l1.a.y, b1 = l1.a.x - l1.b.x, c1 = -(a1 * l1.a.x + b1 * l1.a.y);
    T a2 = l2.b.y - l2.a.y, b2 = l2.a.x - l2.b.x, c2 = -(a2 * l2.a.x + b2 * l2.a.y);
    long double det = (long double)a1 * b2 - (long double)a2 * b1;
    if (fabsl(det) <= cp_geometry::EPS) return {false, point<T>(0, 0)};
    long double x = ((long double)b1 * c2 - (long double)b2 * c1) / det;
    long double y = ((long double)a2 * c1 - (long double)a1 * c2) / det;
    return {true, point<T>((T)x, (T)y)};
}

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

// Time Complexity: O(1)
// Space Complexity: O(1)
template <typename T>
std::pair<bool, point<T>> segment_intersection(const line<T>& s1, const line<T>& s2) {
    int o1 = orientation(s1.a, s1.b, s2.a);
    int o2 = orientation(s1.a, s1.b, s2.b);
    int o3 = orientation(s2.a, s2.b, s1.a);
    int o4 = orientation(s2.a, s2.b, s1.b);

    if (o1 == 0 && on_segment(s2.a, s1.a, s1.b)) return {true, s2.a};
    if (o2 == 0 && on_segment(s2.b, s1.a, s1.b)) return {true, s2.b};
    if (o3 == 0 && on_segment(s1.a, s2.a, s2.b)) return {true, s1.a};
    if (o4 == 0 && on_segment(s1.b, s2.a, s2.b)) return {true, s1.b};

    if (o1 != o2 && o3 != o4) {
        return line_intersection(s1, s2);
    }
    return {false, point<T>(0, 0)};
}

// Time Complexity: O(1)
// Space Complexity: O(1)
template <typename T>
double distance_segment_to_segment(const line<T>& s1, const line<T>& s2) {
    if (do_segments_intersect(s1, s2)) return 0.0;
    double ans = distance_point_to_segment(s1.a, s2);
    ans = std::min(ans, distance_point_to_segment(s1.b, s2));
    ans = std::min(ans, distance_point_to_segment(s2.a, s1));
    ans = std::min(ans, distance_point_to_segment(s2.b, s1));
    return ans;
}
