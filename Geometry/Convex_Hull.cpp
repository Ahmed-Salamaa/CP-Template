// Convex Hull Template (generator-only)
// Dependencies: Point.cpp (include it before this file)

#pragma once

#include <bits/stdc++.h>

using namespace std;

namespace cp_geometry
{
    // Does: Remove duplicate points from sorted list using point::same_point.
    // Takes: pts as vector<point<T>>&.
    // Returns: void.
    // Depends on: point<T>::same_point().
    // Time Complexity: O(n).
    template <typename T>
    static void unique_sorted_points(vector<point<T>>& pts)
    {
        vector<point<T>> out;
        out.reserve(pts.size());

        for (const auto& p : pts)
        {
            if (out.empty() || !point<T>::same_point(out.back(), p)) out.push_back(p);
        }
        pts.swap(out);
    }

    // Does: Compute convex hull points in CCW order (Andrew monotone chain).
    // Takes: pts as vector<point<T>>, keep_collinear as bool.
    // Returns: vector<point<T>> (hull vertices).
    // Notes:
    //  - If all points are collinear:
    //    - keep_collinear=false returns {min, max}.
    //    - keep_collinear=true returns sorted unique points.
    // Depends on: unique_sorted_points(), point<T>::orientation(), point<T>::cross_val(), point<T>::same_point().
    // Time Complexity: O(n log n).
    template <typename T>
    vector<point<T>> convex_hull_points(vector<point<T>> pts, bool keep_collinear)
    {
        if (pts.empty()) return {};

        sort(pts.begin(), pts.end());
        unique_sorted_points(pts);

        if ((int)pts.size() <= 2) return pts;

        bool all_collinear = true;
        for (int i = 2; i < (int)pts.size(); i++)
        {
            if (point<T>::orientation(pts[0], pts[1], pts[i]) != 0)
            {
                all_collinear = false;
                break;
            }
        }

        if (all_collinear)
        {
            if (keep_collinear) return pts;
            return {pts.front(), pts.back()};
        }

        vector<point<T>> lower, upper;
        lower.reserve(pts.size());
        upper.reserve(pts.size());

        for (const auto& p : pts)
        {
            while ((int)lower.size() >= 2)
            {
                long double c = point<T>::cross_val(lower[(int)lower.size() - 2], lower.back(), p);
                if (keep_collinear ? (c < -cp_geometry::EPS) : (c <= cp_geometry::EPS)) lower.pop_back();
                else break;
            }
            lower.push_back(p);
        }

        for (int i = (int)pts.size() - 1; i >= 0; i--)
        {
            const auto& p = pts[i];
            while ((int)upper.size() >= 2)
            {
                long double c = point<T>::cross_val(upper[(int)upper.size() - 2], upper.back(), p);
                if (keep_collinear ? (c < -cp_geometry::EPS) : (c <= cp_geometry::EPS)) upper.pop_back();
                else break;
            }
            upper.push_back(p);
        }

        lower.pop_back();
        upper.pop_back();

        vector<point<T>> hull = lower;
        hull.insert(hull.end(), upper.begin(), upper.end());

        // Clean consecutive duplicates and potential wrap duplicate.
        if (!hull.empty())
        {
            vector<point<T>> clean;
            clean.reserve(hull.size());
            for (const auto& p : hull)
            {
                if (clean.empty() || !point<T>::same_point(clean.back(), p)) clean.push_back(p);
            }
            if ((int)clean.size() >= 2 && point<T>::same_point(clean.front(), clean.back())) clean.pop_back();
            hull.swap(clean);
        }

        if ((int)hull.size() == 2 && point<T>::same_point(hull[0], hull[1])) hull.resize(1);
        return hull;
    }
}

template <typename t>
struct convex_hull
{
    vector<point<t>> points;
    vector<point<t>> hull;

    // Does: Build an empty convex hull generator.
    // Time Complexity: O(1).
    convex_hull() {}

    // Does: Build convex hull from points (computed immediately).
    // Takes: pts as vector<point<t>>, keep_collinear as bool.
    // Time Complexity: O(n log n).
    convex_hull(const vector<point<t>>& pts, bool keep_collinear = false)
    {
        set_points(pts);
        build(keep_collinear);
    }

    // Does: Replace input points (does not auto-build).
    // Takes: pts as vector<point<t>>.
    // Time Complexity: O(n).
    void set_points(const vector<point<t>>& pts)
    {
        points = pts;
    }

    // Does: Get current input points.
    // Returns: const vector<point<t>>&.
    // Time Complexity: O(1).
    const vector<point<t>>& get_points() const
    {
        return points;
    }

    // Does: Compute hull from current input points.
    // Takes: keep_collinear as bool.
    // Depends on: cp_geometry::convex_hull_points().
    // Time Complexity: O(n log n).
    void build(bool keep_collinear = false)
    {
        hull = cp_geometry::convex_hull_points(points, keep_collinear);
    }

    // Does: Get computed hull points in CCW order.
    // Returns: const vector<point<t>>&.
    // Time Complexity: O(1).
    const vector<point<t>>& get_hull() const
    {
        return hull;
    }

    // Does: Return number of hull vertices.
    // Returns: int.
    // Time Complexity: O(1).
    int size() const
    {
        return (int)hull.size();
    }

    // Does: Check if computed hull is empty.
    // Returns: bool.
    // Time Complexity: O(1).
    bool empty() const
    {
        return hull.empty();
    }

    // Does: Add a point to the input set; optionally rebuild hull.
    // Takes: p as point<t>, rebuild as bool, keep_collinear as bool.
    // Time Complexity: O(1) + (rebuild ? O(n log n) : 0).
    void add_point(const point<t>& p, bool rebuild = true, bool keep_collinear = false)
    {
        points.push_back(p);
        if (rebuild) build(keep_collinear);
    }

    // Does: Delete input point by index; optionally rebuild hull.
    // Takes: idx as int, rebuild as bool, keep_collinear as bool.
    // Returns: bool (true if deleted).
    // Time Complexity: O(n) + (rebuild ? O(n log n) : 0).
    bool delete_point_at(int idx, bool rebuild = true, bool keep_collinear = false)
    {
        if (idx < 0 || idx >= (int)points.size()) return false;
        points.erase(points.begin() + idx);
        if (rebuild) build(keep_collinear);
        return true;
    }

    // Does: Delete first input point equal to p (within eps); optionally rebuild hull.
    // Takes: p as point<t>, eps as long double, rebuild as bool, keep_collinear as bool.
    // Returns: bool (true if deleted).
    // Depends on: point<t>::same_point().
    // Time Complexity: O(n) + (rebuild ? O(n log n) : 0).
    bool delete_point(const point<t>& p, long double eps = cp_geometry::EPS, bool rebuild = true, bool keep_collinear = false)
    {
        for (int i = 0; i < (int)points.size(); i++)
        {
            if (point<t>::same_point(points[i], p, eps))
            {
                points.erase(points.begin() + i);
                if (rebuild) build(keep_collinear);
                return true;
            }
        }
        return false;
    }
};
