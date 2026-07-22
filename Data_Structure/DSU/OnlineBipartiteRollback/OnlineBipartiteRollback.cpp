/*
 * Topic: Online Bipartite Rollback DSU
 * Description: A bipartite DSU that also supports rollback operations, allowing backtracking.
 *   Ideal for dynamic
 * connectivity or divide and conquer algorithms. Usage: OnlineBipartiteRollback dsu(N); dsu.add_edge(u, v);
 *    dsu.rollback();
 */
#include "../../../core.h"

// Time Complexity: O(log N) per operation
// Space Complexity: O(N)
struct OnlineBipartiteRollback {
    vector<int> parent, rank, parity;
    vector<bool> bipartite;
    bool whole_graph_bipartite = true;

    struct Change {
        int v;
        int parent, rnk, prt;
        bool bip;
        bool global_bip;
    };

    vector<Change> history;

    OnlineBipartiteRollback(int n) : parent(n + 5), rank(n + 5, 0), parity(n + 5, 0), bipartite(n + 5, true) {
        iota(parent.begin(), parent.end(), 0);
    }

    // Time Complexity: O(log N)
    pair<int, int> find(int v) const {
        if (parent[v] == v) return {v, 0};
        auto pr = find(parent[v]);
        return {pr.first, pr.second ^ parity[v]};
    }

    // Time Complexity: O(steps)
    void rollback_steps(int steps) {
        while (steps-- > 0 && !history.empty()) {
            auto& c = history.back();
            parent[c.v] = c.parent;
            rank[c.v] = c.rnk;
            parity[c.v] = c.prt;
            bipartite[c.v] = c.bip;
            whole_graph_bipartite = c.global_bip;
            history.pop_back();
        }
    }

    // Time Complexity: O(log N)
    bool unite(int a, int b, int& ops) {
        auto [ra, xa] = find(a);
        auto [rb, xb] = find(b);
        if (ra == rb) {
            if (xa == xb) {
                history.push_back(
                    {ra, parent[ra], rank[ra], parity[ra], static_cast<bool>(bipartite[ra]), whole_graph_bipartite});
                bipartite[ra] = false;
                whole_graph_bipartite = false;
            } else {
                history.push_back(
                    {ra, parent[ra], rank[ra], parity[ra], static_cast<bool>(bipartite[ra]), whole_graph_bipartite});
            }
            ops++;
            return bipartite[ra];
        }
        if (rank[ra] < rank[rb]) {
            swap(ra, rb);
            swap(xa, xb);
        }
        ops += 2;
        history.push_back(
            {ra, parent[ra], rank[ra], parity[ra], static_cast<bool>(bipartite[ra]), whole_graph_bipartite});
        history.push_back(
            {rb, parent[rb], rank[rb], parity[rb], static_cast<bool>(bipartite[rb]), whole_graph_bipartite});
        parent[rb] = ra;
        parity[rb] = xa ^ xb ^ 1;
        bipartite[ra] = bipartite[ra] & bipartite[rb];
        if (!bipartite[ra]) whole_graph_bipartite = false;
        if (rank[ra] == rank[rb]) rank[ra]++;
        return bipartite[ra];
    }

    // Time Complexity: O(log N)
    bool is_bipartite(int v) const { return bipartite[find(v).first]; }

    // Time Complexity: O(1)
    bool is_whole_graph_bipartite() const { return whole_graph_bipartite; }
};
