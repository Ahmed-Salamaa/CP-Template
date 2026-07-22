/*
 * Topic: DSU Get Set
 * Description: A DSU structure that explicitly maintains the list of elements belonging to each
 *   connected component,
 * allowing for iterating over the elements of a set. Usage: DSUGetSet dsu(N); dsu.unite(u, v); for (int node :
 * dsu.get_set(dsu.find(u))) { ... }
 */
#include "../../../core.h"

// Time Complexity: O(alpha(N)) per merge/find, O(size of set) for get_set
// Space Complexity: O(N)
struct DSUGetSet {
    int n, cnt;
    vector<int> size, parent, nxt, tail, sets, pos;

    void init(int nn) {
        this->n = nn;
        size.assign(n + 5, 1);
        parent.resize(n + 5);
        nxt.assign(n + 5, -1);
        sets.resize(n + 5);
        pos.resize(n + 5);
        tail.resize(n + 5);
        iota(parent.begin(), parent.end(), 0);
        iota(tail.begin(), tail.end(), 0);
        iota(sets.begin(), sets.end(), 0);
        iota(pos.begin(), pos.end(), 0);
        cnt = nn + 1;
    }

    DSUGetSet(int nn = 0) { init(nn); }

    // Time Complexity: O(alpha(N))
    int find(int child) { return (child == parent[child] ? child : parent[child] = find(parent[child])); }

    // Time Complexity: O(alpha(N))
    bool merge(int u, int v) {
        u = find(u);
        v = find(v);
        if (v == u) return false;
        if (size[u] < size[v]) swap(u, v);
        parent[v] = u;
        size[u] += size[v];
        int p = pos[v];
        pos[sets[p] = sets[--cnt]] = p;
        int& t = tail[u];
        nxt[t] = v;
        t = tail[v];
        return true;
    }

    // Time Complexity: O(size of set)
    vector<int> get_set(int node) {
        node = find(node);
        vector<int> res;
        for (int i = node; i != -1; i = nxt[i]) { res.emplace_back(i); }
        return res;
    }
};
