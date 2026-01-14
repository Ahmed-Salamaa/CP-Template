// وَأَن لَّيْسَ لِلْإِنسَانِ إِلَّا مَا سَعَىٰ (39) وَأَنَّ سَعْيَهُ سَوْفَ يُرَىٰ (40) ثُمَّ يُجْزَاهُ الْجَزَاءَ الْأَوْفَىٰ (41)
#include <bits/stdc++.h>
using namespace std;

    const int dx[4] = { 1 , -1 , 0 , 0 } ;
    const int dy[4] = { 0 , 0 , 1 , -1 } ;
    long double PI = acosl(-1.0L) ;
    const int MOD = 1e9 + 7 ;
    const long long oo = 1e18 ;

#define ll long long
#define int long long
#define ld long double
#define popcnt(x) __builtin_popcountll( x )
#define all(vec) vec.begin(), vec.end()
#define sz(x) (int)x.size()
#define sub_mod(a, b) ((((a) % MOD) - ((b) % MOD) + MOD) % MOD)
#define add_mod(a, b) ((((a) % MOD) + ((b) % MOD)) % MOD)
#define mult_mod(a, b) ((((a) % MOD) * ((b) % MOD)) % MOD)


template < typename t = int >
class SQRT
{
    private:

        int block , n ;
        int MAX_Block ;

        t def ;

        vector <t> arr , blockAnswer ;

        t operation ( const t & curr1 , const t & curr2 )
        {
            // add operation later
        }

        void rebuild ( int b )
        {
            int l = b * block ;
            int r = min ( n - 1 , ( b + 1 ) * block - 1 ) ;

            if ( l >= n ) return ; 

            blockAnswer[b] = def ;

            for ( int i = l ; i <= r ; i ++ )
                blockAnswer[b] = operation( blockAnswer[b] , arr[i] ) ;
        }

        void init ()
        {
            for ( int i = 0 ; i < MAX_Block ; i ++ )
                rebuild( i ) ;
        }

    public:

        SQRT ( vector <t> arr ) : arr ( arr ) 
        {
            def = 0 ;
            n = ( sz ( arr ) ) ;
            block = ( sqrt ( n + 5 ) ) ;
            MAX_Block = ( n / block + 5 ) ;
            blockAnswer.assign( MAX_Block , def ) ;
            init () ;
        }

        SQRT ( int n ) : n ( n ) 
        {
            def = 0 ;
            arr = vector <int> ( n + 5 , def ) ;
            block = ( sqrt ( n + 5 ) ) ;
            MAX_Block = ( n / block + 5 ) ;
            blockAnswer.assign( MAX_Block , def ) ;
            init () ;
        }

        t query ( int l , int r )  // O ( sqrt n )
        {
            t idx = 0 , ans = def ;

            int bl = l / block ;
            int br = r / block ;
        
            if ( bl == br )
            {
                for ( int i = l ; i <= r ; i ++ )
                {
                    ans = operation ( ans , arr[i] ) ;
                }
                return ans ;
            }
        
            int endL = ( bl + 1 ) * block - 1 ;
            for ( int i = l ; i <= endL ; i ++ )
            {
                ans = operation ( ans , arr[i] ) ;
            }
        
            int startR = br * block ;
            for (int i = startR; i <= r; i++)
            {
                ans = operation ( ans , arr[i] ) ;
            }
        
            for ( int b = bl + 1 ; b <= br - 1 ; b ++ )
                ans = operation ( ans , blockAnswer[b] ) ;
                
            return ans ;
        } ;

        void update ( int l , int r , t val ) // O ( n ) take care
        {
            int bl = l / block ;
            int br = r / block ;

            if ( bl == br )
            {
                for ( int i = l ; i <= r ; i ++ )
                    arr[i] = operation(arr[i], val);
                rebuild(bl);
                return;
            }

            int endL = (bl + 1) * block - 1;
            for (int i = l; i <= endL; i++)
                arr[i] = operation(arr[i], val);
            rebuild(bl);

            int startR = br * block;
            for (int i = startR; i <= r; i++)
                arr[i] = operation(arr[i], val);
            rebuild(br);

            for (int b = bl + 1; b <= br - 1; b++)
            {
                int L = b * block;
                int R = min(n - 1 , (b + 1) * block - 1);
                for (int i = L; i <= R; i++)
                    arr[i] = operation(arr[i], val);
                rebuild(b);
            }
        }

        void update ( int idx , t val ) // O ( sqrt n )
        {
            update ( idx , idx , val ) ;
        }
};


