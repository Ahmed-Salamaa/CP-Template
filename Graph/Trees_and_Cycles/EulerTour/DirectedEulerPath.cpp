// Topic: Directed Euler Path - Finds an Eulerian path in a directed graph using Hierholzer's algorithm
#include "../../../core.h"

struct DirectedEulerPath {
    // Time Complexity: O(V + E)
    // Space Complexity: O(V + E)
    int n, m;
    vector<vector<int>> adj;
    vector<int> in, out, tour;
    int st, en;

    DirectedEulerPath(int n) : n(n), m(0), adj(n), in(n, 0), out(n, 0), st(-1), en(-1) {}

    void add_edge(int u, int v) {
        adj[u].push_back(v);
        out[u]++;
        in[v]++;
        m++;
    }

    bool euler() {
        for (int i = 0; i < n; i++) {
            if (abs(in[i] - out[i]) > 1)
                return false;

            if (in[i] == out[i] + 1) {
                if (en != -1) return false;
                en = i;
            }

            if (out[i] == in[i] + 1) {
                if (st != -1) return false;
                st = i;
            }
        }
        if(st == -1 && en == -1) {
            for(int i = 0; i < n; i++) {
                if(out[i] > 0) {
                    st = i;
                    break;
                }
            }
            if (st == -1) st = 0;
        }
        return true;
    }

    void dfs(int node) {
        while (out[node]) {
            dfs(adj[node][--out[node]]);
        }
        tour.emplace_back(node);
    }

    bool get() {
        if (!euler())
            return false;
        
        dfs(st);

        if ((int)tour.size() != m + 1)
            return false;

        reverse(tour.begin(), tour.end());
        return true;
    }
};
