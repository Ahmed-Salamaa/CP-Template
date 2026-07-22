/*
 * Topic: Rollback DSU
 * Description: Rollback DSU maintains a history of its operations to support undoing the most
 *   recent unions, essential for dynamic connectivity and divide-and-conquer
 *   problems.
 * on dynamic graphs (e.g., dynamic connectivity). Usage: RollbackDSU dsu(N); int state = dsu.get_state(); dsu.unite(u,
 * v); dsu.rollback(state);
 */
#include "../../../core.h"

// Time Complexity: O(log N) per operation
// Space Complexity: O(N)
struct RollbackDSU {
    vector<int> parent, size;
    vector<pair<int, int>> history;
    int cnt;

    RollbackDSU(int n) : cnt(n) {
        parent.resize(n + 5);
        size.assign(n + 5, 1);
        iota(parent.begin(), parent.end(), 0);
    }

    // Time Complexity: O(log N)
    int find(int a) { return (parent[a] == a ? a : find(parent[a])); }

    // Time Complexity: O(log N)
    bool merge(int a, int b) {
        a = find(a), b = find(b);
        if (a == b) {
            history.emplace_back(-1, -1);
            return false;
        }
        if (size[a] > size[b]) swap(a, b);
        history.emplace_back(a, b);
        parent[a] = b;
        size[b] += size[a];
        cnt--;
        return true;
    }

    // Time Complexity: O(steps)
    void rollback(int steps) {
        while (steps--) {
            auto [a, b] = history.back();
            history.pop_back();
            if (a == -1) continue;
            parent[a] = a;
            size[b] -= size[a];
            ++cnt;
        }
    }
};
