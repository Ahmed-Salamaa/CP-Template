/*
 * Topic: Triangle
 * Description: Provides 2D Triangle utilities, such as computing area, incenter, circumcenter,
 *   and checking if a point lies strictly inside the triangle.
 * formula, and computes the centers and radii of the incircle and circumcircle. Usage: Point A(0,0), B(3,0), C(0,4);
 *    double area = triangleArea(A, B, C);
 *    Point circumcenter = getCircumcenter(A, B, C);
 *    Circle incircle = getIncircle(A, B, C);
 */
#include "../core.h"
#pragma once
#include <cmath>

#include "Point.cpp"

// Calculates: Represents a triangle defined by three vertices.
// Returns: A triangle object.
// Time Complexity: O(1)
// Space Complexity: O(1)
template <typename T>
struct triangle {
    point<T> a, b, c;

    triangle() {}

    triangle(point<T> a, point<T> b, point<T> c) : a(a), b(b), c(c) {}
};

// Calculates: The area of a triangle.
// Returns: A double representing the area.
// Time Complexity: O(1)
// Space Complexity: O(1)
template <typename T>
double triangle_area(const triangle<T>& t) {
    return abs(cross_val(t.a, t.b, t.c)) / 2.0;
}

// Calculates: The incenter of a triangle (intersection of angle bisectors).
// Returns: A point representing the incenter.
// Time Complexity: O(1)
// Space Complexity: O(1)
template <typename T>
point<double> incenter(const triangle<T>& t) {
    double la = distance(t.b, t.c);
    double lb = distance(t.a, t.c);
    double lc = distance(t.a, t.b);
    double p = la + lb + lc;
    if (p <= EPS) return point<double>(t.a.x, t.a.y);
    return point<double>((la * t.a.x + lb * t.b.x + lc * t.c.x) / p, (la * t.a.y + lb * t.b.y + lc * t.c.y) / p);
}

// Calculates: The orthocenter of a triangle (intersection of altitudes).
// Returns: A point representing the orthocenter.
// Time Complexity: O(1)
// Space Complexity: O(1)
template <typename T>
point<double> orthocenter(const triangle<T>& t) {
    // Derived from Euler line equation: H = A + B + C - 2*O (where O is circumcenter)
    double D = 2.0 * (t.a.x * (t.b.y - t.c.y) + t.b.x * (t.c.y - t.a.y) + t.c.x * (t.a.y - t.b.y));
    if (abs(D) <= EPS) return point<double>(0, 0);  // collinear
    double a2 = t.a.x * t.a.x + t.a.y * t.a.y;
    double b2 = t.b.x * t.b.x + t.b.y * t.b.y;
    double c2 = t.c.x * t.c.x + t.c.y * t.c.y;
    double cx = (a2 * (t.b.y - t.c.y) + b2 * (t.c.y - t.a.y) + c2 * (t.a.y - t.b.y)) / D;
    double cy = (a2 * (t.c.x - t.b.x) + b2 * (t.a.x - t.c.x) + c2 * (t.b.x - t.a.x)) / D;
    return point<double>(t.a.x + t.b.x + t.c.x - 2 * cx, t.a.y + t.b.y + t.c.y - 2 * cy);
}
