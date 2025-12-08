template < typename T = int >
class Segment_tree
{

    #define mid ( start + (end - start) / 2 )
    #define calclute( idx ) arr[idx] = calc ( arr [2*idx] , arr [2*idx+1] ) ;
    #define calclute_propagate( idx ) propagate ( idx , start , end ) ;
    #define range_left  curr * 2 , start , mid
    #define range_right curr * 2 + 1 , mid + 1 , end
    #define range_search left , right
    
    private:

        struct node
        {
            T val , lazy ;
            bool is_lazy ;

            node operator = ( const T rhs ) 
            {
                val = rhs ;
                return *this ;
            }
        } ;
        
        vector < node > arr ;
        node def = { (T)0 , (T)1 , false } ;  // change default 

        T n = 1 ;

        node calc ( node curr1 , node curr2 ) // change operation 
        {
            node ans ;
            
            ans.val = ( curr1.val + curr2.val )  ;

            ans.lazy = def.lazy ;
            ans.is_lazy = false ;
            return ans ;
        }

        void calc_lazy ( int curr , int start , int end , T val )
        {
            arr[curr].val = ( arr[curr].val + val ) ;
            arr[curr].lazy = ( arr[curr].lazy + val ) ;
            arr[curr].is_lazy = true ;
        }

        void propagate( int curr , int start , int end )
        {
            if ( !arr[curr].is_lazy ) return ;
            
            T pending = arr[curr].lazy ;
            
            if ( start != end ) // push down to children
            {
                calc_lazy ( range_left , pending ) ;
                calc_lazy ( range_right , pending ) ;
            }

            // clear current node's lazy
            arr[curr].lazy = def.lazy ;
            arr[curr].is_lazy = false ;
        }


        void build ( int curr , int start , int end , const vector <T> & numbers )
        {
            if ( start == end ) { arr[curr] = numbers[start-1] ; return ; }
            
            build ( range_left , numbers ) ;
            build ( range_right , numbers ) ;

            calclute ( curr ) ;
        }

        void build ( int curr , int start , int end , int val )
        {
            if ( start == end )  { arr[curr] = val ; return ; }

            build ( range_left , val ) ;
            build ( range_right , val ) ;
            
            calclute ( curr ) ;
        }
 
        node query_range ( int curr , int start , int end , int left , int right )
        {
            calclute_propagate ( curr ) ;

            if ( start > right || end < left ) return def ;
            if ( left <= start && end <= right ) return arr[curr] ;

            return calc ( query_range ( range_left , range_search ) , query_range ( range_right , range_search ) ) ;
        }
        
        void update_range( int curr , int start , int end , int left , int right , T val )
        {
            calclute_propagate ( curr ) ;

            if ( start > right || end < left ) return ;

            if ( left <= start && end <= right )
            {
                calc_lazy( curr , start , end , val ) ;
                return ;
            }

            update_range ( range_left , range_search , val ) ;
            update_range ( range_right , range_search , val ) ;
            calclute ( curr ) ;
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

        node query_range ( int left , int right ) { return query_range ( 1 , 1 , n , range_search ) ; }
        void update_range ( int left , int right , int val ) { update_range ( 1 , 1 , n , range_search , val ) ; } 

        friend ostream& operator << ( ostream &out , const node &curr ) // change printing 
        {
            out << curr.val ;
            return out ;
        }

    #undef mid
    #undef calclute
    #undef calclute_propagate
    #undef range_left
    #undef range_right
    #undef range_search
};