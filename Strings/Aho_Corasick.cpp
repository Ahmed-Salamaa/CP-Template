#include <bits/stdc++.h>
#define ll long long
#define ull unsigned long long
#define ld long double
#define int long long
#define nl "\n"
#define oo 1e9 + 1
#define OO 1e18 + 1
#define sp ' '
#define sz(x) (int)(x.size())
#define MOD 1000000007
#define fixed(n) fixed << setprecision(n)
#define sub_mod(a, b, m) ((((a) % m) - ((b) % m) + m) % m)
#define add_mod(a, b, m) ((((a) % m) + ((b) % m)) % m)
#define mult_mod(a, b, m) ((((a) % m) * ((b) % m)) % m)
#define EPS 1e-9
#define PI acos(-1)
using namespace std;




class AhoCorasick
{
    private :

        struct Node
        {
            static int nextNodeId ;
            static const int maxClild = 26 + 26 + 10 + 1 ;

            static int getIdx ( char c )
            {
                int offSet = 0 ;
                
                if ( islower(c) ) return c - 'a' + offSet ;
                offSet += 26 ;

                if ( isupper(c) ) return c - 'A' + offSet ;
                offSet += 26 ;

                if ( isdigit(c) ) return c - '0' + offSet ;
                offSet += 10 ;

                // extra

                if ( c == ' ' ) return offSet ;
                offSet ++ ;

                // Invaild -> to get the of size the range 
                return offSet ;
            }
            
            Node *fail ;
            Node *child[ maxClild ] ;

            vector <int> patIdx ;
            vector <char> chars ;
            
            int id ;
            
            Node() : id ( nextNodeId ++ ) , fail (0)
            {
                memset(child, 0, sizeof(child));
            }
        };

        Node *root ;
        const vector<string> & patterns ;
        const int sizeOfPatterns ;

        void insert( const string & s , int idx )
        {
            Node *cur = root ;
            for ( int i = 0 ; i < sz(s) ; i ++ )
            {
                int c = Node::getIdx( s[i] ) ;
                
                if( !cur->child[c] )
                {
                    cur->child[c] = new Node() ;
                    cur->chars.push_back(c) ;
                }
                cur = cur->child[c] ;
            }
            cur->patIdx.push_back(idx) ;
        }

        void build()
        {
            queue <Node*> q ;

            for ( int i = 0 ; i < Node::maxClild ; i ++ )
            {
                if( root->child[i] )
                {
                    root->child[i]->fail = root ;
                    q.push(root->child[i]) ;
                }
                else root->child[i] = root ;
            }
            
            while( !q.empty() )
            {
                Node *cur = q.front() ; q.pop() ;

                for ( int i = 0 ; i < sz(cur->chars) ; i ++ )
                {
                    int c = cur->chars[i] ;

                    Node *next = cur->child[c];
                    Node *fail = cur->fail;
                    
                    while ( fail != root && !fail->child[c] ) fail = fail->fail ;

                    if ( fail->child[c] ) next->fail = fail->child[c];
                    else next->fail = root ;
                    
                    for( int j = 0 ; j < sz( next->fail->patIdx ) ; j ++ )
                    {
                        next->patIdx.push_back(next->fail->patIdx[j]);
                    }

                    q.push(next) ;
                }
            }
        }

    public :

        AhoCorasick( const vector<string> & patterns ) 
            : root( new Node() ) , patterns ( patterns ) , sizeOfPatterns ( sz(patterns) )
        {
            for ( int i = 0; i < sz(patterns); i++)
            {
                insert(patterns[i], i);
            }

            build() ;
        }

        vector < vector < int > > search ( const string &s )
        {
            vector < vector < int > > res(sz(s)) ; // at Si which patterns matched
            Node *cur = root ;

            for ( int i = 0 ; i < sz(s) ; i ++ )
            {
                int c = s[i] - 'a' ;

                while ( cur != root && !cur->child[c] ) cur = cur->fail ;
                
                if( cur->child[c] ) cur = cur->child[c] ;
                
                for ( int j = 0 ; j < sz(cur->patIdx) ; j ++ )
                {
                    // i -> idx at string s
                    // cur->patIdx[j] idx for matched pattern

                    res[i].push_back(cur->patIdx[j]);
                }
            }

            return res ;
        }
};

int AhoCorasick::Node::nextNodeId = 0 ;


