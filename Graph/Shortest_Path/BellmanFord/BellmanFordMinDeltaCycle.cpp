/*
 * Topic: Bellman-Ford (Min Delta Cycle)
 * Description: Finds the minimum mean weight cycle in a graph.
 * approach. Usage: BellmanFordMinDeltaCycle bf; int min_delta = bf.findMinDeltaNegativeCycle(edges);
 */
#include "../../../core.h"

struct BellmanFordMinDeltaCycle {
    // Finds the minimum total-weight (most negative) cycle-delta in the graph.
    // Time Complexity: O(V * E + V^2)
    // Space Complexity: O(V^2)
    int findMinDeltaNegativeCycle(const vector<array<int, 3>>& edges) {
        int INF_LL = 2e18;  // roughly LLONG_MAX / 4
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
        int min_delta = INF_LL;
        for (int v = 0; v < N; ++v) {
            if (dp[N][v] == INF_LL) continue;
            int temp = -1e18;
            for (int k = 0; k < N; ++k) {
                if (dp[k][v] == INF_LL) continue;
                int delta = dp[N][v] - dp[k][v];
                temp = max(temp, delta);
            }
            min_delta = min(min_delta, temp);
        }
        return min_delta;
    }
};
