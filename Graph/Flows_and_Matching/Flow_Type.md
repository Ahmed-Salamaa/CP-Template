| Problem Type                                     | Graph Type               | Edge Weights | Goal                          | Recommended Algorithm         | Time Complexity            |
|--------------------------------------------------|--------------------------|--------------|-------------------------------|-------------------------------|----------------------------|
| Maximum matching                                 | Bipartite                | No           | Max number of matchings       | Hopcroft-Karp                 | O(√V · E)                  |
| Maximum matching (as max flow)                   | Bipartite or Directed    | No           | Max number of matchings       | Dinic                         | O(E · √V) or O(V² · E)     |
| Maximum matching                                 | General (non-bipartite)  | No           | Max number of matchings       | Blossom (Edmonds' Algorithm)  | O(V³)                      |
| Minimum cost matching                            | Bipartite                | Yes          | Min total weight              | Hungarian (Kuhn–Munkres)      | O(V³)                      |
| Maximum weight matching                          | Bipartite                | Yes          | Max total weight              | Hungarian (Kuhn–Munkres)      | O(V³)                      |
| General matching with augmenting paths           | General (non-bipartite)  | No           | Max matching                  | Blossom                       | O(V³)                      |
| Assignment problem (e.g. task to worker)         | Bipartite (n = m)        | Yes          | Optimal assignment            | Hungarian                     | O(V³)                      |
| Online matching or dynamic graph                 | Bipartite                | Varies       | Real-time updates             | Greedy or Incremental Method  | O(1)–O(E) per update       |
| Edge-disjoint paths / max # of s→t paths         | Directed Bipartite       | No           | Max number of disjoint paths  | Dinic / Ford-Fulkerson        | O(V² · E) or O(E · √V)     |

## Important Theorems and Concepts in Flows and Matching

In Competitive Programming, network flows and bipartite matchings are often used to solve problems that don't look like graph problems at first glance. These theorems provide equivalences that allow us to translate a difficult problem into a standard flow or matching problem.

### 1. Max-Flow Min-Cut Theorem
*   **Explanation:** In any flow network, the maximum amount of flow passing from the source to the sink is exactly equal to the total capacity of the edges in a minimum cut. A cut partitions the vertices into two sets (one containing the source, the other the sink), and its capacity is the sum of capacities of edges going from the source's set to the sink's set.
*   **Rule / Formula:** `Maximum Flow = Minimum Cut`
*   **Usage in CP:** Many problems ask for the "minimum cost to separate two things" or "minimum cost to break all paths". Instead of finding the cut directly, you build a flow network and find the Maximum Flow, which numerically gives the Minimum Cut. It is also used in "Project Selection" problems (choosing between two options with rewards and penalties).
*   **How to construct the Min-Cut Set:**
    1. Run your Max-Flow algorithm (e.g., Dinic's) to find the maximum flow.
    2. Perform a DFS or BFS from the Source `S` in the **residual graph**, traversing only edges with remaining positive capacity (> 0).
    3. The vertices that are reachable from `S` belong to the source set $A$. The unvisited vertices belong to the sink set $B$.
    4. The actual edges forming the Minimum Cut are the original graph edges that go from a visited vertex in $A$ to an unvisited vertex in $B$.


### 2. Kőnig's Theorem
*   **Explanation:** In any **bipartite graph**, the number of edges in a maximum matching is equal to the number of vertices in a minimum vertex cover. A vertex cover is a set of vertices such that every edge in the graph is incident to at least one vertex in the set.
*   **Rule / Formula:** `Maximum Bipartite Matching (MBM) = Minimum Vertex Cover (MVC)`
*   **Usage in CP:** When a problem asks to find the minimum number of items (vertices) to select such that all conditions (edges) are satisfied (covered), it's a Minimum Vertex Cover problem. If the graph is bipartite, you can solve it simply by finding the Maximum Bipartite Matching using Hopcroft-Karp or Dinic's algorithm.
*   **How to construct the Minimum Vertex Cover:**
    1. Find the Maximum Bipartite Matching.
    2. From all **unmatched** vertices in the Left partition, run a DFS/BFS along **alternating paths** (traverse from Left to Right using non-matching edges, and from Right to Left using matching edges).
    3. Mark all visited vertices.
    4. `Minimum Vertex Cover = (Unvisited Left Nodes) ∪ (Visited Right Nodes)`.

### 3. Maximum Independent Set (in Bipartite Graphs)
*   **Explanation:** An independent set is a set of vertices where no two vertices are adjacent (connected by an edge). In a bipartite graph, the size of the maximum independent set is related to the minimum vertex cover. Since every edge must be covered by the vertex cover, the remaining vertices will have no edges between them.
*   **Rule / Formula:** `Maximum Independent Set (MIS) = Total Vertices (V) - Minimum Vertex Cover (MVC)`
    Therefore, `MIS = V - Maximum Bipartite Matching (MBM)`
*   **Usage in CP:** Used when you need to select the maximum number of mutually non-conflicting elements. A classic example is placing the maximum number of non-attacking knights on a chessboard (which is a bipartite graph if colored black/white).
*   **How to construct the Maximum Independent Set:**
    *   Since it is exactly the complement of the Minimum Vertex Cover, you follow the same traversal steps as above.
    *   `Maximum Independent Set = (Visited Left Nodes) ∪ (Unvisited Right Nodes)`.

### 4. Minimum Edge Cover (in Bipartite Graphs)
*   **Explanation:** An edge cover is a set of edges such that every vertex in the graph is incident to at least one edge in the set. (Assumes no isolated vertices). The size of the minimum edge cover is also directly related to the maximum matching.
*   **Rule / Formula:** `Minimum Edge Cover = Total Vertices (V) - Maximum Bipartite Matching (MBM)`
*   **Usage in CP:** Used when a problem asks to pair up or cover all elements with the minimum number of groups or connections.
*   **How to construct the Minimum Edge Cover:**
    1. Take all the edges that are part of the Maximum Bipartite Matching.
    2. For every vertex that remains unmatched, arbitrarily pick exactly one edge incident to it and add it to your cover.

### 5. Dilworth's Theorem
*   **Explanation:** In any finite partially ordered set (poset), which can be represented as a Directed Acyclic Graph (DAG), the size of the maximum anti-chain is equal to the minimum number of chains (paths) needed to cover all elements. An anti-chain is a set of elements where no two are comparable (no path exists between them).
*   **Rule / Formula:** `Maximum Anti-chain = Minimum Path Cover in DAG`
*   **Usage in CP:** The Minimum Path Cover in a DAG can be found using bipartite matching. You split every vertex `v` into `v_in` and `v_out`. An edge `u -> v` becomes an edge from `u_out` to `v_in`. 
    `Minimum Path Cover = V - Maximum Bipartite Matching in the split graph`. This is heavily used in scheduling problems, like minimizing the number of taxis needed to fulfill a set of scheduled rides.
*   **How to construct the paths:**
    The matching in the split graph tells you the edges of the paths. If `u_out` is matched with `v_in`, it means the directed edge `u -> v` is part of a path. You can trace these matched edges to reconstruct the full paths.

### 6. Hall's Marriage Theorem
*   **Explanation:** For a bipartite graph $G = (U \cup V, E)$, there exists a perfect matching that covers every vertex in $U$ if and only if for every subset $S \subseteq U$, the size of its neighborhood $N(S)$ (the set of all vertices in $V$ adjacent to at least one vertex in $S$) is at least as large as $S$ itself.
*   **Rule / Formula:** `$|N(S)| \ge |S|$ for all $S \subseteq U$`
*   **Usage in CP:** Rarely used directly to *compute* a matching, but extremely useful for theoretical proofs, or when dealing with continuous ranges/intervals where checking the condition directly is faster than running a flow algorithm. Often tested in Data Structure + Flow hybrid problems (e.g., checking Hall's condition using Segment Trees).

### 7. Classic CP Tricks & Problem Styles (Codeforces, ICPC, ACPC, ECPC)
*   **Grid as a Bipartite Graph:** A very common trick in grid problems (like placing dominoes or non-attacking pieces) is coloring the grid like a chessboard. Cells where `(row + col)` is even form the Left partition, and `(row + col)` is odd form the Right partition. Then, you can apply Bipartite Matching or Maximum Independent Set.
*   **Project Selection / Max-Weight Closure:** You have a set of "Projects" that give profit but require certain "Tools" which cost money. This is modeled using Max-Flow Min-Cut: Connect Source to Projects (capacity = profit), Tools to Sink (capacity = cost), and infinite capacity edges from Projects to required Tools. `Max Profit = Sum of all Positive Profits - Max Flow (Min Cut)`.
*   **B-Matching / Circulation:** Sometimes a node in the left set can be matched with up to $k_1$ nodes, and a right node with $k_2$ nodes. Instead of Hopcroft-Karp, use Max Flow by setting the capacity of Source $\to$ Left to $k_1$, and Right $\to$ Sink to $k_2$.
*   **DAG vs General Graph in Dilworth:** Dilworth's theorem applies strictly to DAGs. If the problem has a general directed graph with cycles, you must first condense it by finding Strongly Connected Components (using Tarjan's or Kosaraju's) to form a DAG of SCCs before applying Dilworth's.
