#include <bits/stdc++.h>
using namespace std;

#define ll long long int
#define X first
#define Y second

class CentroidDecomposition
{
    private:
        int n ;

        vector<vector<int>> adj;     // original tree
        vector<int> sz;              // subtree sizes
        vector<int> parent;          // centroid-tree parent
        vector<int> level;           // depth in centroid tree
        vector<bool> removed;        // centroid marked

        int centroid_root = -1;      // store root of centroid tree for O(1) access

        // compute subtree sizes
        // Time Complexty : O(size of subtree) per call
        int dfs_size(int u, int p)
        {
            sz[u] = 1;
            for (int v : adj[u])
            {
                if (v == p || removed[v]) continue;
                sz[u] += dfs_size(v, u);
            }
            return sz[u];
        }

        // find centroid in subtree
        // Time Complexty : O(size of subtree) per call
        int dfs_centroid(int u, int p, int total)
        {
            for (int v : adj[u])
            {
                if (v == p || removed[v]) continue;
                if (sz[v] > total / 2)
                    return dfs_centroid(v, u, total);
            }
            return u;
        }

        // recursive centroid decomposition
        // Amortized Time Complexty : O(n log n) total for the whole tree
        void build_cd(int entry, int p, int depth)
        {
            int total = dfs_size(entry, -1);
            int c = dfs_centroid(entry, -1, total);

            parent[c] = p;
            level[c] = depth;
            removed[c] = true;

            // if this centroid has no centroid-parent, it's the root of centroid tree
            if (p == -1) centroid_root = c;

            for (int v : adj[c])
            {
                if (!removed[v])
                    build_cd(v, c, depth + 1);
            }
        }

    public:
        // constructor: empty tree
        // Time Complexty : O(n) for resizing vectors
        CentroidDecomposition(int n) : n(n)
        {
            adj.resize( n + 5 ) ;
            sz.resize( n + 5 ) ;
            parent.assign( n + 5 , -1) ;
            level.resize( n + 5 ) ;
            removed.assign( n + 5 , false) ;
            centroid_root = -1;
            // Note: do NOT automatically call build() here unless you have edges already added.
            // If you want automatic build on empty graph, you can call build() explicitly.
        }

        // constructor: from adjacency list
        // Time Complexty : O(n + edges) to copy the list +  O(n log n) for build
        CentroidDecomposition( int n , const vector<vector<int>> &graph , int root = 1 )
            : n ( n )
            , adj ( graph )
        {
            sz.resize( n + 5 ) ;
            parent.assign( n + 5 , -1) ;
            level.resize( n + 5 ) ;
            removed.assign( n + 5 , false) ;
            centroid_root = -1;
            build( root ) ;
        }

        // add an edge to the original tree
        // Time Complexty : O(1)
        void add_edge(int u, int v)
        {
            adj[u].push_back(v);
            adj[v].push_back(u);
        }

        // delete an edge from the original tree
        // Time Complexty : O(degree(u) + degree(v)) due to linear search
        void delete_edge(int u, int v)
        {
            auto itu = find(adj[u].begin(), adj[u].end(), v);
            if (itu != adj[u].end()) adj[u].erase(itu);

            auto itv = find(adj[v].begin(), adj[v].end(), u);
            if (itv != adj[v].end()) adj[v].erase(itv);
        }

        // build centroid decomposition (call once after edges set)
        // Time Complexty : O(n log n) total
        void build(int root = 1)
        {
            sz.assign( n + 5 , 0 ) ;
            parent.assign( n + 5 , -1) ;
            level.assign( n + 5 , 0 ) ;
            removed.assign( n + 5 , false) ;
            centroid_root = -1;

            build_cd(root, -1, 0);
        }

        // get centroid-tree parent
        // Time Complexty : O(1)
        int get_parent(int u)
        {
            return parent[u];
        }

        // get adjacency list of centroid tree
        // Time Complexty : O(n)
        vector < vector <int> > get_tree ()
        {
            vector < vector <int> > new_graph ( n + 5 ) ;
            for ( int i = 1 ; i <= n ; i ++ )
            {
                if (parent[i] != -1)
                {
                    new_graph[i].push_back(parent[i]);
                    new_graph[parent[i]].push_back(i);
                }
            }
            return new_graph ;
        }

        // get level of node in centroid tree
        // Time Complexty : O(1)
        int get_level(int u)
        {
            return level[u];
        }

        // check if u is ancestor of v in centroid tree
        // Time Complexty : O(log n) worst-case (centroid-tree height <= Time Complexty : O(log n))
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
        // Time Complexty : O(1)
        int get_root() const
        {
            return centroid_root;
        }

        // get centroid of subtree rooted at x 
        // Time Complexty : O(size of subtree) per call
        int get_subtree_centroid(int x)
        {
            int total = dfs_size(x, -1) ;
            return dfs_centroid(x, -1, total) ;
        }
};
