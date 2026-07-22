/*
 * Topic: Tree - Tree Hashing
 * Description: Rooted Tree Hashing assigns unique polynomial or random hash values to rooted
 *   trees, useful for testing isomorphism between two rooted trees.
 */
#include "../../core.h"

struct Tree_Hashing_Rooted {
    map<vector<int>, int> mp;

    void tree_hash(int u, int p, vector<int>& hash, const vector<vector<int>>& adj) {
        vector<int> child;
        for (auto& v : adj[u]) {
            if (v != p) {
                tree_hash(v, u, hash, adj);
                child.emplace_back(hash[v]);
            }
        }

        // Uncomment the following line if the order of children doesn't matter
        // sort(child.begin(), child.end());

        // insert your self in the child array to hash the full subtree
        if (!mp.count(child)) { mp[child] = mp.size() + 1; }
        hash[u] = mp[child];
    }
};
