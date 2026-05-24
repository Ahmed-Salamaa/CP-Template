#include <bits/stdc++.h>
using namespace std;

using ll = long long;


// solve dp[ i , j ] = min ( dp[ i - 1 , k - 1 ] + C( k , j ) )
//            { 0 ⪯ k ⪯ j }


// where cost( k , j ) is quadrangle inequality "wider is worse"
// which means C( a , c ) + C( b , d ) ⪯ C( a , d ) +C( b , c )
//       for a ≤ b ≤ c ≤ d
  

int m , n ;
vector <int> dp_before , dp_curr ;


// implement your cost function , i , r is zero based
int cost( int l , int r )
{
    l ++ ; r ++ ;
}

// compute dp_curr[l], ... dp_curr[r] (inclusive)
void compute ( int l , int r , int optl , int optr )
{
    if ( l > r ) return ;

    int mid = ( l + r ) >> 1 ;
    pair < int , int > best = { LLONG_MAX , -1 } ; // change it to LLONG_MIN if needed

    for ( int k = optl ; k <= min ( mid , optr ) ; k ++ ) 
    {
        pair < int , int > curr = { ( k ? dp_before[ k - 1 ] : 0 ) + cost( k , mid ) , k } ;
        best = min( best , curr ) ; // change it to max if needed
    }

    dp_curr[mid] = best.first ;
    int opt = best.second ;

    compute( l , mid - 1 , optl , opt ) ;
    compute( mid + 1 , r , opt , optr ) ;
}

int solve() 
{
    dp_before.assign(n ,0 ) ;
    dp_curr.assign( n ,0 ) ;

    for ( int i = 0 ; i < n ; i ++ )
        dp_before[i] = cost ( 0 , i ) ;

    for ( int i = 1 ; i < m ; i ++ )
    {
        compute(0 , n - 1 , 0 , n - 1 ) ;
        swap ( dp_before , dp_curr ) ;
    }

    return dp_before[ n - 1 ] ;
}