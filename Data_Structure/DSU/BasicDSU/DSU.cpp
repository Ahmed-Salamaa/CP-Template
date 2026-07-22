/*
 * Topic: Basic DSU
 * Description: Disjoint Set Union (DSU) is a data structure that tracks a set of elements
 *   partitioned into disjoint subsets, supporting near-constant time union and find
 *   operations.
 * nearly O(1) amortized operations. Usage: DSU dsu(N); dsu.unite(u, v); if (dsu.same_set(u, v)) cout << "Connected\n";
 */
#include "../../../core.h"

/*
 * Disjoint Set Union (DSU) / Union-Find
 * ------------------------------------
 * A data structure that keeps track of a set of elements partitioned into a number of disjoint (non-overlapping)
 * subsets. It provides near-constant-time operations to add new sets, to merge existing sets, and to determine whether
 * elements are in the same set. It also maintains the size of each component and the total number of connected
 * components.
 */
struct DSU {
    vector<int> parent, group_size;
    int components_count;

    // O ( N )
    DSU(int n) : parent(n + 5), group_size(n + 5, 1), components_count(n) { iota(parent.begin(), parent.end(), 0); }

    // O ( 1 )
    int find_leader(int node) { return parent[node] == node ? node : parent[node] = find_leader(parent[node]); }

    // O ( 1 )
    bool is_same_set(int u, int v) { return find_leader(u) == find_leader(v); }

    // O ( 1 )
    bool merge_sets(int u, int v) {
        if ((u = find_leader(u)) == (v = find_leader(v))) return false;
        if (group_size[u] < group_size[v]) swap(u, v);
        parent[v] = u;
        group_size[u] += group_size[v];
        components_count--;
        return true;
    }

    // O ( N )
    vector<vector<int>> get_components() {
        vector<vector<int>> result(parent.size()), actual_components;
        for (int i = 1; i < parent.size(); i++) result[find_leader(i)].push_back(i);
        for (auto& comp : result)
            if (!comp.empty()) actual_components.push_back(comp);
        return actual_components;
    }
};
