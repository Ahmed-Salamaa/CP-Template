#include <bits/stdc++.h>
#define ll long long
using namespace std;


template < typename T = int >
class MO 
{
    private :
        const static int Block_Size = 450 ;

        static int64_t gilbertOrder(int x, int y, int pow, int rotate) 
        {
            if (pow == 0) {
                return 0;
            }
            int hpow = 1 << (pow-1);
            int seg = (x < hpow) ? (
                (y < hpow) ? 0 : 3
            ) : (
                (y < hpow) ? 1 : 2
            );
            seg = (seg + rotate) & 3;
            const int rotateDelta[4] = {3, 0, 0, 1};
            int nx = x & (x ^ hpow), ny = y & (y ^ hpow);
            int nrot = (rotate + rotateDelta[seg]) & 3;
            int64_t subSquareSize = int64_t(1) << (2*pow - 2);
            int64_t ans = seg * subSquareSize;
            int64_t add = gilbertOrder(nx, ny, pow-1, nrot);
            ans += (seg == 1 || seg == 2) ? add : (subSquareSize - add - 1);
            return ans;
        }

        struct node 
        {
            int l , r , idx ;
            long long order ;

            node () {} ;
            node ( int l , int r , int idx , int HilbertPow ) 
                : l ( l ) 
                , r ( r ) 
                , idx  ( idx ) 
                , order ( gilbertOrder ( l , r , HilbertPow , 0 ) ) {} ;
            
            bool operator < ( const node & other ) const 
            {
                // if ( l / Block_Size != other.l / Block_Size ) return l / Block_Size < other.l / Block_Size ;
                // return r < other.r ;

                return order < other.order ;
            }
            
        };

        int calculateHilbertPow() const 
        {
            int pow = 1;
            while ((1 << pow) < n) pow++;
            return pow;
        }
        
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

        MO ( const vector <T> & arr , int q ) : arr ( arr ) , n ( arr.size() - 1 ) , ans (0) 
        {
            query.resize( q ) ;
            answer.resize( q ) ;

            // int HilbertPow = calculateHilbertPow() ;
            int HilbertPow = 21 ;

            for ( int i = 0 ; i < q ; i ++ )
            {
                int l , r ; cin >> l >> r ;
                if ( r < l ) swap ( l , r ) ;
                
                query[i] = node ( l , r , i , HilbertPow ) ;
            }

            sort ( all ( query ) ) ;
            init () ;
        }

        void procces() 
        {
            int left = 1 , right = 0 ;

            for ( const auto & [ l , r , idx , _ ] : query ) 
            {
                while ( left > l ) add ( -- left ) ;    
                while ( right < r ) add ( ++ right ) ;
                while ( left < l ) remove ( left ++ ) ; 
                while ( right > r ) remove( right -- ) ;

                answer[idx] = ans ;
            }
        }

        void print ()
        {
            for ( const auto & it : answer )
                cout << it << "\n" ;
        }
} ;






