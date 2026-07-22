/*
 * Topic: Graph - Cycle
 * Description: Detects if a graph contains a cycle and reconstructs the sequence of vertices
 *   forming the cycle.
 */
#include "../../../core.h"

// Time Complexity: O(V + E)
// Space Complexity: O(V + E)
template <typename T = int>
struct CycleUndirected {
    T n;
    std::vector<std::vector<T>> adj;
    std::vector<bool> visited;
    std::vector<T> parent;
    T cycle_start, cycle_end;

    CycleUndirected(T _n) : n(_n), adj(_n + 1), visited(_n + 1, false), parent(_n + 1, -1) {
        cycle_start = -1;
        cycle_end = -1;
    }

    void add_edge(T u, T v) {
        adj[u].push_back(v);
        adj[v].push_back(u);
    }

    bool dfs(T v, T par) {
        visited[v] = true;
        for (auto &u : adj[v]) {
            if (u == par) continue;
            if (visited[u]) {
                cycle_end = v;
                cycle_start = u;
                return true;
            }
            parent[u] = v;
            if (dfs(u, v)) return true;
        }
        return false;
    }

    std::vector<T> get_cycle() {
        cycle_start = -1;
        std::fill(visited.begin(), visited.end(), false);
        std::fill(parent.begin(), parent.end(), -1);
        
        for (T i = 0; i <= n; i++) {
            if (!visited[i] && dfs(i, parent[i])) {
                break;
            }
        }
        
        if (cycle_start == -1) return {};
        
        std::vector<T> res;
        res.push_back(cycle_start);
        for (T v = cycle_end; v != cycle_start; v = parent[v]) {
            res.push_back(v);
        }
        res.push_back(cycle_start);
        std::reverse(res.begin(), res.end());
        return res;
    }
};
