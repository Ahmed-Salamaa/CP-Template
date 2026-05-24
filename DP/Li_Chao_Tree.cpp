#include <bits/stdc++.h>
using namespace std;

using ll = long long;

template<bool MIN_MODE = true>
struct LiChaoTree {

private :
    
    struct Line {
        ll m, c;
        Line(ll m = 0, ll c = MIN_MODE ? INF : -INF) : m(m), c(c) {}
        ll eval(ll x) { return m * x + c; }
    };

    struct Node {
        Line line;
        Node *left = nullptr;
        Node *right = nullptr;
        Node(Line line) : line(line) {}
    };

    ll L, R , cnt;
    Node *root;

    // Helpers to support both min and max toggling
    bool compare(ll a, ll b) { return MIN_MODE ? a < b : a > b; }
    ll best(ll a, ll b) { return MIN_MODE ? min(a, b) : max(a, b); }

    void _add_line(Node*& node, Line nw, ll l, ll r) {
        if (node == nullptr) {
            node = new Node(nw);
            return;
        }
        
        ll m = l + (r - l) / 2;
        bool lef = compare(nw.eval(l), node->line.eval(l));
        bool mid = compare(nw.eval(m), node->line.eval(m));
        
        if (mid) swap(node->line, nw);
        if (l == r) return;
        
        if (lef != mid) _add_line(node->left, nw, l, m);
        else _add_line(node->right, nw, m + 1, r);
    }

    void _add_segment(Node*& node, Line nw, ll l, ll r, ll sqL, ll sqR) {
        if (l > sqR || r < sqL) return;
        if (l >= sqL && r <= sqR) {
            _add_line(node, nw, l, r);
            return;
        }
        
        if (node == nullptr) node = new Node(Line());
        
        ll m = l + (r - l) / 2;
        _add_segment(node->left, nw, l, m, sqL, sqR);
        _add_segment(node->right, nw, m + 1, r, sqL, sqR);
    }

    ll _query(Node* node, ll x, ll l, ll r) {
        if (node == nullptr) return MIN_MODE ? INF : -INF;
        
        ll ans = node->line.eval(x);
        if (l == r) return ans;
        
        ll m = l + (r - l) / 2;
        if (x <= m) return best(ans, _query(node->left, x, l, m));
        else return best(ans, _query(node->right, x, m + 1, r));
    }

    ll _query_segment(Node* node, ll x, ll l, ll r, ll sqL, ll sqR) {
        if (node == nullptr || l > sqR || r < sqL) return MIN_MODE ? INF : -INF;
        
        ll m = l + (r - l) / 2;
        
        // If the node's interval is fully within the query range, behave like a point query
        if (l >= sqL && r <= sqR) {
            ll ans = node->line.eval(x);
            if (l == r) return ans;
            
            if (x <= m) return best(ans, _query_segment(node->left, x, l, m, sqL, sqR));
            else return best(ans, _query_segment(node->right, x, m + 1, r, sqL, sqR));
        }
        
        // Otherwise, split the query and combine the best answers from children
        ll ans = MIN_MODE ? INF : -INF;
        ans = best(ans, _query_segment(node->left, x, l, m, sqL, sqR));
        ans = best(ans, _query_segment(node->right, x, m + 1, r, sqL, sqR));
        return ans;
    }

    void merge ( Node * rhs )
    {
        if (!rhs) return ;
        _add_line(root, rhs->line, L, R);

        merge( rhs->left ) ;
        merge( rhs->right ) ;

        delete rhs->left ;
        rhs->left = nullptr ;
        
        delete rhs->right ;
        rhs->right = nullptr ;
    }

    void erase ( Node * r )
    {
        if (!r) return ;

        erase( r->left ) ;
        erase( r->right ) ;

        delete r ;
    }

public:

    static const ll INF = 2e18; // Safely large infinity
    LiChaoTree(ll L = -2e9, ll R = 2e9) : L(L), R(R), cnt(0), root(nullptr) {}

    // 1. Add a global line ( y = mx + c )
    void add(ll m, ll c) {
        cnt ++ ;
        _add_line(root, Line(m, c), L, R);
    }

    // 2. Add a line segment bounded to [sqL, sqR] , sqL, sqR added to the range
    void add(ll m, ll c, ll sqL, ll sqR) {
        cnt ++ ;
        _add_segment(root, Line(m, c), L, R, sqL, sqR);
    }

    // 3. Query the optimal value at x globally
    ll query(ll x) {
        return _query(root, x, L, R);
    }

    // 4. Query the optimal value at x, restricted only to nodes active inside [sqL, sqR]
    ll query(ll x, ll sqL, ll sqR) {
        return _query_segment(root, x, L, R, sqL, sqR);
    }

    void merge ( LiChaoTree < true > & rhs )
    {
        if ( rhs.cnt > cnt ) this->my_swap ( rhs ) ;
        merge( rhs.root ) ;
        rhs.root = nullptr ;
    }

    void my_swap ( LiChaoTree < true > & rhs )
    {
        swap ( root , rhs.root ) ;
        swap ( cnt , rhs.cnt ) ;
    }

    void erase ()
    {
        erase( root ) ;
        root = nullptr ;
    }

    ~LiChaoTree()
    {
        erase() ;
    }
};



