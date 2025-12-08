
#define query_of_the_segment Segment_tree

template < typename T = int , int VAL_ON_EDGE = 0 > 
class HLD 
{
    // VAL_ON_EDGE = 0 if value on nodes
    // VAL_ON_EDGE = 1 if value on edges

    private :
        const vector < vector < pair < int , T > > > & graph ;
        vector <int> dep , par , root , pos , SubtreeSz , child ;
        int nxtPos , n , worest_log ;
        query_of_the_segment < T > RangeDS ;

        // init the tree
        void init ( int node , int p = -1 , int d = 0 ) 
        {
            dep[node] = d ;
            par[node] = p ;
            SubtreeSz[node] = 1 ;

            for ( const auto & [ curr , __ ] : graph[node] ) 
            {
                if ( curr == p ) continue ;

                init ( curr , node , d + 1 ) ;
                
                SubtreeSz[node] += SubtreeSz[curr] ;

                if ( SubtreeSz[curr] > SubtreeSz[ child[node] ] ) child[node] = curr ;
            }
        }

        // build the chains
        void build( int node , bool newChain = true) 
        {
            root[node] = newChain ? node : root[ par[node] ] ;
            pos[node] = nxtPos ++ ;

            if ( child[node] ) build ( child[node] , false ) ;
            for ( const auto& [ curr , __ ] : graph[node] ) 
            {
                if ( curr == par[node] || curr == child[node] ) continue ;
                build ( curr , true ) ;
            }
        }

        // get the root of the chain
        int get_lca ( int u , int v )
        {
            while ( root[u] != root[v] ) 
            {
                if ( dep[ root[u] ] < dep[ root[v] ] ) swap( u , v ) ;
                u = par[ root[u] ] ;
            }
            return dep[u] < dep[v] ? u : v ;
        }

        // make u lower
        void makeULower ( int & u , int & v ) 
        {
            if ( dep[ root[u] ] < dep[ root[v] ] || ( root[u] == root[v] && dep[u] < dep[v] ) )
                swap ( u , v ) ;
        }

        // move up the chain and also change the next position
        pair < T , T > moveUp ( int & u ) 
        {
            pair < T , T > ans = { pos [ root[u] ] , pos[u] } ;
            u = par[ root[u] ] ;
            return ans ;
        }

        vector < pair < T , T > > queryPath ( int u , int v ) 
        {
            // return all ranges in segment tree
            vector < pair < T , T > > ans ;
            ans.reserve ( worest_log ) ;

            while ( root[u] != root[v] )
            {
                makeULower( u , v ) ;
                ans.push_back( moveUp( u ) ) ;
            }
            // add range between u and v
            makeULower( u , v ) ;

            if ( !VAL_ON_EDGE ) // value on nodes
                ans.push_back( { pos[v] , pos[u] } ) ;
            else if ( u != v ) // don't include the root node
                ans.push_back( { pos[v] + 1 , pos[u] } ) ;
            
            return ans ;
        }

        // get child of a node for problem that value on edges
        int getChild( int u , int v ) { return ( par[u] == v ) ? u : v ; }

    public :

        HLD ( const vector < vector < pair < T , T > > > & G , int treeRoot = 1 ) 
            : graph(G) , n ( graph.size( ) ) , RangeDS ( query_of_the_segment <T> (n) ) 
        {
            nxtPos = 1 ;
            worest_log = __lg( n + 5 ) + 1 ;
            // child is heavy child
            dep = par = root = pos = SubtreeSz = child = vector<T>(n + 5);
            init( treeRoot ) ;
            build( treeRoot ) ;
        }

        int getLca ( int u , int v ) { return get_lca( u , v ) ; } 

        // update value of node u
        void update ( int node , T val ) { RangeDS.update( pos[node] , val ) ; }

        // update value of edge u-v
        void update ( int u , int v , T val ) { RangeDS.update( pos[ getChild ( u , v ) ] , val ) ; }

        // query value
        T query ( int u_q , int v_q ) 
        {
            T ans = 0 ;
            for ( const auto & [ u , v ] : queryPath ( u_q , v_q ) ) ans += RangeDS.query ( u , v ) ;
            return ans ;
        }
} ;