// Regular Polygon Template
// Dependencies: Point.cpp, Line.cpp (must be included before this file)
// Usage: Include Point.cpp and Line.cpp first, then this file

#include <bits/stdc++.h>
using namespace std;

const double PI = acos(-1.0);

template <typename t>
struct regular_polygon
{
    vector<point<t>> vertices;
    int n;                    // number of sides
    point<t> center;
    double radius;            // circumradius (center to vertex)

    // ---------- Constructors ----------

    // Default
    regular_polygon() : n(0), radius(0) {}

    // Construct from center, radius (circumradius), and number of sides
    // starting_angle is the angle of the first vertex (radians by default)
    regular_polygon(const point<t>& c, double r, int num_sides, double starting_angle = 0, bool in_degrees = false)
        : center(c), radius(r), n(num_sides)
    {
        if (in_degrees) starting_angle = starting_angle * PI / 180.0;
        vertices.resize(n);
        double angle_step = 2.0 * PI / n;
        
        for (int i = 0; i < n; i++)
        {
            double angle = starting_angle + i * angle_step;
            vertices[i] = point<t>(
                (t)(c.x + r * cos(angle)),
                (t)(c.y + r * sin(angle))
            );
        }
    }

    // Construct from number of sides and side length
    // Places center at origin, first vertex on positive x-axis
    regular_polygon(int num_sides, double side_length)
        : n(num_sides)
    {
        // Calculate circumradius from side length
        // side_length = 2 * R * sin(PI / n)
        radius = side_length / (2.0 * sin(PI / n));
        center = point<t>(0, 0);
        
        vertices.resize(n);
        double angle_step = 2.0 * PI / n;
        
        for (int i = 0; i < n; i++)
        {
            double angle = i * angle_step;
            vertices[i] = point<t>(
                (t)(radius * cos(angle)),
                (t)(radius * sin(angle))
            );
        }
    }

    // Construct from a set of vertices (validates if regular)
    regular_polygon(const vector<point<t>>& verts, bool validate = true)
        : vertices(verts), n(verts.size())
    {
        if (n < 3)
        {
            cerr << "Error: A polygon must have at least 3 vertices\n";
            n = 0;
            return;
        }

        compute_center_and_radius();

        if (validate && !is_regular())
        {
            cerr << "Warning: Given vertices do not form a regular polygon\n";
        }
    }

    // Construct from center, first vertex, and number of sides
    regular_polygon(const point<t>& c, const point<t>& first_vertex, int num_sides)
        : center(c), n(num_sides)
    {
        radius = point<t>::dist(c, first_vertex);
        double starting_angle = atan2((double)(first_vertex.y - c.y), (double)(first_vertex.x - c.x));
        
        vertices.resize(n);
        double angle_step = 2.0 * PI / n;
        
        for (int i = 0; i < n; i++)
        {
            double angle = starting_angle + i * angle_step;
            vertices[i] = point<t>(
                (t)(c.x + radius * cos(angle)),
                (t)(c.y + radius * sin(angle))
            );
        }
    }

    // Construct from number of sides and some known points (at least 2)
    // Reconstructs the full regular polygon by fitting from given points
    regular_polygon(int num_sides, const vector<point<t>>& known_points)
        : n(num_sides)
    {
        if (known_points.size() < 2)
        {
            cerr << "Error: Need at least 2 points to construct regular polygon\n";
            n = 0;
            return;
        }

        // Estimate center from known points
        t sum_x = 0, sum_y = 0;
        for (const auto& p : known_points)
        {
            sum_x += p.x;
            sum_y += p.y;
        }
        center = point<t>(sum_x / (t)known_points.size(), sum_y / (t)known_points.size());

        // Refine center: optimize to minimize variance of distances
        for (int iter = 0; iter < 10; iter++)
        {
            t cx = 0, cy = 0, total_weight = 0;
            for (const auto& p : known_points)
            {
                double dist = point<t>::dist(center, p);
                if (dist > 1e-9)
                {
                    double weight = 1.0 / dist;
                    cx += p.x * weight;
                    cy += p.y * weight;
                    total_weight += weight;
                }
            }
            if (total_weight > 1e-9)
                center = point<t>(cx / total_weight, cy / total_weight);
        }

        // Calculate radius as average distance from center
        double sum_dist = 0;
        for (const auto& p : known_points)
        {
            sum_dist += point<t>::dist(center, p);
        }
        radius = sum_dist / known_points.size();

        // Determine angular position of first known point
        double first_angle = atan2((double)(known_points[0].y - center.y), 
                                   (double)(known_points[0].x - center.x));

        // Generate all vertices
        vertices.resize(n);
        double angle_step = 2.0 * PI / n;
        
        for (int i = 0; i < n; i++)
        {
            double angle = first_angle + i * angle_step;
            vertices[i] = point<t>(
                (t)(center.x + radius * cos(angle)),
                (t)(center.y + radius * sin(angle))
            );
        }
    }

    // ---------- Utility & Validation ----------

    // Compute center and radius from vertices
    void compute_center_and_radius()
    {
        if (n == 0) return;
        
        // Calculate centroid as center
        t sum_x = 0, sum_y = 0;
        for (const auto& v : vertices)
        {
            sum_x += v.x;
            sum_y += v.y;
        }
        center = point<t>(sum_x / (t)n, sum_y / (t)n);
        
        // Calculate average distance to center as radius
        double sum_dist = 0;
        for (const auto& v : vertices)
        {
            sum_dist += point<t>::dist(center, v);
        }
        radius = sum_dist / n;
    }

    // Check if vertices form a regular polygon
    bool is_regular(double tolerance = 1e-6) const
    {
        if (n < 3) return false;
        
        // Check all distances from center are equal
        double first_dist = point<t>::dist(center, vertices[0]);
        for (int i = 1; i < n; i++)
        {
            double dist = point<t>::dist(center, vertices[i]);
            if (abs(dist - first_dist) > tolerance) return false;
        }
        
        // Check all side lengths are equal
        double first_side = point<t>::dist(vertices[0], vertices[1]);
        for (int i = 1; i < n; i++)
        {
            int next = (i + 1) % n;
            double side = point<t>::dist(vertices[i], vertices[next]);
            if (abs(side - first_side) > tolerance) return false;
        }
        
        return true;
    }

    // ---------- Geometric Properties ----------

    // Get perimeter
    double perimeter() const
    {
        return n * side_length();
    }

    // Get side length
    double side_length() const
    {
        if (n < 2) return 0;
        return point<t>::dist(vertices[0], vertices[1]);
    }

    // Get area
    double area() const
    {
        return 0.5 * n * radius * radius * sin(2.0 * PI / n);
    }

    // Get apothem (distance from center to middle of a side)
    double apothem() const
    {
        return radius * cos(PI / n);
    }

    // Get interior angle (in degrees by default)
    double interior_angle(bool in_degrees = true) const
    {
        double angle = (n - 2.0) * PI / n;
        if (in_degrees) angle = angle * 180.0 / PI;
        return angle;
    }

    // Get exterior angle (in degrees by default)
    double exterior_angle(bool in_degrees = true) const
    {
        double angle = 2.0 * PI / n;
        if (in_degrees) angle = angle * 180.0 / PI;
        return angle;
    }

    // Get all edges as line segments
    vector<line<t>> edges() const
    {
        vector<line<t>> result;
        for (int i = 0; i < n; i++)
        {
            int next = (i + 1) % n;
            result.push_back(line<t>(vertices[i], vertices[next]));
        }
        return result;
    }

    // Get bounding box (min and max x, y)
    pair<point<t>, point<t>> bounding_box() const
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

    // ---------- Transformations ----------

    // Rotate polygon around its center by angle (radians by default)
    regular_polygon<t> rotate(double angle, bool in_degrees = false) const
    {
        return rotate_around(center, angle, in_degrees);
    }

    // Rotate polygon around a pivot point by angle (radians by default)
    regular_polygon<t> rotate_around(const point<t>& pivot, double angle, bool in_degrees = false) const
    {
        vector<point<t>> new_verts;
        for (const auto& v : vertices)
        {
            new_verts.push_back(v.rotate_around(pivot, angle, in_degrees));
        }
        
        regular_polygon<t> result(new_verts, false);
        return result;
    }

    // Translate polygon by a vector
    regular_polygon<t> translate(const point<t>& delta) const
    {
        vector<point<t>> new_verts;
        for (const auto& v : vertices)
        {
            new_verts.push_back(v + delta);
        }
        
        regular_polygon<t> result(new_verts, false);
        return result;
    }

    // Scale polygon from its center by a factor
    regular_polygon<t> scale(double factor) const
    {
        return scale_from(center, factor);
    }

    // Scale polygon from a pivot point by a factor
    regular_polygon<t> scale_from(const point<t>& pivot, double factor) const
    {
        vector<point<t>> new_verts;
        for (const auto& v : vertices)
        {
            new_verts.push_back(v.scale_from(pivot, factor));
        }
        
        regular_polygon<t> result(new_verts, false);
        return result;
    }

    // Reflect polygon across a line
    regular_polygon<t> reflect(const line<t>& axis) const
    {
        vector<point<t>> new_verts;
        for (const auto& v : vertices)
        {
            new_verts.push_back(v.reflect(axis.a, axis.b));
        }
        
        regular_polygon<t> result(new_verts, false);
        return result;
    }

    // Reflect across x-axis
    regular_polygon<t> reflect_x() const
    {
        vector<point<t>> new_verts;
        for (const auto& v : vertices)
        {
            new_verts.push_back(v.reflect_x());
        }
        
        regular_polygon<t> result(new_verts, false);
        return result;
    }

    // Reflect across y-axis
    regular_polygon<t> reflect_y() const
    {
        vector<point<t>> new_verts;
        for (const auto& v : vertices)
        {
            new_verts.push_back(v.reflect_y());
        }
        
        regular_polygon<t> result(new_verts, false);
        return result;
    }

    // ---------- Vertex Manipulation ----------

    // Apply a custom transformation function to all vertices
    template<typename Func>
    regular_polygon<t> transform(Func func) const
    {
        vector<point<t>> new_verts;
        for (const auto& v : vertices)
        {
            new_verts.push_back(func(v));
        }
        
        regular_polygon<t> result(new_verts, false);
        return result;
    }

    // Get vertex at index (with wrapping)
    point<t> vertex(int index) const
    {
        index = ((index % n) + n) % n; // Handle negative indices
        return vertices[index];
    }

    // Get all vertices
    vector<point<t>> get_vertices() const
    {
        return vertices;
    }

    // Set vertices (recomputes center and radius)
    void set_vertices(const vector<point<t>>& new_verts)
    {
        vertices = new_verts;
        n = vertices.size();
        compute_center_and_radius();
    }

    // ---------- Point Containment ----------

    // Check if a point is inside the polygon (using winding number)
    bool contains(const point<t>& p) const
    {
        if (n < 3) return false;
        
        int winding = 0;
        for (int i = 0; i < n; i++)
        {
            int j = (i + 1) % n;
            point<t> v1 = vertices[i];
            point<t> v2 = vertices[j];
            
            if (v1.y <= p.y)
            {
                if (v2.y > p.y) // upward crossing
                {
                    if (point<t>::cross(v1, v2, p) > 0) // p left of edge
                        winding++;
                }
            }
            else
            {
                if (v2.y <= p.y) // downward crossing
                {
                    if (point<t>::cross(v1, v2, p) < 0) // p right of edge
                        winding--;
                }
            }
        }
        
        return winding != 0;
    }

    // Check if point is on the boundary
    bool on_boundary(const point<t>& p, double tolerance = 1e-9) const
    {
        for (int i = 0; i < n; i++)
        {
            int next = (i + 1) % n;
            line<t> edge(vertices[i], vertices[next]);
            if (edge.distance(p).first < tolerance)
                return true;
        }
        return false;
    }

    // ---------- Distance Functions ----------

    // Distance from point to polygon (0 if inside)
    double distance_to(const point<t>& p) const
    {
        if (contains(p)) return 0.0;
        
        double min_dist = 1e18;
        for (int i = 0; i < n; i++)
        {
            int next = (i + 1) % n;
            line<t> edge(vertices[i], vertices[next]);
            min_dist = min(min_dist, edge.distance(p).first);
        }
        return min_dist;
    }

    // Closest point on polygon to a given point
    point<t> closest_point(const point<t>& p) const
    {
        if (contains(p)) return p;
        
        double min_dist = 1e18;
        point<t> closest;
        
        for (int i = 0; i < n; i++)
        {
            int next = (i + 1) % n;
            line<t> edge(vertices[i], vertices[next]);
            auto [dist, pt] = edge.distance(p);
            
            if (dist < min_dist)
            {
                min_dist = dist;
                closest = pt;
            }
        }
        
        return closest;
    }

    // ---------- Inscribed & Circumscribed Circles ----------

    // Get circumcircle (center and radius)
    pair<point<t>, double> circumcircle() const
    {
        return {center, radius};
    }

    // Get incircle (center and radius = apothem)
    pair<point<t>, double> incircle() const
    {
        return {center, apothem()};
    }

    // ---------- I/O ----------
    friend ostream& operator << (ostream& out, const regular_polygon<t>& poly)
    {
        out << "Regular Polygon with " << poly.n << " sides:\n";
        out << "Center: " << poly.center << "\n";
        out << "Radius: " << poly.radius << "\n";
        out << "Vertices:\n";
        for (int i = 0; i < poly.n; i++)
        {
            out << "  " << i << ": " << poly.vertices[i] << "\n";
        }
        return out;
    }
};



