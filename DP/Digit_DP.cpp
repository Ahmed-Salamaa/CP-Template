#include <bits/stdc++.h>
using namespace std;
#define ll long long
#define sz(x) x.size()

ll dp[20][2][2][90], k;
string L, R;

ll rec(int idx = 0,
       bool not_smaller = true,
       bool not_greater = true,
       int sum = 0)
{
    if (idx == (int)L.size())
        return !sum;

    ll &ret = dp[idx][not_smaller][not_greater][sum];
    if (~ret) return ret;

    ret = 0;

    int L_digit = L[idx] - '0';
    int R_digit = R[idx] - '0';

    int low_digit  = not_smaller ? L_digit : 0;
    int high_digit = not_greater ? R_digit : 9;

    for (int digit = low_digit; digit <= high_digit; digit++)
    {
        bool next_not_smaller =
            not_smaller ? (digit == L_digit) : 0;

        bool next_not_greater =
            not_greater ? (digit == R_digit) : 0;

        int next_sum = (sum + digit) % k;

        ret += rec(
            idx + 1,
            next_not_smaller,
            next_not_greater,
            next_sum
        );
    }

    return ret;
}

void solve(ll l, ll r)
{
    L = to_string(l);
    R = to_string(r);

    L = string((int)R.size() - (int)L.size(), '0') + L;

    memset(dp, -1, sizeof(dp));

    cout << rec();
}

///////////////////////////////

ll dp[20][2][90];
ll k;
string S;

ll rec(int idx = 0,
       bool tight = true,
       int sum = 0)
{
    if (idx == (int)S.size())
        return !sum;

    ll &ret = dp[idx][tight][sum];
    if (~ret) return ret;

    ret = 0;

    int S_digit = S[idx] - '0';

    int low_digit  = 0;
    int high_digit = tight ? S_digit : 9;

    for (int digit = low_digit; digit <= high_digit; digit++)
    {
        bool next_tight =
            tight ? (digit == S_digit) : 0;

        int next_sum = (sum + digit) % k;

        ret += rec(
            idx + 1,
            next_tight,
            next_sum
        );
    }

    return ret;
}

ll calc(ll x)
{
    if (x < 0) return 0;

    S = to_string(x);

    memset(dp, -1, sizeof(dp));

    return rec();
}