#include "../core.h"

/** Topic: Implicit Splay Tree */

template <typename T = int>
struct ImplicitSplayTree {
    struct Data {
        long long val, sum, pref, suff, max_seg;

        Data() : val(0), sum(0), pref(-1e18), suff(-1e18), max_seg(-1e18) {}

        Data(long long v) : val(v), sum(val), pref(val), suff(val), max_seg(val) {}
    };

    Data combine(const Data& a, const Data& b) {
        Data res;
        res.sum = a.sum + b.sum;
        res.pref = max(a.pref, a.sum + b.pref);
        res.suff = max(b.suff, b.sum + a.suff);
        res.max_seg = max({a.max_seg, b.max_seg, a.suff + b.pref});
        return res;
    }

    struct Node {
        Node *ch[2], *par;
        Data val;
        T update;
        int sub_size;
        bool is_lazy;

        Node() : sub_size(0), update(0), is_lazy(false) { par = ch[0] = ch[1] = this; }

        Node(Data v) : val(v), sub_size(1), update(0), is_lazy(false) { par = ch[0] = ch[1] = EMPTY; }

        void update_node(ImplicitSplayTree* tree) {
            sub_size = ch[0]->sub_size + ch[1]->sub_size + 1;
            auto v = val.val;
            val = tree->combine(ch[0]->val, tree->combine(Data(v), ch[1]->val));
            val.val = v;
        }

        void push_down() {
            if (this == EMPTY || !is_lazy) { return; }
            val = Data(update * sub_size);
            ch[0]->lazy_update(update);
            ch[1]->lazy_update(update);
            is_lazy = false;
        }

        void lazy_update(long long c) {
            if (this == EMPTY) { return; }
            update = c;
            is_lazy = true;
        }
    };

    static Node* EMPTY;
    Node* root;

    enum dir { LEFT, RIGHT };

    ImplicitSplayTree() { root = EMPTY; }

    void link(Node* p, Node* c, int d) {
        if (p != EMPTY) {
            p->ch[d] = c;
            p->update_node(this);
        }
        if (c != EMPTY) { c->par = p; }
    }

    int get_dir(Node* p, Node* c) { return p->ch[RIGHT] == c; }

    void rotate(Node* p, int d) {
        Node* q = p->ch[d];
        Node* gp = p->par;
        int gd = get_dir(gp, p);
        link(p, q->ch[!d], d);
        link(q, p, !d);
        link(gp, q, gd);
    }

    void splay(Node* q) {
        while (q->par != EMPTY) {
            Node* p = q->par;
            Node* gp = p->par;
            int d1 = get_dir(p, q);
            int d2 = get_dir(gp, p);
            if (gp == EMPTY) {
                rotate(p, d1);
            } else if (d1 == d2) {
                rotate(gp, d2);
                rotate(p, d1);
            } else {
                rotate(p, d1);
                rotate(gp, d2);
            }
        }
        root = q;
    }

    void split(Node* p, int idx, Node*& ls, Node*& ge) {
        if (idx >= p->sub_size) {
            ls = p;
            ge = EMPTY;
            return;
        }
        p = splay_by_idx(p, idx);
        ls = p->ch[LEFT];
        ge = p;
        link(ge, EMPTY, LEFT);
        link(EMPTY, ls, RIGHT);
    }

    Node* splay_by_idx(Node* p, int idx) {
        p = at(p, idx);
        splay(p);
        return p;
    }

    Node* merge(Node* ls, Node* ge) {
        if (ls == EMPTY) return ge;
        if (ge == EMPTY) return ls;
        ge = splay_by_idx(ge, 0);
        link(ge, ls, LEFT);
        return ge;
    }

    void merge(Node* p) { root = merge(root, p); }

    Node* at(Node* p, int k) {
        if (p == EMPTY) return EMPTY;
        p->push_down();
        if (k > p->sub_size) return EMPTY;
        int sz = p->ch[LEFT]->sub_size;
        if (sz > k) return at(p->ch[LEFT], k);
        if (sz + 1 <= k) return at(p->ch[RIGHT], k - sz - 1);
        return p;
    }

    Data at(int k) {
        auto p = splay_by_idx(root, k);
        return p->val;
    }

    void insert(int idx, T val) {
        Node *before, *after;
        split(root, idx, before, after);
        Node* between = new Node(Data(val));
        root = merge(merge(before, between), after);
    }

    void erase(int idx) {
        Node *before, *after, *between;
        split(root, idx + 1, before, after);
        split(before, idx, before, between);
        delete between;
        root = merge(before, after);
    }

    void replace(int idx, T val) {
        Node *before, *after, *between;
        split(root, idx + 1, before, after);
        split(before, idx, before, between);
        between->val = Data(val);
        root = merge(merge(before, between), after);
    }

    long long query(int s, int e) {
        Node *before, *after, *between;
        split(root, e + 1, before, after);
        split(before, s, before, between);
        long long ans = between->val.max_seg;
        root = merge(merge(before, between), after);
        return ans;
    }

    void push_down_to_root(Node* p) {
        if (p == EMPTY) return;
        push_down_to_root(p->par);
        p->push_down();
    }

    int get_idx(Node* p) {
        push_down_to_root(p);
        splay(p);
        return p->ch[LEFT]->sub_size;
    }

    int get_size() { return root->sub_size; }
};

template <typename T>
typename ImplicitSplayTree<T>::Node* ImplicitSplayTree<T>::EMPTY = new typename ImplicitSplayTree<T>::Node();
