const ll N = 2e5 + 5, M = 1e6 + 2, MAXK = 22, INF = 1e9 + 5, OO = 1e9;
 
struct Edge {
    int u, v, flow = 0, cap = 0, cost;
    // keep the order: u, v, flow, cap, cost
    Edge(int u, int v, int c, int cost)
      : u(u), v(v), cap(c), cost(cost) {}
    int rem() const { return cap - flow; }
};
 
struct MCMF {
    int n, s, t;
    ll cost = 0, flow = 0, need;
    vector<Edge> edges;
    vector<vector<int>> adj;
    vector<int> from;
 
    MCMF(int n, int s, int t, ll need)
      : n(n), s(s), t(t), need(need)
    {
        adj.assign(n, {});
        from.assign(n, -1);
    }
 
    // undir = 1 if you want to add reverse cap = w, else 0
    void add_edge(int u, int v, int w, int cost, int undir = 0) {
        adj[u].push_back(edges.size());
        edges.emplace_back(u, v, w, cost);
        adj[v].push_back(edges.size());
        edges.emplace_back(v, u, w * undir, -cost);
    }
 
    // push flow up to `need`
    void go() {
        while (flow < need && bfs()) {
            // find bottleneck
            int u = t, addf = OO;
            while (u != s) {
                Edge &e = edges[from[u]];
                addf = min(addf, e.rem());
                u = e.u;
            }
            if (flow + addf > need)
                addf = need - flow;
 
            // apply
            u = t;
            while (u != s) {
                int ei = from[u];
                edges[ei].flow += addf;
                edges[ei^1].flow -= addf;
                cost += (edges[ei].cost) * addf;
                u = edges[ei].u;
            }
            flow += addf;
        }
    }
 
    // SPFA-like (deque) to find shortest augmenting path
    bool bfs() {
        deque<int> q;
        vector<int> dist(n, OO), inq(n, 0);
        fill(from.begin(), from.end(), -1);
 
        dist[s] = 0;
        inq[s] = 1;
        q.push_back(s);
 
        while (!q.empty()) {
            int u = q.front(); q.pop_front();
            inq[u] = 0;
            for (int ei : adj[u]) {
                auto &e = edges[ei];
                int v = e.v;
                if (e.rem() <= 0 || dist[v] <= dist[u] + e.cost) 
                    continue;
                dist[v] = dist[u] + e.cost;
                from[v] = ei;
                if (!inq[v]) {
                    inq[v] = 1;
                    // small label first
                    if (!q.empty() && dist[v] < dist[q.front()])
                        q.push_front(v);
                    else
                        q.push_back(v);
                }
            }
        }
        return from[t] != -1;
    }
};



