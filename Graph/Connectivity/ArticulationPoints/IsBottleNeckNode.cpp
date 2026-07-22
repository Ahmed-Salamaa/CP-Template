/*
 * Topic: Bottleneck Node
 * Description: A specialized connectivity check that determines if a specific node acts as a
 *   bottleneck, heavily restricting the flow or connectivity between parts of the
 *   graph. It often builds on articulation point logic to ensure robust network
 *   design.
 * Usage:
 *    BottleNeck solver(N);
 *    solver.add_edge(u, v);
 *    solver.build();
 *    bool bn = solver.is_bottle_neck(a, b, c);
 */
#include "../../../core.h"

struct BottleNeck {
    // Time Complexity: O(V + E) for building, O(log V) for querying
    // Space Complexity: O(V + E)
    int n, ndfn, ID;
    vector<vector<int>> adj;
    vector<int> dfn, LowLink, comp, p, tin, tout, ArtPoints;
    vector<bool> IsArtPoints;
    vector<vector<pair<int, int>>> artIntervals;

    BottleNeck(int n)
        : n(n),
          ndfn(0),
          ID(1),
          adj(n),
          dfn(n, -1),
          LowLink(n, 0),
          comp(n, 0),
          p(n, -1),
          tin(n, 0),
          tout(n, 0),
          IsArtPoints(n, false),
          artIntervals(n) {}

    void add_edge(int u, int v) {
        adj[u].push_back(v);
        adj[v].push_back(u);
    }

    void tarjan(int node, int parent) {
        p[node] = parent;
        dfn[node] = LowLink[node] = ndfn++;
        tin[node] = dfn[node];
        tout[node] = dfn[node];
        comp[node] = ID;
        int child = 0;
        for (auto& val : adj[node]) {
            if (dfn[val] == -1) {
                child++;
                tarjan(val, node);
                LowLink[node] = min(LowLink[node], LowLink[val]);
                tout[node] = max(tout[node], tout[val]);
                if (LowLink[val] >= dfn[node]) {
                    if (parent == -1 && child <= 1) continue;
                    IsArtPoints[node] = true;
                }
            } else if (parent != val) {
                LowLink[node] = min(LowLink[node], dfn[val]);
            }
        }
    }

    void build() {
        for (int i = 0; i < n; i++) {
            if (dfn[i] == -1) {
                tarjan(i, -1);
                ID++;
            }
        }
        for (int i = 0; i < n; i++) {
            if (IsArtPoints[i]) ArtPoints.push_back(i);
        }
        for (int c : ArtPoints) {
            vector<pair<int, int>> intervals;
            for (int child : adj[c]) {
                if (p[child] != c) continue;
                if (LowLink[child] >= dfn[c]) { intervals.emplace_back(tin[child], tout[child]); }
            }
            sort(all(intervals));
            artIntervals[c] = move(intervals);
        }
    }

    bool is_bottle_neck(int a, int b, int c) {
        if (a == c || b == c || comp[a] != comp[b]) { return true; }
        if (!IsArtPoints[c]) { return false; }
        auto& v = artIntervals[c];
        auto find_idx = [&](int x) -> int {
            if (v.empty()) return -1;
            int t = tin[x];
            auto it = upper_bound(all(v), make_pair(t, 1000000000));
            if (it == v.begin()) return -1;
            --it;
            if (it->first <= t && t <= it->second) return int(it - v.begin());
            return -1;
        };
        int ida = find_idx(a), idb = find_idx(b);
        if ((ida == -1 && idb == -1) || (ida != -1 && idb != -1 && ida == idb)) return false;
        return true;
    }
};
