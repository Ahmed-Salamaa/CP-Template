/*
 * Topic: DSU Online Bipartite
 * Description: Extends the standard DSU to keep track of parity (colors) to check if a
 *   component is bipartite as edges
 * are added dynamically online. Usage: DSUOnlineBipartite dsu(N); dsu.add_edge(u, v); if
 * (!dsu.is_bipartite(dsu.find(u))) cout << "Not Bipartite\n";
 */
#include "../../../core.h"

// Time Complexity: O(alpha(N)) per operation
// Space Complexity: O(N)
struct DSUOnlineBipartite {
    vector<int> rank, bipartite;
    vector<pair<int, int>> parent;

    DSUOnlineBipartite(int _n) {
        rank.resize(_n + 5);
        bipartite.resize(_n + 5);
        parent.resize(_n + 5);
        for (int i = 1; i <= _n; i++) make_set(i);
    }

    // Time Complexity: O(1)
    void make_set(int v) {
        parent[v] = make_pair(v, 0);
        rank[v] = 0;
        bipartite[v] = true;
    }

    // Time Complexity: O(alpha(N))
    pair<int, int> find(int v) {
        if (v != parent[v].first) {
            int parity = parent[v].second;
            parent[v] = find(parent[v].first);
            parent[v].second ^= parity;
        }
        return parent[v];
    }

    // Time Complexity: O(alpha(N))
    void add_edge(int a, int b) {
        pair<int, int> pa = find(a);
        a = pa.first;
        int x = pa.second;
        pair<int, int> pb = find(b);
        b = pb.first;
        int y = pb.second;
        if (a == b) {
            if (x == y) bipartite[a] = false;
        } else {
            if (rank[a] < rank[b]) swap(a, b);
            parent[b] = make_pair(a, x ^ y ^ 1);
            bipartite[a] &= bipartite[b];
            if (rank[a] == rank[b]) ++rank[a];
        }
    }

    // Time Complexity: O(alpha(N))
    bool is_bipartite(int v) { return bipartite[find(v).first]; }
};
