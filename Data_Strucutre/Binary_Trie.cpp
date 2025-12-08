#include <bits/stdc++.h>

class BinaryTrie 
{
    struct Node 
    {
        Node * child[2] ;
        int freq ;
        
        Node() 
        {
            memset(child, 0, sizeof(child));
            freq = 0;
        }
    };

    Node *root ;
    int maxBit ;

    void erase(const int n, int idx, Node *curr)
    {
        if ( idx == -1 ) return ; 

        int bit = ( n >> idx ) & 1 ;

        erase ( n , idx - 1 , curr->child[bit] ) ;
        
        curr->child[bit]->freq -- ;

        if ( curr->child[bit]->freq == 0 ) 
        {
            delete curr->child[bit] ;
            curr->child[bit] = 0 ;
        }
    }

    public :

    BinaryTrie () : maxBit ( 40 ) , root ( new Node )
    {
        insert(0);
    }

    BinaryTrie ( const int maxBit ) : maxBit ( maxBit ) , root ( new Node )
    {
        insert(0);
    }

    void insert(const int n) 
    {
        Node *curr = root ;
        for ( int i = maxBit ; i >= 0 ; i--) 
        {
            int bit = (n >> i) & 1;
            if (!curr->child[bit]) curr->child[bit] = new Node;
            curr = curr->child[bit];
            curr->freq++;
        }
    }

    void erase( const int n ) { erase( n , maxBit , root ) ; }

    int max_xor(const int n) 
    {
        Node *curr = root ;
        int ans = 0 ;

        for (int i = maxBit; i >= 0; i--) 
        {
            int bit = ( n >> i ) & 1 ;
            
            if (curr->child[!bit]) 
            {
                ans |= (1LL << i);
                curr = curr->child[!bit];
            } 
            else curr = curr->child[bit];
        }
        
        return ans ;
    }

    int min_xor(const int n) 
    {
        Node *curr = root;
        int ans = 0;
        for (int i = maxBit; i >= 0; i--) 
        {
            int bit = ( n >> i ) & 1 ;
            if (curr->child[bit]) curr = curr->child[bit];  
            else 
            {
                ans |= (1LL << i);
                curr = curr->child[!bit];
            }
        }

        return ans;
    }
};