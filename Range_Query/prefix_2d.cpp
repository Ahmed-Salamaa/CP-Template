#include <iostream>
#include <vector>
using namespace std;

vector<int> prefixSum2D(vector<vector<int>> &arr, vector<vector<int>> &queries) {
    int n = arr.size();
    int m = arr[0].size();

    // Construct prefix sum matrix using 0-based indexing
    vector<vector<int>> prefix(n, vector<int>(m, 0));

    for (int i = 0; i < n; ++i) {
        for (int j = 0; j < m; ++j) {

            // Start with original value
            prefix[i][j] = arr[i][j];

            // Add value from top cell if it exists
            if (i > 0)
                prefix[i][j] += prefix[i - 1][j];

            // Add value from left cell if it exists
            if (j > 0)
                prefix[i][j] += prefix[i][j - 1];

            // Subtract overlap from top-left diagonal if it exists
            if (i > 0 && j > 0)
                prefix[i][j] -= prefix[i - 1][j - 1];
        }
    }

    vector<int> result;

    // Process each query using inclusion-exclusion
    for (auto &q : queries) {
        int r1 = q[0], c1 = q[1];
        int r2 = q[2], c2 = q[3];

        int total = prefix[r2][c2];
        int top = (r1 > 0) ? prefix[r1 - 1][c2] : 0;
        int left = (c1 > 0) ? prefix[r2][c1 - 1] : 0;
        int overlap = (r1 > 0 && c1 > 0) ? prefix[r1 - 1][c1 - 1] : 0;

        int sum = total - top - left + overlap;
        result.push_back(sum);
    }

    return result;
}

int main() {
    vector<vector<int>> arr = {
        {1, 2, 3, 4},
        {5, 6, 7, 8},
        {9, 10, 11, 12},
        {13, 14, 15, 16}
    };

    vector<vector<int>> queries = {
        {1, 1, 2, 2},  
    };

    vector<int> result = prefixSum2D(arr, queries);

    for (int sum : result) {
        cout << sum << " ";
    }

    cout << endl;
}