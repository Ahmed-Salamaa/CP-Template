/*
 * Topic: Tree - Pruefer Code
 * Description: Prüfer Code is a unique sequence of N-2 integers that represents a labeled tree
 *   on N vertices, commonly used in combinatorial enumeration and constructing
 *   random trees.
 */
#include "../../core.h"

struct Pruefer_Code {
    static vector<pair<int, int>> decode(const vector<int>& code) {
        int n = code.size() + 2;
        vector<int> degree(n + 5, 1);
        for (int i : code) degree[i]++;

        int ptr = 0;
        while (degree[ptr] != 1) ptr++;
        int leaf = ptr;

        vector<pair<int, int>> edges;
        for (int v : code) {
            edges.emplace_back(leaf, v);
            if (--degree[v] == 1 && v < ptr) {
                leaf = v;
            } else {
                ptr++;
                while (degree[ptr] != 1) ptr++;
                leaf = ptr;
            }
        }
        edges.emplace_back(leaf, n - 1);
        return edges;
    }

    static vector<int> encode(vector<vector<int>> adj) {
        int n = adj.size();
        vector<int> parent(n + 5);
        parent[n - 1] = -1;
        auto dfs = [&](auto& self, int v, int p) -> void {
            for (int u : adj[v]) {
                if (u != p) {
                    parent[u] = v;
                    self(self, u, v);
                }
            }
        };
        dfs(dfs, n - 1, -1);

        int ptr = -1;
        vector<int> degree(n + 5);
        for (int i = 0; i < n; i++) {
            degree[i] = adj[i].size();
            if (degree[i] == 1 && ptr == -1) ptr = i;
        }

        vector<int> code(n - 2);
        int leaf = ptr;
        for (int i = 0; i < n - 2; i++) {
            int next = parent[leaf];
            code[i] = next;
            if (--degree[next] == 1 && next < ptr) {
                leaf = next;
            } else {
                ptr++;
                while (degree[ptr] != 1) ptr++;
                leaf = ptr;
            }
        }

        return code;
    }
};
