/*
 * Topic: DSU Distance To Parent
 * Description: A modified DSU that maintains the distance or weight from each element to its
 *   representative root.
 * Useful for difference constraints or weighted queries on trees. Usage: DSUDistantToParent dsu(N); dsu.unite(u, v,
 * weight); cout << dsu.dist(u) << endl;
 */
#include "../../../core.h"

// Time Complexity: O(alpha(N)) per operation
// Space Complexity: O(N)
struct DSUDistantToParent {
    vector<pair<int, int>> parent;  // parent and distant to the leader
    vector<int> rank;

    DSUDistantToParent(int n) {
        parent.resize(n + 5);
        rank.resize(n + 5);
        for (int i = 1; i <= n; i++) make_set(i);
    }

    // Time Complexity: O(1)
    void make_set(int v) {
        parent[v] = make_pair(v, 0);
        rank[v] = 0;
    }

    // Time Complexity: O(alpha(N))
    pair<int, int> find(int v) {
        if (v != parent[v].first) {
            int len = parent[v].second;
            parent[v] = find(parent[v].first);
            parent[v].second += len;
        }
        return parent[v];
    }

    // Time Complexity: O(alpha(N))
    bool merge(int a, int b) {
        a = find(a).first;
        b = find(b).first;
        if (a != b) {
            if (rank[a] < rank[b]) swap(a, b);
            parent[b] = make_pair(a, 1);
            if (rank[a] == rank[b]) rank[a]++;
            return true;
        }
        return false;
    }
};
