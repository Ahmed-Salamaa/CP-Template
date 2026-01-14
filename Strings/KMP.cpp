#include <bits/stdc++.h>
using namespace std;
#define sz(x) (int)(x.size())



struct KMP {
    string pattern;
    vector<int> lp;
    vector < vector <int> > failureTable ;

    KMP(const string &str = "") : pattern(str) { build(); }

    void build() {
        int n = sz(pattern);
        lp.resize(n);
        for (int i = 1; i < n; i++) lp[i] = failure(lp[i - 1], pattern[i]);
    }

    int failure(int idx, char nxt) {
        while (idx > 0 && pattern[idx] != nxt) idx = lp[idx - 1];
        return idx + (pattern[idx] == nxt);
    }

    void failure_Table_build ()
    {
        int n = pattern.size() ;

        failureTable.assign( n + 1 , vector <int> ( 26 ) ) ;

        for ( int i = 0 ; i <= n ; i ++ )
        {
            for ( char c = 'a' ; c <= 'z' ; c ++ )
            {
                if (i > 0 && c != pattern[i])
                    failureTable[i][c-'a'] = failureTable[lp[i-1]][c-'a'];
                else
                    failureTable[i][c-'a'] = i + (c == pattern[i]);    
            }
        }   
    }

    // get next fail idx if I am at character `c` , thats verv fast if you use dp
    int get_next_fail_idx ( int fail , char c )
    {
        int new_fail = failureTable[fail][ c - 'a' ] ;
        return new_fail ;

    } ;

    // used in dp
    // auto calc = [&]( char c )
    // {
    //     int new_fail = kmp[fail][ c - 'a' ] ;
    //     int add = ( new_fail == n ? 1 : 0 ) ;
    //     return add + last_dp[new_fail == n ? kmp[ n - 1 ][ c - 'a' ] : new_fail] ;
    // } ;


    vector<int> match(const string &str) {
        int n = sz(str), m = sz(pattern);
        vector<int> ret;
        for (int i = 0, k = 0; i < n; i++) {
            k = failure(k, str[i]);
            if (k == m) ret.push_back(i - m + 1); // 0-based indexing
        }

        for (auto &idx : ret) idx++; // 1-based indexing

        return ret;
    }

    vector<int> count(const string &str) { // count number of occurrences of each prefix
        int n = sz(str), m = sz(pattern);
        vector<int> cnt(m + 1);
        for (int i = 1, k = 0; i < n; i++) {
            k = failure(k, str[i]);
            cnt[k]++;
        }

        for (int i = m - 1; i > 0; i--)
            cnt[lp[i - 1]] += cnt[i]; // number of occurrences of prefix of length lp[i - 1] is at least cnt[i]

        for (int i = 0; i <= m; i++) cnt[i]++; // add the whole substring itself

        return cnt;
    }

    vector<int> get_prefixes() {
        vector<int> indexes = {sz(pattern)};
        for (int i = lp[sz(pattern) - 1]; i > 0; i = lp[i - 1]) indexes.push_back(i);
        sort(indexes.begin(), indexes.end()); // indexes of prefixes increasing
        return indexes;
    }
};




