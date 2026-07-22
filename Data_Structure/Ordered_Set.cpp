/*
 * Topic: Data Structure - Data Structure
 * Description: Implementation of Ordered Set
 */
#include <ext/pb_ds/assoc_container.hpp>
#include <ext/pb_ds/tree_policy.hpp>

#include "../core.h"
using namespace __gnu_pbds;
/*
 * Ordered Set / Ordered Multiset (Policy Based Data Structures)
 * -------------------------------------------------------------
 * Provides a Red-Black tree structure that supports querying the k-th smallest element,
 * and finding the number of elements strictly less than a given value, all in logarithmic time.
 */
template <typename T>
using ordered_set = tree<T, null_type, less<T>, rb_tree_tag, tree_order_statistics_node_update>;

template <typename T>
struct ordered_multiset {
    tree<pair<T, int>, null_type, less<pair<T, int>>, rb_tree_tag, tree_order_statistics_node_update> tree_structure;
    int timer = 0;

    // O ( log(N) )
    void insert(T element) { tree_structure.insert({element, ++timer}); }

    // O ( log(N) )
    bool erase_one(T element) {
        auto it = tree_structure.lower_bound({element, 0});
        if (it != tree_structure.end() && it->first == element) {
            tree_structure.erase(it);
            return true;
        }
        return false;
    }

    // O ( K * log(N) )
    int erase_all(T element) {
        int count = 0;
        while (erase_one(element)) count++;
        return count;
    }

    // O ( log(N) )
    int order_of_key(T element) { return tree_structure.order_of_key({element, 0}); }

    // O ( log(N) )
    T find_by_order(int index) { return tree_structure.find_by_order(index)->first; }

    // O ( 1 )
    int size() { return tree_structure.size(); }

    // O ( 1 )
    bool empty() { return tree_structure.empty(); }
};
