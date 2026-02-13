
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

        // binary lifting table
        vector< vector<int> > up ;
        int LOG;

        // init the tree
        void init ( int node , int p = -1 , int d = 0 ) 
        {
            dep[node] = d ;
            par[node] = (p == -1 ? 0 : p);
            SubtreeSz[node] = 1 ;

            up[0][node] = (p == -1 ? 0 : p);

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
        int get_lca_internal ( int u , int v )
        {
            if ( u == 0 || v == 0 ) return u ^ v;
            if ( dep[u] < dep[v] ) swap( u , v ) ;
            int diff = dep[u] - dep[v];
            for ( int i = 0 ; i <= LOG ; ++i )
                if ( diff & (1 << i ) ) u = up[i][u];
            if ( u == v ) return u;
            for ( int i = LOG ; i >= 0 ; --i )
                if ( up[i][u] != up[i][v] ) {
                    u = up[i][u];
                    v = up[i][v];
                }
            return par[u];
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
            n = (int)graph.size() - 1;
            nxtPos = 1 ;
            worest_log = __lg( n + 5 ) + 1 ;
            // child is heavy child
            dep = par = root = pos = SubtreeSz = child = vector<int>(n + 5, 0);

            // prepare binary lifting
            LOG = max(0, (int)floor(log2(max(1, n))) ) + 2;
            up.assign(LOG+1, vector<int>(n + 5, 0));

            init( treeRoot ) ;

            // fill up table
            for ( int j = 1 ; j <= LOG ; ++j )
                for ( int i = 1 ; i <= n ; ++i )
                    up[j][i] = up[j-1][ i ] ? up[j-1][ up[j-1][ i ] ] : 0 ;

            build( treeRoot ) ;
        }

        int get_par( int u )
        {
            int rr = root[u];
            if ( rr <= 0 || rr >= (int)par.size() ) return -1 ;
            if ( par[ rr ] == 0 ) return -1 ;
            return par[ rr ] ;
        }

        // kth ancestor: move u up by k steps. returns -1 if not exist
        int kth_ancestor(int u, int k) {
            if (u <= 0) return -1;
            for ( int i = 0 ; i <= LOG && u ; ++i ) {
                if ( k & (1 << i) ) u = up[i][u];
            }
            if (u == 0) return -1;
            return u;
        }

        // lca public
        int lca(int u, int v) {
            if (u == 0 || v == 0) return u ^ v;
            if (dep[u] < dep[v]) swap(u,v);
            int diff = dep[u] - dep[v];
            for (int i = 0; i <= LOG; ++i)
                if (diff & (1 << i)) u = up[i][u];
            if (u == v) return u;
            for (int i = LOG; i >= 0; --i)
                if (up[i][u] != up[i][v]) {
                    u = up[i][u];
                    v = up[i][v];
                }
            return par[u];
        }

        // distance between u and v
        int dist(int u, int v) {
            int w = lca(u,v);
            if (w == 0) return 0;
            return dep[u] + dep[v] - 2*dep[w];
        }

        // return the k-th node on path u->v (0-based). If k==0 returns u. If k==dist(u,v) returns v.
        // returns -1 on invalid
        int get_kth_on_path(int u, int v, int k) {
            int w = lca(u,v);
            int du = dep[u] - dep[w];
            if (k <= du) {
                // move up from u by k
                int ans = kth_ancestor(u, k);
                if (ans == -1) return -1;
                return ans;
            } else {
                int rem = k - du; // steps down from w toward v
                int total_down = dep[v] - dep[w];
                int steps_up_from_v = total_down - rem;
                if (steps_up_from_v < 0) return -1;
                int ans = kth_ancestor(v, steps_up_from_v);
                if (ans == -1) return -1;
                return ans;
            }
        }

        // update value of edge u-v (mark path)
        void update ( int u_q , int v_q )
        {
            for ( const auto & [ u , v ] : queryPath ( u_q , v_q ) ) 
                RangeDS.update ( u , v , 1 ) ;
        }

        // query value
        T query ( int u_q , int v_q ) 
        {
            T ans = 0 ;
            for ( const auto & [ u , v ] : queryPath ( u_q , v_q ) ) ans += RangeDS.query ( u , v ) ;
            return ans ;
        }
} ;