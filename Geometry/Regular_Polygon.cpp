// Regular Polygon Template (generator-only)
// Dependencies: Point.cpp (include it before this file)

#pragma once

#include <bits/stdc++.h>

using namespace std;

template <typename t>
struct regular_polygon
{
    vector<point<t>> vertices;
    int n;              // number of sides
    point<t> center;
    double radius;      // circumradius (center to vertex)

    // Does: Create an empty regular polygon generator.
    // Time Complexity: O(1).
    regular_polygon() : n(0), center(), radius(0.0) {}

    // Does: Build vertices from center, circumradius, and side count.
    // Takes: c as point<t>, r as double, num_sides as int, starting_angle as double, in_degrees as bool.
    // Depends on: (none).
    // Time Complexity: O(n).
    regular_polygon(const point<t>& c, double r, int num_sides, double starting_angle = 0, bool in_degrees = false)
        : n(num_sides), center(c), radius(r)
    {
        vertices.clear();

        if (n < 3) { n = 0; return; }

        if (radius < 0) radius = -radius;

        if (in_degrees) starting_angle = starting_angle * (double)cp_geometry::PI / 180.0;

        vertices.resize(n);
        double angle_step = 2.0 * (double)cp_geometry::PI / n;

        for (int i = 0; i < n; i++)
        {
            double ang = starting_angle + i * angle_step;
            vertices[i] = point<t>(
                (t)(center.x + radius * cos(ang)),
                (t)(center.y + radius * sin(ang))
            );
        }
    }

    // Does: Build vertices from side count and side length (center at origin, first vertex on +x).
    // Takes: num_sides as int, side_length as double.
    // Depends on: (none).
    // Time Complexity: O(n).
    regular_polygon(int num_sides, double side_length)
        : n(num_sides), center(point<t>(0, 0)), radius(0.0)
    {
        vertices.clear();

        if (n < 3) { n = 0; return; }

        if (side_length < 0) side_length = -side_length;

        double s = sin((double)cp_geometry::PI / n);
        if (fabs(s) <= (double)cp_geometry::EPS) { n = 0; return; }

        radius = side_length / (2.0 * s);

        vertices.resize(n);
        double angle_step = 2.0 * (double)cp_geometry::PI / n;

        for (int i = 0; i < n; i++)
        {
            double ang = i * angle_step;
            vertices[i] = point<t>(
                (t)(radius * cos(ang)),
                (t)(radius * sin(ang))
            );
        }
    }

    // Does: Build vertices from center, a known first vertex, and side count.
    // Takes: c as point<t>, first_vertex as point<t>, num_sides as int.
    // Depends on: point<t>::dist().
    // Time Complexity: O(n).
    regular_polygon(const point<t>& c, const point<t>& first_vertex, int num_sides)
        : n(num_sides), center(c), radius(0.0)
    {
        vertices.clear();

        if (n < 3) { n = 0; return; }

        radius = point<t>::dist(center, first_vertex);

        double starting_angle = atan2((double)(first_vertex.y - center.y), (double)(first_vertex.x - center.x));

        vertices.resize(n);
        double angle_step = 2.0 * (double)cp_geometry::PI / n;

        for (int i = 0; i < n; i++)
        {
            double ang = starting_angle + i * angle_step;
            vertices[i] = point<t>(
                (t)(center.x + radius * cos(ang)),
                (t)(center.y + radius * sin(ang))
            );
        }
    }

    // Does: Return number of vertices (sides).
    // Time Complexity: O(1).
    int size() const
    {
        return n;
    }

    // Does: Check if polygon is empty (invalid side count).
    // Time Complexity: O(1).
    bool empty() const
    {
        return n == 0;
    }

    // Does: Get vertices (CCW order as generated).
    // Time Complexity: O(1).
    const vector<point<t>>& get_vertices() const
    {
        return vertices;
    }

    // Does: Get vertex at circular index (supports negative index).
    // Depends on: (none).
    // Time Complexity: O(1).
    point<t> vertex(int index) const
    {
        if (n == 0) return point<t>(0, 0);
        index = ((index % n) + n) % n;
        return vertices[index];
    }
};
