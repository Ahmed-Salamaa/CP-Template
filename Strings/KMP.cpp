/* Topic: KMP
 * Description: The Knuth-Morris-Pratt (KMP) algorithm finds occurrences of a pattern within a
 *   text in linear time using a prefix function to bypass re-evaluating matched
 *   characters.
 * Usage: KMP kmp(pattern); vector<int> matches = kmp.match(text);
 */
#include "../../core.h"

struct KMP {
    string pattern;
    vector<int> lp;
    vector<vector<int>> failureTable;

    KMP(const string& str = "") : pattern(str) { build(); }

    inline void build() {
        int n = sz(pattern);
        lp.assign(n, 0);
        for (int i = 1; i < n; i++) lp[i] = failure(lp[i - 1], pattern[i]);
    }

    inline void build_non_overlapping() {
        int n = sz(pattern);
        lp.assign(n, 0);
        for (int i = 1; i < n; i++) {
            int j = lp[i - 1];
            while (j > 0 && pattern[j] != pattern[i]) j = lp[j - 1];
            lp[i] = j + (pattern[j] == pattern[i]);
            if (lp[i] * 2 == i + 1) lp[i] = lp[lp[i] - 1];
        }
    }

    inline int failure(int idx, char nxt) {
        // Time Complexity: O(1) amortized
        while (idx > 0 && pattern[idx] != nxt) idx = lp[idx - 1];
        return idx + (pattern[idx] == nxt);
    }

    inline void build_failure_table() {
        int n = sz(pattern);
        failureTable.assign(n + 1, vector<int>(26));
        for (int i = 0; i <= n; i++)
            for (char c = 'a'; c <= 'z'; c++)
                failureTable[i][c - 'a'] =
                    (i > 0 && c != pattern[i]) ? failureTable[lp[i - 1]][c - 'a'] : i + (c == pattern[i]);
    }

    inline int get_next_fail_idx(int fail, char c) { return failureTable[fail][c - 'a']; }

    inline vector<int> match(const string& str) {
        // Time Complexity: O(|str|)
        // Space Complexity: O(matches)
        int n = sz(str), m = sz(pattern);
        vector<int> ret;
        for (int i = 0, k = 0; i < n; i++) {
            k = failure(k, str[i]);
            if (k == m) ret.push_back(i - m + 1);
        }
        return ret;
    }

    inline vector<int> count(const string& str) {
        // Time Complexity: O(|str|)
        // Space Complexity: O(|pattern|)
        int n = sz(str), m = sz(pattern);
        vector<int> cnt(m + 1);
        for (int i = 1, k = 0; i < n; i++) cnt[k = failure(k, str[i])]++;
        for (int i = m - 1; i > 0; i--) cnt[lp[i - 1]] += cnt[i];
        for (int i = 0; i <= m; i++) cnt[i]++;
        return cnt;
    }

    inline vector<int> count_in_another(const string& t) {
        // Time Complexity: O(|pattern| + |t|)
        // Space Complexity: O(|pattern| + |t|)
        string s = pattern + "#" + t;
        KMP kmp(s);
        vector<int> cnt(sz(s));
        for (int i = sz(pattern) + 1; i < sz(s); i++) cnt[kmp.lp[i]]++;
        for (int i = sz(pattern); i > 0; i--) cnt[kmp.lp[i - 1]] += cnt[i];
        return vector<int>(cnt.begin() + 1, cnt.begin() + sz(pattern) + 1);
    }

    inline vector<int> get_prefixes() {
        vector<int> indexes = {sz(pattern)};
        for (int i = lp.back(); i > 0; i = lp[i - 1]) indexes.push_back(i);
        reverse(all(indexes));
        return indexes;
    }
};
