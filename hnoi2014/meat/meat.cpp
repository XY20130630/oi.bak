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

const int MAXN = 500010;

long double rot[MAXN], val[MAXN];
int n, w[MAXN], tot[MAXN];
int cnt, head[MAXN], data[MAXN << 1], nxt[MAXN << 1];

void add(int x, int y)
{
	nxt[cnt] = head[x]; data[cnt] = y; head[x] = cnt++;
	nxt[cnt] = head[y]; data[cnt] = x; head[y] = cnt++;
}

void dfs1(int now, int pa)
{
	for (int i = head[now]; i != -1; i = nxt[i])
		if (data[i] != pa) {
			tot[now]++;
			dfs1(data[i], now);
		}
}

void dfs2(int now, int pa)
{
	for (int i = head[now]; i != -1; i = nxt[i]) {
		if (data[i] != pa) {
			val[data[i]] = log(tot[now]) + val[now];
			dfs2(data[i], now);
		}
	}
}

const long double eps = 1e-6;

int main()
{
	freopen("meat.in", "r", stdin);
	freopen("meat.out", "w", stdout);
	
	memset(head, -1, sizeof head);
	
	in(n);
	FOR(i, 1, n) in(w[i]);
	FOR(i, 1, n - 1) {int u, v; in(u); in(v); add(u, v);}
	dfs1(1, 0); dfs2(1, 0);
	FOR(i, 1, n) rot[i] = val[i] + log(w[i]);
	
	std::sort(rot + 1, rot + n + 1);

	int ret = 0, line = 0;
	FOR(i, 1, n) {
		if (i == 1 || fabs(rot[i] - rot[i - 1]) > eps) {
			chkmax(ret, line);
			line = 1;
		}
		else line++;
	}

	chkmax(ret, line);
	printf("%d\n", n - ret);
	
	return 0;
}
