/*
 * Topic: ManhattanMST
 * Description: Manhattan Minimum Spanning Tree algorithm efficiently computes the MST for a set
 *   of points in 2D space using Manhattan distance, reducing the O(N^2) edges to
 *   O(N) by dividing the space into octants. It is a key optimization in VLSI design
 *   and routing.
 * efficiently. Usage: ManhattanMST solver; vector<ManhattanMST::Point> pts = {{0, 0}, {1, 2}};
 *    vector<ManhattanMST::Edge> edges = solver.build(pts);
 *    // Pass edges to Kruskal's to get MST
 */
#include "../core.h"

// Time Complexity: O(N log N) total work for build
// Space Complexity: O(N)
struct ManhattanMST {
    struct Point {
        int x, y;
    };

    struct Edge {
        int w;
        int u, v;
    };

    // Returns a list of edges in the format {weight, u, v}.
    // Passing this list to Kruskal's algorithm will give the Manhattan MST.
    // Time Complexity: O(N log N)
    // Space Complexity: O(N)
    vector<Edge> build(vector<Point> ps) {
        int n = sz(ps);
        vector<int> ids(n);
        iota(all(ids), 0);
        vector<Edge> edges;
        for (int rot = 0; rot < 4; rot++) {
            sort(all(ids), [&](int i, int j) { return (ps[i].x + ps[i].y) < (ps[j].x + ps[j].y); });
            map<int, int, greater<int>> active;  // (xs, id)
            for (auto i : ids) {
                for (auto it = active.lower_bound(ps[i].x); it != active.end(); active.erase(it++)) {
                    int j = it->second;
                    if (ps[i].x - ps[i].y > ps[j].x - ps[j].y) break;
                    edges.push_back({(ps[i].x - ps[j].x) + (ps[i].y - ps[j].y), i, j});
                }
                active[ps[i].x] = i;
            }
            for (auto& p : ps) {
                if (rot & 1)
                    p.x *= -1;
                else
                    swap(p.x, p.y);
            }
        }
        return edges;
    }
};
