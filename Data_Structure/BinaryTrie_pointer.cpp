/*
 * Topic: Data Structure - Trie
 * Description: Implementation of BinaryTrie pointer based
 */
#include "../core.h"

/*
 * Binary Trie (Pointer Based)
 * ---------------------------
 * A specialized Trie used for bitwise operations, finding maximum/minimum XOR pairs,
 * counting elements in ranges, and answering boolean logic queries.
 * It uses a pointer-based structure for dynamic allocation.
 */
template <typename T = int>
class BinaryTrie_pointer {
   private:
    struct Node {
        int freq;
        Node* child[2];

        Node() : freq(0), child{nullptr, nullptr} {}
    };

    Node* root;
    int LOG, cnt;

    // Helper to get a specific bit
    inline bool get_bit(const T x, const int bit) const { return (x >> bit) & 1; }

    // Helper to count elements in range [l, r]
    inline T count_in_range(const T l, const T r) {
        if (l > r) return 0;
        T right_cnt = count(0, r);
        T left_cnt = (l == 0) ? 0 : count(0, l - 1);
        return right_cnt - left_cnt;
    }

   public:
    // Constructor taking the maximum number of bits (LOG)
    BinaryTrie_pointer(int log = 31) : root(new Node()), LOG(log), cnt(0) {}

    /*
     * Inserts the value `x` into the Binary Trie.
     * @param x: The value to be inserted.
     */
    inline void insert(const T x) {
        ++cnt;  // inc number of elements in the trie
        Node* cur = root;
        for (int i = LOG; ~i; i--) {
            bool bit = get_bit(x, i);
            if (!cur->child[bit]) cur->child[bit] = new Node();
            cur = cur->child[bit];
            cur->freq++;
        }
    }

    /*
     * Erases one occurrence of the value `x` from the Binary Trie.
     * Asserts that `x` is present in the trie before erasing.
     * @param x: The value to be erased.
     */
    inline void erase(const T x) {
        assert(search(x));  // element must be present in the trie
        --cnt;
        Node* path[65];
        path[LOG + 1] = root;
        Node* cur = root;
        for (int i = LOG; ~i; i--) {
            bool bit = get_bit(x, i);
            cur = cur->child[bit];
            path[i] = cur;
        }
        for (int i = 0; i <= LOG; i++) {
            bool bit = get_bit(x, i);
            Node* current = path[i];
            Node* parent = path[i + 1];
            if (--current->freq == 0) {
                delete current;
                parent->child[bit] = nullptr;
            }
        }
    }

    /*
     * Searches for the presence of the value `x` in the Binary Trie.
     * @param x: The value to search for.
     * @return: True if `x` is in the trie, False otherwise.
     */
    inline bool search(const T x) {
        Node* cur = root;
        for (int i = LOG; ~i; i--) {
            bool bit = get_bit(x, i);
            if (!cur->child[bit] or !cur->child[bit]->freq) return false;
            cur = cur->child[bit];
        }
        return true;
    }

    /*
     * Returns the total number of elements currently stored in the trie.
     * @return: The total count of elements.
     */
    inline T size() { return cnt; }

    /*
     * Counts the number of elements `p` in the trie such that (p ^ x) <= k.
     * @param x: The base value for the XOR operation.
     * @param k: The upper bound for the XOR result.
     * @return: The count of matching elements.
     */
    inline T count(const T x, const T k) {
        T ans = 0;
        Node* cur = root;
        for (int i = LOG; ~i; i--) {
            if (!cur) break;
            bool val_bit = get_bit(x, i), k_bit = get_bit(k, i);
            if (k_bit) {
                if (cur->child[val_bit]) ans += cur->child[val_bit]->freq;
                cur = cur->child[val_bit ^ 1];
            } else {
                cur = cur->child[val_bit];
            }
        }
        if (cur) ans += cur->freq;
        return ans;
    }

    /*
     * Counts the number of elements `p` in the trie such that (p ^ x) > k.
     * @param x: The base value for the XOR operation.
     * @param k: The lower bound threshold (exclusive) for the XOR result.
     * @return: The count of matching elements.
     */
    inline T count_greater(const T x, const T k) { return cnt - count(x, k); }

    /*
     * Finds the maximum possible XOR result of `x` with any element `p` in the trie,
     * such that the element `p` falls within the inclusive value range [l, r].
     * @param x: The value to maximize the XOR with.
     * @param l: The lower bound for elements to consider (default is 0).
     * @param r: The upper bound for elements to consider (default is maximum value of T).
     * @return: The maximum XOR result found, or -1 if no valid element exists in the range.
     */
    inline T max_xor(const T x, const T l = 0, const T r = std::numeric_limits<T>::max()) {
        T ans = 0, prefix = 0;
        Node* cur = root;
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
            } else
                return -1;
        }
        return ans;
    }

    /*
     * Finds the minimum possible XOR result of `x` with any element `p` in the trie,
     * such that the element `p` falls within the inclusive value range [l, r].
     * @param x: The value to minimize the XOR with.
     * @param l: The lower bound for elements to consider (default is 0).
     * @param r: The upper bound for elements to consider (default is maximum value of T).
     * @return: The minimum XOR result found, or -1 if no valid element exists in the range.
     */
    inline T min_xor(const T x, const T l = 0, const T r = std::numeric_limits<T>::max()) {
        T ans = 0, prefix = 0;
        Node* cur = root;
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
            } else
                return -1;
        }
        return ans;
    }

    /*
     * Finds the maximum possible OR result of `x` with any element `p` in the trie,
     * such that the element `p` falls within the inclusive value range [l, r].
     * @param x: The value to maximize the OR with.
     * @param l: The lower bound for elements to consider (default is 0).
     * @param r: The upper bound for elements to consider (default is maximum value of T).
     * @return: The maximum OR result found, or -1 if no valid element exists in the range.
     */
    inline T max_or(const T x, const T l = 0, const T r = std::numeric_limits<T>::max()) {
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
                } else
                    return -1;
            } else {
                // OR bit is already 1
                ans |= (1LL << i);
                if (check_branch(1)) {
                    prefix |= (1LL << i);
                    cur = cur->child[1];
                } else if (check_branch(0)) {
                    cur = cur->child[0];
                } else
                    return -1;
            }
        }
        return ans;
    }

    /*
     * Finds the minimum possible OR result of `x` with any element `p` in the trie,
     * such that the element `p` falls within the inclusive value range [l, r].
     * @param x: The value to minimize the OR with.
     * @param l: The lower bound for elements to consider (default is 0).
     * @param r: The upper bound for elements to consider (default is maximum value of T).
     * @return: The minimum OR result found, or -1 if no valid element exists in the range.
     */
    inline T min_or(const T x, const T l = 0, const T r = std::numeric_limits<T>::max()) {
        T ans = 0, prefix = 0;
        Node* cur = root;
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
                } else
                    return -1;
            } else {
                if (check_branch(0)) {
                    cur = cur->child[0];
                } else if (check_branch(1)) {
                    ans |= (1LL << i);
                    prefix |= (1LL << i);
                    cur = cur->child[1];
                } else
                    return -1;
            }
        }
        return ans;
    }
};
