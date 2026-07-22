#pragma once
#include <bits/stdc++.h>
#include <assert.h>
using namespace std;

const int dx[4] = { 1, -1, 0, 0 };
const int dy[4] = { 0, 0, 1, -1 };
const long double PI = acosl(-1.0L);
const long double EPS = 1e-6 ;
const int MOD = 1e9 + 7;
const int inf = 0x3f3f3f3f;
const long long INF = 1ll << 62;

#define int long long
#define ll long long
#define ld long double
#define popcnt(x) __builtin_popcountll(x)
#define all(vec) vec.begin(), vec.end()
#define sz(x) (int)(x).size()

inline void ultimate_tips() {

#ifndef ONLINE_JUDGE
    freopen("input.txt", "r", stdin);
    freopen("output.txt", "w", stdout);
#endif

    ios_base::sync_with_stdio(false);
    cin.tie(nullptr);
}
