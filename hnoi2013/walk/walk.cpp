#include <bits/stdc++.h>
 
const int MAXN = 510;
 
bool f[MAXN][MAXN];
int n, m, du[MAXN], all;
long double matrix[MAXN][MAXN], times[MAXN], timess[MAXN * MAXN];
 
template <typename Tp> void in(Tp &x) {
    char ch = getchar(); x = 0;
    while (ch < '0' || ch > '9') ch = getchar();
    while (ch >= '0' && ch <= '9') x = x * 10 + ch - '0', ch = getchar();
}
 
void gauss() {
    for (int j = 1; j <= n; j++) {
        long double maxu = -1;
        int maxv = 0;
        for (int i = j; i <= n + 1; i++)
            if (maxu < fabs(matrix[i][j])) {
                maxu = fabs(matrix[i][j]);
                maxv = i;
            }
        for (int i = 1; i <= n + 1; i++) {
            long double t = matrix[j][i];
            matrix[j][i] = matrix[maxv][i];
            matrix[maxv][i] = t;
        }
        long double eps = matrix[j][j];
        if (fabs(eps) < 1e-10) continue;
        for (int i = 1; i <= n + 1; i++)
            matrix[j][i] /= eps;
        for (int i = 1; i <= n + 1; i++)
            if (i != j) {
                long double epss = matrix[i][j];
                if (fabs(epss) < 1e-10) continue;
                for (int k = 1; k <= n + 1; k++)
                    matrix[i][k] -= matrix[j][k] * epss;
            }
    }
    for (int i = 1; i <= n; i++)
        times[i] = matrix[i][n + 1] / matrix[i][i];
    for (int i = 1; i <= n; i++)
        for (int j = i + 1; j <= n; j++)
            if (f[i][j]) {
                if (j == n) timess[++all] = times[i] / du[i];
                else timess[++all] = times[i] / du[i] + times[j] / du[j];
            }
    std::sort(timess + 1, timess + all + 1);
    long double ans = 0;
    for (int i = 1; i <= all; i++) {
        ans += timess[i] * (all - i + 1);
    }
    printf("%.3Lf\n", ans);
}
 
int main() {
	freopen("walk.in", "r", stdin);
	freopen("walk.out", "w", stdout);
    in(n); in(m);
    for (int i = 1; i <= m; i++) {
        int u, v;
        in(u); in(v);
        f[u][v] = f[v][u] = true;
        if (u != n) du[u]++;
        if (v != n) du[v]++;
    }
    for (int i = 1; i <= n; i++) {
        for (int j = 1; j < n; j++)
            if (f[i][j])
                matrix[i][j] = (long double)(1) / du[j];
        matrix[i][i] = -1;
        if (i == 1) matrix[i][n + 1] = -1;
    }
    matrix[n + 1][n] = 1;
    matrix[n + 1][n + 1] = 1;
    gauss();
    return 0;
}
