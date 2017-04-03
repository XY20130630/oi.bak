#include <bits/stdc++.h>

typedef long long LL;

#define FOR(i, a, b) for (LL i = (a), i##_END_ = (b); i <= i##_END_; i++)
#define DNF(i, a, b) for (LL i = (a), i##_END_ = (b); i >= i##_END_; i--)

template <typename Tp> void in(Tp &x) {
	char ch = getchar(); x = 0;
	while (ch < '0' || ch > '9') ch = getchar();
	while (ch >= '0' && ch <= '9') x = x * 10 + ch - '0', ch = getchar();
}

template <typename Tp> Tp Min(Tp x, Tp y) {return x < y ? x : y;}
template <typename Tp> Tp Max(Tp x, Tp y) {return x > y ? x : y;}
template <typename Tp> Tp chkmax(Tp &x, Tp y) {return x > y ? x : x=y;}
template <typename Tp> Tp chkmin(Tp &x, Tp y) {return x < y ? x : x=y;}

const LL MAXN = 80;

LL n, A[MAXN][MAXN], B[MAXN][MAXN], tmp[MAXN][MAXN];

namespace KM
{
	using std::queue; queue<LL>q;
	bool in_stack[MAXN << 1];
	
	const LL ss = 0, tt = 159;
	
	LL cnt, ret1, ret2, dis[MAXN << 1], pre[MAXN << 1];
	LL head[MAXN << 1], data[MAXN * MAXN << 2], flow[MAXN * MAXN << 2];
	LL wei1[MAXN * MAXN << 2], wei2[MAXN * MAXN << 2], nxt[MAXN * MAXN << 2], wei[MAXN * MAXN << 2];

	void add(LL x, LL y, LL z, LL l, LL l1, LL l2)
	{
		nxt[cnt] = head[x]; data[cnt] = y; flow[cnt] = z;
		wei[cnt] = l; wei1[cnt] = l1; wei2[cnt] = l2; head[x] = cnt++;
		
		nxt[cnt] = head[y]; data[cnt] = x; flow[cnt] = 0;
		wei[cnt] =-l; wei1[cnt] =-l1; wei2[cnt] =-l2; head[y] = cnt++;
	}
	
	bool bfs()
	{
		memset(pre, -1, sizeof pre);
		memset(dis, 0x3f, sizeof dis);
		dis[ss] = 0; q.push(ss); in_stack[ss] = true;
		
		while (!q.empty()) {
			LL now = q.front(); q.pop();
			in_stack[now] = false;
			for (LL i = head[now]; i != -1; i = nxt[i]) {
				if (flow[i] && dis[data[i]] > dis[now] + wei[i]) {
					dis[data[i]] = dis[now] + wei[i];
					pre[data[i]] = i;
					if (!in_stack[data[i]]) {
						in_stack[data[i]] = true;
						q.push(data[i]);
					}
				}
			}
		}

		return pre[tt] != -1;
	}

	void dfs()
	{
		for (LL i = tt; pre[i] != -1; i = data[pre[i] ^ 1]) ret1 += wei1[pre[i]];
		for (LL i = tt; pre[i] != -1; i = data[pre[i] ^ 1]) ret2 += wei2[pre[i]];
		for (LL i = tt; pre[i] != -1; i = data[pre[i] ^ 1]) flow[pre[i]]--, flow[pre[i] ^ 1]++;
	}
	
	std::pair<LL, LL>main(LL argv[MAXN][MAXN])
	{
		ret1 = ret2 = 0; cnt = 0;
		memset(head, -1, sizeof head);
		FOR(i, 1, n) add(ss, i, 1, 0, 0, 0);
		FOR(i, 1, n) FOR(j, 1, n) {
			add(i, j + n, 1, argv[i][j], A[i][j], B[i][j]);
		}
		FOR(i, 1, n) add(i + n, tt, 1, 0, 0, 0);
		while (bfs()) dfs();
		return std::make_pair(ret1, ret2);
	}
}

bool gongxian(std::pair<LL, LL>l, std::pair<LL, LL>mid, std::pair<LL, LL>r)
{
	return (l.second - r.second) * (l.first - mid.first) == (l.second - mid.second) * (l.first - r.first);
}

LL get_ans(std::pair<LL, LL>l, std::pair<LL, LL>r)
{
	if (l.first == r.first || l.second == r.second)
		return Min(l.first * l.second, r.first * r.second);
	FOR(i, 1, n) FOR(j, 1, n)
		tmp[i][j] = -((r.second - l.second) * A[i][j] + (l.first - r.first) * B[i][j]);
	std::pair<LL, LL>mid = KM::main(tmp);
	if (gongxian(l, mid, r)) return Min(l.first * l.second, r.first * r.second);
	return Min(get_ans(l, mid), get_ans(mid, r));
}

int main()
{
	freopen("frame.in", "r", stdin);
	freopen("frame.out", "w", stdout);
	
	LL tcase; in(tcase);

	while (tcase--) {
		in(n);
		FOR(i, 1, n) FOR(j, 1, n) in(A[i][j]);
		FOR(i, 1, n) FOR(j, 1, n) in(B[i][j]);
		std::pair<LL, LL>retA = KM::main(A), retB = KM::main(B);
		printf("%lld\n", get_ans(retA, retB));
	}
	
	return 0;
}
