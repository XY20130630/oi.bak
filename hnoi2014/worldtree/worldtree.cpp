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

int n, q, num[MAXN], fa[MAXN], dfn[MAXN], sa[MAXN], depth[MAXN], sz[MAXN];
int INDEX, cnt, head[MAXN], data[MAXN << 1], nxt[MAXN << 1], log_num[1000010];

int anc[MAXN][20], xsz[MAXN], val[MAXN];

void add(int x, int y)
{
	nxt[cnt] = head[x]; data[cnt] = y; head[x] = cnt++;
	nxt[cnt] = head[y]; data[cnt] = x; head[y] = cnt++;
}

void dfs(int now, int pa)
{
	sz[now] = 1;
	depth[now] = depth[pa] + 1;
	fa[now] = pa; dfn[now] = ++INDEX;
	for (int i = head[now]; i != -1; i = nxt[i]) {
		if (data[i] != pa) {
			dfs(data[i], now);
			sz[now] += sz[data[i]];
		}
	}
}

void dfs_anc(int now, int pa)
{
	anc[now][0] = pa;
	for (int i = 1; anc[now][i - 1] != -1; i++) {
		anc[now][i] = anc[anc[now][i - 1]][i - 1];
	}
	for (int i = head[now]; i != -1; i = nxt[i]) {
		if (data[i] != pa) dfs_anc(data[i], now);
	}
}

namespace get_lca
{
	int eula[MAXN * 3], st[MAXN], ed[MAXN], IND, to[MAXN];
	int minx[MAXN * 3][21];
	
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
		FOR(i, 1, 20) {
			FOR(j, 1, eula[0]) {
				if (j + (1 << i) - 1 > eula[0]) break;
				minx[j][i] =
					Min(minx[j][i - 1], minx[j + (1 << (i - 1))][i - 1]);
			}
		}
		FOR(i, 1, 1000000) log_num[i] = log2(i);
	}
}

int stack[MAXN], top, in_tree[MAXN];
int head_xu[MAXN], cnt_xu, nxt_xu[MAXN << 1], data_xu[MAXN << 1];

namespace build_tree
{
	void add_xu(int x, int y)
	{
		nxt_xu[cnt_xu] = head_xu[x]; data_xu[cnt_xu] = y; head_xu[x] = cnt_xu++;
		nxt_xu[cnt_xu] = head_xu[y]; data_xu[cnt_xu] = x; head_xu[y] = cnt_xu++;
	}

	bool cmp(int x, int y)
	{
		return dfn[x] < dfn[y];
	}

	void builds_tree()
	{
		cnt_xu = 0;
		stack[top = 1] = 0;
		in_tree[0] = 0;
		FOR(i, 1, num[0]) {
			is_chs[num[i]] = true;
			in_tree[++in_tree[0]] = num[i];
		}
		in_tree[++in_tree[0]] = 0;
		std::sort(in_tree + 1, in_tree + in_tree[0] + 1, cmp);
		int now_in_tree = in_tree[0];
		FOR(i, 2, now_in_tree) {
			int now = in_tree[i];
			int anc = get_lca::lca(stack[top], now);
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
						in_tree[++in_tree[0]] = anc;
						add_xu(anc, tp); stack[top] = anc; break;
					}
				}
				stack[++top] = now;
			}
		}
		while (top != 1) {
			add_xu(stack[top - 1], stack[top]); top--;
		}
	}
}

namespace find_father
{
	int find(int x, int y)
	{
		for (int i = 19; i >= 0; i--) {
			if (y & (1 << i)) {
				x = anc[x][i];
			}
		}
		return x;
	}
}

namespace DP
{

	int f[MAXN][2], g[MAXN][2], ret[MAXN];

	void dp1(int now, int pa)
	{
		val[now] = 0;
		xsz[now] = 1;
		f[now][0] = is_chs[now] ? 0 : 0x3f3f3f3f;
		f[now][1] = now;
		for (int i = head_xu[now]; i != -1; i = nxt_xu[i]) {
			if (data_xu[i] != pa) {
				dp1(data_xu[i], now);
				xsz[now] += xsz[data_xu[i]];
				int frm = depth[data_xu[i]] - depth[now];
				if (f[now][0] > f[data_xu[i]][0] + frm ||
					f[now][0] == f[data_xu[i]][0] + frm &&
					f[now][1] > f[data_xu[i]][1])
				{
					f[now][1] = f[data_xu[i]][1];
					f[now][0] = f[data_xu[i]][0] + frm;
				}
			}
		}
	}

	void dp2(int now, int pa)
	{
		g[now][0] = f[now][0];
		g[now][1] = f[now][1];
		if (pa != -1) {
			int frm = depth[now] - depth[pa];
			if (g[now][0] > g[pa][0] + frm ||
				g[now][0] == g[pa][0] + frm && g[now][1] > g[pa][1])
			{
				g[now][1] = g[pa][1]; g[now][0] = g[pa][0] + frm;
			}
		}
		for (int i = head_xu[now]; i != -1; i = nxt_xu[i]) {
			if (data_xu[i] != pa) dp2(data_xu[i], now);
		}
	}

	int getr(int x, int y, int z, bool t)
	{
		if (y - x + z < 0) return 0;
		if (t) return (y - x + z) / 2;
		else {
			if ((y - x + z) % 2 == 0) return (y - x + z) / 2;
			else return (y - x + z) / 2 + 1;
		}
	}

	void dp3(int now, int pa)
	{
		if (!pa) {
			ret[g[now][1]] += n - sz[now];
		}
		bool flag = false;
		for (int i = head_xu[now]; i != -1; i = nxt_xu[i])
			if (data_xu[i] != pa) {
				flag = true;
				int nnum = data_xu[i], all;
				if (now) {
					all = depth[nnum] - depth[now] - 1;
					if (g[now][1] == g[nnum][1]) {
						if (all) {
							int pos = find_father::find(nnum, all);
							ret[g[now][1]] += sz[pos] - sz[nnum];
							val[now] -= sz[pos];
						}
						else val[now] -= sz[nnum];
					}
					else {
//					ret[g[now][1]] += getr(g[now][0], g[nnum][0], all,
//										   g[now][1] > g[nnum][1]);
//					ret[g[nnum][1]] += getr(g[nnum][0], g[now][0], all,
//											g[nnum][1] > g[now][1]);
						if (!all) {
							val[now] -= sz[nnum];
						}
						else {
							int Anc = getr(g[nnum][0], g[now][0], all,
										   g[nnum][1] > g[now][1]);
							int pos = find_father::find(nnum, Anc);
							ret[g[nnum][1]] += sz[pos] - sz[nnum];
							
							int pos2 = find_father::find(nnum, all);
							ret[g[now][1]] += sz[pos2] - sz[pos];
							val[now] -= sz[pos2];
						}
					}
				}
				dp3(nnum, now);
			}
	}

	void dp4(int now, int pa)
	{
		if (now) ret[g[now][1]] += sz[now] + val[now];
		for (int i = head_xu[now]; i != -1; i = nxt_xu[i]) {
			if (data_xu[i] != pa) dp4(data_xu[i], now);
		}
	}

	void work()
	{
		build_tree::builds_tree();
		FOR(i, 1, num[0]) ret[num[i]] = 0;
		dp1(0, -1); dp2(0, -1); dp3(0, -1); dp4(0, -1);
		FOR(i, 1, num[0]) printf("%d ", ret[num[i]]);
		putchar(10);
	}
}

int main()
{
	freopen("worldtree.in", "r", stdin);
	freopen("worldtree.out", "w", stdout);
	
	memset(head, -1, sizeof head);
	memset(head_xu, -1, sizeof head_xu);
	
	in(n);
	FOR(i, 1, n - 1) {
		int x, y; in(x); in(y); add(x, y);
	}

	memset(anc, -1, sizeof anc);
	
	dfs(1, 0); dfs_anc(1, 0);
	get_lca::dfs2(1); get_lca::start();
	FOR(i, 1, n) sa[dfn[i]] = i;

	in(q);
	FOR(i, 1, q) {
		FOR(i, 1, num[0])
			is_chs[num[i]] = false;
		FOR(i, 1, in_tree[0])
			head_xu[in_tree[i]] = -1;
		in(num[0]);
		FOR(j, 1, num[0]) in(num[j]);
		DP::work();
	}
	
	return 0;
}
