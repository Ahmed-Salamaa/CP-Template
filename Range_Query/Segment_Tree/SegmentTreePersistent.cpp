/* Topic: Persistent Segment Tree
 * Description: Persistent Segment Tree preserves its previous versions across updates, allowing
 *   queries on historical states. Frequently used for 2D range counting queries or
 *   finding the K-th smallest element in a subsegment.
 * Usage: PST<int> pst(n, 1, n);
 */
#include "../../core.h"

template <typename T = int>
struct PST {
    struct Node {
        T val;
        Node *left, *right;
        Node(T _val = 0) {
            val = _val;
            left = right = this;
        }
        Node(Node *node, Node *l = new Node, Node *r = new Node) {
            val = node->val;
            left = l;
            right = r;
        }
    };

    vector<Node *> roots = {new Node()};
    int N;
    T Lx, Rx;

    // Time Complexity: Build O(N log N), Update/Query O(log(Rx - Lx))
    // Space Complexity: O(N log N + Q log(Rx - Lx))
    PST(int n = 0, T lx = -1e9, T rx = 1e9) : N(n), Lx(lx), Rx(rx) {}

    Node *build(const vector<T> &nums, T l, T r) {
        if (l == r) return new Node(nums[l - 1]);
        T mx = l + (r - l) / 2;
        Node *L = build(nums, l, mx);
        Node *R = build(nums, mx + 1, r);
        return new Node(merge(L, R), L, R);
    }

    void build(const vector<T> &nums) { roots.push_back(build(nums, Lx, Rx)); }

    Node *merge(Node *a, Node *b) {
        Node *node = new Node();
        node->val = a->val + b->val;
        return node;
    }

    Node *update(Node *root, int idx, T val, T lx, T rx) {
        if (idx < lx || idx > rx) return root;
        if (lx == rx) return new Node(val);
        T mx = lx + (rx - lx) / 2;
        Node *L = update(root->left, idx, val, lx, mx);
        Node *R = update(root->right, idx, val, mx + 1, rx);
        return new Node(merge(L, R), L, R);
    }

    void update(int idx, T val) { roots.push_back(update(roots.back(), idx, val, Lx, Rx)); }
    void update_version(int idx, T val, int version) { roots[version] = update(roots[version], idx, val, Lx, Rx); }

    Node *query(Node *root, int l, int r, T lx, T rx) {
        if (root == nullptr) return new Node();
        if (lx > r || l > rx) return new Node();
        if (lx >= l && rx <= r) return root;
        T mx = lx + (rx - lx) / 2;
        Node *L = query(root->left, l, r, lx, mx);
        Node *R = query(root->right, l, r, mx + 1, rx);
        return merge(L, R);
    }

    T query(int l, int r, int version) { return query(roots[version], l, r, Lx, Rx)->val; }

    void copy(int version) { roots.push_back(roots[version]); }
    T get(int time, int idx) { return query(idx, idx, time); }
};
