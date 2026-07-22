# General Game Theory Heuristics

While the formal theories (Sprague-Grundy, Combinatorial Games) solve exact impartial games, many Competitive Programming game theory problems can be solved using simple logical observations without writing heavy Grundy DP. Here are some of the most common CP tricks:

## 1. The Symmetry (Copycat) Strategy
Often, a game state is perfectly symmetric, or the First Player can make exactly one move to *make* it perfectly symmetric.
- **Example:** Placing non-overlapping coins on a circular table. 
  - **Strategy:** The First Player places a coin exactly in the center. The board is now perfectly symmetric. Whenever the Second Player places a coin, the First Player places a coin in the exact mirrored position on the opposite side. First Player guarantees a win.
- **Example:** Two identical piles of stones, you can take any amount from one pile. 
  - **Strategy:** This is Nim with $P_1 = P_2$. The XOR sum is always 0, so the Second Player wins by simply taking the exact same number of stones from the other pile that the First Player just took.

## 2. Parity Arguments
If every valid move strictly decreases the "size" of the game by exactly 1, and the game must end, the winner is determined entirely by the parity (odd/even) of the initial state.
- **Example:** You have a grid with $K$ empty cells. A move consists of coloring exactly 1 cell.
  - If $K$ is odd, First Player wins. If $K$ is even, Second Player wins.
- **Always Check:** Before writing a complex DP, ask: "Can the game just be boiled down to checking if $N \pmod 2 == 1$?"
