/*
 * Topic: Data Structure - Data Structure
 * Description: Implementation of BinaryTrie array
 */
#include "../core.h"

/*
 * Binary Trie
 * -----------
 * A specialized Trie used for bitwise operations, primarily for finding maximum/minimum XOR pairs.
 * It is array-based for maximum speed and memory compactness.
 */
struct BinaryTrie_array {
    int max_bit;
    vector<array<int, 2>> child_node;
    vector<int> frequency;

    // O ( 1 )
    BinaryTrie_array(int max_bit = 30) : max_bit(max_bit), child_node(1, {0, 0}), frequency(1, 0) {}

    // O ( max_bit )
    void insert(int number) {
        int current_node = 0;
        for (int i = max_bit; i >= 0; i--) {
            int bit = (number >> i) & 1;
            if (!child_node[current_node][bit]) {
                child_node[current_node][bit] = child_node.size();
                child_node.push_back({0, 0});
                frequency.push_back(0);
            }
            current_node = child_node[current_node][bit];
            frequency[current_node]++;
        }
    }

    // O ( max_bit )
    void erase(int number) {
        int current_node = 0;
        for (int i = max_bit; i >= 0; i--) {
            int bit = (number >> i) & 1;
            if (!child_node[current_node][bit]) return;
            current_node = child_node[current_node][bit];
            frequency[current_node]--;
        }
    }

    // O ( max_bit )
    int get_max_xor(int number) {
        int current_node = 0, answer = 0;
        for (int i = max_bit; i >= 0; i--) {
            int bit = (number >> i) & 1;
            if (child_node[current_node][bit ^ 1] && frequency[child_node[current_node][bit ^ 1]]) {
                answer |= (1LL << i);
                current_node = child_node[current_node][bit ^ 1];
            } else {
                current_node = child_node[current_node][bit];
            }
        }
        return answer;
    }

    // O ( max_bit )
    int get_min_xor(int number) {
        int current_node = 0, answer = 0;
        for (int i = max_bit; i >= 0; i--) {
            int bit = (number >> i) & 1;
            if (child_node[current_node][bit] && frequency[child_node[current_node][bit]]) {
                current_node = child_node[current_node][bit];
            } else {
                answer |= (1LL << i);
                current_node = child_node[current_node][bit ^ 1];
            }
        }
        return answer;
    }
};
