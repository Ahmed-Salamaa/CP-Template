/*
 * Topic: DP - Matrix Power
 * Description: Dynamic Matrix Exponentiation allows for solving linear recurrences and paths in
 *   graphs in O(M^3 log P) time, where the modulo can be set at runtime.
 */
#include "../../core.h"

/*
 * Dynamic Matrix Exponentiation
 * -----------------------------
 * Useful for solving linear recurrences and paths in graphs in O(M^3 log P) time.
 * Modulo is a member variable (allows changing modulo at runtime).
 */
template <typename T = int>
struct Matrix {
    vector<vector<T>> mat;
    int rows, cols;
    T mod_value;

    // O ( rows * cols )
    Matrix(int rows, int cols, T mod_value = 1000000007)
        : rows(rows), cols(cols), mod_value(mod_value), mat(rows + 5, vector<T>(cols + 5, 0)) {}

    // O ( size * size )
    Matrix(int size, T mod_value = 1000000007) : Matrix(size, size, mod_value) {}

    // O ( rows * cols )
    Matrix(const vector<vector<T>>& grid, T mod_value = 1000000007)
        : rows(grid.empty() ? 0 : (int)grid.size()),
          cols(grid.empty() ? 0 : (int)grid[0].size()),
          mod_value(mod_value) {
        mat.assign(rows + 5, vector<T>(cols + 5, 0));
        for (int i = 1; i <= rows; i++) {
            for (int j = 1; j <= cols; j++) {
                mat[i][j] = grid[i - 1][j - 1];  // Assume grid is 0-indexed, but our mat is 1-indexed.
            }
        }
    }

    // O ( size )
    static Matrix get_identity(int size, T mod_value = 1000000007) {
        Matrix res(size, size, mod_value);
        for (int i = 1; i <= size; i++) res.mat[i][i] = 1 % mod_value;
        return res;
    }

    // O ( 1 )
    T add_mod(T a, T b) const { return (a + b) % mod_value; }

    // O ( 1 )
    T mult_mod(T a, T b) const { return (T)(((__int128)a * b) % mod_value); }

    // O ( rows * cols * b.cols )
    Matrix operator*(const Matrix& b) const {
        Matrix res(rows, b.cols, mod_value);
        for (int i = 1; i <= rows; i++) {
            for (int k = 1; k <= cols; k++) {
                // Skip zeroes to significantly speed up multiplication for sparse matrices
                if (mat[i][k] == 0) continue;
                for (int j = 1; j <= b.cols; j++) {
                    res.mat[i][j] = add_mod(res.mat[i][j], mult_mod(mat[i][k], b.mat[k][j]));
                }
            }
        }
        return res;
    }

    // O ( rows^3 * log(power) )
    Matrix operator^(int power) const {
        Matrix res = get_identity(rows, mod_value);
        Matrix base = *this;
        // Binary exponentiation for fast matrix power
        while (power > 0) {
            if (power & 1) res = res * base;
            base = base * base;
            power >>= 1;
        }
        return res;
    }
};
