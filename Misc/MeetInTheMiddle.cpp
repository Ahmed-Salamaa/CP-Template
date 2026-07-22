/*
 * Topic: Meet in the Middle
 * Description: Meet-in-the-Middle is a search optimization algorithm that splits the search
 *   space in half, solves each independently, and combines the results. Used to
 *   reduce the time complexity from O(2^N) to O(2^(N/2)) in subset and permutation
 *   problems.
 * Usage:
 * auto left = get_subset_sums(0, n/2);
 */
#include "../core.h"

int main() {
    int n, x;
    cin >> n >> x;
    vector<int> a(n);
    for (int i = 0; i < n; i++) { cin >> a[i]; }
    // stores all possible subset sums in the interval [l, r]
    auto get_subset_sums = [&](int l, int r) -> vector<ll> {
        int len = r - l + 1;
        vector<ll> res;
        // loop through all subsets
        for (int i = 0; i < (1 << len); i++) {
            ll sum = 0;
            for (int j = 0; j < len; j++) {
                if (i & (1 << j)) { sum += a[l + j]; }
            }
            res.push_back(sum);
        }
        return res;
    };
    vector<ll> left = get_subset_sums(0, n / 2 - 1);
    vector<ll> right = get_subset_sums(n / 2, n - 1);
    sort(all(left));
    sort(all(right));
    ll ans = 0;
    for (ll i : left) {
        auto low_iterator = lower_bound(all(right), x - i);
        auto high_iterator = upper_bound(all(right), x - i);
        ans += high_iterator - low_iterator;
    }
    cout << ans << endl;
}