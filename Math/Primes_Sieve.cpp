#include <bits/stdc++.h>
using namespace std;

#define ll long long
#define endl "\n"
#define sz(x) (int)x.size()
#define all(x) x.begin(), x.end()

// O(sqrt(n))
bool isprime(ll x) {
    if (x <= 1) return false;
    if (x == 2 || x == 3) return true;
    if (x % 2 == 0 || x % 3 == 0) return false;
    for (int i = 5; i * i <= x; i += 6)
        if (x % i == 0 || x % (i + 2) == 0) return false;
    return true;
}

// O(MAXV)
    const int  MAXV = 1e7 + 5 ;
    vector <int> spf(MAXV) , primes ;

void sieve()
{
    for (int i = 1; i < MAXV; i++) spf[i] = i ;

    for (int i = 4; i < MAXV; i += 2) spf[i] = 2 ;

    for (int i = 3; i * i < MAXV; i += 2) 
        if (spf[i] == i)  
            for (int j = i * i; j < MAXV; j += i) 
                if (spf[j] == j) spf[j] = i ;

    for ( int i = 2 ; i < MAXV ; i ++ )
        if ( spf[i] == i )
            primes.push_back( i ) ;
}

// O( log(MAXV) )
bool isPrime ( const int & n )
{
    return binary_search( primes.begin() , primes.end() , n ) ;
} 
