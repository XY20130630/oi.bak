#include <bits/stdc++.h>

#define PLA(i, j, k) ((i) * 2500 + (j) * 50 + (k))

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

int p, qs, r, d;

int height[50][50][50];

int cnt, dis[200010], head[200010], data[200010 << 1], flow[200010 << 1], cur[200010], nxt[200010 << 1];

using std::queue; queue<int>q;

const int ss = 0, tt = 200009, INF = 0x3f3f3f3f;

bool bfs()
{
	memset(dis, -1, sizeof dis);
	dis[ss] = 0; q.push(ss);
	while (!q.empty()) {
		int now = q.front(); q.pop();
		for (int i = head[now]; i != -1; i = nxt[i]) {
			if (dis[data[i]] == -1 && flow[i]) {
				dis[data[i]] = dis[now] + 1;
				q.push(data[i]);
			}
		}
	}
	return dis[tt] != -1;
}

int dfs(int now, int fl)
{
	if (now == tt) return fl;
	int flo;
	for (int &i = cur[now]; i != -1; i = nxt[i]) {
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

void add(int x, int y, int z)
{
	nxt[cnt] = head[x]; data[cnt] = y; flow[cnt] = z; head[x] = cnt++;
	nxt[cnt] = head[y]; data[cnt] = x; flow[cnt] = 0; head[y] = cnt++;
}

int main()
{
	freopen("cake.in", "r", stdin);
	freopen("cake.out", "w", stdout);
	
	memset(head, -1, sizeof head);
	
	in(p); in(qs); in(r); in(d);
	
	FOR(i, 1, r) {
		FOR(j, 1, p) FOR(k, 1, qs) in(height[i][j][k]);
	}
	
	FOR(i, 1, p) FOR(j, 1, qs) {
		add(ss, PLA(i, j, 1), INF);
		FOR(k, 1, r) {
			add(PLA(i, j, k), PLA(i, j, k + 1), height[k][i][j]);
		}
		add(PLA(i, j, r + 1), tt, INF);
	}

	FOR(i, 1, p) FOR(j, 1, qs) {
		FOR(k, d + 1, r + 1) {
			if (i != p) add(PLA(i, j, k), PLA(i + 1, j, k - d), INF);
			if (i != 1) add(PLA(i, j, k), PLA(i - 1, j, k - d), INF);
			if (j != qs) add(PLA(i, j, k), PLA(i, j + 1, k - d), INF);
			if (j != 1) add(PLA(i, j, k), PLA(i, j - 1, k - d), INF);
		}
	}

	int fl = 0;

	while (bfs()) {
		memcpy(cur, head, sizeof head);
		int tmp; while (tmp = dfs(ss, INF)) fl += tmp;
	}

	printf("%d\n", fl);
	
	return 0;
}
