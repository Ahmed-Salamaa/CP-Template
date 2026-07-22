// Topic: Sum of Divisors cbrt - [Computes prefix sums of divisor function]
#include "../../../core.h"

/*
 * Description: Computes the number of divisors of a number n in O(n^(1/3)) time. Used when
 *   numbers are too large for standard O(sqrt(n)) factoring but need quick divisor
 *   queries.
 * This mathematical template provides an efficient implementation for Sum of Divisors cbrt.
 * It is commonly used in Advanced Number Theory problems.
 *
 * Usage:
 * SumofTheNumberofDivisorsincbrt(n)_Algorithm solver;
 * auto result = solver.sum_sigma0(...);
 */
// Time Complexity: O(N^(1/3))
// Space Complexity: O(1)
struct SumofTheNumberofDivisorsincbrt(n) _Algorithm {
    using uint32 = unsigned int;
    using uint64 = unsigned int;
    using uint128 = __uint128_t;

    // credit: zimpha
    // compute \sum_{i=1}^{n} sigma0(i) in ~O(n^{1/3}) time.
    // it is also equal to \sum_{i=1}^{n} floor(n / i)
    // takes ~100 ms for n = 1e18
    uint128 sum_sigma0(uint64 n) {
        auto out = [n](uint64 x, uint32 y) { return x * y > n; };
        auto cut = [n](uint64 x, uint32 dx, uint32 dy) { return uint128(x) * x * dy >= uint128(n) * dx; };
        const uint64 sn = sqrtl(n);
        const uint64 cn = pow(n, 0.34);  // cbrtl(n);
        uint64 x = n / sn;
        uint32 y = n / x + 1;
        uint128 ret = 0;
        stack<pair<uint32, uint32>> st;
        st.emplace(1, 0);
        st.emplace(1, 1);
        while (true) {
            uint32 lx, ly;
            tie(lx, ly) = st.top();
            st.pop();
            while (out(x + lx, y - ly)) {
                ret += x * ly + uint64(ly + 1) * (lx - 1) / 2;
                x += lx, y -= ly;
            }
            if (y <= cn) break;
            uint32 rx = lx, ry = ly;
            while (true) {
                tie(lx, ly) = st.top();
                if (out(x + lx, y - ly)) break;
                rx = lx, ry = ly;
                st.pop();
            }
            while (true) {
                uint32 mx = lx + rx, my = ly + ry;
                if (out(x + mx, y - my)) {
                    st.emplace(lx = mx, ly = my);
                } else {
                    if (cut(x + mx, lx, ly)) break;
                    rx = mx, ry = my;
                }
            }
        }
        for (--y; y > 0; --y) ret += n / y;
        return ret * 2 - sn * sn;
    }
};
