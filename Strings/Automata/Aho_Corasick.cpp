/* Topic: Aho-Corasick
 * Description: Aho-Corasick is a string searching algorithm that builds a finite state machine
 *   (trie with failure links) to efficiently find all occurrences of a set of
 *   dictionary strings in a text.
 * Usage: AhoCorasick ac(patterns); vector<vector<int>> res = ac.search(s);
 */
#include "../../../core.h"

struct AhoCorasick {
    struct Node {
        Node* fail;
        Node* child[128];
        vector<int> patIdx;
        vector<char> chars;

        Node() : fail(nullptr) { memset(child, 0, sizeof(child)); }
    };

    Node* root;
    const vector<string>& patterns;

    explicit AhoCorasick(const vector<string>& patterns) : root(new Node()), patterns(patterns) {
        for (int i = 0; i < sz(patterns); i++) insert(patterns[i], i);
        build();
    }

    inline void insert(const string& s, int idx) {
        Node* cur = root;
        for (char c : s) {
            if (!cur->child[c]) cur->child[c] = new Node(), cur->chars.push_back(c);
            cur = cur->child[c];
        }
        cur->patIdx.push_back(idx);
    }

    inline void build() {
        queue<Node*> q;
        for (int i = 0; i < 128; i++) {
            if (root->child[i])
                root->child[i]->fail = root, q.push(root->child[i]);
            else
                root->child[i] = root;
        }
        while (!q.empty()) {
            Node* cur = q.front();
            q.pop();
            for (char c : cur->chars) {
                Node *next = cur->child[c], *fail = cur->fail;
                while (fail != root && !fail->child[c]) fail = fail->fail;
                next->fail = fail->child[c] ? fail->child[c] : root;
                for (int p : next->fail->patIdx) next->patIdx.push_back(p);
                q.push(next);
            }
        }
    }

    inline vector<vector<int>> search(const string& s) {
        vector<vector<int>> res(sz(s));
        Node* cur = root;
        for (int i = 0; i < sz(s); i++) {
            char c = s[i];
            while (cur != root && !cur->child[c]) cur = cur->fail;
            if (cur->child[c]) cur = cur->child[c];
            for (int p : cur->patIdx) res[i].push_back(p);
        }
        return res;
    }
};
