/*
 * Topic: Tree - Euler Tour
 * Description: Euler Tour is a tree traversal technique that flattens a tree into an array,
 *   allowing subtree operations to be converted into continuous subarray range
 *   updates or queries.
 */
#include "../../core.h"

struct EulerTour {
    int n;
    int timer;
    vector<int> start_time;
    vector<int> end_time;
    vector<int> euler_tree;
    vector<vector<int>> adj;

    EulerTour(int nodes) : n(nodes), timer(0) {
        start_time.resize(n + 5, 0);
        end_time.resize(n + 5, 0);
        euler_tree.resize(2 * n + 5, 0);
        adj.resize(n + 5);
    }

    void add_edge(int u, int v) {
        adj[u].push_back(v);
        adj[v].push_back(u);
    }

    // 1 node in range
    void dfs_linearization_one_node(int u, int p) {
        euler_tree[timer] = u;
        start_time[u] = timer;
        timer++;
        for (int v : adj[u]) {
            if (v != p) { dfs_linearization_one_node(v, u); }
        }
        end_time[u] = timer - 1;
    }

    // 2 nodes in range
    void dfs_linearization_two_nodes(int u, int p) {
        euler_tree[timer] = u;
        start_time[u] = timer;
        timer++;
        for (int v : adj[u]) {
            if (v != p) { dfs_linearization_two_nodes(v, u); }
        }
        euler_tree[timer] = u;
        end_time[u] = timer;
        timer++;
    }

    // classical Euler Tour
    void dfs_euler_tour(int u, int p) {
        euler_tree[timer] = u;
        start_time[u] = timer;
        timer++;
        for (int v : adj[u]) {
            if (v != p) {
                dfs_euler_tour(v, u);
                euler_tree[timer++] = u;
            }
        }
    }
};
