# 1. Fundamentals & Sprague-Grundy Theorem

## Impartial vs Partisan Games
- **Impartial Games:** The set of valid moves from any given position is exactly the same for both players. (CP almost exclusively deals with impartial games).
- **Partisan Games:** The valid moves depend on whose turn it is (e.g., Chess).

## Normal vs Misère Play
- **Normal Play:** The last player to make a valid move **wins**.
- **Misère Play:** The last player to make a valid move **loses**.

## Sprague-Grundy Theorem
Every impartial game under normal play is equivalent to a Nim pile of a certain size (Grundy value).
- **MEX (Minimum Excluded Value):** $G(S) = \text{MEX}( \{ G(S_1), G(S_2), \dots, G(S_k) \} )$
- **Combining Games:** $G_{\text{total}} = G(\text{game}_1) \oplus G(\text{game}_2) \oplus \dots \oplus G(\text{game}_n)$

```cpp
#include "../core.h"

// O ( N )
int nim_value(const vector<int>& piles) {
    int x = 0;
    for (int p : piles) x ^= p;
    return x;
}

// O ( N )
int get_mex(const unordered_set<int>& S) {
    int m = 0;
    while (S.count(m)) ++m;
    return m;
}

// O ( states * moves )
int compute_grundy(int h, const vector<int>& moves, vector<int>& dp) {
    if (dp[h] != -1) return dp[h];
    unordered_set<int> S;
    for (int mv : moves) if (h >= mv) S.insert(compute_grundy(h - mv, moves, dp));
    return dp[h] = get_mex(S);
}
```

## When to ignore Sprague-Grundy
- **Partisan Games:** If Alice can only move red pieces and Bob can only move blue pieces, Sprague-Grundy **DOES NOT APPLY**. You must use standard Minimax DP, Alpha-Beta pruning, or Ad-hoc logic.
- **Loops/Draws:** Sprague-Grundy assumes games are finite and must end. If players can move in circles forever (causing a Draw), you cannot use standard MEX. You must use the Remoteness/Suspense graph algorithms.

## Common Grundy Value Patterns (Cheat Sheet)
Instead of running $O(N^2)$ DP for MEX, many standard game rules produce predictable, $O(1)$ mathematical patterns for $G(N)$. Always print the first 50 values of your DP to look for these patterns!

### 1. Subtraction Rules
- **Take any number of stones (Standard Nim):** 
  $G(N) = N$
- **Take $1$ to $K$ stones (Bachet's Game):** 
  $G(N) = N \pmod{K + 1}$
- **Take any ODD number of stones:** 
  $G(N) = N \pmod 2$
- **Take any EVEN number of stones:** 
  $G(N) = 0$ if $N$ is odd, $G(N) = N / 2$ if $N$ is even.
- **Take a Power of $K$ stones ($1, K, K^2, \dots$):**
  - If $K$ is **odd**: $G(N) = N \pmod 2$
  - If $K$ is **even**: $G(N) = N \pmod{K + 1}$

### 2. Division & Factor Rules
- **Divide $N$ by any of its divisors $D > 1$ (replace $N$ with $N/D$):**
  $G(N) = \text{Total number of prime factors of } N \text{ (with multiplicity)}$. 
  *(Example: $N = 12 = 2^2 \times 3^1 \implies G(12) = 2 + 1 = 3$. Dividing by $D$ is just removing prime factors, making it exactly Standard Nim).*
- **Take at most half the stones ($1 \le X \le \lfloor N/2 \rfloor$):**
  $G(N) = \lfloor \log_2 N \rfloor$. (The position of the most significant bit).

### 3. Splitting Rules (Lasker's Nim)
- **Take any number of stones OR split a pile into two non-empty piles:**
  - $N \equiv 1 \text{ or } 2 \pmod 4 \implies G(N) = N$
  - $N \equiv 3 \pmod 4 \implies G(N) = N + 1$
  - $N \equiv 0 \pmod 4 \implies G(N) = N - 1$
