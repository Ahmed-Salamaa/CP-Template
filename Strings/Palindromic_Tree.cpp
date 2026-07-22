#include "../core.h"

/** Topic: Palindromic Tree */
template <typename T = int, int Base = 1>
struct PalindromicTree {
    struct Node {
        vector<int> nxt;
        int lnk, len, pos;
        long long occ;

        Node(int S = 0, int L = 0) : nxt(S, 0), lnk(0), len(L), pos(-1), occ(0) {}
    };

    vector<Node> t;
    string s;
    int last;
    int SIGMA;
    char BASE;

    PalindromicTree(int mxL, int S = 26, char B = 'a') : s("#"), last(1), SIGMA(S), BASE(B) {
        t.reserve(mxL + 3);
        t.emplace_back(S, -1);
        t.emplace_back(S, 0);
    }

    void add(char ch, int p) {
        int c = ch - BASE;
        s += ch;
        int u = last;

        while (s[p - 1 - t[u].len] != ch) {
            u = t[u].lnk;
        }

        if (t[u].nxt[c]) {
            last = t[u].nxt[c];
            t[last].occ++;
            return;
        }

        last = t[u].nxt[c] = t.size();
        t.emplace_back(SIGMA, t[u].len + 2);
        t[last].occ = 1;
        t[last].pos = p;

        if (t[last].len == 1) {
            t[last].lnk = 1;
        } else {
            int v = t[u].lnk;
            while (s[p - 1 - t[v].len] != ch) {
                v = t[v].lnk;
            }
            t[last].lnk = t[v].nxt[c];
        }
    }

    void prop() {
        for (int i = (int)t.size() - 1; i > 1; i--) {
            t[t[i].lnk].occ += t[i].occ;
        }
    }

    int distinct() {
        return (int)t.size() - 2;
    }

    long long total() {
        long long sum = 0;
        for (int i = 2; i < (int)t.size(); i++) {
            sum += t[i].occ;
        }
        return sum;
    }
};
