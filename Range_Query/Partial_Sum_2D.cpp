// وَأَن لَّيْسَ لِلْإِنسَانِ إِلَّا مَا سَعَىٰ (39) وَأَنَّ سَعْيَهُ سَوْفَ يُرَىٰ (40) ثُمَّ يُجْزَاهُ الْجَزَاءَ الْأَوْفَىٰ (41)
#include <bits/stdc++.h>
#include <chrono>
using namespace std::chrono;
using namespace std;

    const int dx[4] = { 1 , -1 , 0 , 0 } ;
    const int dy[4] = { 0 , 0 , 1 , -1 } ;
    long double PI = acosl(-1.0L) ;
    const int MOD = 1e9 + 7 ;
    const int oo = 0x3f3f3f3f ;

#define ll long long
#define int long long
#define ld long double
#define popcnt(x) __builtin_popcountll( x )
#define all(vec) vec.begin(), vec.end()
#define sz(x) (int)x.size()
#define sub_mod(a, b) ((((a) % MOD) - ((b) % MOD) + MOD) % MOD)
#define add_mod(a, b) ((((a) % MOD) + ((b) % MOD)) % MOD)
#define mult_mod(a, b) ((((a) % MOD) * ((b) % MOD)) % MOD)

#ifndef ONLINE_JUDGE
auto start = high_resolution_clock::now();
#endif

template < typename t1 , typename t2 > ostream& operator << (ostream &out , const pair < t1 , t2 > & p ){
out << p.first << ' ' << p.second ;
return out ;
}
template < typename t1 = int , typename t2 = int > istream& operator >> (istream &in , pair < t1 , t2 > & p) {
in >> p.first >> p.second ;
return in ;
}
template < typename T = int > istream& operator >> (istream &in, vector < T > &v) {
    for (auto &x : v) in >> x;
    return in;
}
template < typename T = int > istream& operator >> (istream &in, deque < T > &v) {
    for (auto &x : v) in >> x;
    return in;
}

template < typename T = int > ostream& operator << (ostream &out, const vector < T > &v) { 
    for (const T &x : v) out << x << ' '; 
    return out;
}
template < typename T = int > ostream& operator << (ostream &out, const deque < T > &v) { 
    for (const T &x : v) out << x << ' '; 
    return out;
}

void end()
{
    #ifndef ONLINE_JUDGE   
    auto stop = high_resolution_clock::now();
    auto duration = duration_cast<microseconds>(stop - start);
    cout << "\n\nTime Complexity is :\n O( 10 power " << ( ceil(log10( duration.count() )) ) << " )" ;
    #endif
}

void ultimate_tips(){
    #ifndef ONLINE_JUDGE
    freopen("input.txt", "r", stdin);
    freopen("output.txt", "w", stdout);
    #endif
    ios_base::sync_with_stdio(false);
    cin.tie(nullptr);
}

struct Hash_2D {

  int n, m;
  static bool done;
  vector < int > PWX, PWY;
  vector < vector < int > > hs;
  static constexpr int PX = 3731, PY = 2999, mod = 998244353;

  ll add(ll a, ll b) { return (a + b) % mod; }
  ll sub(ll a, ll b) { return (a - b + 2 * mod) % mod; }
  ll mul(ll a, ll b) { return ((a % mod) * (b % mod)) % mod; }
  
  Hash_2D() = default;

  template < typename T > const inline void calc(const T & s){ // s = {string, vector}

    for (int i = 0; i < n; i++) PWX[i + 1] = mul(PWX[i], PX);
    for (int i = 0; i < m; i++) PWY[i + 1] = mul(PWY[i], PY);

    for (int i = 0; i < n; i++) 
      for (int j = 0; j < m; j++) 
        hs[i + 1][j + 1] = s[i][j];

    for (int i = 0; i <= n; i++) 
      for (int j = 0; j < m; j++) 
        hs[i][j + 1] = add(hs[i][j + 1], mul(hs[i][j], PY));

    for (int i = 0; i < n; i++) 
      for (int j = 0; j <= m; j++) 
        hs[i + 1][j] = add(hs[i + 1][j], mul(hs[i][j], PX));
  }

  template < typename T > Hash_2D(const T & s) {
    n = (int)s.size();
    m = (int)s[0].size();
    
    PWX.assign(n + 1, 1);
    PWY.assign(m + 1, 1);
    hs.assign(n + 1, vector < int > (m + 1, 0));
   
    calc(s);
  }

  const inline int get_hash() {
    return get_hash(1, 1, n, m);
  }
  
  // 1 - indexed
  const inline int get_hash(int x1, int y1, int x2, int y2) { 
    // check the range 
    assert(1 <= x1 && x1 <= x2 && x2 <= n);
    assert(1 <= y1 && y1 <= y2 && y2 <= m);
    
    x1--, y1--;
    int dx = x2 - x1, dy = y2 - y1;
    ll term1 = sub(hs[x2][y2], mul(hs[x2][y1], PWY[dy]));
    ll term2 = sub(hs[x1][y2], mul(hs[x1][y1], PWY[dy]));
    return sub(term1, mul(term2, PWX[dx]));
  }


};


template < typename T = int > struct Partial_2D {

    vector < vector < T > > partial;
    int n, m;

    Partial_2D(int N, int M){
        n = N, m = M;
        partial.assign(n + 5, vector < T > (m + 5));
    }

    void query (int x1, int y1, int x2, int y2)
    {
            int k = 1;
            if(x1 > x2) swap(x1, x2);
            if(y1 > y2) swap(y1, y2);
            partial[x2][y2] += k, partial[x2][y1 - 1] -= k; 
            partial[x1 - 1][y2] -= k, partial[x1 - 1][y1 - 1] += k;
        
    }

    void init ()
    {
        for(int i = n; i >= 0; i--)
            for(int j = m; j >= 0; j--)
                partial[i][j] += partial[i][j + 1];
        for(int i = n; i >= 0; i--)
            for(int j = m; j >= 0; j--)
                partial[i][j] += partial[i + 1][j];
    }

    T get(int x, int y){
        return partial[x][y];
    }

    void print(){
        for(int i = 1; i <= n; i++, cout << "\n")
            for(int j = 1; j <= m && cout << partial[i][j] << " "; j++);
    }
};

void solve ()
{

    int x , y ; cin >> x >> y ;
    vector <string> pattern ( x ) ; cin >> pattern ;

    int n , m ; cin >> n >> m ;
    vector <string> grid ( n ) ; cin >> grid ;

    Hash_2D p ( pattern ) ;

    int key = p.get_hash() ;

    p = Hash_2D ( grid ) ;

    Partial_2D ds ( n + 5 , m + 5 ) ;


    for ( int i = x ; i <= n ; i ++ )
    {
        for ( int k = y ; k <= m ; k ++ )
        {
            if ( key == p.get_hash( i - x + 1 , k - y + 1 , i , k ) )
            {
                ds.query( i - x + 1 , k - y + 1 , i , k ) ;
            }
        }
    }

    ds.init() ;

    for ( int i = 1 ; i <= n ; i ++ )
    {
        for ( int k = 1 ; k <= m ; k ++ )
        {
            if ( ds.get( i , k ) ) cout << grid[ i - 1 ][ k - 1 ] ;
            else cout << '.' ;
        }
        cout << "\n" ;
    }
    
}

signed main() {
    ultimate_tips();

    solve () ;


    // end() ;
    return 0;
}
