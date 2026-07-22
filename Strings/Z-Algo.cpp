/* Topic: Z-Algorithm
 * Description: Z-Algorithm computes an array where the i-th element is the length of the
 *   longest substring starting from i that matches a prefix of the string. Often
 *   used for fast pattern matching and substring search.
 * Usage: ZAlgorithm z(s); vector<int> matches = z.match(pattern);
 */
#include "../../core.h"

struct ZAlgorithm {
    vector<int> z;
    string s;

    explicit ZAlgorithm(const string& s) : s(s) { build(s); }

    inline void build(const string& str) {
        int n = sz(str);
        z.assign(n, 0);
        for (int i = 1, l = 0, r = 0; i < n; i++) {
            if (i <= r) z[i] = min(r - i + 1, z[i - l]);
            while (i + z[i] < n && str[z[i]] == str[i + z[i]]) z[i]++;
            if (i + z[i] - 1 > r) l = i, r = i + z[i] - 1;
        }
    }

    inline int operator[](int i) { return z[i]; }

    inline vector<int> match(const string& pattern) {
        string t = pattern + "#" + s;
        ZAlgorithm z_t(t);
        vector<int> ans;
        for (int i = sz(pattern) + 1; i < sz(t); i++)
            if (z_t[i] >= sz(pattern)) ans.push_back(i - sz(pattern) - 1);
        return ans;
    }
};
