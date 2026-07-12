#include <limits>
#include <algorithm>
#include <cassert>

template <typename T = int> class Binary_Trie {
  
    private:
        struct Node {
            int freq;
            Node *child[2];
            Node() : freq(0), child{nullptr, nullptr} {}
        };

        Node *root;
        int LOG, cnt;

        inline void add(const T x) {
            ++cnt; // inc number of elements in the trie
            Node *cur = root;
            for (int i = LOG; ~i; i--) {
                bool bit = get_bit(x, i);
                if (!cur->child[bit]) cur->child[bit] = new Node();
                cur = cur->child[bit];
                cur->freq++;
            }
        }

        void erase(const T x, const int bit, Node *curr) {
            if (bit < 0) return void(--cnt);
            bool bit_val = get_bit(x, bit);
            erase(x, bit - 1, curr->child[bit_val]);
            if (--curr->child[bit_val]->freq == 0) {
                delete curr->child[bit_val];
                curr->child[bit_val] = nullptr;
            }
        }

        inline bool find(const T x) {
            Node *cur = root;
            for (int i = LOG; ~i; i--) {
                bool bit = get_bit(x, i);
                if (!cur->child[bit] or !cur->child[bit]->freq) return false;
                cur = cur->child[bit];
            }
            return true;
        }

        // number of pairs such that (p ^ val) <= k
        inline T count_less_equal(const T x, const T k) {
            T ans = 0;
            Node *cur = root;
            for (int i = LOG; ~i; i--) {
                if (!cur) break;
                bool val_bit = get_bit(x, i), k_bit = get_bit(k, i);
                if (k_bit) {
                    if (cur->child[val_bit]) ans += cur->child[val_bit]->freq;
                    cur = cur->child[val_bit ^ 1];
                } else
                    cur = cur->child[val_bit];
            }
            if (cur) ans += cur->freq;
            return ans;
        }

        inline T count_in_range(const T l, const T r) {
            if (l > r) return 0;
            T right_cnt = count_less_equal(0, r);
            T left_cnt = (l == 0) ? 0 : count_less_equal(0, l - 1);
            return right_cnt - left_cnt;
        }

        inline T get_max_xor(const T x, const T l, const T r) {
            T ans = 0, prefix = 0;
            Node *cur = root;
            for (int i = LOG; ~i; i--) {
                bool bit = get_bit(x, i);
                bool target = !bit;
                
                auto check_branch = [&](bool b) {
                    if (!cur->child[b] or !cur->child[b]->freq) return false;
                    T P_min = prefix | (T(b) << i);
                    T P_max = P_min | ((1LL << i) - 1);
                    T L = std::max(l, P_min);
                    T R = std::min(r, P_max);
                    if (L > R) return false;
                    if (L <= P_min and R >= P_max) return true;
                    return count_in_range(L, R) > 0;
                };

                if (check_branch(target)) {
                    ans |= (1LL << i);
                    prefix |= (T(target) << i);
                    cur = cur->child[target];
                } else if (check_branch(!target)) {
                    prefix |= (T(!target) << i);
                    cur = cur->child[!target];
                } else return -1;
            }
            return ans;
        }

        inline T get_min_xor(const T x, const T l, const T r) {
            T ans = 0, prefix = 0;
            Node *cur = root;
            for (int i = LOG; ~i; i--) {
                bool bit = get_bit(x, i);
                bool target = bit;
                
                auto check_branch = [&](bool b) {
                    if (!cur->child[b] or !cur->child[b]->freq) return false;
                    T P_min = prefix | (T(b) << i);
                    T P_max = P_min | ((1LL << i) - 1);
                    T L = std::max(l, P_min);
                    T R = std::min(r, P_max);
                    if (L > R) return false;
                    if (L <= P_min and R >= P_max) return true;
                    return count_in_range(L, R) > 0;
                };

                if (check_branch(target)) {
                    prefix |= (T(target) << i);
                    cur = cur->child[target];
                } else if (check_branch(!target)) {
                    ans |= (1LL << i);
                    prefix |= (T(!target) << i);
                    cur = cur->child[!target];
                } else return -1; 
            }
            return ans;
        }

        inline T get_max_or(const T x, const T l, const T r) {
            T ans = 0, prefix = 0;
            Node* cur = root;
            for (int i = LOG; i >= 0; i--) {
                bool xb = get_bit(x, i);

                auto check_branch = [&](bool b) {
                    if (!cur->child[b] or !cur->child[b]->freq) return false;
                    T P_min = prefix | (T(b) << i);
                    T P_max = P_min | ((1LL << i) - 1);
                    T L = std::max(l, P_min);
                    T R = std::min(r, P_max);
                    if (L > R) return false;
                    if (L <= P_min and R >= P_max) return true;
                    return count_in_range(L, R) > 0;
                };

                if (xb == 0) {
                    // try to make OR bit = 1
                    if (check_branch(1)) {
                        ans |= (1LL << i);
                        prefix |= (1LL << i);
                        cur = cur->child[1];
                    } else if (check_branch(0)) {
                        cur = cur->child[0];
                    } else return -1;
                } else {
                    // OR bit is already 1
                    ans |= (1LL << i);
                    if (check_branch(1)) {
                        prefix |= (1LL << i);
                        cur = cur->child[1];
                    } else if (check_branch(0)) {
                        cur = cur->child[0];
                    } else return -1;
                }
            }
            return ans;
        }

        inline T get_min_or(const T x, const T l, const T r) {
            T ans = 0, prefix = 0;
            Node *cur = root;
            for (int i = LOG; ~i; i--) {
                bool xb = get_bit(x, i);
                
                auto check_branch = [&](bool b) {
                    if (!cur->child[b] or !cur->child[b]->freq) return false;
                    T P_min = prefix | (T(b) << i);
                    T P_max = P_min | ((1LL << i) - 1);
                    T L = std::max(l, P_min);
                    T R = std::min(r, P_max);
                    if (L > R) return false;
                    if (L <= P_min and R >= P_max) return true;
                    return count_in_range(L, R) > 0;
                };

                if (xb == 1) {
                    ans |= (1LL << i);
                    if (check_branch(0)) {
                        cur = cur->child[0];
                    } else if (check_branch(1)) {
                        prefix |= (1LL << i);
                        cur = cur->child[1];
                    } else return -1;
                } else {
                    if (check_branch(0)) {
                        cur = cur->child[0];
                    } else if (check_branch(1)) {
                        ans |= (1LL << i);
                        prefix |= (1LL << i);
                        cur = cur->child[1];
                    } else return -1;
                }
            }
            return ans;
        }

    public:
        Binary_Trie(int log = 31) : root(new Node()), LOG(log) , cnt (0) {} ;

        inline bool get_bit(const T x, const int bit) const { return (x >> bit) & 1; }

        inline void insert(const T x) { add(x); }

        inline void erase(const T x) 
        {
            assert(search(x)); // element must be present in the trie
            erase(x, LOG, root);
        }

        inline bool search(const T x) { return find(x); }

        inline T size() { return cnt ; }

        inline T count(const T x, const T k) { return count_less_equal(x, k); }

        inline T count_greater(const T x, const T k) { return cnt - count_less_equal(x, k); }

        inline T max_xor(const T x, const T l = 0, const T r = std::numeric_limits<T>::max()) { return get_max_xor(x, l, r) ; }

        inline T min_xor(const T x, const T l = 0, const T r = std::numeric_limits<T>::max()) { return get_min_xor(x, l, r) ; }

        inline T max_or(const T x, const T l = 0, const T r = std::numeric_limits<T>::max()) { return get_max_or(x, l, r) ; } 

        inline T min_or(const T x, const T l = 0, const T r = std::numeric_limits<T>::max()) { return get_min_or(x, l, r) ; }
};

void Accepted() {}

int main() {

    ios::sync_with_stdio(0);
    cin.tie(0);
    cout.tie(0);
    if (fopen("input.txt", "r")) {
        freopen("input.txt", "r", stdin), freopen("output.txt", "w", stdout);
    }

    tt = tc = 1; // cin >> tt;
    while (tt--) Accepted(), ++tc;

    Time;
    return 0;
}
