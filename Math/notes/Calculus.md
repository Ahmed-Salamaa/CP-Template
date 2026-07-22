# From Calculus_Snippets.md
# Calculus Snippets

## Numerical Differentiation

Numerical differentiation is the process of finding the numerical value of a derivative of a given function at a specific point. The fundamental idea is based on the limit definition of a derivative:
$$ f'(x) = \lim_{h \to 0} \frac{f(x+h) - f(x)}{h} $$

However, in computational contexts, we cannot make $h$ infinitely small due to floating-point precision limitations. A very small $h$ can lead to subtractive cancellation (round-off error), while a large $h$ leads to truncation error. Usually, $h \approx 10^{-6}$ provides a good balance for double-precision floating-point formats when calculating the first derivative.

#

## Central Difference Approximation

To approximate the first derivative more accurately, we use the central difference formula, which averages the forward and backward differences:
$$ f'(x) \approx \frac{f(x+h) - f(x-h)}{2h} $$
This method has a truncation error of $O(h^2)$, making it superior to the forward difference $O(h)$.

#

## Second Derivative

The second derivative is the rate of change of the first derivative. It can be approximated using the central difference of the first derivative:
$$ f''(x) \approx \frac{f(x+h) - 2f(x) + f(x-h)}{h^2} $$
This also yields a truncation error of $O(h^2)$.

**Time Complexity:** $\mathcal{O}(1)$ assuming the function evaluation is $\mathcal{O}(1)$.
**Space Complexity:** $\mathcal{O}(1)$.
**When to Use:** Use when you need the slope or convexity of a function at a point, such as in gradient descent optimization, root-finding algorithms (like Newton-Raphson), or physical simulations.

#

## C++ Code

```cpp
const double h = 1e-6;

template<typename F>
double first_derivative(F f, double x) {
    return (f(x + h) - f(x - h)) / (2.0 * h);
}

template<typename F>
double second_derivative(F f, double x) {
    return (f(x + h) - 2.0 * f(x) + f(x - h)) / (h * h);
}
```

## Simpson's Rule (Numerical Integration)

Numerical integration is used to calculate the approximate value of a definite integral $\int_a^b f(x) dx$. While the trapezoidal rule approximates the area under the curve using trapezoids (linear interpolation), Simpson's rule provides a more accurate approximation by using parabolas (quadratic interpolation).

#

## Mathematics

Simpson's rule approximates the function $f(x)$ over small subintervals with a quadratic polynomial. For the interval $[a, b]$ divided into an even number of $n$ equal subintervals of width $h = \frac{b-a}{n}$, the integral is approximated as:
$$ \int_a^b f(x) dx \approx \frac{h}{3} \left[ f(x_0) + 4f(x_1) + 2f(x_2) + 4f(x_3) + 2f(x_4) + \dots + 4f(x_{n-1}) + f(x_n) \right] $$
where $x_i = a + i \cdot h$. 

This rule is highly accurate, specifically yielding a zero error for polynomials of degree 3 or less. Its composite error term is proportional to $O(h^4)$, making it converge much faster than the trapezoidal rule ($O(h^2)$) as $n$ increases.

**Time Complexity:** $\mathcal{O}(N)$ where $N$ is the number of intervals.
**Space Complexity:** $\mathcal{O}(1)$.
**When to Use:** Use when an exact analytical integral is impossible or computationally expensive to find, such as finding continuous probability distributions, calculating areas/volumes, or solving differential equations. Ensure $n$ is sufficiently large ($10^5$ or $10^6$) for functions with high curvature, but a smaller $n$ is often enough for smooth functions.

#

## C++ Code

```cpp
template<typename F>
double simpson(F f, double a, double b, int n = 1000000) {
    double h = (b - a) / n;
    double s = f(a) + f(b);
    for (int i = 1; i < n; ++i) {
        double x = a + h * i;
        s += f(x) * ((i & 1) ? 4.0 : 2.0);
    }
    return s * (h / 3.0);
}
```

## PolynomialDifferentiation

**Topic:** Polynomial Differentiation - Differentiates a string representation of a polynomial

**Time Complexity:** O(length of term)
**Space Complexity:** O(length of term)

```cpp
//
// Description:
// Parses a polynomial string in the form of "axb-cxd+kxl" and returns the derivative as a string.
// Supports negative coefficients and zero elements.
//
// How to use it:
// string poly = "3x2+2x1+5";
// string derivative = PolynomialDifferentiation::PolynomialDifferentiation(poly);
// cout << derivative << endl; // outputs "6x+2"
#include "../../../core.h"

namespace PolynomialDifferentiation {
            void handle(string &temp, string &ans) {
        string cof;
        int ind = -1;
        for (int i = 0; i < sz(temp); i++) {
            if (temp[i] == 'x') {
                ind = i;
                break;
            }
            cof += temp[i];
        }
        if (ind == -1) return;
        string deg;
        for (int i = ind + 1; i < sz(temp); i++) {
            deg += temp[i];
        }
        if (deg.empty() && cof.empty()) {
            ans += '1';
            return;
        }
        if (deg.empty()) {
            ans += cof;
            return;
        }
        if (cof.empty()) {
            ans += deg;
            if (deg == "1") return;
            ans += 'x';
            if (deg != "2") ans += to_string(stoll(deg) - 1);
            return;
        }
        ans += to_string(stoll(cof) * stoll(deg));
        if (deg == "1") return;
        ans += 'x';
        if (deg != "2") ans += to_string(stoll(deg) - 1);
    }

    // Time Complexity: O(length of s)
    // Space Complexity: O(length of s)
    string PolynomialDifferentiation(string &s) {
        // format: axb-cxd+kxl where powers are sorted decreasing.
        string temp;
        if (count(all(s), 'x') == 0) {
            return "0";
        }
        string ans;
        for (auto &val : s) {
            if (val == '-' || val == '+') {
                handle(temp, ans);
                ans += val;
                temp.clear();
                continue;
            }
            temp += val;
        }
        handle(temp, ans);
        while (!ans.empty() && (ans.back() == '+' || ans.back() == '-')) 
            ans.pop_back();
        return ans;
    }
}
```

