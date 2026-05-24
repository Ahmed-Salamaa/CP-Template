#include <bits/stdc++.h>
using namespace std;

#define ll long long int
#define X first
#define Y second

class CentroidDecomposition
{
    private:
        int n;

        // original tree: stores {neighbor, edge_weight}
        vector<vector<pair<int, long long>>> adj; 
        
        vector<int> sz;              // subtree sizes (number of nodes)
        vector<int> parent;          // centroid-tree parent
        vector<int> level;           // depth in centroid tree
        vector<bool> removed;        // centroid marked

        int centroid_root = -1;      // store root of centroid tree for O(1) access

        // compute subtree sizes (based on node count, ignoring edge weights)
        // Time Complexity : O(size of subtree) per call
        int dfs_size(int u, int p)
        {
            sz[u] = 1;
            for (auto& edge : adj[u])
            {
                int v = edge.first;
                if (v == p || removed[v]) continue;
                sz[u] += dfs_size(v, u);
            }
            return sz[u];
        }

        // find centroid in subtree (balancing by node count)
        // Time Complexity : O(size of subtree) per call
        int dfs_centroid(int u, int p, int total)
        {
            for (auto& edge : adj[u])
            {
                int v = edge.first;
                if (v == p || removed[v]) continue;
                if (sz[v] > total / 2)
                    return dfs_centroid(v, u, total);
            }
            return u;
        }

        // recursive centroid decomposition
        // Amortized Time Complexity : O(n log n) total for the whole tree
        void build_cd(int entry, int p, int depth)
        {
            int total = dfs_size(entry, -1);
            int c = dfs_centroid(entry, -1, total);

            parent[c] = p;
            level[c] = depth;
            removed[c] = true;

            if (p == -1) centroid_root = c;

            for (auto& edge : adj[c])
            {
                int v = edge.first;
                if (!removed[v])
                    build_cd(v, c, depth + 1);
            }
        }

    public:
        // constructor: empty tree
        CentroidDecomposition(int n) : n(n)
        {
            adj.resize(n + 5);
            sz.resize(n + 5);
            parent.assign(n + 5, -1);
            level.resize(n + 5);
            removed.assign(n + 5, false);
            centroid_root = -1;
        }

        // constructor: from existing edge-weighted adjacency list
        CentroidDecomposition(int n, const vector<vector<pair<int, long long>>> &graph, int root = 1)
            : n(n), adj(graph)
        {
            sz.resize(n + 5);
            parent.assign(n + 5, -1);
            level.resize(n + 5);
            removed.assign(n + 5, false);
            centroid_root = -1;
            build(root);
        }

        // add a weighted edge to the original tree
        // Time Complexity : O(1)
        void add_edge(int u, int v, long long w)
        {
            adj[u].push_back({v, w});
            adj[v].push_back({u, w});
        }

        // delete an edge from the original tree
        // Time Complexity : O(degree(u) + degree(v))
        void delete_edge(int u, int v)
        {
            auto itu = find_if(adj[u].begin(), adj[u].end(), [v](const pair<int, long long>& edge) { return edge.first == v; });
            if (itu != adj[u].end()) adj[u].erase(itu);

            auto itv = find_if(adj[v].begin(), adj[v].end(), [u](const pair<int, long long>& edge) { return edge.first == u; });
            if (itv != adj[v].end()) adj[v].erase(itv);
        }

        // build centroid decomposition
        // Time Complexity : O(n log n) total
        void build(int root = 1)
        {
            sz.assign(n + 5, 0);
            parent.assign(n + 5, -1);
            level.assign(n + 5, 0);
            removed.assign(n + 5, false);
            centroid_root = -1;

            build_cd(root, -1, 0);
        }

        // get centroid-tree parent
        // Time Complexity : O(1)
        int get_parent(int u)
        {
            return parent[u];
        }

        // get adjacency list of centroid tree
        // Note: The centroid tree itself is UNWEIGHTED. Distances are calculated on the original tree.
        vector<vector<int>> get_tree()
        {
            vector<vector<int>> new_graph(n + 5);
            for (int i = 1; i <= n; i++)
            {
                if (parent[i] != -1)
                {
                    new_graph[i].push_back(parent[i]);
                    new_graph[parent[i]].push_back(i);
                }
            }
            return new_graph;
        }

        // get level of node in centroid tree
        // Time Complexity : O(1)
        int get_level(int u)
        {
            return level[u];
        }

        // check if u is ancestor of v in centroid tree
        // Time Complexity : O(log n) worst-case
        bool is_ancestor(int u, int v)
        {
            while (v != -1)
            {
                if (v == u) return true;
                v = parent[v];
            }
            return false;
        }

        // get the root of centroid tree
        // Time Complexity : O(1)
        int get_root() const
        {
            return centroid_root;
        }
};

