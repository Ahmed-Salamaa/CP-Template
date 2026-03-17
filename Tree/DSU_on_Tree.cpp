#include <bits/stdc++.h>
using namespace std;

const int N = 1e5 + 9;
vector<int> g[N];
int ans[N], col[N], sz[N], cnt[N];
bool big[N];
void dfs(int u, int p) {
    sz[u] = 1;
    for (auto v : g[u]) {
        if (v == p) continue;
        dfs(v, u);
        sz[u] += sz[v];
    }
}
void add(int u, int p, int x) {
    cnt[col[u]] += x;
    for (auto v : g[u]) {
        if (v == p || big[v] == 1) continue;
        add(v, u, x);
    }
}
void dsu(int u, int p, bool keep) {
    int bigchild = -1, mx = -1;
    for (auto v : g[u]) {
        if (v == p) continue;
        if (sz[v] > mx) mx = sz[v], bigchild = v;
    }
    for (auto v : g[u]) {
        if (v == p || v == bigchild) continue;
        dsu(v, u, 0);
    }
    if (bigchild != -1) dsu(bigchild, u, 1), big[bigchild] = 1;
    add(u, p, 1);
    ans[u] = cnt[u];
    if (bigchild != -1) big[bigchild] = 0;
    if (keep == 0) add(u, p, -1);
}

template < typename t = int >
class DSU_Tree
{
    private:
 
        const vector < vector <int> > & graph ;
        vector <int> ans , sz ;
        const vector <t> & colors ;
        int n ;
 
        vector < map < int , int > * > cnt ;
        
        void init ( int node , int parent )
        {
            sz[node] = 1 ;  // every vertex has itself in its subtree
            for ( const auto & curr : graph[node] )
            {
                if ( curr == parent ) continue ;
                
                init ( curr , node ) ;
                sz[node] += sz[curr] ;
            }
        }
 
        void dfs ( int node , int parent )
        {
            int bigChild = n - 1 ; sz[bigChild] = -1 ;
 
            for ( const auto & curr : graph[node] )
            {
                if ( curr == parent ) continue ;
 
                dfs ( curr , node ) ;
 
                if ( sz[curr] > sz[bigChild] )
                    bigChild = curr ;
            }
            
            if ( bigChild != n - 1 ) cnt[node] = cnt[bigChild] ;
            else cnt[node] = new map < int , int > () ;
 
            ( *cnt[node] )[ colors[ node - 1 ] ] ++ ; // colors 0 - based
 
            for ( const auto & curr : graph[node] )
            {
                if ( curr == parent || curr == bigChild ) continue ;
                
                for ( const auto & it : ( * cnt [ curr ] ) )
                    ( * cnt [ node ] )[ it.first ] += it.second ;
 
                delete ( cnt [ curr ] ) ;
            }
 
            //now (*cnt[v])[c] is the number of vertices in subtree of vertex v that has color c. You can answer the queries easily.
 
            ans[node] = ( *cnt[node] ).size() ;
        }
    
    public:
 
        DSU_Tree ( const vector < vector <int> > & adj , const vector <t> & c , int root = 1 ) : graph(adj) , colors(c)
        {
            n = graph.size() + 5 ;
 
            ans = sz = vector <int> ( n + 5 ) ;
            cnt = vector < map < int , int > * > ( n + 5 ) ;
 
            init ( root , -1 ) ;
            dfs ( root , -1 ) ;
 
            delete cnt[ root ] ;
        }
 
        t query ( int node ) { return ans[node] ; }
 
};
