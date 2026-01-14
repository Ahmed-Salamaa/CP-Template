#include <bits/stdc++.h>
#define ll long long
using namespace std;



template < typename T = int >
class MO 
{
    private :

        const int Block_Size = 450 ;

        struct node 
        {
            int l , r , idx ;

            node () {} ;
            node ( int l , int r , int idx ) : l ( l ) , r ( r ) , idx  ( idx ) {} ;
            
            bool operator < ( const node & other ) const 
            {
                if ( l / Block_Size != other.l / Block_Size ) return l / Block_Size < other.l / Block_Size ;
                return r < other.r ;
            }
            
        };
        
        const vector <T> & arr ;
        vector <node> query ;
        vector <int> answer ;
        T ans ;

        int n ;

        void init ()
        {
            // add the function
        }

        void add ( int idx ) 
        {
            // add the function
        }

        void remove ( int idx ) 
        {
            // add the function
        }

    public :

        MO ( vector <T> arr , int q ) : arr ( arr ) , n ( arr.size() - 1 ) , ans (0) 
        {
            query.resize( q ) ;
            answer.resize( q ) ;

            for ( int i = 0 ; i < q ; i ++ )
            {
                int l , r ; cin >> l >> r ;
                if ( r < l ) swap ( l , r ) ;
                
                query[i] = { l , r , i } ;
            }

            sort ( all ( query ) ) ;
            init () ;
        }

        void procces() 
        {
            int left = 1 , right = 0 ;

            for ( const auto & [ l , r , idx ] : query ) 
            {
                while ( left < l ) remove ( left ++ ) ;
                while ( left > l ) add ( -- left ) ;
                while ( right < r ) add ( ++ right ) ;
                while ( right > r ) remove( right -- ) ;

                ans[idx] = ans ;
            }
        }

        void print ()
        {
            for ( const auto & it : answer )
                cout << it << "\n" ;
        }
} ;






