template <typename T = int> struct Bridges {

    T dfs_timer;                                   // global timer for DFS order
    vector<vector<T>> adj;                         // adjacency list of original graph
    vector<pair<T, T>> bridges;                    // list of bridges found
    vector<T> dfs_num, dfs_low, vis;               // tarjan arrays

    vector<T> comp_id;                             // component id for each node
    vector<vector<T>> comp_adj;                    // adjacency list of compressed graph
    T comp_cnt = 0;                                // number of components after removing bridges

    Bridges() = default;

    Bridges(T n) {
        dfs_timer = 0;
        adj = vector<vector<T>>(n + 1);
        dfs_num = dfs_low = vis = vector<T>(n + 1);
    }

    void add_edge(T u, T v, bool bidirectional = true) {
        adj[u].push_back(v);
        if (bidirectional) adj[v].push_back(u);
    }

    void tarjan(T node, T parent) {

        dfs_num[node] = dfs_low[node] = ++dfs_timer;   // assign discovery time
        vis[node] = true;

        for (auto child : adj[node]) {

            if (child == parent) continue;              // ignore edge to parent

            if (!dfs_num[child]) {                      // tree edge

                tarjan(child, node);

                dfs_low[node] = min(dfs_low[node], dfs_low[child]);

                // if child's low is strictly greater than node's discovery time
                // then no back-edge connects child subtree to ancestors of node
                // so this edge is a bridge
                if (dfs_low[child] > dfs_num[node]) {
                    bridges.push_back({node, child});
                }

            } else if (vis[child]) {                    // back edge
                dfs_low[node] = min(dfs_low[node], dfs_num[child]);
            }
        }
    }

    void solve() {
        for (T i = 1; i < (T)adj.size(); ++i)
            if (!dfs_num[i])
                tarjan(i, -1);
    }

    void print() {
        cout << bridges.size() << "\n";
        for (auto &x : bridges)
            cout << x.first << " " << x.second << "\n";
    }

    vector<pair<T, T>> get_bridges() { return bridges; }

    void make_scc_graph() {

        // Step 1: remove bridges and find connected components
        // Each component becomes one node in the new graph

        T n = (T)adj.size() - 1;
        comp_id.assign(n + 1, 0);
        comp_cnt = 0;

        // Store bridges in a set for O(1) checking
        // We normalize (u,v) so that u < v
        unordered_set<long long> is_bridge;
        long long base = (long long)(n + 1);

        for (auto &e : bridges) {
            T u = e.first, v = e.second;
            T a = min(u, v), b = max(u, v);
            long long key = (long long)a * base + b;
            is_bridge.insert(key);
        }

        // Step 2: DFS ignoring bridges
        // All nodes reachable without crossing a bridge
        // belong to the same component

        for (T i = 1; i <= n; ++i) {

            if (comp_id[i]) continue;

            ++comp_cnt;
            stack<T> st;
            st.push(i);
            comp_id[i] = comp_cnt;

            while (!st.empty()) {

                T u = st.top();
                st.pop();

                for (T v : adj[u]) {

                    T a = min(u, v), b = max(u, v);
                    long long key = (long long)a * base + b;

                    // skip bridge edges
                    if (is_bridge.find(key) != is_bridge.end())
                        continue;

                    if (!comp_id[v]) {
                        comp_id[v] = comp_cnt;
                        st.push(v);
                    }
                }
            }
        }

        // Step 3: build new graph (bridge-tree)
        // Each component is one node
        // Each bridge becomes an edge between components

        comp_adj.assign(comp_cnt + 1, vector<T>());
        unordered_set<long long> used;

        for (auto &e : bridges) {

            T u = e.first, v = e.second;
            T cu = comp_id[u], cv = comp_id[v];

            if (cu == cv) continue;

            T a = min(cu, cv), b = max(cu, cv);
            long long key = (long long)a * base + b;

            // avoid duplicate edges
            if (used.insert(key).second) {
                comp_adj[cu].push_back(cv);
                comp_adj[cv].push_back(cu);
            }
        }
    }

    vector<T> get_comp_id() { return comp_id; }
    vector<vector<T>> get_comp_adj() { return comp_adj; }
    T get_comp_cnt() { return comp_cnt; }
};