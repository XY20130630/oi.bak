#include <bits/stdc++.h>

typedef long long LL;

#define FOR(i, a, b) for (int i = (a), i##_END_ = (b); i <= i##_END_; i++)
#define DNF(i, a, b) for (int i = (a), i##_END_ = (b); i >= i##_END_; i--)

template <typename Tp> void in(Tp &x) {
	char ch = getchar(); x = 0;
	while (ch < '0' || ch > '9') ch = getchar();
	while (ch >= '0' && ch <= '9') x = x * 10 + ch - '0', ch = getchar();
}

template <typename Tp> Tp Min(Tp x, Tp y) {return x < y ? x : y;}
template <typename Tp> Tp Max(Tp x, Tp y) {return x > y ? x : y;}
template <typename Tp> Tp chkmax(Tp &x, Tp y) {return x > y ? x : x=y;}
template <typename Tp> Tp chkmin(Tp &x, Tp y) {return x < y ? x : x=y;}

const int MAXN = 230, MAXR = 140;

int T, n, r, d[MAXN];
double p[MAXN], f[MAXN][MAXR];

double power(double x, int y)
{
	double ret = 1;
	while (y) {
		if (y & 1) ret *= x;
		x *= x;
		y >>= 1;
	}
	return ret;
}

int main()
{
	freopen("arthur.in", "r", stdin);
	freopen("arthur.out", "w", stdout);
	
	in(T);

	while (T--) {
		in(n); in(r);

		FOR(i, 1, n) {
			scanf("%lf", &p[i]); in(d[i]);
		}

		memset(f, 0, sizeof f);
		
		f[1][r] = 1;
		
		FOR(i, 2, n) FOR(j, 1, r) {
			f[i][j] = 0;
			f[i][j] += f[i - 1][j + 1] * (1 - power(1 - p[i - 1], j + 1));
			f[i][j] += f[i - 1][j]     * power(1 - p[i - 1], j);
		}

		double ans = 0;

		FOR(i, 1, n) FOR(j, 1, r) {
			ans += f[i][j] * (1 - power(1 - p[i], j)) * d[i];
		}

		printf("%.10lf\n", ans);
	}
	
	return 0;
}
