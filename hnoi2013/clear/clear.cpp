#include <bits/stdc++.h>

typedef long long LL;

#define debug(...) fprintf(stderr, __VA_ARGS__)

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

using std::vector;

const int MAXN = 5002;

bool chose[20];

int a[4], ans;

bool map[MAXN][MAXN];

std::vector<std::vector<int> >g[MAXN];

void init()
{
	ans = 0x3f3f3f3f;
	
	FOR(i, 1, 3) in(a[i]);

	FOR(i, 0, a[1] + 1) {
		g[i].resize(a[2] + 2);
		FOR(j, 0, a[2] + 1) g[i][j].resize(a[3] + 2);
	}
   
	FOR(i, 1, a[1]) {
		FOR(j, 1, a[2]) FOR(k, 1, a[3]) in(g[i][j][k]);
	}
}

const int ss = 0, tt = 5008, INF = 0x3f3f3f3f;

using std::queue;

queue<int>q;
bool link[MAXN];
int dis[5010], cur[5010];
int cnt, head[5010], data[20010], flow[20010], nxt[20010];

void add(int x, int y, int z)
{
	nxt[cnt] = head[x]; data[cnt] = y; flow[cnt] = z; head[x] = cnt++;
	nxt[cnt] = head[y]; data[cnt] = x; flow[cnt] = 0; head[y] = cnt++;
}

int times, maxx = 1, alledge;

struct Edge {
	int x, y;
	Edge(int a=0, int b=0): x(a), y(b) {}
};

std::vector<Edge>edge[20];

bool bfs()
{
	dis[ss] = times; q.push(ss);
	while (!q.empty()) {
		int now = q.front(); q.pop();
		for (int i = head[now]; i != -1; i = nxt[i]) {
			if (dis[data[i]] < times && flow[i]) {
				dis[data[i]] = dis[now] + 1;
				q.push(data[i]);
				chkmax(maxx, dis[now] + 1);
			}
		}
	}
	return dis[tt] >= times;
}

int dfs(int now, int fl)
{
	if (now == tt) return fl;
	int flo;
	for (int i = head[now]; i != -1; i = nxt[i]) {
		if (flow[i] && dis[data[i]] == dis[now] + 1) {
			if (flo = dfs(data[i], Min(fl, flow[i]))) {
				flow[i] -= flo;
				flow[i ^ 1] += flo;
				return flo;
			}
		}
	}
	return 0;
}

void check(int minx)
{
	int tmptot = 0;
	FOR(i, 1, minx) if (chose[i]) tmptot++;

	if (tmptot >= ans) return;

	cnt = 0;

	int tot = tmptot;

// ***********************************************

// make_Edge

	int Addition;

	if (minx == a[1]) Addition = a[2];
	else if (minx == a[2]) Addition = a[1];
	else Addition = a[1];

	FOR(k, 1, minx) if (!chose[k]) {
		FOR(i, 0, edge[k].size() - 1) {
			int fr = edge[k][i].x, to = edge[k][i].y + Addition;
			if (!map[fr][to - Addition]) {
				add(fr, to, 1);
				if (!link[fr]) {link[fr] = true; add(ss, fr, 1);}
				if (!link[to]) {link[to] = true; add(to, tt, 1);}
			}
		}
	}

	

// ***********************************************
	
	int fl = 0;

	times = maxx + 1;
	
	while (bfs()) {
		//memcpy(cur, head, sizeof head);
		int tmp;
		while (tmp = dfs(ss, INF)) fl += tmp;

		times = maxx + 1;
	}

	chkmin(ans, fl + tot);

	FOR(k, 1, minx) if (!chose[k]) FOR(i, 0, edge[k].size() - 1) {
		map[edge[k][i].x][edge[k][i].y] = false;
		link[edge[k][i].x] = link[edge[k][i].y + Addition] = false;
		head[edge[k][i].x] = head[edge[k][i].y + Addition] = -1;
	}

	head[ss] = head[tt] = -1;
}

void search(int now, int minx)
{
	chose[now] = false;
	if (now == minx) check(minx);
	else search(now + 1, minx);
	chose[now] = true;
	if (now == minx) check(minx);
	else search(now + 1, minx);
	chose[now] = false;
}

void work()
{
	memset(head, -1, sizeof head);
	
	int minx = 5000;
	FOR(i, 1, 3) chkmin(minx, a[i]);

	alledge = 0;

	FOR(i, 1, minx) {
		edge[i].clear();
	}
	
	if (minx == a[1]) {
		FOR(k, 1, a[1]) {
			FOR(i, 1, a[2])	FOR(j, 1, a[3])
				if (g[k][i][j]) edge[k].push_back(Edge(i, j));
		}
	}

	else if (minx == a[2]) {
		
			FOR(i, 1, a[1]) FOR(k, 1, a[2]) { FOR(j, 1, a[3])
				if (g[i][k][j]) edge[k].push_back(Edge(i, j));
		}
	}

	else {
		
			FOR(i, 1, a[1]) FOR(j, 1, a[2]) FOR(k, 1, a[3]) {
				if (g[i][j][k]) edge[k].push_back(Edge(i, j));
			}
	}
	
	search(1, minx);
	printf("%d\n", ans);
	debug("%d\n", ans);
}

int main()
{
	freopen("clear.in", "r", stdin);
	freopen("clear.out", "w", stdout);
	
	int tcase; in(tcase);

	while (tcase--) {
		init();
		work();
	}
	
    return 0;
}
