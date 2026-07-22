/*
 * Topic: DP - Matrix Power
 * Description: Static Matrix Exponentiation allows for solving linear recurrences in O(M^3 log
 *   P) time, where the modulo is passed as a template parameter for aggressive
 *   compile-time optimizations.
 */
#include "../../core.h"

/*
 * Static Matrix Exponentiation
 * ----------------------------
 * Useful for solving linear recurrences and paths in graphs in O(M^3 log P) time.
 * Modulo is a template parameter (faster due to compile-time optimizations).
 */
template <typename T = int, int MOD_VALUE = 1000000007>
struct Matrix {
    vector<vector<T>> mat;
    int rows, cols;

    // O ( rows * cols )
    Matrix(int rows, int cols) : rows(rows), cols(cols), mat(rows + 5, vector<T>(cols + 5, 0)) {}

    // O ( size * size )
    Matrix(int size) : Matrix(size, size) {}

    // O ( rows * cols )
    Matrix(const vector<vector<T>>& grid)
        : rows(grid.empty() ? 0 : (int)grid.size()), cols(grid.empty() ? 0 : (int)grid[0].size()) {
        mat.assign(rows + 5, vector<T>(cols + 5, 0));
        for (int i = 1; i <= rows; i++) {
            for (int j = 1; j <= cols; j++) {
                mat[i][j] = grid[i - 1][j - 1];  // Assume grid is 0-indexed but matrix is 1-indexed
            }
        }
    }

    // O ( size )
    static Matrix get_identity(int size) {
        Matrix res(size, size);
        for (int i = 1; i <= size; i++) res.mat[i][i] = 1 % MOD_VALUE;
        return res;
    }

    // O ( 1 )
    static T add_mod(T a, T b) { return (a + b) % MOD_VALUE; }

    // O ( 1 )
    static T mult_mod(T a, T b) { return (T)(((__int128)a * b) % MOD_VALUE); }

    // O ( rows * cols * b.cols )
    Matrix operator*(const Matrix& b) const {
        Matrix res(rows, b.cols);
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
        Matrix res = get_identity(rows);
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
