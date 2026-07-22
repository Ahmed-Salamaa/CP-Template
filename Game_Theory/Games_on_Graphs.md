# 7. General Games on Graphs

## 7.1 Terminal States & Propagation
- Vertices with out-degree $0$ are losing.
- A state is **Winning** if it reaches at least one Losing state.
- A state is **Losing** if ALL transitions go to Winning states.

```cpp
#include "../core.h"

// O ( V + E )
void propagate_win_loss(int v, const vector<vector<int>>& adj_rev, vector<bool>& winning, vector<bool>& losing, vector<bool>& visited, vector<int>& degree) {
    visited[v] = true;
    for (int u : adj_rev[v]) {
        if (!visited[u]) {
            if (losing[v]) winning[u] = true;
            else if (--degree[u] == 0) losing[u] = true;
            else continue;
            propagate_win_loss(u, adj_rev, winning, losing, visited, degree);
        }
    }
}
```

## 7.2 Remoteness and Suspense
- **Remoteness (Minimax):** Min moves to force a win.
- **Suspense (Maximin):** Max moves to survive a loss.

```cpp
#include "../core.h"

// O ( V + E )
bool is_winning(int u, map<int, vector<int>>& graph, map<int, bool>& winmemo) {
    if (winmemo.count(u)) return winmemo[u];
    if (graph[u].empty()) return winmemo[u] = false;
    for (int v : graph[u]) if (!is_winning(v, graph, winmemo)) return winmemo[u] = true;
    return winmemo[u] = false;
}

// O ( V + E )
int remoteness(int u, map<int, vector<int>>& graph, map<int, bool>& winmemo, map<int, int>& rememo) {
    if (rememo.count(u)) return rememo[u];
    if (graph[u].empty() || !is_winning(u, graph, winmemo)) return rememo[u] = 0;
    int best = inf;
    for (int v : graph[u]) if (!is_winning(v, graph, winmemo)) best = min(best, remoteness(v, graph, winmemo, rememo));
    return rememo[u] = 1 + best;
}

// O ( V + E )
int suspense(int u, map<int, vector<int>>& graph, map<int, bool>& winmemo, map<int, int>& sememo) {
    if (sememo.count(u)) return sememo[u];
    if (graph[u].empty()) return sememo[u] = 0;
    int worst = 0;
    bool win_state = is_winning(u, graph, winmemo);
    for (int v : graph[u]) if (is_winning(v, graph, winmemo) != win_state) worst = max(worst, suspense(v, graph, winmemo, sememo));
    return sememo[u] = 1 + worst;
}
```

## 7.3 Super Win / Lose States
Tracking forced partisan-like constraints in impartial games.

```cpp
#include "../core.h"

const int SUPER_LOSING = 1e8;
const int SUPER_WINNING = SUPER_LOSING + 1;

// O ( states * moves )
int grundy_normal(int state, function<vector<int>(int)> get_moves, unordered_map<int, int>& memo) {
    if (memo.count(state)) return memo[state];
    auto moves = get_moves(state);
    if (moves.empty()) return memo[state] = SUPER_LOSING;
    unordered_set<int> next;
    for (int to : moves) {
        int val = grundy_normal(to, get_moves, memo);
        if (val == SUPER_LOSING) return memo[state] = SUPER_WINNING;
        if (val != SUPER_WINNING) next.insert(val);
    }
    int m = 0;
    while (next.count(m)) ++m;
    return memo[state] = m;
}

// O ( states * moves )
int grundy_misere(int state, function<vector<int>(int)> get_moves, unordered_map<int, int>& memo) {
    if (memo.count(state)) return memo[state];
    auto moves = get_moves(state);
    if (moves.empty()) return memo[state] = SUPER_WINNING;
    unordered_set<int> next;
    for (int to : moves) {
        int val = grundy_misere(to, get_moves, memo);
        if (val != SUPER_WINNING) next.insert(val);
    }
    int m = 0;
    while (next.count(m)) ++m;
    return memo[state] = m;
}
```

## 7.4 Bipartite Graph Games (Matching Strategy)
A token is on a vertex of a graph. Players take turns moving it to an adjacent unvisited vertex.
- **Trick:** If the graph is bipartite, you can solve this using Maximum Bipartite Matching.
- **Winning Condition:** The first player has a winning strategy if and only if the starting vertex is part of **EVERY** maximum matching. If there exists at least one maximum matching that *omits* the start vertex, the second player wins.

## 7.5 Flow Game Reductions
- **Rule:** Games with capacities, degree constraints, or tournament scenarios (like "Baseball Elimination") often reduce to **Max Flow** or **Min-Cost Max-Flow**.
- **Gadgets:** The game is usually impartial and deterministic. You build a graph where edges represent possible moves/wins, to check if a valid "flow" (sequence of non-violating moves) exists that satisfies all constraints.
