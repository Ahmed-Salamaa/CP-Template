#include <bits/stdc++.h>
using namespace std;


// note if you need to use custom Mod from runtime 
// , you need to use deffrent idea from send it with templete 
template <typename T = int, int MODD = 1000000007>
struct Matrix
{
    vector<vector<T>> mat;
    int n, m;

    /* =========================================================
                       Type Detection Helpers
    ========================================================= */

    static constexpr bool is_float_type()
    {
        return is_same<T, double>::value ||
               is_same<T, long double>::value;
    }

    /* =========================================================
                        Modular Operations
    ========================================================= */

    static T add_modd(T a, T b)
    {
        if constexpr (is_float_type())
        {
            return a + b;
        }
        else
        {
            a %= MODD;
            b %= MODD;

            T res = a + b;
            res %= MODD;

            if (res < 0) res += MODD;
            return res;
        }
    }

    static T sub_modd(T a, T b)
    {
        if constexpr (is_float_type())
        {
            return a - b;
        }
        else
        {
            a %= MODD;
            b %= MODD;

            T res = a - b;
            res %= MODD;

            if (res < 0) res += MODD;
            return res;
        }
    }

    static T mult_modd(T a, T b)
    {
        if constexpr (is_float_type())
        {
            return a * b;
        }
        else
        {
            a %= MODD;
            b %= MODD;

            __int128 prod = (__int128)a * (__int128)b;

            T res = (T)(prod % MODD);
            if (res < 0) res += MODD;

            return res;
        }
    }

    // constructors
    Matrix(int n , int m )
    {
        this->n = n;
        this->m = m;
        mat.assign(n, vector<T>(m, 0));
    }
    Matrix(int n ) : Matrix( n , n ) {} ;
    Matrix(const vector<vector<T>> &mat_)
    {
        this->mat = mat_;
        this->n = (int)mat_.size();
        this->m = this->n ? (int)mat_[0].size() : 0;
    }

    // identity (named GetIdentity to match your original k_th usage)
    static Matrix GetIdentity(int size)
    {
        Matrix I(size, size);
        for (int i = 0; i < size; i++)
            I.mat[i][i] = (T)(1 % MODD);
        return I;
    }

    // arithmetic operators (const-correct)
    Matrix operator +(const Matrix &b) const
    {
        Matrix c(n, m);
        for (int i = 0; i < n; i++)
            for (int j = 0; j < m; j++)
                c.mat[i][j] = add_modd(mat[i][j], b.mat[i][j]);
        return c;
    }
    Matrix operator -(const Matrix &b) const
    {
        Matrix c(n, m);
        for (int i = 0; i < n; i++)
            for (int j = 0; j < m; j++)
                c.mat[i][j] = sub_modd(mat[i][j], b.mat[i][j]);
        return c;
    }
    Matrix operator *(const Matrix &b) const
    {
        Matrix c(n, b.m);
        for (int i = 0; i < n; i++)
        {
            for (int k = 0; k < m; k++)
            {
                if (mat[i][k] == 0) continue;
                for (int j = 0; j < b.m; j++)
                {
                    c.mat[i][j] = add_modd(c.mat[i][j], mult_modd(mat[i][k], b.mat[k][j]));
                }
            }
        }
        return c;
    }

    // exponentiation (returns this^p)
    Matrix operator ^(long long p) const
    {
        Matrix res = GetIdentity(n);
        Matrix base = *this;
        while (p > 0)
        {
            if (p & 1) res = res * base;
            base = base * base;
            p >>= 1;
        }
        return res;
    }

    // assignment from Matrix
    Matrix& operator = (const Matrix& b)
    {
        if (this != &b)
        {
            mat = b.mat;
            n = b.n;
            m = b.m;
        }
        return *this;
    }
    // assignment from raw 2D vector
    Matrix& operator = (const vector<vector<T>> &b)
    {
        mat = b;
        n = (int)b.size();
        m = n ? (int)b[0].size() : 0;
        return *this;
    }

    // indexing
    vector<T>& operator[](int index)
    {
        return mat[index];
    }
    const vector<T>& operator[](int index) const
    {
        return mat[index];
    }
};



