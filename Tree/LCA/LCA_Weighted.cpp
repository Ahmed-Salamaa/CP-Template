/*
 * Topic: Tree - LCA
 * Description: Weighted LCA finds the Lowest Common Ancestor in a tree with weighted edges,
 *   often simultaneously querying properties like the maximum edge weight or
 *   distance along the path.
 */

#include "../../core.h"


template <typename T = int>
struct LCA_Weighted {
    vector<vector<T>> cost;
    vector<vector<int>> table;
    vector<int> depth;
    int n, root, LOG, invalid;
    T def;

    T operation(T a, T b) { return max(a, b); }

    void dfs(const vector<vector<pair<int, T>>>& adj, int node, int parent, T val) {
        depth[node] = depth[parent] + 1;
        table[node][0] = parent;
        cost[node][0] = val;
        for (int bit = 1; bit < LOG; bit++) {
            table[node][bit] = table[table[node][bit - 1]][bit - 1];
            cost[node][bit] = operation(cost[node][bit - 1], cost[table[node][bit - 1]][bit - 1]);
        }
        for (const auto& edge : adj[node]) {
            int curr_node = edge.first;
            T curr_cost = edge.second;
            if (curr_node != parent) { dfs(adj, curr_node, node, curr_cost); }
        }
    }

    void build_table(const vector<vector<pair<int, T>>>& adj) {
        depth.assign(n + 1, invalid);
        table.assign(n + 1, vector<int>(LOG, 0));
        cost.assign(n + 1, vector<T>(LOG, def));
        dfs(adj, root, 0, def);
    }

    LCA_Weighted(int n, const vector<vector<pair<int, T>>>& adj, T default_value = 0, int root_node = 1)
        : n(n), root(root_node), def(default_value), invalid(0) {
        LOG = __lg(n) + 2;
        build_table(adj);
    }

    T get_cost(int node, const int dist) {
        if (depth[node] < dist) return def;
        T answer = def;
        for (int bit = 0; bit < LOG; bit++) {
            if ((dist >> bit) & 1) {
                answer = operation(answer, cost[node][bit]);
                node = table[node][bit];
            }
        }
        return answer;
    }

    int kth_ancestor(int node, int k) {
        if (depth[node] < k) return invalid;
        for (int bit = LOG - 1; bit >= 0; bit--) {
            if ((k >> bit) & 1) { node = table[node][bit]; }
        }
        return node;
    }

    int get_lca(int u, int v) {
        if (depth[u] < depth[v]) swap(u, v);
        u = kth_ancestor(u, depth[u] - depth[v]);
        if (u == v) return u;
        for (int bit = LOG - 1; bit >= 0; bit--) {
            if (table[u][bit] != table[v][bit]) {
                u = table[u][bit];
                v = table[v][bit];
            }
        }
        return table[u][0];
    }

    int distance(int u, int v) { return depth[u] + depth[v] - 2 * depth[get_lca(u, v)]; }

    int get_lvl(int node) { return depth[node]; }

    T query(const int u, const int v) {
        int lca = get_lca(u, v);
        return operation(get_cost(u, depth[u] - depth[lca]), get_cost(v, depth[v] - depth[lca]));
    }
};
