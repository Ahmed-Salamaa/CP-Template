/*
 * Topic: TwoSAT
 * Description: 2-SAT (2-Satisfiability) solves boolean satisfiability problems where clauses
 *   have at most two literals by reducing them to finding SCCs in an implication
 *   graph. It is applied in logic puzzles, scheduling, and constraints checking
 *   where choices are binary.
 * Usage:
 *    TwoSAT solver(N);
 *    solver.solve();
 */
#include "../../../core.h"

// Time Complexity: O(V + E)
// Space Complexity: O(V + E)
struct TwoSAT {
    int n;
    vector<vector<int>> g;
    vector<bool> ans;

    TwoSAT(int n) : n(n), g(2 * n), ans(n) {}

    void add_or(int i, bool f, int j, bool g_val) {
        this->g[2 * i + !f].push_back(2 * j + g_val);
        this->g[2 * j + !g_val].push_back(2 * i + f);
    }

    void xor_clause(int i, bool f, int j, bool g_val) {
        add_or(i, f, j, g_val);
        add_or(i, !f, j, !g_val);
    }

    void imply(int i, bool f, int j, bool g_val) { add_or(i, !f, j, g_val); }

    void bi_imply(int i, bool f, int j, bool g_val) {
        imply(i, f, j, g_val);
        imply(j, f, i, g_val);
    }

    void force_true(int i) { add_or(i, true, i, true); }

    void force_false(int i) { add_or(i, false, i, false); }

    bool solve() {
        int sz = 2 * n;
        vector<int> tin(sz, -1), low(sz), id(sz), stk;
        int timer = 0, scc_cnt = 0;
        stk.reserve(sz);
        auto dfs = [&](auto& self, int v) -> void {
            low[v] = tin[v] = timer++;
            stk.push_back(v);
            for (int to : g[v]) {
                if (tin[to] == -1) {
                    self(self, to);
                    low[v] = min(low[v], low[to]);
                } else if (tin[to] < sz) {
                    low[v] = min(low[v], tin[to]);
                }
            }
            if (low[v] == tin[v]) {
                while (true) {
                    int u = stk.back();
                    stk.pop_back();
                    tin[u] = sz;
                    id[u] = scc_cnt;
                    if (u == v) break;
                }
                scc_cnt++;
            }
        };
        for (int i = 0; i < sz; i++)
            if (tin[i] == -1) dfs(dfs, i);
        for (int i = 0; i < sz; i++) id[i] = scc_cnt - 1 - id[i];
        for (int i = 0; i < n; i++) {
            if (id[2 * i] == id[2 * i + 1]) return false;
            ans[i] = id[2 * i] < id[2 * i + 1];
        }
        return true;
    }
};
