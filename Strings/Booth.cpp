/* Topic: Booth's Algorithm
 * Description: Booth's Algorithm is an algorithm that computes the lexicographically minimal
 *   string rotation of a given string in linear time.
 * Usage: string rot = Booth::least_rotation(s);
 */
#include "../../core.h"

struct Booth {
    static inline string least_rotation(string s) {
        s += s;
        vector<int> f(sz(s), -1);
        int k = 0;
        for (int j = 1; j < sz(s); j++) {
            char sj = s[j];
            int i = f[j - k - 1];
            while (i != -1 && sj != s[k + i + 1]) {
                if (sj < s[k + i + 1]) k = j - i - 1;
                i = f[i];
            }
            if (sj != s[k + i + 1]) {
                if (sj < s[k]) k = j;
                f[j - k] = -1;
            } else
                f[j - k] = i + 1;
        }
        return s.substr(k, sz(s) / 2);
    }
};
