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

const LL MAXN = 1010;

LL n, k, l, r, tot, A[MAXN], fa[MAXN];

LL find(LL x)
{
	LL tmp = x, pre;
	while (tmp != fa[tmp]) tmp = fa[tmp];
	while (x != tmp) pre = fa[x], fa[x] = tmp, x = pre;
	return tmp;
}

void merge(LL x, LL y)
{
	LL fx = find(x), fy = find(y);
	if (fx != fy) fa[fx] = fy;
}

LL query(LL x)
{
	if (x < 0) return 0;

	LL ret = 0, tmp = tot;
	static bool chose[100];
	static bool how_chose[100];

	memset(chose, false, sizeof chose);

	if (x > (1ll << (k)) - 1) return (1ll << tmp);
	
	DNF(i, k, 1) {
		if (!(x & (1ll << (i - 1)))) {
			LL fx = find(i);
			if (chose[fx]) {
				if (how_chose[fx] == 1) return ret;
			}
			else {
				chose[fx] = true;
				how_chose[fx] = 0;
				tmp--;
				continue;
			}
		}
		else {
			LL fx = find(i);
			if (chose[fx]) {
				if (how_chose[fx] == 1) continue;
				else {
					ret += (1ll << tmp);
					return ret;
				}
			}
			else {
				chose[fx] = true;
				how_chose[fx] = 1;
				tmp--;
				ret += (1ll << tmp);
				continue;
			}
		}
	}

	return ret + 1;
}

int main()
{
	freopen("nand.in", "r", stdin);
	freopen("nand.out", "w", stdout);
	
	in(n); in(k); in(l); in(r);

	FOR(i, 1, n) in(A[i]);

	FOR(i, 1, k) fa[i] = i;
	
	FOR(i, 1, k) {
		LL tmp = (1ll << (k)) - 1;
		FOR(j, 1, n) {
			if (A[j] & (1ll << (i - 1))) {
				tmp &= A[j];
			}
			else tmp &= (A[j] ^ ((1ll << k) - 1));
		}
		if (!(tmp & (1ll << (i - 1)))) {
			puts("WA");
		}
		FOR(j, 1, k) if (j != i) {
			if (bool(tmp & (1ll << (j - 1))) == bool(tmp & (1ll << (i - 1))))
				merge(i, j);
		}
	}

	FOR(i, 1, k) if (fa[i] == i) tot++;

	printf("%lld\n", query(r) - query(l - 1));

	return 0;
}
