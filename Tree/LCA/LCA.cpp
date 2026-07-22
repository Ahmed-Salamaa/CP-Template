/*
 * Topic: Tree - LCA
 * Description: Lowest Common Ancestor (LCA) finds the lowest node in a tree that has both nodes
 *   u and v as descendants, typically implemented using binary lifting.
 */
#include "../../core.h"

template <typename CostType = long long>
struct LCA {
    int n, root, LOG;
    vector<vector<pair<int, CostType>>> adj;
    vector<vector<int>> up;
    vector<vector<CostType>> cost;
    vector<int> depth;
    CostType def;

    LCA(int n, int root = 1, CostType def = 0) : n(n), root(root), def(def) {
        LOG = __lg(n + 5) + 2;
        adj.assign(n + 5, vector<pair<int, CostType>>());
        up.assign(n + 5, vector<int>(LOG));
        cost.assign(n + 5, vector<CostType>(LOG, def));
        depth.assign(n + 5, 0);
    }

    void add_edge(int u, int v, CostType c = 1) {
        adj[u].push_back({v, c});
        adj[v].push_back({u, c});
    }

    CostType operation(CostType a, CostType b) { return max(a, b); }

    void dfs(int u, int p, CostType c) {
        depth[u] = depth[p] + 1;
        up[u][0] = p;
        cost[u][0] = c;
        for (int i = 1; i < LOG; i++) {
            up[u][i] = up[up[u][i - 1]][i - 1];
            cost[u][i] = operation(cost[u][i - 1], cost[up[u][i - 1]][i - 1]);
        }
        for (auto& edge : adj[u]) {
            int v = edge.first;
            CostType w = edge.second;
            if (v != p) { dfs(v, u, w); }
        }
    }

    void build() {
        depth[root] = 0;
        dfs(root, root, def);
    }

    int kth_ancestor(int u, int k) {
        if (depth[u] - depth[root] < k) return -1;
        for (int i = LOG - 1; i >= 0; i--) {
            if ((k >> i) & 1) { u = up[u][i]; }
        }
        return u;
    }

    int get_lca(int u, int v) {
        if (depth[u] < depth[v]) swap(u, v);
        u = kth_ancestor(u, depth[u] - depth[v]);
        if (u == v) return u;
        for (int i = LOG - 1; i >= 0; i--) {
            if (up[u][i] != up[v][i]) {
                u = up[u][i];
                v = up[v][i];
            }
        }
        return up[u][0];
    }

    CostType get_cost(int u, int dist) {
        if (depth[u] - depth[root] < dist) return def;
        CostType ans = def;
        for (int i = 0; i < LOG; i++) {
            if ((dist >> i) & 1) {
                ans = operation(ans, cost[u][i]);
                u = up[u][i];
            }
        }
        return ans;
    }

    CostType query_path(int u, int v) {
        int lca_node = get_lca(u, v);
        return operation(get_cost(u, depth[u] - depth[lca_node]), get_cost(v, depth[v] - depth[lca_node]));
    }

    int distance(int u, int v) { return depth[u] + depth[v] - 2 * depth[get_lca(u, v)]; }

    int kth_node_on_path(int u, int v, int k) {
        int lca_node = get_lca(u, v);
        int d_u_lca = depth[u] - depth[lca_node];
        k--;
        if (d_u_lca >= k) { return kth_ancestor(u, k); }
        k -= d_u_lca;
        int d_v_lca = depth[v] - depth[lca_node];
        if (d_v_lca < k) return -1;
        return kth_ancestor(v, d_v_lca - k);
    }

    int common_nodes_in_path(int a, int b, int c, int d) {
        int dist_ab = distance(a, b);
        int dist_cd = distance(c, d);
        int dist_ac = distance(a, c);
        int dist_ad = distance(a, d);
        int dist_bc = distance(b, c);
        int dist_bd = distance(b, d);
        int overlap = 2 * (dist_ab + dist_cd) - dist_ac - dist_ad - dist_bc - dist_bd;
        return max(0, overlap / 2 + 1);
    }
};
