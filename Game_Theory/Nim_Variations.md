# 3. Nim and its Variations

## 3.1 Standard Nim & Misère Nim
$N$ piles of stones. A player can remove any positive number of stones from exactly one pile.
- **Normal Play:** A position is losing if the XOR sum $S = P_1 \oplus P_2 \dots = 0$.
  - **Winning Move ($O(1)$):** If $S \ne 0$, find any pile $P_i$ where $P_i \oplus S < P_i$. This condition is true **if and only if** the Most Significant Bit (MSB) of $S$ is also set (is `1`) in $P_i$. Reduce that pile to $P_i \oplus S$.
- **Misère Play (Bouton's Theorem):** Play exactly like Normal Nim, UNLESS the move leaves ONLY piles of size 1 or 0.
  - **The Edge Case ($O(1)$):** Count the number of piles with size $> 1$. If this count is `0`, the first player wins if the number of piles of size $1$ is **EVEN** (forcing the opponent to take the last one). Otherwise, play normally ($S \ne 0$).

### Nim-Sum Properties ($X \oplus X = 0$)
- If the XOR sum $X \neq 0$, there is *always* at least one pile $P_i$ such that $P_i \oplus X < P_i$.
- **Finding Winning Moves:** The number of different winning first moves in standard Nim is exactly the count of piles where $P_i \oplus X < P_i$.
- If you can add a pile to a Nim game, and you want the First Player to lose, you must add a pile of size exactly equal to the current XOR sum $X$ (because $X \oplus X = 0$).

## 3.2 Nim with Limited Takes (Bachet's Game)
Players can take between $1$ and $K$ stones. Grundy value is $G(N) = N \pmod{K + 1}$.

### Take $1$ or $K$ Chips (Parity & Cycle Trick)
A single pile of $N$ chips. A player can take either $1$ or $K$ chips. First to take the last chip wins.
- **Case 1: $K$ is ODD ($O(1)$):**
  - **Logic:** Both available moves ($1$ and $K$) are odd, meaning every turn strictly flips the parity of the pile (even to odd, odd to even).
  - **First Player Wins if:** $N$ is Odd.
- **Case 2: $K$ is EVEN ($O(1)$):**
  - **Logic:** Winning and losing states repeat in a consistent cycle of length $K+1$.
  - **Formula:** Find the remainder $R = N \pmod{K + 1}$.
  - **First Player Wins if:** $R$ is Odd OR $R == K$.

*(Note: If the problem alters the initial input $N$ — e.g., scaling it up — apply that math to $N$ before evaluating these rules!)*

If subtraction rules are arbitrary, we can use Bitsets to find cycles rapidly for massive constraints.
```cpp
#include "../core.h"

// Nim with limited takes (Arbitrary Subtraction Game)
const int MAX_SUBTRACTION = 1005;
using NimState = bitset<MAX_SUBTRACTION>;

// O ( |arr| )
NimState next_nim_state(const NimState& mask, const vector<int>& arr) {
    NimState new_mask = mask << 1;
    for (const auto& m : arr) {
        if (m > MAX_SUBTRACTION) continue;
        if (mask[m - 1] == 0) { new_mask[0] = 1; break; }
    }
    return new_mask;
}

// O ( pre_period + cycle_length * |arr| )
pair<int, int> find_nim_cycle(const vector<int>& arr) {
    NimState slow = 0, fast = 0;
    do {
        slow = next_nim_state(slow, arr);
        fast = next_nim_state(next_nim_state(fast, arr), arr);
    } while (fast != slow);
    int before = 0, cycle = 1;
    slow = 0;
    while (slow != fast) {
        before++;
        slow = next_nim_state(slow, arr);
        fast = next_nim_state(fast, arr);
    }
    fast = next_nim_state(fast, arr);
    while (slow != fast) {
        fast = next_nim_state(fast, arr);
        cycle++;
    }
    return {before, cycle};
}
```

## 3.3 Moore's Nim_k & Staircase Nim
- **Moore's Nim_k:** Remove from at most $K$ piles. Write sizes in binary. If the sum of $1$s at *every* bit position is a multiple of $(K+1)$, it's losing.
- **Staircase Nim:** Move stones to lower steps. Treat stones on **odd-numbered steps** as standard Nim. Even steps act as reversible "garbage" zones.

## 3.4 Fibonacci Nim
A single pile of $N$ stones. The first player can take $1$ to $N-1$ stones. In subsequent turns, a player can take up to **$2 \times$ the amount the previous player took**.
- **$O(1)$ Win Condition:** The first player loses if and only if $N$ is a **Fibonacci number**.
- **Winning Move (Zeckendorf's Theorem):** If $N$ is not Fibonacci, uniquely decompose $N$ into a sum of non-consecutive Fibonacci numbers. The winning move is to remove exactly the **smallest Fibonacci number** in this Zeckendorf representation.

## 3.5 Turning Turtles
A line of coins (Heads and Tails). You can pick a Head, flip it to Tail, and *optionally* flip any one coin to its left.
- **Logic:** Equivalent to playing Nim where every Head at 0-indexed position `i` acts as a Nim pile of size `i`.

```cpp
// O ( N )
int turning_turtles_game(const string &s) {
    int xor_sum = 0;
    for(int i = 1; i <= s.size(); ++i) if (s[i - 1] == 'H') xor_sum ^= (i - 1);
    return xor_sum ? 1 : 2; // 1 means first player wins, 2 means second wins
}
```

## 3.5 Silver Dollar Game
A semi-infinite tape of squares. Some squares have coins. You can move any coin leftwards, without jumping over other coins. No square can have >1 coin.
- **Logic:** Pairing the coins from the right side. The number of empty spaces between pairs acts exactly like Nim piles.

```cpp
// O ( N )
int silver_dollar_game(vector<int>& locations) {
    if (locations.size() & 1) locations.insert(locations.begin(), 0);
    int xor_sum = 0;
    for (int i = 0; i < locations.size(); i += 2) xor_sum ^= (locations[i + 1] - locations[i] - 1);
    return xor_sum ? 1 : 2;
}
```
