#include <bits/stdc++.h>

typedef long long LL;

#define FOR(i, a, b) for (int i = (a), i##_END_ = (b); i <= i##_END_; i++)
#define DNF(i, a, b) for (int i = (a), i##_END_ = (b); i >= i##_END_; i--)

template <typename Tp> void in(Tp &x) {
	char ch = getchar(), f = 1; x = 0;
	while (ch != '-' && (ch < '0' || ch > '9')) ch = getchar();
	if (ch == '-') f = -1, ch = getchar();
	while (ch >= '0' && ch <= '9') x = x * 10 + ch - '0', ch = getchar();
	x *= f;
}

template <typename Tp> Tp chkmax(Tp &x, Tp y) {return x > y ? x : x=y;}
template <typename Tp> Tp chkmin(Tp &x, Tp y) {return x < y ? x : x=y;}
template <typename Tp> Tp Max(Tp x, Tp y) {return x > y ? x : y;}
template <typename Tp> Tp Min(Tp x, Tp y) {return x < y ? x : y;}

int p, q, r, d;

int height[50][50][50];

int main()
{
	in(p); in(q); in(r);
	in(d);
	FOR(i, 1, r) {
		FOR(j, 1, p) FOR(k, 1, q) in(height[i][j][k]);
	}
	FOR(i, 1, p) FOR(j, 1, q) {
		add(ss, PLA(i, j, 1), INF);
		FOR(k, 1, r) {
			add(PLA(i, j, k), PLA(i, j, k + 1), height[i][j][k]);
		}
		add(PLA(i, j, r + 1), tt, INF);
	}
	FOR(i, 1, p) FOR(j, 1, q) {
		FOR(k, d + 1, r + 1) {
			if (i != p) add(PLA(i, j, k), PLA(i + 1, j, k - d), INF);
			if (i != 1) add(PLA(i, j, k), PLA(i - 1, j, k - d), INF);
			if (j != q) add(PLA(i, j, k), PLA(i, j + 1, k - d), INF);
			if (j != 1) add(PLA(i, j, k), PLA(i, j - 1, k - d), INF);
		}
	}
	
	return 0;
}
