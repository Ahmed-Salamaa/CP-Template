template < typename T = int >
class Sparse_Table
{
    private:

        struct point
        {
            T num ;

            point operator = ( const T rhs ) 
            {
                num = rhs ;
                return *this ;
            }
        };
            
        int n , LOG = 1 ;
        point def = { (T)(1e18) } ;
        vector < vector < point > > table ;

        point calc ( point a , point b )
        {
            point ans ;
            ans.num = min ( a.num , b.num ) ;
            return ans ;
        }

        void build ()
        {
            for ( int bit = 1 ; bit < LOG ; bit ++ )
                for ( int idx = 1 ; idx + ( 1 << bit ) - 1  <= n ; idx ++ )
                    table [bit][idx] = calc ( table [bit-1][idx] , table [bit-1][idx+(1<<(bit-1))] ) ;
        }

    public:

        Sparse_Table ( const vector <T> & arr )
        {
            n = arr.size() ; LOG = __lg(n) + 1 ;
            
            table.assign( LOG + 1 , vector <point> ( n + 10 ) ) ;

            // Level 0: copy values, shift to 1-based
            for ( int i = 1 ; i <= n ; i ++ )  table[0][i] = arr[i - 1] ;

            // Build higher levels
            build () ;
        }

        void print ()
        {
            for ( int i = 0 ; i < n ; i ++ )
            {
                cout << "idx is " << i << " : " ;
                for ( int bit = 0 ; bit < LOG ; bit ++ ) cout << table [bit][i] << " " ;
                cout << "\n" ;
            }
        }

        point query_range_log ( int left , int right )
        {
            int idx = left , size = right - left + 1 ;
            if ( size <= 0 ) return def ;
            point ans = def ;

            // Process the range by selecting non-overlapping segments
            while ( size > 0 )
            {
                int length = 31 - __builtin_clz(size) ; // Largest power of 2 <= size
                ans = calc ( table[length][idx] , ans ) ;
                idx += ( 1 << length ) ;
                size -= ( 1 << length ) ;
            }
            return ans ;
        }

        point query_range_one ( int left , int right )
        {
            int size = right - left + 1 ;
            int bit = __lg ( size ) ;

            return calc ( table [bit][left] , table [bit][right-(1<<bit)+1] ) ;
        }

        friend ostream& operator << ( ostream &out , const point &curr ) // change printing 
        {
            out << curr.num ;
            return out ;
        }
};