# 5. Splitting Games (Dawson's Kay & Kayles)

Games where a single move breaks a component into two smaller independent components. Usually requires DP to find periodic sequences (Octal Games).

- **Kayles (Octal .77):** You can knock down 1 bowling pin, or 2 adjacent bowling pins. Knocking down pins splits the row into two smaller rows.
  - $G(N) = \text{MEX}( \{ G(i) \oplus G(N-i-1) \} \cup \{ G(j) \oplus G(N-j-2) \} )$
  - **Periodicity:** Exact period length of **12** (starting after a pre-period of length 70).
- **Dawson's Chess (Octal .137):** You capture a pawn, removing it and making the adjacent squares unplayable.
  - $G(N) = \text{MEX}( \{ G(i) \oplus G(N - i - 3) \} )$ for $0 \le i \le N-3$.
  - **Periodicity:** Exact period length of **34** (Dawson's Kayles behaves identically).

## Periodicity in Splitting & Subtraction Games
In games where a pile of size $N$ splits into smaller piles, or decreases by specific sets of numbers (like primes, or powers of 2):
- The sequence of Grundy values $G(0), G(1), G(2), \dots$ will **always** eventually become periodic.
- If $N$ is massive (e.g., $10^{18}$), you only need to compute $G(i)$ using DP up to a few thousand (e.g., $N=2000$). Then, write a loop to find the cycle length (period) and the pre-period.
- The answer for $N$ is then just $G(\text{pre\_period} + (N - \text{pre\_period}) \pmod{\text{period}})$.
