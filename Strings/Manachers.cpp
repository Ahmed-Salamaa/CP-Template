/* Topic: Manacher
 * Description: Manacher's Algorithm is a linear-time algorithm to find the longest palindromic
 *   substring centered at each character in a string. Crucial for advanced string
 *   and palindrome-related problems.
 * Usage: Manacher m(s); int len = m.check_palindrome(l, r);
 */
#include "../../core.h"

struct Manacher {
    vector<int> p;
    string s;

    explicit Manacher(const string& str) {
        s = "#";
        for (char c : str) s += c, s += "#";
        p.assign(sz(s), 1);
        int n = sz(s), l = 0, r = 0;
        for (int i = 1; i < n; i++) {
            p[i] = (i <= r) ? min(p[l + r - i], r - i + 1) : 1;
            while (i - p[i] >= 0 && i + p[i] < n && s[i - p[i]] == s[i + p[i]]) p[i]++;
            if (i + p[i] - 1 > r) l = i - p[i] + 1, r = i + p[i] - 1;
        }
    }

    inline int get_longest(int cen, bool odd) { return p[2 * cen + 1 + (!odd)] - 1; }

    inline bool check_palindrome(int l, int r) {
        int len = r - l + 1, cen = (l + r) / 2;
        return get_longest(cen, len & 1) >= len;
    }

    inline string get_longest_palindrome() {
        int idx = max_element(all(p)) - p.begin();
        string res = "";
        for (char c : s.substr(idx - p[idx] + 1, 2 * p[idx] - 1))
            if (c != '#') res += c;
        return res;
    }
};