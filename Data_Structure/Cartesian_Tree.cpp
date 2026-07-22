#include "../core.h"

/** Topic: Cartesian Tree */
template <typename T = int>
struct CartesianTree {
    int n, root;
    vector<int> left_child, right_child, parent;
    vector<T> a;

    CartesianTree(const vector<T>& arr) {
        n = arr.size() - 1;
        a = arr;
        a.resize(n + 5);
        left_child.assign(n + 5, 0);
        right_child.assign(n + 5, 0);
        parent.assign(n + 5, 0);

        vector<int> st;
        for (int i = 1; i <= n; i++) {
            int last_popped = 0;
            while (!st.empty() && a[st.back()] > a[i]) {  // min-heap Cartesian Tree
                last_popped = st.back();
                st.pop_back();
            }
            if (last_popped) {
                left_child[i] = last_popped;
                parent[last_popped] = i;
            }
            if (!st.empty()) {
                right_child[st.back()] = i;
                parent[i] = st.back();
            }
            st.push_back(i);
        }

        root = st.empty() ? 0 : st.front();
    }
};
