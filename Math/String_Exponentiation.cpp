#include <iostream>
#include <vector>
#include <string>

using namespace std;

/*
    Generic String Exponentiation
    -----------------------------
    - Calculates (Base ^ Exponent_String) in any numeral base.
    - Works for any data type with an overloaded '*' operator.
    - This template inherently supports any modulus 
    - TIME COMPLEXITY: O(L * B * O(mult))
      Where:
        L = The length of the exponent string.
        B = The numeral base of the string (e.g., 10 for decimal, 2 for binary).
        O(mult) = The time complexity of the '*' operator of your data structure 
*/

// Converts a character (0-9, A-Z, a-z) to its integer value
int char_to_val(char c) {
    if (c >= '0' && c <= '9') return c - '0';
    if (c >= 'A' && c <= 'Z') return c - 'A' + 10;
    if (c >= 'a' && c <= 'z') return c - 'a' + 10;
    return 0;
}

// Standard binary exponentiation for generic type T
template <typename T>
T binpow(T base, long long exp, T identity) {
    T res = identity;
    while (exp > 0) {
        if (exp & 1) res = res * base;
        base = base * base;
        exp >>= 1;
    }
    return res;
}

/*
    Calculates (a ^ exp_str)
    - T a          : The base value being multiplied (integer, matrix, or custom struct).
    - exp_str      : The massive exponent represented as a string.
    - numeral_base : The numeral base of the exponent string (e.g., 2, 10, 16).
    - identity     : The identity element for multiplication (e.g., 1 for ints, Identity Matrix for matrices).
*/
template <typename T>
T string_pow(T a, const string& exp_str, int numeral_base, T identity) {
    // Precompute a^0, a^1, ..., a^(numeral_base - 1)
    vector<T> p_base(numeral_base, identity);
    for (int i = 1; i < numeral_base; ++i) {
        p_base[i] = p_base[i - 1] * a;
    }

    T res = identity;
    for (char c : exp_str) {
        int d = char_to_val(c);
        // res = (res ^ numeral_base) * (a ^ d)
        res = binpow(res, (long long)numeral_base, identity) * p_base[d];
    }
    
    return res;
}

const long long MOD = 1e9 + 7;

// A 2x2 Matrix that handles its own modulo multiplication
struct Matrix {
    long long mat[2][2];

    Matrix() {
        for (int i = 0; i < 2; i++)
            for (int j = 0; j < 2; j++)
                mat[i][j] = 0;
    }

    Matrix operator*(const Matrix& other) const {
        Matrix result;
        for (int i = 0; i < 2; ++i) {
            for (int j = 0; j < 2; ++j) {
                for (int k = 0; k < 2; ++k) {
                    long long v = (mat[i][k] * other.mat[k][j]) % MOD;
                    result.mat[i][j] = (result.mat[i][j] + v) % MOD;
                }
            }
        }
        return result;
    }
};

int main() {
    Matrix T;
    T.mat[0][0] = 1; T.mat[0][1] = 1;
    T.mat[1][0] = 1; T.mat[1][1] = 0;
    
    Matrix identity;
    identity.mat[0][0] = 1; identity.mat[0][1] = 0;
    identity.mat[1][0] = 0; identity.mat[1][1] = 1;

    string exp_hex = "A"; // Exponent is 10 (in hex)

    Matrix result = string_pow(T, exp_hex, 16, identity);
    
    cout << "Fib Matrix ^ A (Hex) mod " << MOD << " = " << result.mat[0][1] << "\n";

    return 0;
}