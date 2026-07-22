/* Topic: Palindromic Tree
 * Description: Palindromic Tree (Eertree) is a specialized data structure used to represent and
 *   efficiently process all distinct palindromic substrings of a string.
 * Usage: PalindromicTree pt(n); for(char c: s) pt.add_char(c); pt.propagate();
 */
#include "../../../core.h"

struct PalindromicTree {
    struct Node {
        vector<int> next;
        int link, len, occ, firstPos;
        Node(int sigma = 0, int L = 0) : next(sigma, 0), link(0), len(L), occ(0), firstPos(-1) {}
    };

    vector<Node> tree;
    string s;
    int last, SIGMA;
    char BASE;

    explicit PalindromicTree(int maxLen, int sigma = 26, char base = 'a') : s("#"), last(1), SIGMA(sigma), BASE(base) {
        tree.reserve(maxLen + 3);
        tree.emplace_back(sigma, -1);
        tree.emplace_back(sigma, 0);
        tree[0].link = tree[1].link = 0;
    }

    inline void add_char(char ch, int pos) {
        int c = ch - BASE;
        s.push_back(ch);
        int cur = last;
        while (pos - 1 - tree[cur].len < 0 || s[pos - 1 - tree[cur].len] != ch) cur = tree[cur].link;
        if (tree[cur].next[c]) { last = tree[cur].next[c], tree[last].occ++; return; }
        last = tree[cur].next[c] = sz(tree);
        tree.emplace_back(SIGMA, tree[cur].len + 2);
        tree[last].occ = 1, tree[last].firstPos = pos;
        if (tree[last].len == 1) tree[last].link = 1;
        else {
            int linkCandidate = tree[cur].link;
            while (pos - 1 - tree[linkCandidate].len < 0 || s[pos - 1 - tree[linkCandidate].len] != ch) linkCandidate = tree[linkCandidate].link;
            tree[last].link = tree[linkCandidate].next[c];
        }
    }

    inline void propagate() {
        vector<int> order(sz(tree));
        iota(all(order), 0);
        sort(all(order), [&](int a, int b) { return tree[a].len > tree[b].len; });
        for (int v : order) if (v > 1) tree[tree[v].link].occ += tree[v].occ;
    }

    inline int distinct_count() const { return sz(tree) - 2; }

    inline int total_occurrences() const {
        int sum = 0;
        for (int i = 2; i < sz(tree); i++) sum += tree[i].occ;
        return sum;
    }
};
