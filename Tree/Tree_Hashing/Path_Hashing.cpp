/*
 * Topic: Tree - Tree Hashing
 * Description: Path Hashing is a randomized technique used to hash paths in a tree,
 *   facilitating fast comparisons between different paths to check if their multiset
 *   of values are identical.
 */
#include "../../core.h"

struct Path_Hashing {
    int n;
    int K;
    int mod1, mod2, base1, base2;
    vector<int> pw1, inv1, pw2, inv2;
    vector<vector<int>> adj, UP;
    vector<pair<int, int>> h, h_r;
    vector<int> parentArr, depthArr;
    vector<char> vals;

    Path_Hashing(int n) : n(n), K(20), mod1(1000000009), mod2(1000000021), base1(91), base2(53) {
        pw1.assign(n + 5, 0);
        inv1.assign(n + 5, 0);
        pw2.assign(n + 5, 0);
        inv2.assign(n + 5, 0);
        adj.assign(n + 5, vector<int>());
        UP.assign(n + 5, vector<int>(K, -1));
        h.assign(n + 5, {0, 0});
        h_r.assign(n + 5, {0, 0});
        parentArr.assign(n + 5, -1);
        depthArr.assign(n + 5, 0);
        vals.assign(n + 5, ' ');
    }

    inline int norm(long long x, int mod) {
        x %= mod;
        if (x < 0) x += mod;
        return (int)x;
    }

    inline int add(int a, int b, int mod) { return norm((long long)a + b, mod); }

    inline int mul(int a, int b, int mod) { return norm((long long)a * b, mod); }

    int modpow(int a, long long e, int mod) {
        long long r = 1;
        long long x = a % mod;
        while (e > 0) {
            if (e & 1) r = (r * x) % mod;
            x = (x * x) % mod;
            e >>= 1;
        }
        return (int)r;
    }

    void PreCalc() {
        pw1[0] = pw2[0] = 1;
        inv1[0] = inv2[0] = 1;
        int invBase1 = modpow(base1, mod1 - 2, mod1);
        int invBase2 = modpow(base2, mod2 - 2, mod2);
        for (int i = 1; i <= n; ++i) {
            pw1[i] = mul(pw1[i - 1], base1, mod1);
            inv1[i] = mul(inv1[i - 1], invBase1, mod1);
            pw2[i] = mul(pw2[i - 1], base2, mod2);
            inv2[i] = mul(inv2[i - 1], invBase2, mod2);
        }
    }

    void dfs(int u, int p, pair<int, int> curr_h, pair<int, int> curr_h_r, int l) {
        parentArr[u] = p;
        depthArr[u] = l;

        curr_h.first = add(curr_h.first, mul(pw1[l], vals[u] - 'a' + 1, mod1), mod1);
        curr_h.second = add(curr_h.second, mul(pw2[l], vals[u] - 'a' + 1, mod2), mod2);

        curr_h_r.first = add(mul(curr_h_r.first, pw1[1], mod1), mul(pw1[0], vals[u] - 'a' + 1, mod1), mod1);
        curr_h_r.second = add(mul(curr_h_r.second, pw2[1], mod2), mul(pw2[0], vals[u] - 'a' + 1, mod2), mod2);

        h[u] = curr_h;
        h_r[u] = curr_h_r;

        for (int v : adj[u])
            if (v != p) { dfs(v, u, curr_h, curr_h_r, l + 1); }
    }

    int Get_Kth_Anc(int u, int k) {
        for (int i = 0; i < K && u != -1; ++i) {
            if (k & (1 << i)) {
                u = UP[u][i];
                if (u == -1) return -1;
            }
        }
        return u;
    }

    void Build() {
        dfs(1, -1, {0, 0}, {0, 0}, 0);
        for (int i = 1; i <= n; ++i) UP[i][0] = parentArr[i];
        for (int i = 1; i < K; ++i) {
            for (int j = 1; j <= n; ++j) {
                int mid = UP[j][i - 1];
                if (mid == -1)
                    UP[j][i] = -1;
                else
                    UP[j][i] = UP[mid][i - 1];
            }
        }
    }

    int Get_LCA(int u, int v) {
        if (depthArr[u] < depthArr[v]) swap(u, v);
        int diff = depthArr[u] - depthArr[v];
        u = Get_Kth_Anc(u, diff);
        if (u == v) return u;
        for (int i = K - 1; i >= 0; --i) {
            if (UP[u][i] != UP[v][i]) {
                u = UP[u][i];
                v = UP[v][i];
            }
        }
        return UP[u][0];
    }

    pair<int, int> Get_Hash(int u, int v) {
        int common = Get_LCA(u, v);
        int depth_common = depthArr[common];
        int parent_common = parentArr[common];

        int dist_left = depthArr[u] - depth_common;
        int left1 = h_r[u].first;
        int left2 = h_r[u].second;
        if (parent_common != -1) {
            left1 = add(left1, -mul(h_r[parent_common].first, pw1[dist_left + 1], mod1), mod1);
            left2 = add(left2, -mul(h_r[parent_common].second, pw2[dist_left + 1], mod2), mod2);
        }

        int len_right_excl = depthArr[v] - depth_common;
        int right1 = 0, right2 = 0;
        if (len_right_excl > 0) {
            int raw1 = add(h[v].first, -h[common].first, mod1);
            int raw2 = add(h[v].second, -h[common].second, mod2);
            right1 = mul(raw1, inv1[depth_common + 1], mod1);
            right2 = mul(raw2, inv2[depth_common + 1], mod2);
        }

        int left_len = dist_left + 1;
        int combined1 = add(left1, mul(right1, pw1[left_len], mod1), mod1);
        int combined2 = add(left2, mul(right2, pw2[left_len], mod2), mod2);

        return {combined1, combined2};
    }

    pair<int, int> GetFullHash(const string& s) {
        int ln = (int)s.length();
        int x1 = 0, x2 = 0;
        for (int i = 0; i < ln; ++i) {
            x1 = add(x1, mul(pw1[i], s[i] - 'a' + 1, mod1), mod1);
            x2 = add(x2, mul(pw2[i], s[i] - 'a' + 1, mod2), mod2);
        }
        return {x1, x2};
    }
};
