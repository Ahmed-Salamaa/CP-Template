/*
 * Topic: Data Structure - Data Structure
 * Description: Implementation of mex
 */
#include "../core.h"

/*
 * MEX (Minimum Excluded Value) Data Structure
 * -----------------------------------------
 * Tracks the count of elements up to MaxValue and dynamically maintains the minimum non-negative integer not currently
 * present in the set. Relies on bitset for very fast operations. Best suited for limits like MaxValue <= 1e5 or 1e6.
 */
template <int MaxValue = 100005>
struct MEX {
    bitset<MaxValue> missing_elements;
    vector<int> count;

    // O ( MaxValue )
    MEX() : count(MaxValue, 0) { missing_elements.set(); }

    // O ( 1 )
    void add(int element) {
        if (element < MaxValue && ++count[element] == 1) missing_elements[element] = 0;
    }

    // O ( 1 )
    void remove(int element) {
        if (element < MaxValue && --count[element] == 0) missing_elements[element] = 1;
    }

    // O ( MaxValue / 64 ) - Very Fast Like O ( 1 )
    int get_mex() { return missing_elements._Find_first(); }
};
