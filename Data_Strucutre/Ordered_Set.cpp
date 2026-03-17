#include <bits/stdc++.h>
using namespace std;


#include <ext/pb_ds/assoc_container.hpp>
#include <ext/pb_ds/tree_policy.hpp>
using namespace __gnu_pbds;
template < typename T >
using ordered_set = tree < T, null_type, less < T >,
    rb_tree_tag, tree_order_statistics_node_update >;

template < typename T >
using ordered_multiset = tree < T, null_type, less_equal < T >,
    rb_tree_tag, tree_order_statistics_node_update >;

//////////////////////

#include <ext/pb_ds/assoc_container.hpp>
#include <ext/pb_ds/tree_policy.hpp>

using namespace __gnu_pbds;

template<typename T, class Compare = std::less<T>>
struct _pair_cmp {
    Compare cmp;
    _pair_cmp(Compare c = Compare()) : cmp(c) {}
    bool operator()(const std::pair<T,uint64_t>& a, const std::pair<T,uint64_t>& b) const {
        if (cmp(a.first, b.first)) return true;
        if (cmp(b.first, a.first)) return false;
        return a.second < b.second;
    }
};

template<typename T, class Compare = std::less<T>>
class ordered_multiset {
public:
    using key_type = T;
    using id_type = uint64_t;
    using value_type = std::pair<T, id_type>;
    using tree_type = tree<value_type, null_type, _pair_cmp<T,Compare>, rb_tree_tag, tree_order_statistics_node_update>;
    using iterator = typename tree_type::iterator;

    ordered_multiset(): _ctr(0) {}

    void insert(const T& x) {
        tr.insert({x, ++_ctr});
    }

    bool erase_one(const T& x) {
        auto it = tr.lower_bound({x, 0});
        if (it == tr.end()) return false;
        Compare cmp;
        if (cmp(it->first, x) || cmp(x, it->first)) return false;
        tr.erase(it);
        return true;
    }

    size_t erase_all(const T& x) {
        auto lo = tr.lower_bound({x, 0});
        auto hi = tr.upper_bound({x, std::numeric_limits<id_type>::max()});
        size_t cnt = 0;
        for (auto it = lo; it != hi; ) { it = tr.erase(it); ++cnt; }
        return cnt;
    }

    size_t count(const T& x) const {
        return tr.order_of_key({x, std::numeric_limits<id_type>::max()}) - tr.order_of_key({x, 0});
    }

    size_t order_of_key(const T& x) const {
        return tr.order_of_key({x, 0});
    }

    T find_by_order(size_t k) const {
        return tr.find_by_order(k)->first;
    }

    size_t size() const { return tr.size(); }
    bool empty() const { return tr.empty(); }
    void clear() { tr.clear(); _ctr = 0; }

    void swap(ordered_multiset &other)
    {
        std::swap(tr, other.tr);
        std::swap(_ctr, other._ctr);
    }

    iterator begin() { return tr.begin(); }
    iterator end() { return tr.end(); }
    iterator lower_bound(const T& x) { return tr.lower_bound({x,0}); }
    iterator upper_bound(const T& x) { return tr.upper_bound({x, std::numeric_limits<id_type>::max()}); }

private:
    tree_type tr;
    id_type _ctr;
};

int main() {
    ordered_set <int> st;
    st.insert(1);
    st.insert(2);
    cout << *st.find_by_order(0) << endl; /// k th smallest element
    cout << st.order_of_key(2) << endl;   /// number of elements less than k
}