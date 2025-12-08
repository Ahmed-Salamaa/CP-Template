#include <bits/stdc++.h>
using namespace std;

class Lowest_Common_Ancestor
{
    private:
       
        const vector < vector <int> > & adj_of_graph ;
        vector < vector <int> > table ;
        vector < int > depth ;
        int n , root , LOG = 1 , invalid ;

        void dfs ( int node , int parent )
        {
            depth [node] = depth [parent] + 1 ;

            table [node] [0] = parent ;
            for ( int bit = 1 ; bit < LOG ; bit ++ ) table [node] [bit] = table [ table [node] [ bit - 1 ] ] [ bit - 1 ] ;

            for ( const auto & curr : adj_of_graph [node] )
                if ( curr != parent ) dfs ( curr , node ) ;
        }

        void build_table ()
        {
            depth.resize ( n + 10 , invalid ) ;
            table.resize ( n + 10 , vector <int> ( LOG + 1 ) ) ;
            dfs ( root , 0 ) ;
        }

    public:
    
        Lowest_Common_Ancestor ( int n , const vector < vector <int> > & adj , int root_node = 1 ) 
            : adj_of_graph(adj) , root(root_node)
        {
            this->n = n ;
            LOG = __lg(n) + 1 ;
            build_table () ;
        }

        int kth_ancestor ( int node , int k )
        {
            if ( depth [node] < k ) return invalid ;

            for ( int bit = LOG - 1 ; bit > -1 ; bit -- )
                if ( ( k >> bit ) & 1 ) node = table [node] [bit] ;

            return node ;
        }

        int get_lca ( int u , int v )
        {
            if ( depth [u] < depth [v] ) swap ( u , v ) ;

            u = kth_ancestor ( u , depth [u] - depth [v] ) ;
            if ( u == v ) return u ;

            for ( int bit = LOG - 1 ; bit > -1 ; bit -- )
                if ( table [u] [bit] != table [v] [bit] ) u = table [u] [bit] , v = table [v] [bit] ;

            return table [u][0] ; 
        }

        int distance ( int u , int v )
        {
            return depth[u] + depth[v] - 2 * depth[ get_lca ( u , v ) ] ;
        }

        int get_lvl ( int node )
        {
            return depth[node] ;
        }

};