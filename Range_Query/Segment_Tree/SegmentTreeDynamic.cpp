/* Topic: Dynamic Segment Tree
 * Description: Dynamic Segment Tree creates nodes on the fly as needed rather than allocating
 *   them upfront. Ideal for range queries over an extremely large or sparse index
 *   space.
 * Usage: DynamicSegmentTree st(1, 1e9);
 */
#include "../../core.h"

struct DynamicSegmentTree {
    struct Node {
        int val;
        Node(int _val = 0) : val(_val) {}
        Node operator=(int x) { val = x; return *this; }
    };
    struct TreeNode {
        Node data;
        TreeNode *left, *right;
        TreeNode() : data(), left(nullptr), right(nullptr) {}
    };

    TreeNode *root;
    int Lx, Rx;

    // Time Complexity: Update/Query O(log(Rx - Lx))
    // Space Complexity: O(Q log(Rx - Lx)) where Q is number of updates
    DynamicSegmentTree(int lx = -1e9, int rx = 1e9) : Lx(lx), Rx(rx) {
        root = new TreeNode();
    }

    Node merge(Node a, Node b) {
        Node res;
        res.val = a.val + b.val;
        return res;
    }

    void update(TreeNode *&treenode, int idx, int val, int lx, int rx) {
        if (!treenode) treenode = new TreeNode();
        if (lx == rx) {
            treenode->data.val += val;
            return;
        }
        int mx = lx + (rx - lx) / 2;
        if (idx <= mx) update(treenode->left, idx, val, lx, mx);
        else update(treenode->right, idx, val, mx + 1, rx);

        Node leftNode = treenode->left ? treenode->left->data : Node(0);
        Node rightNode = treenode->right ? treenode->right->data : Node(0);
        treenode->data = merge(leftNode, rightNode);
    }

    void update(int idx, int val) { update(root, idx, val, Lx, Rx); }

    Node query(TreeNode *treenode, int l, int r, int lx, int rx) {
        if (!treenode || lx > r || rx < l) return Node(0);
        if (lx >= l && rx <= r) return treenode->data;
        int mx = lx + (rx - lx) / 2;
        Node leftNode = query(treenode->left, l, r, lx, mx);
        Node rightNode = query(treenode->right, l, r, mx + 1, rx);
        return merge(leftNode, rightNode);
    }

    int query(int l, int r) { return query(root, l, r, Lx, Rx).val; }
};
