#include <bits/stdc++.h>
using namespace std;

#define ll long long
#define int long long
#define all(v) v.begin(), v.end()
#define x first
#define y second

const long long INF = 4e18;
vector<int> u, v, p, way;
int hungarian(const vector<vector<int>> &cost)
{
      int n = cost.size() - 1;
      int m = cost[0].size() - 1;

      // u: potentials for rows, v: potentials for cols
      u.assign(n + 1, 0);
      v.assign(m + 1, 0);
      p.assign(m + 1, 0);
      way.assign(m + 1, 0);

      for (int i = 1; i <= n; ++i)
      {
            p[0] = i;
            int j0 = 0;
            vector<int> minv(m + 1, INF);
            vector<bool> used(m + 1, false);

            do
            {
                  used[j0] = true;
                  int i0 = p[j0], delta = INF, j1 = 0;

                  for (int j = 1; j <= m; ++j)
                  {
                        if (!used[j])
                        {
                              int cur = cost[i0][j] - u[i0] - v[j];
                              if (cur < minv[j])
                              {
                                    minv[j] = cur;
                                    way[j] = j0;
                              }
                              if (minv[j] < delta)
                              {
                                    delta = minv[j];
                                    j1 = j;
                              }
                        }
                  }

                  for (int j = 0; j <= m; ++j)
                  {
                        if (used[j])
                        {
                              u[p[j]] += delta;
                              v[j] -= delta;
                        }
                        else
                        {
                              minv[j] -= delta;
                        }
                  }
                  j0 = j1;
            } while (p[j0] != 0);

            do
            {
                  int j1 = way[j0];
                  p[j0] = p[j1];
                  j0 = j1;
            } while (j0 > 0);
      }

      return -v[0];
}
void solve()
{
      int n;
      cin >> n;
      vector<vector<int>> a(n + 1, vector<int>(n + 1));
      for (int i = 1; i <= n; i++)
            for (int j = 1; j <= n; j++)
                  cin >> a[i][j];

      cout << hungarian(a) << '\n';
      vector<int> ans(n + 1);
      for (int i = 1; i <= n; i++)
            ans[p[i]] = i;
      for (int i = 1; i <= n; i++)
            cout << i << " " << ans[i] << '\n';
}
signed main()
{

      ios_base::sync_with_stdio(false), cin.tie(nullptr), cout.tie(nullptr);

      int t = 1;
      //    cin >> t;
      while (t--)
      {
            solve();
            cout << "\n";
      }
}