#include <bits/stdc++.h>
using namespace std;


// https://cp-algorithms.com/string/lyndon_factorization.html
string get_max_suffix = ( string & ss )
{
    int n = ss.size() ;
    int idx = 0 , j = 1 , k = 0 ;
    
    while ( j + k < n ) 
    {
        if ( ss[ idx + k ] == ss[ j + k ] ) k ++ ; 
        else if ( ss[ idx + k ] < ss[ j + k ] )
        {
            idx = max( idx + k + 1 , j ) ;
            j = idx + 1 ;
            k = 0 ;
        }
        else 
        {
            j = j + k + 1 ;
            k = 0 ;
        }
    }

    return ss.substr( idx ) ;
} ;