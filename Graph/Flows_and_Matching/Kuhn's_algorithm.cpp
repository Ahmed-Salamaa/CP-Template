#include <bits/stdc++.h>
using namespace std;

// O( n * m )
int n, m, k ;
vector<int> adj[505] ;
int match[505] ;
bool used[505] = {} ;

bool Try_Kuhn( int u )
{
      if ( used[u] )
            return false ;

      used[u] = true ;
      for ( int v : adj[u] )
      {
            if ( match[v] == -1 || Try_Kuhn( match[v] ) )
            {
                  match[v] = u ;
                  return true ;
            }
      }
      return false ;
}
void solve()
{
      cin >> n >> m >> k ;
      memset( match, -1, sizeof match ) ;
      while ( k-- )
      {
            int u, v ;
            cin >> u >> v ;
            adj[u].push_back( v ) ;
      }

      int res = 0 ;
      for ( int i = 1 ; i <= n ; i++ )
      {
            for ( int j = 0 ; j < 505 ; j++ )
                  used[j] = false ;
            if ( Try_Kuhn( i ) )
                  res++ ;
      }

      cout << res << '\n' ; // Maximum matching
      for ( int i = 1 ; i <= m ; i++ )
      {
            if ( match[i] != -1 )
            {
                  cout << match[i] << " " << i << '\n' ;
            }
      }
}
int main()
{
      ios_base::sync_with_stdio(false), cin.tie(nullptr), cout.tie(nullptr);
      solve() ;
}