template < typename T = int >
class Segment_tree
{
    #define mid ( start + (end - start) / 2 )
    #define calclute( idx ) arr[idx] = calc ( arr [2*idx] , arr [2*idx+1] ) ;
    #define range_left  curr * 2 , start , mid
    #define range_right curr * 2 + 1 , mid + 1 , end
    #define range_search left , right

    private:

        struct node
        {
            T val ;

            node operator = ( const T rhs ) 
            {
                val = rhs ;
                return *this ;
            }
        } ;
        
        vector < node > arr ;
        node def = {(T)(0)} ;  // change default 

        T n = 1 ;

        node calc ( node curr1 , node curr2 ) // change operation 
        {
            node ans ;
            ans = curr1.val + curr2.val ;
            return ans ;
        }

        void build ( int curr , int start , int end , const vector <T> & numbers )
        {
            if ( start == end ) { arr[curr] = numbers[start-1] ; return ; }

            build ( range_left , numbers ) ;
            build ( range_right , numbers ) ;
            
            calclute( curr ) ;
        }

        void build ( int curr , int start , int end , int val )
        {
            if ( start == end )  { arr[curr] = val ; return ; }

            build ( range_left , val ) ;
            build ( range_right , val ) ;
            
            calclute( curr ) ;
        }
 
        node query ( int curr , int start , int end , int left , int right )
        {
            if ( start > right || end < left ) return def ;
            if ( left <= start && end <= right ) return arr[curr] ;
            if ( start == end ) return arr[curr] ;

            return calc ( query ( range_left , left , right ) , query ( range_right , left , right ) ) ;
        }
        
        void update ( int curr , int start , int end , int idx , int val )
        {
            if ( start == end ) { arr[curr] = val ; return ; }

            if ( idx <= mid ) update ( range_left , idx , val ) ;
            else update ( range_right , idx , val ) ;

            calclute( curr ) ;
        }

    public:

        Segment_tree ( const vector <T> & numbers )
        {
            n = numbers.size() ;

            int size = 1 ;
            while ( size < n ) size *= 2 ;
            arr.assign ( 2 * size , def ) ;

            build ( 1 , 1 , n , numbers ) ;
        }

        Segment_tree ( int tt , int val = MOD )
        {
            n = tt ;

            int size = 1 ;
            while ( size < n ) size *= 2 ;
            arr.assign ( 2 * size , def ) ;
            if ( val == MOD ) val = def.val ;

            build ( 1 , 1 , n , val ) ;
        }

        T query ( int left , int right )
        { 
            return query ( 1 , 1 , n , left , right ).val ;
        }

        void update ( int idx , int val ) { update ( 1 , 1 , n , idx , val ) ; } 

        friend ostream& operator << ( ostream &out, const node &curr ) // change printing 
        {
            out << curr.val ;
            return out ;
        }

    
    #undef mid
    #undef calclute
    #undef range_left
    #undef range_right
    #undef range_search
};
/*
    // find k‑th c in [L,R], or –1
    int kth(int L, int R, char c, int k)
    {
        return kth_rec(L, R, c, k, 1, 1, size);
    }

    int kth_rec(int L, int R, char c, int k,
                int idx, int lx, int rx)
    {
        // no overlap
        if (lx > R || rx < L)
            return -1;

        // fully inside [L,R], maybe finish here
        if (lx >= L && rx <= R)
        {
            if (tree[idx].val < k)
                return -1;
            if (lx == rx)
                return lx;
        }

        int mid = (lx + rx) / 2;
        // count in left child ∩ [L,R]
        Node leftNode = query(L, R, 2 * idx, lx, mid);
        int leftCount = leftNode.val;

        if (leftCount >= k)
            return kth_rec(L, R, c, k, 2 * idx, lx, mid);
        else
            return kth_rec(L, R, c, k - leftCount, 2 * idx + 1, mid + 1, rx);
    }
*/