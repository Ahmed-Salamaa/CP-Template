/*
 * Topic: Tree - Diameter
 * Description: Tree Diameter computes the longest path between any two nodes in a tree, usually
 *   found using two consecutive Breadth-First or Depth-First Searches.
 */
#include "../../core.h"

template <typename CostType = long long>
struct Tree_Diameter {
    int n;
    vector<vector<pair<int, CostType>>> adj;
    vector<CostType> dist;
    vector<int> parent;

    Tree_Diameter(int n) : n(n) {
        adj.assign(n + 5, vector<pair<int, CostType>>());
        dist.assign(n + 5, 0);
        parent.assign(n + 5, -1);
    }

    void add_edge(int u, int v, CostType w = 1) {
        adj[u].push_back({v, w});
        adj[v].push_back({u, w});
    }

    void bfs(int start) {
        fill(dist.begin(), dist.end(), -1);
        fill(parent.begin(), parent.end(), -1);
        queue<int> q;
        q.push(start);
        dist[start] = 0;
        while (!q.empty()) {
            int u = q.front();
            q.pop();
            for (auto& edge : adj[u]) {
                int v = edge.first;
                CostType w = edge.second;
                if (dist[v] == -1) {
                    dist[v] = dist[u] + w;
                    parent[v] = u;
                    q.push(v);
                }
            }
        }
    }

    pair<CostType, pair<int, int>> get_diameter() {
        bfs(1);
        int farthest1 = 1;
        for (int i = 1; i <= n; i++) {
            if (dist[i] > dist[farthest1]) { farthest1 = i; }
        }
        bfs(farthest1);
        int farthest2 = farthest1;
        CostType diam = 0;
        for (int i = 1; i <= n; i++) {
            if (dist[i] > dist[farthest2]) {
                farthest2 = i;
                diam = dist[i];
            }
        }
        return {diam, {farthest1, farthest2}};
    }

    vector<int> get_diameter_path() {
        auto diam_info = get_diameter();
        int u = diam_info.second.second;
        vector<int> path;
        while (u != -1) {
            path.push_back(u);
            u = parent[u];
        }
        return path;
    }
};
