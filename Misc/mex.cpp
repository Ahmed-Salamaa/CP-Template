#include <bits/stdc++.h>
#define ll long long
using namespace std;

template <typename T = int, const int N = 100005>
class MEX
{
    public:
        
        MEX() 
        {
            init();
        }
        
        bitset <N> mex ;
        vector <int> cnt ;
        
        inline void init()
        {
            cnt.assign ( N , 0 ) ;

            for ( int i = 0 ; i < N ; i ++ )
                mex[i] = 1;
        }

        inline void add(const T x)
        {
            insert(x);
        }
        
        inline void remove(const T x)
        {
            erase(x);
        }
        
        inline int get_mex()
        {
            return calculate_mex();
        }
 
    private:
        inline void insert(const T &x)
        {
            if (x >= N)
                return;
            cnt[x]++;
            if (cnt[x] == 1)
                mex[x] = 0;
        }
        
        inline void erase(const T &x)
        {
            if (x >= N)
                return;
            cnt[x]--;
            if (cnt[x] == 0)
                mex[x] = 1;
        }
        
        inline int calculate_mex()
        {
            return mex._Find_first();
        }
};