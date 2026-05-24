#include <vector>

using namespace std;

struct CHT {
    vector<long long> m, b;
    int ptr = 0;
    bool is_max;

    // max(m*x + b) is exactly mathematically equal to -min(-m*x - b).
    CHT(bool is_max = false) : is_max(is_max) {}

    
    long double intersect(int i, int j) {
        return (long double)(b[j] - b[i]) / (m[i] - m[j]);
    }

    bool bad(int l1, int l2, int l3) {
        return intersect(l1, l3) <= intersect(l1, l2);
    }

    // Add a line y = _m * x + _b
    // Slopes must be added in monotonic order (either always increasing or decreasing)
    void add(long long _m, long long _b) {
       
        if (is_max) {
            _m = -_m;
            _b = -_b;
        }

        // Handle parallel lines to avoid division by zero in intersect()
        if (!m.empty() && m.back() == _m) {
            if (b.back() <= _b) return; // Existing line is better
            m.pop_back();               // New line is better, pop the old one
            b.pop_back();
        }

        m.push_back(_m);
        b.push_back(_b);
        int s = m.size();

        // Maintain the convex hull
        while (s >= 3 && bad(s - 3, s - 2, s - 1)) {
            m.erase(m.end() - 2);
            b.erase(b.end() - 2);
            s--;
        }
    }

    // Evaluate line i at x
    long long f(int i, long long x) {
        return m[i] * x + b[i];
    }

    // O(1) amortized query 
    // Query x values must be monotonic (sorted)
    long long query_linear(long long x) {
        if (m.empty()) return 0;
        if (ptr >= m.size()) ptr = m.size() - 1;

        // Advance pointer while the next line gives a better (lower) result
        while (ptr < m.size() - 1 && f(ptr + 1, x) <= f(ptr, x)) {
            ptr++;
        }
        
        long long ans = f(ptr, x);
        return is_max ? -ans : ans; // Reverse the inversion trick if MAX
    }

    // O(log N) query 
    // Query x values are NOT sorted
    long long query_binary(long long x) {
        if (m.empty()) return 0;
        int l = 0, r = m.size() - 1;
        
        while (l < r) {
            int mid = l + (r - l) / 2;
            // If the next line is better or equal, the true minimum is to the right
            if (f(mid + 1, x) <= f(mid, x)) {
                l = mid + 1;
            } else {
                r = mid;
            }
        }
        
        long long ans = f(l, x);
        return is_max ? -ans : ans; // Reverse the inversion trick if MAX
    }
};


