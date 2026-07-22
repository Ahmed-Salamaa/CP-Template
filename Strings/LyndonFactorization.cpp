/* Topic: Lyndon Factorization
 * Description: Duval's algorithm factors a string into a sequence of lexicographically non-
 *   increasing Lyndon words in linear time.
 * Usage: string max_suffix = Lyndon::get_max_suffix(s);
 */
#include "../../core.h"

struct Lyndon {
    static inline string get_max_suffix(const string& ss) {
        int n = sz(ss), idx = 0, j = 1, k = 0;
        while (j + k < n) {
            if (ss[idx + k] == ss[j + k])
                k++;
            else if (ss[idx + k] < ss[j + k])
                idx = max(idx + k + 1, j), j = idx + 1, k = 0;
            else
                j = j + k + 1, k = 0;
        }
        return ss.substr(idx);
    }
};