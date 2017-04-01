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

LL ans = 0;
LL n, k, m, p;

LL power(LL x, LL y, LL p)
{
	x %= p;
	
	LL ret = 1;
	while (y) {
		if (y & 1) ret = 1ll * ret * x % p;
		x = 1ll * x * x % p;
		y >>= 1;
	}
	return ret;
}

int main()
{
	freopen("seq.in", "r", stdin);
	freopen("seq.out", "w", stdout);
	
	in(n); in(k); in(m); in(p);
	n %= p;
	
	ans = 1ll * power(m, k - 1, p) * n % p;
	ans = ((ans - 1ll * (1ll * m * (m + 1) / 2) % p *
		   (k - 1) % p * power(m, k - 2, p) % p) % p + p) % p;
	printf("%lld\n", ans);
	
	return 0;
}
