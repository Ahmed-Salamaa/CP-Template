/* Topic: DSU on Tree (Sack)
 * Description: Sack (DSU on Tree) is an elegant technique that processes offline subtree
 *   queries efficiently in O(N log N) time by retaining the data of the heavy child.
 * Usage:
 * Sack sack(n);
 * sack.col[i] = color;
 * sack.add_edge(u, v);
 * sack.dfs(1, 0);
 * sack.dsu(1, 0, false);
 */
#include "../../core.h"

struct Sack {
    int n;
    vector<vector<int>> g;
    vector<int> col, sz, cnt, ans;
    vector<bool> big;

    Sack(int n, int max_color_value = 1e5 + 9)
        : n(n), g(n + 5), col(n + 5), sz(n + 5), cnt(max_color_value + 5, 0), ans(n + 5), big(n + 5, false) {}

    void add_edge(int u, int v) {
        g[u].push_back(v);
        g[v].push_back(u);
    }

    void dfs(int u, int p) {
        sz[u] = 1;
        for (int v : g[u]) {
            if (v == p) continue;
            dfs(v, u);
            sz[u] += sz[v];
        }
    }

    void add(int u, int p, int x) {
        cnt[col[u]] += x;
        for (int v : g[u]) {
            if (v == p || big[v]) continue;
            add(v, u, x);
        }
    }

    void dsu(int u, int p, bool keep) {
        int bigchild = -1, mx = -1;
        for (int v : g[u]) {
            if (v == p) continue;
            if (sz[v] > mx) {
                mx = sz[v];
                bigchild = v;
            }
        }
        for (int v : g[u]) {
            if (v == p || v == bigchild) continue;
            dsu(v, u, false);
        }
        if (bigchild != -1) {
            dsu(bigchild, u, true);
            big[bigchild] = true;
        }
        add(u, p, 1);

        // --- Process node u's queries here ---
        // For example, if we want to store how many nodes in the subtree share u's color:
        // ans[u] = cnt[col[u]];

        if (bigchild != -1) big[bigchild] = false;
        if (!keep) add(u, p, -1);
    }
};
