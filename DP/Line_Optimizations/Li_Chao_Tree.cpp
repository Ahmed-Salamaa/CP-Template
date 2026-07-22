/*
 * Topic: DP - Line Optimizations
 * Description: Li Chao Tree is a segment tree structure that maintains a set of lines or
 *   segments, allowing O(log X) queries for the minimum or maximum value at any
 *   given coordinate x.
 */
#include "../../core.h"

/*
 * Li Chao Tree
 * ------------
 * A segment tree structure that maintains a set of lines and allows querying the minimum (or maximum) value at any
 * given x. Supports adding lines globally or within a specific segment [sqL, sqR]. Supports merging trees and dynamic
 * node creation.
 */
template <bool IS_MIN_MODE = true>
struct LiChaoTree {
    struct Line {
        int slope, intercept;

        // O ( 1 )
        Line(int slope = 0, int intercept = IS_MIN_MODE ? 2e9 : -2e9) : slope(slope), intercept(intercept) {}

        // O ( 1 )
        int evaluate(int x) { return slope * x + intercept; }
    };

    struct Node {
        Line line;
        Node* left = nullptr;
        Node* right = nullptr;

        // O ( 1 )
        Node(Line line) : line(line) {}
    };

    int limit_left, limit_right, total_lines;
    Node* root;

    // O ( 1 )
    bool compare(int a, int b) { return IS_MIN_MODE ? a < b : a > b; }

    // O ( 1 )
    int get_best(int a, int b) { return IS_MIN_MODE ? min(a, b) : max(a, b); }

    // O ( log(limit_right - limit_left) )
    void insert_line_internal(Node*& node, Line new_line, int current_l, int current_r) {
        if (!node) {
            node = new Node(new_line);
            return;
        }
        int mid = current_l + (current_r - current_l) / 2;
        // Compare the new line with the current dominant line at the left and mid points
        bool better_left = compare(new_line.evaluate(current_l), node->line.evaluate(current_l));
        bool better_mid = compare(new_line.evaluate(mid), node->line.evaluate(mid));
        // The line that is better at the midpoint becomes the new dominant line for this node
        if (better_mid) {
            Line temp = node->line;
            node->line = new_line;
            new_line = temp;
        }
        if (current_l == current_r) return;
        // The line that lost at the midpoint might still be better on one of the halves
        if (better_left != better_mid)
            insert_line_internal(node->left, new_line, current_l, mid);
        else
            insert_line_internal(node->right, new_line, mid + 1, current_r);
    }

    // O ( log^2(limit_right - limit_left) )
    void insert_segment_internal(Node*& node, Line new_line, int current_l, int current_r, int q_left, int q_right) {
        if (current_l > q_right || current_r < q_left) return;
        if (current_l >= q_left && current_r <= q_right) {
            insert_line_internal(node, new_line, current_l, current_r);
            return;
        }
        if (!node) node = new Node(Line());
        int mid = current_l + (current_r - current_l) / 2;
        insert_segment_internal(node->left, new_line, current_l, mid, q_left, q_right);
        insert_segment_internal(node->right, new_line, mid + 1, current_r, q_left, q_right);
    }

    // O ( log(limit_right - limit_left) )
    int query_internal(Node* node, int x, int current_l, int current_r) {
        if (!node) return IS_MIN_MODE ? 2e9 : -2e9;
        int answer = node->line.evaluate(x);
        if (current_l == current_r) return answer;
        int mid = current_l + (current_r - current_l) / 2;
        // Walk down the path toward x, taking the best value seen at any node along the way
        if (x <= mid)
            return get_best(answer, query_internal(node->left, x, current_l, mid));
        else
            return get_best(answer, query_internal(node->right, x, mid + 1, current_r));
    }

    // O ( log^2(limit_right - limit_left) )
    int query_segment_internal(Node* node, int x, int current_l, int current_r, int q_left, int q_right) {
        if (!node || current_l > q_right || current_r < q_left) return IS_MIN_MODE ? 2e9 : -2e9;
        int mid = current_l + (current_r - current_l) / 2;
        if (current_l >= q_left && current_r <= q_right) {
            int answer = node->line.evaluate(x);
            if (current_l == current_r) return answer;
            if (x <= mid)
                return get_best(answer, query_segment_internal(node->left, x, current_l, mid, q_left, q_right));
            else
                return get_best(answer, query_segment_internal(node->right, x, mid + 1, current_r, q_left, q_right));
        }
        int answer = IS_MIN_MODE ? 2e9 : -2e9;
        answer = get_best(answer, query_segment_internal(node->left, x, current_l, mid, q_left, q_right));
        answer = get_best(answer, query_segment_internal(node->right, x, mid + 1, current_r, q_left, q_right));
        return answer;
    }

    // O ( N log(limit_right - limit_left) )
    void merge_internal(Node* other) {
        if (!other) return;
        insert_line_internal(root, other->line, limit_left, limit_right);
        merge_internal(other->left);
        merge_internal(other->right);
        delete other->left;
        other->left = nullptr;
        delete other->right;
        other->right = nullptr;
    }

    // O ( N )
    void clear_internal(Node* node) {
        if (!node) return;
        clear_internal(node->left);
        clear_internal(node->right);
        delete node;
    }

    // O ( 1 )
    LiChaoTree(int left = -2e9, int right = 2e9)
        : limit_left(left), limit_right(right), total_lines(0), root(nullptr) {}

    // O ( log(limit_right - limit_left) )
    void add_line(int slope, int intercept) {
        total_lines++;
        insert_line_internal(root, Line(slope, intercept), limit_left, limit_right);
    }

    // O ( log^2(limit_right - limit_left) )
    void add_segment(int slope, int intercept, int q_left, int q_right) {
        total_lines++;
        insert_segment_internal(root, Line(slope, intercept), limit_left, limit_right, q_left, q_right);
    }

    // O ( log(limit_right - limit_left) )
    int query(int x) { return query_internal(root, x, limit_left, limit_right); }

    // O ( log^2(limit_right - limit_left) )
    int query_segment(int x, int q_left, int q_right) {
        return query_segment_internal(root, x, limit_left, limit_right, q_left, q_right);
    }

    // O ( min(N1, N2) log(limit_right - limit_left) )
    void merge(LiChaoTree<IS_MIN_MODE>& other) {
        // Small-To-Large optimization: swap trees to guarantee O(N log^2 N) overall merging time
        if (other.total_lines > total_lines) swap_tree(other);
        merge_internal(other.root);
        other.root = nullptr;
    }

    // O ( 1 )
    void swap_tree(LiChaoTree<IS_MIN_MODE>& other) {
        Node* temp_root = root;
        root = other.root;
        other.root = temp_root;

        int temp_lines = total_lines;
        total_lines = other.total_lines;
        other.total_lines = temp_lines;
    }

    // O ( N )
    void clear() {
        clear_internal(root);
        root = nullptr;
    }

    // O ( N )
    ~LiChaoTree() { clear(); }
};
