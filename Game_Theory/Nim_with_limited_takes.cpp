#include <bits/stdc++.h>
using namespace std;

/*
    ===========================================================================
    Complexity Analysis:
    Let 'm' be the number of allowed subtraction moves (size of arr).
    Let 'M' be the maximum allowed subtraction value (MAX_M).
    Let 'μ' (mu) be the length of the pre-period (before the cycle starts).
    Let 'λ' (lambda) be the length of the repeating cycle.

    Overall Time Complexity: O( ( μ + λ ) * m )
    - Finding the cycle takes O(μ + λ) states, and each state takes O(m) to compute.
    - Simulating the reduced target takes at most O(μ + λ) states.
    Because (μ + λ) is generally small for subtraction games, this easily 
    handles massive values of 'n' (e.g., n = 10^18).

    Overall Space Complexity: O(m + M)
    - The array stores 'm' elements.
    - The bitset state takes 'M' bits. 
    - Auxiliary space is O(1) beyond the bitset and array.
    ===========================================================================
*/

// Set this to the maximum possible subtraction value in your array.
// std::bitset allows us to easily exceed the 60-bit limit of 'long long'.
const int MAX_M = 1005 ;            // max element in array
using State = bitset <MAX_M> ;

vector <int> arr ;

// Generates the next win/loss state based on the sliding window of previous states
State next_state( const State & mask )
{
    // Shift the window left by 1. 
    // This makes room at index 0 for the state we are about to calculate.
    // The oldest state falls off the end automatically in a bitset.
    State new_mask = mask << 1 ;

    // We want to determine if the CURRENT state is a winning state (1) or losing state (0).
    for (const auto& m : arr) 
    {
        if ( m > MAX_M ) continue ; // Safety check

        // In Game Theory (Nim variants), a state is a WINNING state if there is 
        // AT LEAST ONE valid move that forces the opponent into a LOSING state.
        // mask[m - 1] looks exactly 'm' steps backward. 
        // If it equals 0, that historical state was a losing state.
        if ( mask[ m - 1 ] == 0 ) 
        {
            new_mask[0] = 1 ; // Mark current state as a win!
            break ;           // We found a winning move, no need to check others.
        }
    }

    return new_mask;
}

// Floyd's Tortoise and Hare Algorithm to find the pre-period and cycle length
pair < int , int > floyed() 
{
    // We start with an empty state (all zeros)
    State slow = 0 ;
    State fast = 0 ;

    // Phase 1: Finding the intersection point.
    // The 'fast' pointer moves twice as fast as the 'slow' pointer.
    // Because the number of possible states is finite, they are guaranteed to meet in a cycle.
    do
    {
        slow = next_state(slow);
        fast = next_state(next_state(fast));
    } while ( fast != slow );

    int before = 0 ;
    int cycle = 1 ;

    // Phase 2: Finding the start of the cycle (the pre-period length).
    // If we reset 'slow' to the beginning and move both pointers at the same speed,
    // they will meet exactly at the start of the cycle.
    slow = 0 ;
    while ( slow != fast ) 
    {
        before ++ ;
        slow = next_state(slow);
        fast = next_state(fast);
    }
    
    // Phase 3: Finding the length of the cycle.
    // We keep 'slow' stationary at the start of the cycle and move 'fast' 
    // until it wraps around and meets 'slow' again.
    fast = next_state(fast);
    while ( slow != fast )
    {
        fast = next_state(fast);
        cycle ++ ;
    }

    // Return the number of elements before the cycle begins, and the length of the cycle.
    return { before , cycle } ;
}

signed main() {

    int t = 1 ; 
    // cin >> t ;
    while ( t -- )
    {
        int n , m ; cin >> n >> m ;
        arr.assign( m , 0 ) ; cin >> arr ;
        sort ( all ( arr ) ) ;

        auto [ before , cycle ] = floyed() ;

        // 1. Reduce n using the pre-period (before) and the period (cycle)
        int target = n ;
        if ( n >= before )
            target = before + ( n - before ) % cycle ;

        // 2. Simulate the game up to the reduced target using bitset
        State mask = 0; // Initialize an empty bitset (all 0s)
        for (int i = 0; i < target; i++) {
            mask = next_state(mask);
        }

        // 3. The 0-th bit of the bitset represents the win/loss state for 'target' (and thus 'n')
        // 1 means the first player has a guaranteed winning strategy.
        // 0 means it's a losing state (second player wins).
        if (mask[0] == 1) {
            cout << "First\n"; // Or whatever your specific problem expects for a Win
        } else {
            cout << "Second\n"; // Or whatever your specific problem expects for a Loss
        }
    }

    return 0;
}