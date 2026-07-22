/* Topic: Suffix Array Int
 * Description: Integer Suffix Array is a variant of the Suffix Array tailored for sequences
 *   composed of integers rather than characters, often used when the alphabet size
 *   is large.
 * Usage: SuffixArrayInt sa(vector<int>);
 */
#include "../../core.h"

struct SuffixArrayInt {
    vector<int> s;
    int n;
    vector<int> p, c, lcp;

    explicit SuffixArrayInt(vector<int> _s = {}) : n(sz(_s) + 1) {
        s = _s;
        s.push_back(0);  // 0 plays the role of $
        p = c = lcp = vector<int>(n);
        build();
        build_lcp();
    }

    inline void count_sort() {
        vector<int> cnt(n, 0), pos(n, 0), p_new(n);
        for (auto& x : c) cnt[x]++;
        for (int i = 1; i < n; i++) pos[i] = pos[i - 1] + cnt[i - 1];
        for (auto& x : p) p_new[pos[c[x]]++] = x;
        p = p_new;
    }

    inline void build() {
        vector<pair<int, int>> a(n);
        for (int i = 0; i < n; i++) a[i] = {s[i], i};
        sort(all(a));
        for (int i = 0; i < n; i++) p[i] = a[i].second;
        c[p[0]] = 0;
        for (int i = 1; i < n; i++) c[p[i]] = c[p[i - 1]] + (a[i].first != a[i - 1].first);
        int k = 0;
        while (((1 << k) < n) && p.back() != n - 1) {
            for (int i = 0; i < n; i++) p[i] = (p[i] - (1 << k) + n) % n;
            count_sort();
            vector<int> c_new(n);
            c_new[p[0]] = 0;
            for (int i = 1; i < n; i++) {
                pair<int, int> prev = {c[p[i - 1]], c[(p[i - 1] + (1 << k)) % n]};
                pair<int, int> now = {c[p[i]], c[(p[i] + (1 << k)) % n]};
                c_new[p[i]] = c_new[p[i - 1]] + (prev != now);
            }
            c = c_new;
            k++;
        }
    }

    inline void build_lcp() {
        int k = 0;
        for (int i = 0; i < n - 1; i++) {
            int pi = c[i], j = p[pi - 1];
            while (s[i + k] == s[j + k]) k++;
            lcp[pi] = k;
            k = max(k - 1, 0LL);
        }
    }
};
