# Miscellaneous Snippets

This document contains several small but highly useful algorithms and code snippets that are commonly used in competitive programming.

## Base Conversion

### Mathematical Explanation
Base conversion (or radix conversion) is the process of translating a number from one base (or radix) to another. The base dictates the number of unique digits (including zero) used to represent numbers. The standard numeral system is decimal (base 10), which uses digits 0 through 9.

**Decimal to Any Base**
To convert a number from decimal (base $10$) to another base $B$, we use the **successive division method**.
Let $N$ be our decimal number. The representation of $N$ in base $B$ can be written as a polynomial:
$$ N = d_k \cdot B^k + d_{k-1} \cdot B^{k-1} + \dots + d_1 \cdot B^1 + d_0 \cdot B^0 $$
where $d_i \in \{0, 1, \dots, B-1\}$.

If we divide $N$ by $B$, the quotient is:
$$ q_1 = d_k \cdot B^{k-1} + \dots + d_1 $$
And the remainder is $d_0$. 
Thus, $N \pmod B$ gives the least significant digit, $d_0$. We can repeat this process on the quotient $q_1$ to find $d_1$, and so on, until the quotient becomes $0$.

**Any Base to Decimal**
To convert a number from base $B$ to decimal, we evaluate the polynomial directly.
Given a string of digits $S = d_k d_{k-1} \dots d_1 d_0$ in base $B$, its decimal value is:
$$ \sum_{i=0}^{k} d_i \cdot B^i $$
We can compute this iteratively. To do it efficiently from left to right (most significant digit to least), we can use Horner's method:
$$ N = (...((d_k \cdot B + d_{k-1}) \cdot B + d_{k-2}) \cdot B \dots ) \cdot B + d_0 $$
Alternatively, computing from right to left, we maintain the current power of $B$ and multiply it by the corresponding digit, summing them all.

### Complexity
- **Time Complexity:** $\mathcal{O}(\log_B(N))$ for both operations, where $N$ is the value being converted. This is because the number of digits in base $B$ is bounded by $\lfloor \log_B(N) \rfloor + 1$.
- **Space Complexity:** $\mathcal{O}(\log_B(N))$ to store the resulting string representation.

### C++ Code
```cpp
// Convert Decimal to any base
string decimal_to_any_base(long long decimal, int base) {
    if (decimal == 0) return "0";
    string num = "0123456789ABCDEFGHIJKLMNOPQRSTUVWXYZ";
    string result;
    do {
        result.push_back(num[decimal % base]);
        decimal /= base;
    } while (decimal != 0);
    return string(result.rbegin(), result.rend());
}

// Convert any base to decimal
long long any_base_to_decimal(string str, int base) {
    auto val = [](char c) { 
        return (c >= '0' && c <= '9' ? (int)(c - '0') : (int)(c - 'A' + 10)); 
    };
    long long len = str.length(), power = 1, num = 0;
    for (int i = len - 1; i >= 0; i--) {
        num += val(str[i]) * power;
        power = power * base;
    }
    return num;
}
```

## Gray Code

### Mathematical Explanation
The **Gray Code** (or reflected binary code) is an ordering of the binary numeral system such that two successive values differ in only one bit (binary digit). For example, the 2-bit Gray code sequence is $00, 01, 11, 10$.

**Constructing Gray Code**
Let $G(n)$ be the $n$-th Gray code. A well-known mathematical formula to find the $n$-th Gray code directly from the integer $n$ (0-indexed) is:
$$ G(n) = n \oplus \left\lfloor \frac{n}{2} \right\rfloor $$
In C++, this is simply `n ^ (n >> 1)`.

*Proof:*
Let the binary representation of $n$ be $b_k b_{k-1} \dots b_0$.
The binary representation of $n/2$ is $0 b_k b_{k-1} \dots b_1$.
Then the $i$-th bit of $G(n)$ is $g_i = b_i \oplus b_{i+1}$.
When we increment $n$ to $n+1$, only the least significant bits up to the first $0$ in $n$ are flipped. Let the lowest $0$ in $n$ be at position $j$. 
In $n+1$, bits $0$ to $j-1$ become $0$, and bit $j$ becomes $1$. 
Consequently, in the calculation of $G(n+1)$, only the bit at position $j$ will differ from $G(n)$. Thus, adjacent numbers differ by exactly one bit.

**Inverse Gray Code**
To find $n$ given $G(n)$, we can reverse the process. Since $g_i = b_i \oplus b_{i+1}$, and knowing that the most significant bit is the same ($g_k = b_k$ because $b_{k+1} = 0$), we can deduce:
$$ b_k = g_k $$
$$ b_{k-1} = b_k \oplus g_{k-1} $$
$$ b_{k-2} = b_{k-1} \oplus g_{k-2} $$
In general, $b_i = b_{i+1} \oplus g_i$. Expanding this recursively, we see that $b_i = \bigoplus_{j=i}^{k} g_j$.
This translates to XOR-ing $G(n)$ with all its shifted versions until it becomes $0$.

### Complexity
- **Time Complexity:** $\mathcal{O}(1)$ for converting $n$ to Gray code, and $\mathcal{O}(B)$ for reversing it, where $B$ is the number of bits (at most $32$ or $64$).
- **Space Complexity:** $\mathcal{O}(1)$ auxiliary space.

### C++ Code
```cpp
// Returns the n-th Gray code (two successive values differ by exactly one bit)
int g(int n) { 
    return n ^ (n >> 1); 
}

// Given a Gray code, restores the original number n
int rev_g(int g) {
    int n = 0;
    for (; g; g >>= 1) {
        n ^= g;
    }
    return n;
}
```

## Counting Inversions

### Mathematical Explanation
An **inversion** in an array $A$ is a pair of indices $(i, j)$ such that $i < j$ and $A[i] > A[j]$. The total number of inversions provides a measure of how far the array is from being sorted. For example, a completely sorted array has $0$ inversions, while an array sorted in reverse order has $\frac{n(n-1)}{2}$ inversions.

While one can count inversions in $\mathcal{O}(n \log n)$ time using Merge Sort or a Fenwick Tree / Segment Tree, an elegant and easy-to-implement method in C++ involves using the **Policy-Based Data Structures (PBDS)**. Specifically, the `tree` data structure from `__gnu_pbds` configured as an **order statistic tree**.

This tree supports standard `std::set` operations, plus two special operations in $\mathcal{O}(\log n)$ time:
1. `find_by_order(k)`: Returns an iterator to the $k$-th smallest element (0-indexed).
2. `order_of_key(k)`: Returns the number of items strictly smaller than $k$.

**Algorithm**
To find the number of inversions:
Iterate through the array from right to left (from $n-1$ down to $0$). When we are at index $i$, all elements we have processed so far are to the right of $i$ (i.e., they have index $j > i$). 
If we maintain these processed elements in our PBDS set, then `order_of_key(A[i])` will return the exact count of elements strictly smaller than $A[i]$ that appear after $A[i]$. This count is precisely the number of $j$ such that $i < j$ and $A[i] > A[j]$. We add this to our total answer and then insert $A[i]$ into the set.

*(Note: If the array contains duplicate elements, one must use a less_equal comparator or store pairs `{A[i], index}` to maintain a multiset behavior).*

### Complexity
- **Time Complexity:** $\mathcal{O}(n \log n)$, as we perform $n$ insertions and queries on a balanced binary search tree, each taking $\mathcal{O}(\log n)$ time.
- **Space Complexity:** $\mathcal{O}(n)$ to store the elements in the order statistic tree.

### C++ Code
```cpp
#include <ext/pb_ds/assoc_container.hpp>
#include <ext/pb_ds/tree_policy.hpp>
using namespace __gnu_pbds;
using namespace std;

template <typename T> 
using o_set = tree<T, null_type, less<T>, rb_tree_tag, tree_order_statistics_node_update>;

// Function to count the number of inversions in an array
long long count_inversions(const vector<int>& a) {
    int n = a.size();
    o_set<int> se;
    long long ans = 0;
    
    // Iterate from right to left
    for (int i = n - 1; i >= 0; i--) {
        // order_of_key(x) returns the number of elements strictly less than x
        ans += se.order_of_key(a[i]);
        se.insert(a[i]);
    }
    return ans;
}
```
