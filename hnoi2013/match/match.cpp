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

const int MAXN = 20, MOD = 1000000007;

LL a[MAXN];

std::map<LL, LL>mp;

LL hash(int now)
{
	LL res = now, tmp[MAXN];
	FOR(i, 1, now) tmp[i] = a[i];
	std::sort(tmp + 1, tmp + now + 1, std::less<int>());
	FOR(i, 1, now) res += res * 28 + tmp[i];
	return res;
}

LL DFS(int now, int n)
{
	if (a[n] > 3 * (n - now)) return -1;

	LL res = 0;

	if (now == n) {
		if (n == 1) return 1;
		else {
			LL h = hash(n - 1);
			if (mp[h]) return mp[h];
			return mp[h] = DFS(1, n - 1);
		}
	}
	
	if (a[n] >= 3) {
		LL tmp = 0;
		a[n] -= 3, tmp = DFS(now + 1, n);
		if (tmp != -1) (res += tmp) %= MOD;
		a[n] += 3;
	}
	
	if (a[n] && a[now]) {
		LL tmp = 0;
		a[n]--, a[now]--, tmp = DFS(now + 1, n);
		if (tmp != -1) (res += tmp) %= MOD;
		a[n]++, a[now]++;
	}
	
	if (a[now] >= 3) {
		LL tmp = 0;
		a[now] -= 3, tmp = DFS(now + 1, n);
		if (tmp != -1) (res += tmp) %= MOD;
		a[now] += 3;
	}
	
	return res ? res : -1;
}

int n;

int main()
{
	freopen("match.in", "r", stdin);
	freopen("match.out", "w", stdout);

	in(n);

	FOR(i, 1, n) in(a[i]);

	std::sort(a + 1, a + n + 1, std::less<int>());
	
	printf("%lld\n", DFS(1, n));
	
	return 0;
}
