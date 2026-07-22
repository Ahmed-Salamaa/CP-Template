/*
 * Topic: Augmented DSU
 * Description: Augmented DSU is a Disjoint Set Union variant that stores additional data such
 *   as the size, maximum element, or parity of each set to answer more complex
 *   connectivity queries.
 * component, the sum of values, or the maximum element. Usage: AugmentedDSU dsu(N); dsu.unite(u, v); cout <<
 * dsu.get_sum(dsu.find(u)) << endl;
 */
#include "../../../core.h"

// Time Complexity: O(alpha(N)) per operation
// Space Complexity: O(N)
struct AugmentedDSU {
    int flaw;  // counting numbers of inconsistent assertions
    vector<int> diff, parent;

    AugmentedDSU(int n) {
        flaw = 0;
        diff.assign(n + 5, 0);
        parent.resize(n + 5);
        iota(parent.begin(), parent.end(), 0);
    }

    // Time Complexity: O(alpha(N))
    int find(int x) {
        if (parent[x] == x) return x;
        int rx = find(parent[x]);             // rx is the root of x
        diff[x] = diff[parent[x]] + diff[x];  // add all potentials along the path
        parent[x] = rx;
        return rx;
    }

    // Time Complexity: O(alpha(N))
    bool merge(int a, int b, int d) {
        int ra = find(a);
        int rb = find(b);
        if (ra == rb && diff[a] - diff[b] != d) {
            flaw++;
            return false;
        } else if (ra != rb) {
            diff[ra] = d + diff[b] - diff[a];
            parent[ra] = rb;
        }
        return true;
    }

    // Time Complexity: O(alpha(N))
    int query(int a, int b) {
        int ra = find(a);
        int rb = find(b);
        if (ra != rb) {
            // can't get answer, returning large negative value
            return -1e18;
        }
        return diff[a] - diff[b];
    }
};
