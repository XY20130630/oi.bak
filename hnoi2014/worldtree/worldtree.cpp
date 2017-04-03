#include <bits/stdc++.h>

typedef long long LL;

#define debug(...) fprintf(stderr, __VA_ARGS__)

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

const int MAXN = 300010;

bool is_chs[MAXN];

int n, q, num[MAXN], fa[MAXN], dfn[MAXN], sa[MAXN];
int INDEX, cnt, head[MAXN], data[MAXN << 1], nxt[MAXN << 1], log_num[MAXN];

void add(int x, int y)
{
	nxt[cnt] = head[x]; data[cnt] = y; head[x] = cnt++;
	nxt[cnt] = head[y]; data[cnt] = x; head[y] = cnt++;
}

void dfs(int now, int pa)
{
	fa[now] = pa; dfn[now] = ++INDEX;
	for (int i = head[now]; i != -1; i = nxt[i]) {
		if (data[i] != pa) dfs(data[i], now);
	}
}

int eula[MAXN * 3], st[MAXN], ed[MAXN], IND, to[MAXN];
int minx[MAXN * 3][19];

void dfs2(int now)
{
	eula[++eula[0]] = ++IND;
	st[now] = eula[0]; to[IND] = now;
	for (int i = head[now]; i != -1; i = nxt[i]) {
		if (data[i] != fa[now]) {
			dfs2(data[i]);
			eula[++eula[0]] = eula[st[now]];
		}
	}
	ed[now] = eula[0];
}

int lca(int x, int y)
{
	if (!x || !y) return 0;
	if (st[x] > ed[y]) std::swap(x, y); x = st[x]; y = ed[y];
	int delta = log_num[y - x + 1];
	return to[Min(minx[x][delta], minx[y - (1 << delta) + 1][delta])];
}

void start()
{
	FOR(i, 1, eula[0]) minx[i][0] = eula[i];
	FOR(i, 1, 19) {
		FOR(j, 1, eula[0]) {
			if (j + (1 << i) - 1 > eula[0]) break;
			minx[j][i] = Min(minx[j][i - 1], minx[j + (1 << (i - 1))][i - 1]);
		}
	}
	FOR(i, 1, 300000) log_num[i] = log2(i);
}

int stack[MAXN], top, in_tree[MAXN];
int head_xu[MAXN], cnt_xu, nxt_xu[MAXN << 1], data_xu[MAXN << 1];

void add_xu(int x, int y)
{
	nxt_xu[cnt_xu] = head_xu[x]; data_xu[cnt_xu] = y; head_xu[x] = cnt_xu++;
	nxt_xu[cnt_xu] = head_xu[y]; data_xu[cnt_xu] = x; head_xu[y] = cnt_xu++;
}

void build_tree()
{
	cnt_xu = 0;
	stack[top = 1] = 0;
	in_tree[0] = 0;
	FOR(i, 1, num[0]) {
		is_chs[num[i]] = true;
		in_tree[++in_tree[0]] = num[i];
	}
	in_tree[++in_tree[0]] = 0;
	FOR(i, 1, n) if (is_chs[sa[i]]) {
		int now = sa[i];
		int anc = lca(stack[top], now);
		if (anc == stack[top]) stack[++top] = now;
		else {
			while (true) {
				int tp = stack[top], tp_l = stack[top - 1];
				if (tp_l == anc) {
					add_xu(tp_l, tp); top--; break;
				}
				else if (dfn[tp_l] > dfn[anc]) {
					add_xu(tp_l, tp); top--;
				}
				else {
					add_xu(anc, tp); stack[top] = anc; break;
				}
			}
			stack[++top] = now;
		}
	}
}

int f[MAXN][2], g[MAXN][2], ret[MAXN];

void dp1(int now, int pa)
{
	f[now][0] = is_chs[now] ? 0 : 0x3f3f3f3f;
	for (int i = head_xu[now]; i != -1; i = nxt_xu[i]) {
		if (data_xu[i] != pa) {
			dp1(data_xu[i], now);
			if (f[now][0] > f[data_xu[i]][0] + 1 ||
				f[now][0] == f[data_xu[i]][0] + 1 && f[now][1] > f[data_xu[i]][1]) {
				f[now][1] = f[data_xu[i]][1]; f[now][0] = f[data_xu[i]][0] + 1;
			}
		}
	}
}

void dp2(int now, int pa)
{
	g[now][0] = f[now][0];
	g[now][1] = f[now][1];
	if (pa != -1) {
		if (g[now][0] > g[pa][0] + 1 ||
			g[now][0] == g[pa][0] + 1 && g[now][1] > g[pa][1]) {
			g[now][1] = g[pa][1]; g[now][0] = g[pa][0] + 1;
		}
	}
	for (int i = head_xu[now]; i != -1; i = nxt_xu[i]) {
		if (data_xu[i] != pa) dp2(data_xu[i], now);
	}
}

void dp3(int now, int pa)
{
	for (int i = head_xu[now]; i != -1; i = nxt_xu[i]) {
		int nnum = data_xu[now];
		if (is_chs[now]) {
			g[now] = 0; ret[now]++;
			if (is_chs[nnum]) {
				int delta = depth[nnum] - depth[now] - 1;
				if (delta % 2 == 0)
					ret[nnum] += delta / 2, ret[now] += delta / 2;
				else if (nnum < now)
					ret[nnum] += delta / 2 + 1, ret[now] += delta / 2;
				else
					ret[nnum] += delta / 2, ret[now] += delta / 2 + 1;
			}
			else {
				if (g[nnum][1] == now) {
					ret[now] += depth[nnum] - depth[now] - 1;
				}
				else {
					
				}
			}
		}
	}
}

void work()
{
	build_tree();
	FOR(i, 1, num[0]) ret[i] = 0;
	dp1(0, -1); dp2(0, -1); dp3(0, -1);
}

int main()
{
	memset(head, -1, sizeof head);
	memset(head_xu, -1, sizeof head_xu);
	
	in(n);
	FOR(i, 1, n - 1) {
		int x, y; in(x); in(y); add(x, y);
	}

	dfs(1, 0);
	dfs2(1); start();
	FOR(i, 1, n) sa[dfn[i]] = i;

	in(q);
	FOR(i, 1, q) {
		FOR(i, 1, num[0])
			is_chs[num[i]] = false;
		FOR(i, 1, in_tree[0])
			head_xu[i] = -1;
		in(num[0]);
		FOR(j, 1, num[0]) in(num[j]);
		work();
	}
	
	return 0;
}
