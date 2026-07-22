// Topic: Pell's Equation - [Solves x^2 - D y^2 = 1]
#include "../../../core.h"
// Time Complexity: O(log N)
// Space Complexity: O(1)

import sys
from math import sqrt

class PellsEquation_Algorithm:
    # continued fraction of sqrt(x)
    # there will always be a cycle of size <= 2 * sqrt(x) in the continued fraction
    def get_continued_fraction(self, ans, x):
        a0 = int(sqrt(x))
        n, m, a = 1, 0, 0
        f = True
        if (a0 * a0 != x):
            while (a != 2 * a0):
                m = n * a - m;
                n = (x - m * m) / n;
                a = int((a0 + m) / n);
                if not f:
                    ans.append(a)
                f = False

    # generating (x, y)
    def gen(self, a, x, y, i, cur):
        if (i >= cur):
            return x * a[i] + 1, a[i]
        else:
            rx, ry = self.gen(a, y, a[i + 1], i + 1, cur)
            x = x * rx + ry
            y = rx
            return x, y

    def solve(self, n):
        a = []
        if n >= 0:
            self.get_continued_fraction(a, n)
        cycle = len(a) - 1
        p, q = 1, 0
        if (cycle > 0):
            if (cycle % 2 == 0):
                p, q = self.gen(a, a[0], a[1], 1, cycle - 2)
            else:
                a += a[1:] + a[1:]
                p, q = self.gen(a, a[0], a[1], 1, 2 * cycle - 1)
        return p, q