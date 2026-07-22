/*
 * Topic: Floyd-Warshall (Strongly Connected Components)
 * Description: A technique integrating Floyd-Warshall to identify Strongly Connected Components
 *   (SCCs) by utilizing the all-pairs reachability matrix. Helpful in small graphs
 *   for quickly decomposing directed graphs into mutually reachable subgraphs.
 * matrix. Usage: FloydSCC fw(n, adj_matrix); vector<vector<int>> condensed_graph = fw.scc();
 */
#include "../../../core.h"

struct FloydSCC {
    int n;
    vector<vector<int>> adj;
    vector<int> comp;

    FloydSCC(int nodes, const vector<vector<int>>& adjacency_matrix)
        : n(nodes), adj(adjacency_matrix), comp(nodes, -1) {}

    // Time Complexity: O(V^2)
    // Space Complexity: O(V^2)
    vector<vector<int>> scc() {
        int cnt = 0;
        for (int i = 0; i < n; i++) {
            if (comp[i] == -1) {
                comp[i] = cnt++;
                for (int j = 0; j < n; j++) {
                    if (adj[i][j] < 2e9 && adj[j][i] < 2e9)  // Assuming 2e9 is infinity
                        comp[j] = comp[i];
                }
            }
        }
        vector<vector<int>> CompGraph(cnt, vector<int>(cnt, 0));
        for (int i = 0; i < n; i++) {
            for (int j = 0; j < n; j++) {
                if (adj[i][j] != 2e9) { CompGraph[comp[i]][comp[j]] += 1; }
            }
        }
        return CompGraph;
    }
};
