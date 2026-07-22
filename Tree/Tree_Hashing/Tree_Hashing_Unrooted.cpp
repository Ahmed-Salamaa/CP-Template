/*
 * Topic: Tree - Tree Hashing
 * Description: Unrooted Tree Hashing computes a unique hash for an unrooted tree (often by
 *   rooting at its centroid), allowing rapid structural isomorphism checks against
 *   other unrooted trees.
 */
#include "../../core.h"

struct Tree_Hashing_Unrooted {
    static inline long long split(long long x) {
        x += 0x9e3779b974a7c15ULL;
        x = (x ^ (x >> 30)) * 0xbf58476d1e4e5b9LL;
        x = (x ^ (x >> 27)) * 0x94d049bb33111ebLL;
        return x ^ (x >> 31);
    }

    static long long combine(const vector<long long>& child) {
        long long h = 146959810393466537LL;
        for (long long x : child) {
            long long k = x + 0x9e3779b97f4a7c15ULL;
            h ^= k + (h << 6) + (h >> 2);
            h *= 1099511628211ULL;
        }
        h ^= (0xff51afd7ed558cLL ^ (long long)child.size());
        return split(h);
    }

    static vector<int> find_centers(int n, const vector<vector<int>>& adj) {
        if (n == 1) return {1};
        vector<int> deg(n + 5);
        queue<int> q;
        for (int i = 1; i <= n; ++i) {
            deg[i] = (int)adj[i].size();
            if (deg[i] <= 1) q.push(i);
        }
        int removed = 0;
        while (!q.empty()) {
            if (n - removed <= 2) {
                vector<int> centers;
                while (!q.empty()) {
                    centers.push_back(q.front());
                    q.pop();
                }
                return centers;
            }
            int sz = q.size();
            removed += sz;
            for (int i = 0; i < sz; ++i) {
                int u = q.front();
                q.pop();
                for (int v : adj[u]) {
                    if (--deg[v] == 1) q.push(v);
                }
            }
        }
        return {};
    }

    static long long rooted_hash(int root, const vector<vector<int>>& adj) {
        int n = (int)adj.size() - 1;
        vector<int> parent(n + 5, -1), order;
        order.reserve(n);
        stack<int> st;
        st.push(root);
        parent[root] = 0;
        while (!st.empty()) {
            int u = st.top();
            st.pop();
            order.push_back(u);
            for (int v : adj[u])
                if (parent[v] == -1) {
                    parent[v] = u;
                    st.push(v);
                }
        }
        vector<long long> h(n + 5, 0);
        for (int i = (int)order.size() - 1; i >= 0; --i) {
            int u = order[i];
            vector<long long> ch;
            ch.reserve(adj[u].size());
            for (int v : adj[u])
                if (v != parent[u]) ch.push_back(h[v]);
            sort(ch.begin(), ch.end());
            h[u] = combine(ch);
        }
        return h[root];
    }

    static long long tree_hash(const vector<vector<int>>& adj) {
        int n = (int)adj.size() - 1;
        if (n <= 0) return 0;
        if (n == 1) return 1;
        vector<int> centers = find_centers(n, adj);
        if (centers.size() == 1) return rooted_hash(centers[0], adj);
        long long h1 = rooted_hash(centers[0], adj);
        long long h2 = rooted_hash(centers[1], adj);
        if (h1 > h2) swap(h1, h2);
        vector<long long> pairv = {h1, h2};
        return combine(pairv);
    }
};
