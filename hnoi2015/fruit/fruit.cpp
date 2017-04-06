#include <bits/stdc++.h>

using std::map;

typedef long long LL;

#define FOR(i, a, b) for (int i = (a), i##_END_ = (b); i <= i##_END_; i++)
#define DNF(i, a, b) for (int i = (a), i##_END_ = (b); i >= i##_END_; i--)

#define debug(...) fprintf(stderr, __VA_ARGS__)

template <typename Tp> void in(Tp &x) {
	char ch = getchar(); x = 0;
	while (ch < '0' || ch > '9') ch = getchar();
	while (ch >= '0' && ch <= '9') x = x * 10 + ch - '0', ch = getchar();
}

template <typename Tp> Tp Min(Tp x, Tp y) {return x < y ? x : y;}
template <typename Tp> Tp Max(Tp x, Tp y) {return x > y ? x : y;}
template <typename Tp> Tp chkmax(Tp &x, Tp y) {return x > y ? x : x=y;}
template <typename Tp> Tp chkmin(Tp &x, Tp y) {return x < y ? x : x=y;}

const int MAXN = 100010;

const int appear = 0, disappear = 2, query = 1;

int n, p, q, ev_tot;
int cnt, head[MAXN], data[MAXN << 1], nxt[MAXN << 1];

int INDEX, dfn[MAXN], ed[MAXN], depth[MAXN];

int anc[MAXN][18];

int ans[MAXN];

map<int, int>M; int M_point, to[MAXN << 3];

struct Event {
	int type, num;
	int time, l, r, w;
} ev[MAXN << 3];

namespace segment_tree
{
	const int MAXS = MAXN * 250;
	int seg_tot = 0, rot[MAXN], sz[MAXS], ch[MAXS][2];

	int should_remove[250], should_add[250];

	void build(int &now, int l, int r)
	{
		now = ++seg_tot;
		if (l == r) return;
		int mid = (l + r) >> 1;
		build(ch[now][0], l, mid);
		build(ch[now][1], mid + 1, r);
	}

	void initialize()
	{
		build(rot[0], 1, M_point);
		FOR(i, 1, n) rot[i] = rot[0];
	}

	void push(int &now, int l, int r, int x, int modi)
	{
		int tmp = now;
 		now = ++seg_tot;

		sz[now] = sz[tmp];
		ch[now][0] = ch[tmp][0];
		ch[now][1] = ch[tmp][1];
		
		sz[now] += modi;

		if (l == r) return;
		
		int mid = (l + r) >> 1;
		if (x <= mid) push(ch[now][0], l, mid, x, modi);
		else push(ch[now][1], mid + 1, r, x, modi);
	}

	void getdown(int &ret, bool t)
	{
		ret = 0;
		FOR(i, 1, should_add[0]) ret += sz[ch[should_add[i]][t]];
		FOR(i, 1, should_remove[0]) ret -= sz[ch[should_remove[i]][t]];
	}

	void godown(bool t)
	{
		FOR(i, 1, should_add[0]) should_add[i] = ch[should_add[i]][t];
		FOR(i, 1, should_remove[0]) should_remove[i] = ch[should_remove[i]][t];
	}

	void _query(int w, int l, int r, int num)
	{
		if (l == r) {
			ans[num] = l;
			return;
		}

		int ldata; getdown(ldata, 0);
		
		int mid = (l + r) >> 1;
		if (ldata >= w) {godown(0); _query(w, l, mid, num);}
		else {godown(1); _query(w - ldata, mid + 1, r, num);}
	}

	void insert(int l, int r, int w)
	{
		if (l > r) {
			debug("WA");
		} //This line is for gdb
		while (l <= n) {
			push(rot[l], 1, M_point, w, 1);
			l += l & -l;
		}
		
		r++;
		while (r <= n) {
			push(rot[r], 1, M_point, w, -1);
			r += r & -r;
		}
	}

	void remove(int l, int r, int w)
	{
		while (l <= n) {
			push(rot[l], 1, M_point, w, -1);
			l += l & -l;
		}
		r++;
		while (r <= n) {
			push(rot[r], 1, M_point, w, 1);
			r += r & -r;
		}
	}

	void query(int l, int r, int w, int num)
	{
		l--;
		should_remove[0] = should_add[0] = 0;
		while (l) {
			should_remove[++should_remove[0]] = rot[l];
			l -= l & -l;
		}
		while (r) {
			should_add[++should_add[0]] = rot[r];
			r -= r & -r;
		}
		_query(w, 1, M_point, num);
	}
}

int lca(int x, int y)
{
	if (depth[x] < depth[y]) x ^= y ^= x ^= y;
	if (depth[x] != depth[y]) {
		int delta = depth[x] - depth[y];
		DNF(i, 17, 0) if (delta >> i & 1) x = anc[x][i];
	}
	if (x == y) return x;
	DNF(i, 17, 0) if (anc[x][i] && anc[y][i] && anc[x][i] != anc[y][i]) {
		x = anc[x][i]; y = anc[y][i];
	}
	return anc[x][0];
}

void work(int now)
{
	if (ev[now].type == appear) {
		segment_tree::insert(ev[now].l, ev[now].r, ev[now].w);
	}
	else if (ev[now].type == disappear) {
		segment_tree::remove(ev[now].l, ev[now].r, ev[now].w);
	}
	else {
		segment_tree::query(1, ev[now].l, ev[now].w, ev[now].num);
	}
}

bool cmp(const Event &a, const Event &b)
{
	return a.time < b.time || a.time == b.time && a.type < b.type;
}

bool cmp2(const Event &a, const Event &b)
{
	return a.w < b.w;
}

void add(int x, int y)
{
	nxt[cnt] = head[x]; data[cnt] = y; head[x] = cnt++;
	nxt[cnt] = head[y]; data[cnt] = x; head[y] = cnt++;
}

void dfs(int now, int pa)
{
	depth[now] = depth[pa] + 1;
	
	anc[now][0] = pa;
	for (int i = 1; anc[now][i - 1]; i++)
		anc[now][i] = anc[anc[now][i - 1]][i - 1];

	dfn[now] = ++INDEX;
	for (int i = head[now]; i != -1; i = nxt[i]) {
		if (data[i] != pa) dfs(data[i], now);
	}
	ed[now] = INDEX;
}

int find_fa(int now, int depth)
{
	FOR(i, 0, 17) if (depth >> i & 1) now = anc[now][i];
	return now;
}

int main()
{
	freopen("fruit.in", "r", stdin);
	freopen("fruit.out", "w", stdout);
	
	memset(head, -1, sizeof head);
	
	in(n); in(p); in(q);

	FOR(i, 1, n - 1) {
		int u, v; in(u); in(v); add(u, v);
	}

	dfs(1, 0);

	FOR(i, 1, p) {
		int u, v, w;
		in(u); in(v); in(w);

		int anc = lca(u, v);

		if (dfn[u] > dfn[v]) u ^= v ^= u ^= v;

		if (u == anc) {
			int nodes = find_fa(v, depth[v] - depth[u] - 1);
			int ss = dfn[nodes], tt = ed[nodes];

			if (ss != 1) {
				ev[++ev_tot].time = 1;
				ev[ev_tot].l = dfn[v]; ev[ev_tot].r = ed[v];
				ev[ev_tot].type = appear; ev[ev_tot].w = w;

				ev[++ev_tot].time = ss - 1;
				ev[ev_tot].l = dfn[v]; ev[ev_tot].r = ed[v];
				ev[ev_tot].type = disappear; ev[ev_tot].w = w;
			}

			if (tt != n) {
				ev[++ev_tot].time = dfn[v];
				ev[ev_tot].l = tt + 1; ev[ev_tot].r = n;
				ev[ev_tot].type = appear; ev[ev_tot].w = w;

				ev[++ev_tot].time = ed[v];
				ev[ev_tot].l = tt + 1; ev[ev_tot].r = n;
				ev[ev_tot].type = disappear; ev[ev_tot].w = w;
			}
		}
		
		else {
			ev[++ev_tot].time = dfn[u]; ev[ev_tot].l = dfn[v];
			ev[ev_tot].r = ed[v]; ev[ev_tot].type = appear; ev[ev_tot].w = w;
			ev[++ev_tot].time = ed[u]; ev[ev_tot].l = dfn[v];
			ev[ev_tot].r = ed[v]; ev[ev_tot].type = disappear; ev[ev_tot].w = w;
		}
	}

	std::sort(ev + 1, ev + ev_tot + 1, cmp2);

	FOR(i, 1, ev_tot) {
		if (i != 1 && ev[i].w == ev[i - 1].w) continue;
		M[ev[i].w] = ++M_point;
		to[M_point] = ev[i].w;
	}

	FOR(i, 1, ev_tot) {
		ev[i].w = M[ev[i].w];
	}

	FOR(i, 1, q) {
		int u, v, w;
		in(u); in(v); in(w);
		if (dfn[u] > dfn[v]) u ^= v ^= u ^= v;
		
		ev[++ev_tot].time = dfn[u];
		ev[ev_tot].l = dfn[v]; ev[ev_tot].w = w;
		ev[ev_tot].num = i; ev[ev_tot].type = query;
	}

	std::sort(ev + 1, ev + ev_tot + 1, cmp);

	segment_tree::initialize();
	
	FOR(i, 1, ev_tot) work(i);
	
	FOR(i, 1, q) printf("%d\n", to[ans[i]]);
	
	return 0;
}
