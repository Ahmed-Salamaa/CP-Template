#include "../core.h"

/** Topic: Splay Tree */
template <typename T = int>
struct SplayTree {
    struct Node {
        Node *ch[2], *par;
        T val;
        int sub_size, freq;

        Node() : sub_size(0), freq(0) {
            par = ch[0] = ch[1] = this;
            val = numeric_limits<T>::min();
        }

        Node(T v) : val(v), sub_size(1), freq(1) { par = ch[0] = ch[1] = EMPTY; }

        void update() { sub_size = freq + ch[0]->sub_size + ch[1]->sub_size; }
    };

    static Node* EMPTY;
    Node* root;

    enum dir { LEFT, RIGHT };

    SplayTree() { root = EMPTY; }

    void link(Node* p, Node* c, int d) {
        if (p != EMPTY) {
            p->ch[d] = c;
            p->update();
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

    Node* find(Node* p, T val) {
        if (p == EMPTY) { return EMPTY; }
        Node* ch = p->ch[val > p->val];
        if (p->val == val || ch == EMPTY) { return p; }
        return find(ch, val);
    }

    Node* splay_by_value(Node* p, T val) {
        p = find(p, val);
        splay(p);
        return p;
    }

    Node* insert(Node* p, T val) {
        if (p == EMPTY) { return new Node(val); }
        p = splay_by_value(p, val);
        if (p->val == val) {
            p->freq++;
            p->sub_size++;
            return p;
        }
        Node* q = new Node(val);
        if (p->ch[val > p->val] != EMPTY) {
            auto ch = p->ch[val > p->val];
            link(p, EMPTY, val > p->val);
            link(q, ch, q->val < ch->val);
            link(q, p, q->val < p->val);
            p = q;
        } else {
            link(p, q, val > p->val);
        }
        return p;
    }

    void insert(T val) { root = insert(root, val); }

    void split(Node* p, T val, Node*& ls, Node*& ge) {
        p = splay_by_value(p, val);
        if (p->val < val) {
            ls = p;
            ge = p->ch[RIGHT];
            link(ls, EMPTY, RIGHT);
            link(EMPTY, ge, LEFT);
        } else {
            ls = p->ch[LEFT];
            ge = p;
            link(ge, EMPTY, LEFT);
            link(EMPTY, ls, RIGHT);
        }
    }

    Node* merge(Node* ls, Node* ge) {
        if (ls == EMPTY) { return ge; }
        if (ge == EMPTY) { return ls; }
        ge = splay_by_value(ge, numeric_limits<T>::min());
        link(ge, ls, LEFT);
        return ge;
    }

    Node* erase(Node* p, T val) {
        p = splay_by_value(p, val);
        if (p->val != val) { return p; }
        if (p->freq > 1) {
            p->freq--;
            p->sub_size--;
            return p;
        } else {
            Node* ls = p->ch[LEFT];
            Node* ge = p->ch[RIGHT];
            delete p;
            link(EMPTY, ls, LEFT);
            link(EMPTY, ge, RIGHT);
            return merge(ls, ge);
        }
    }

    void erase(T val) { root = erase(root, val); }

    Node* kth(Node* p, T k) {
        if (p == EMPTY) { return EMPTY; }
        if (k > p->sub_size) { return EMPTY; }
        int sz = p->ch[LEFT]->sub_size;
        if (sz > k) { return kth(p->ch[LEFT], k); }
        if (sz + p->freq <= k) { return kth(p->ch[RIGHT], k - sz - p->freq); }
        return p;
    }

    T kth(T k) {
        auto p = kth(root, k);
        splay(p);
        root = p;
        return p->val;
    }

    int count_less(T val) {
        root = splay_by_value(root, val);
        return root->ch[LEFT]->sub_size + (root->val < val ? root->freq : 0);
    }

    int get_size() { return root->sub_size; }

    bool search(T val) {
        root = splay_by_value(root, val);
        return root->val == val;
    }
};

template <typename T>
typename SplayTree<T>::Node* SplayTree<T>::EMPTY = new typename SplayTree<T>::Node();
