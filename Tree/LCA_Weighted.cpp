#include <bits/stdc++.h>
using namespace std;


template < typename graphType = int >
class Lowest_Common_Ancestor
{
    private:
       
        const vector < vector < pair < graphType , graphType > > > & adj_of_graph ;
        vector < vector < graphType > > cost;
        vector < vector <graphType> > table ;
        vector < graphType > depth ;
        graphType n , root , LOG = 1 , invalid ;
        graphType def ;

        graphType operation ( graphType a , graphType b )
        {
            return max ( a , b ) ;
        }

        void dfs ( graphType node , graphType parent , graphType val )
        {
            depth [node] = depth [parent] + 1 ;

            table [node] [0] = parent ;
            cost [node] [0] = val ;

            for ( graphType bit = 1 ; bit < LOG ; bit ++ )
            {
                table [node] [bit] = table [ table [node] [ bit - 1 ] ] [ bit - 1 ] ;
                cost [node] [bit] = operation ( cost [node] [ bit - 1 ] , cost [ table [node] [ bit - 1 ] ] [ bit - 1 ] ) ;
            }

            for ( const auto & [ curr_node , curr_cost ] : adj_of_graph [node] )
                if ( curr_node != parent ) dfs ( curr_node , node , curr_cost ) ;
        }

        void build_table ()
        {
            depth.resize ( n + 10 , invalid ) ;
            table.resize ( n + 10 , vector <graphType> ( LOG + 1 ) ) ;
            cost.resize ( n + 5 , vector < graphType > ( LOG , def ) ) ;
            dfs ( root , 0 , def ) ;
        }

        graphType get_cost ( graphType node , const graphType dist )
        {
            if ( depth[node] < dist ) return def ;
            
            graphType answer = def ;
            
            for ( graphType bit = 0 ; bit < LOG ; bit ++ )
            {
                if ( dist & ( 1 << bit ) )
                {
                    answer = operation ( answer , cost[node][bit] ) ;
                    node = table[node][bit] ;
                }
            }

            return answer ;
        }

    public:
    
        Lowest_Common_Ancestor ( graphType n , const vector < vector < pair < graphType , graphType > > > & adj , graphType default_value = 0 , graphType root_node = 1 ) 
            : adj_of_graph(adj) , def ( (graphType)(default_value) ) , root(root_node) 
        {
            this->n = n ;
            LOG = __lg(n) + 1 ;
            build_table () ;
        }

        graphType kth_ancestor ( graphType node , graphType k )
        {
            if ( depth [node] < k ) return invalid ;

            for ( graphType bit = LOG - 1 ; bit > -1 ; bit -- )
                if ( ( k >> bit ) & 1 ) node = table [node] [bit] ;

            return node ;
        }

        graphType get_lca ( graphType u , graphType v )
        {
            if ( depth [u] < depth [v] ) swap ( u , v ) ;

            u = kth_ancestor ( u , depth [u] - depth [v] ) ;
            if ( u == v ) return u ;

            for ( graphType bit = LOG - 1 ; bit > -1 ; bit -- )
                if ( table [u] [bit] != table [v] [bit] ) u = table [u] [bit] , v = table [v] [bit] ;

            return table [u][0] ; 
        }

        graphType distance ( graphType u , graphType v )
        {
            return depth[u] + depth[v] - 2 * depth[ get_lca ( u , v ) ] ;
        }

        graphType get_lvl ( graphType node )
        {
            return depth[node] ;
        }

        graphType query ( const graphType u , const graphType v ) 
        {
            graphType lca = get_lca ( u , v ) ;
            return operation ( get_cost ( u , depth[u] - depth[lca] ) , get_cost ( v , depth[v] - depth[lca] ) ) ;
        }
};