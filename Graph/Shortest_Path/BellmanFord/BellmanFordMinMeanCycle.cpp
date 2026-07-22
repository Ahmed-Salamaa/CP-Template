/*
 * Topic: Bellman-Ford (Min Mean Cycle)
 * Description: Karp's Minimum Mean Weight Cycle algorithm, which finds a cycle in a directed
 *   graph that minimizes the average weight of its edges.
 * Usage:
 *    BellmanFordMinMeanCycle bf;
 *    ld min_mean = bf.findMinMeanCycle(edges);
 */
#include "../../../core.h"

struct BellmanFordMinMeanCycle {
    // Finds the minimum mean-weight cycle in a directed graph.
    // Time Complexity: O(V * E + V^2)
    // Space Complexity: O(V^2)
    ld findMinMeanCycle(const vector<array<int, 3>>& edges) {
        int INF_LL = 2e18;
        int N = 0;
        for (auto& e : edges) { N = max<int>(N, max(e[0], e[1]) + 1); }
        vector<vector<int>> dp(N + 1, vector<int>(N, INF_LL));
        for (int v = 0; v < N; ++v) dp[0][v] = 0;
        for (int k = 1; k <= N; ++k) {
            for (auto& e : edges) {
                int u = e[0], v = e[1];
                int w = e[2];
                if (dp[k - 1][u] < INF_LL) { dp[k][v] = min(dp[k][v], dp[k - 1][u] + w); }
            }
        }
        ld mu = 1e18;  // Using large double as infinity
        for (int v = 0; v < N; ++v) {
            if (dp[N][v] == INF_LL) continue;
            ld local_max = -1e18;
            for (int k = 0; k < N; ++k) {
                if (dp[k][v] == INF_LL) continue;
                ld mean = (ld)(dp[N][v] - dp[k][v]) / (ld)(N - k);
                local_max = max(local_max, mean);
            }
            mu = min(mu, local_max);
        }
        return mu;
    }
};
