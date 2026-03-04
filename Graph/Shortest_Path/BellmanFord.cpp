// Overall Time Complexity: O(n * m)
// Overall Space Complexity: O(n + m)
template <typename T = int>
struct BellmanFord
{
    // Edge Structure
    struct Edge
    {
        T U, V, W;
        Edge(T u, T v, T w) : U(u), V(v), W(w) {}
    };

    // default constructor
    BellmanFord() = default;

    bool WasProcessed = false;
    T n, m, INF = 1LL << 60;
    vector<T> dist, par;
    vector<Edge> edges;

    // add edge
    // Time Complexity: O(1)
    void addEdge(T u, T v, T w)
    {
        edges.push_back({u, v, w});
    }

    // constructor to initialize the graph
    // Time Complexity: O(m)
    BellmanFord(T n, T m) : n(n), m(m), dist(n + 1, INF), par(n + 1, -1)
    {
        for (T i = 0, u, v, w; i < m && cin >> u >> v >> w; ++i)
        {
            addEdge(u, v, w);
        }
    }

    // return shortest path from source to all nodes
    // Time Complexity: O(n * m) in the worst case
    vector<T> shortestPath(T source)
    {
        dist.assign(n + 1, INF); 
        dist[source] = 0;
        WasProcessed = true;
        
        for (T i = 0; i < n - 1; ++i)
        {
            bool updated = false; 
            for (auto &e : edges)
            {
                // Check if e.U is actually reachable before relaxing
                if (dist[e.U] != INF && dist[e.U] + e.W < dist[e.V])
                {
                    dist[e.V] = dist[e.U] + e.W;
                    par[e.V] = e.U;
                    updated = true;
                }
            }
            if (!updated)
                break; // Early exit optimization: Best case becomes O(m)
        }
        return dist;
    }

    // return shortest path from source to target
    // Time Complexity: O(n * m)
    T shortestPath(T source, T target)
    {
        return shortestPath(source)[target];
    }

    // 1. Detect ANY loop reachable from the source
    // Time Complexity: O(n * m) if shortestPath hasn't run yet, otherwise O(m)
    bool negativeCycle(T source = 1)
    {
        if (!WasProcessed)
            shortestPath(source);
            
        for (auto &e : edges)
        {
            // Check if reachable and if it still relaxes
            if (dist[e.U] != INF && dist[e.U] + e.W < dist[e.V])
                return true; 
        }
        return false;
    }

    // 2. Detect a loop ONLY if it affects the specific path from source to target
    // Time Complexity: O(n * m)
    bool negativeCycle(T source, T target)
    {
        if (!WasProcessed)
            shortestPath(source);
            
        vector<bool> in_neg_cycle(n + 1, false);
        
        // Propagate the cycle N times to ensure the effect reaches the target
        for (T i = 0; i < n; ++i)
        {
            for (auto &e : edges)
            {
                if (dist[e.U] != INF && dist[e.U] + e.W < dist[e.V])
                {
                    dist[e.V] = dist[e.U] + e.W;
                    in_neg_cycle[e.V] = true; 
                }
                // If the parent is in a negative cycle, the child is too
                if (in_neg_cycle[e.U]) 
                {
                    in_neg_cycle[e.V] = true;
                }
            }
        }
        return in_neg_cycle[target];
    }

    // return the path from source to target
    // Time Complexity: O(n * m) to compute paths, then O(n) to reconstruct
    vector<T> path(T source, T target)
    {
        if (!WasProcessed)
            shortestPath(source);
        vector<T> p;
        for (T i = target; i != -1; i = par[i])
            p.push_back(i);
        reverse(all(p));
        return p;
    }
};