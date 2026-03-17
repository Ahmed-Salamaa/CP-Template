int n;
vector<vector<int>> adj;
vector<int> vis;
vector<int> ans;
bool bad;

void dfs(int v) {
    vis[v] = 1;
    for (int u : adj[v]) {
        if (vis[u] == 0) dfs(u);
        else if (vis[u] == 1) bad = true;
    }
    vis[v] = 2;
    ans.push_back(v);
}

void topological_sort() {
    vis.assign(n + 1, 0);
    ans.clear();
    bad = false;

    for (int i = 1; i <= n; ++i)
        if (!vis[i])
            dfs(i);

    reverse(all(ans));
}
