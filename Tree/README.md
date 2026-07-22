# Tree Algorithms

| Template | When to use | Time Complexity | Notes |
| :--- | :--- | :--- | :--- |
| **`LCA.cpp`** (Binary Lifting) | Finding the Lowest Common Ancestor of two nodes, or the distance between them. | Build: $O(N \log N)$, Query: $O(\log N)$ | The standard for tree queries. |
| **`HLD.cpp`** (Heavy-Light Decomposition) | When you need to do **Segment Tree updates/queries on a path** between two nodes in a tree. | Update/Query: $O(\log^2 N)$ | Flattens tree paths into arrays. Very powerful. |
| **`Euler_Tour.cpp`** | When you need to do **Segment Tree updates/queries on a subtree** (e.g., add $X$ to all nodes in subtree of $U$). | Build: $O(N)$, Query: $O(\log N)$ | Flattens subtrees into contiguous array segments. |
| **`DSU_on_Tree.cpp`** (Sack) | When you need to answer offline queries about subtrees (e.g., count distinct colors in subtree) and Mo's is too slow. | $O(N \log N)$ | Much faster than Mo's algorithm on trees. |
| **`centroid_decomposition.cpp`** | When you need to solve "all pairs" path problems on a tree (e.g., number of paths of length $K$). | Build: $O(N \log N)$ | Divides the tree recursively by removing the centroid. |
