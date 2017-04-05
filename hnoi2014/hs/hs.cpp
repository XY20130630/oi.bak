#include <bits/stdc++.h>

typedef long long LL;
typedef unsigned long long ULL;

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

const int MAXN = 2000010;

using std::string;
using std::cin;
using std::cout;

string str[MAXN];
int t, n, len[MAXN];
ULL hs[MAXN];
const ULL BASE = 31;

int prefix[MAXN], suffix[MAXN], numb[MAXN];

void gethash(int num)
{
	hs[num] = 0;
	FOR(i, 0, len[num] - 1) {
		hs[num] = hs[num] * BASE + str[num][i];
	}
}

bool isop[MAXN];
int nopera[MAXN];

ULL rhs[MAXN], po[MAXN];

ULL power(ULL x, ULL y)
{
	if (!y) return 1;
	return po[y];
}

void work()
{
	string *a = str;
	
	FOR(i, 1, nopera[0])
		isop[nopera[i]] = false;
	
	nopera[0] = 0;
	
	FOR(i, 1, n) {
		bool flag = true;
		FOR(j, 0, len[i] - 1) {
			if (str[i][j] == '*') {
				flag = false;
				break;
			}
		}
		if (flag) {
			isop[i] = true;
			nopera[++nopera[0]] = i;
		}
	}
	FOR(i, 2, nopera[0]) {
		if (hs[nopera[i]] != hs[nopera[i - 1]]) {
			puts("N");
			return;
		}
	}

	int need_compare = nopera[1];
	ULL TBASE = 1;

	FOR(i, 1, len[need_compare]) {
		rhs[i] = rhs[i - 1] + str[need_compare][i - 1] * TBASE;
		TBASE = TBASE * BASE;
	}
	
	FOR(i, 1, n) if (!isop[i]) {
		int lastpos = 0;
		for (int j = 0, k; j <= len[i] - 1; j = k + 1) {
			k = j;
			while (k < len[i] && str[i][k] == '*') k++;

			j = k;
			while (k < len[i] && str[i][k] != '*') k++; k--;
			
			if (str[i][k] == '*') break;

			ULL nowhs = 0, TBASE = 1;
			FOR(l, j, k) {
				nowhs = nowhs + str[i][l] * TBASE;
				TBASE = TBASE * BASE;
			}

			while (true) {
				if (lastpos + (k - j + 1) > len[need_compare]) {
					puts("N");
					return;
				}
				if (k == len[i] - 1) {
					lastpos = len[need_compare] - (k - j + 1);
				}
				ULL tmpnow = nowhs;
				if (rhs[lastpos + (k - j + 1)] - rhs[lastpos] == tmpnow * power(31, lastpos)) {
					lastpos += (k - j + 1);
					break;
				}
				if (j != 0) {
					lastpos++;
				}
				else {
					puts("N");
					return;
				}
			}
		}
	}
	puts("Y");
}

bool cmp_prefix(int x, int y)
{
	return prefix[x] < prefix[y];
}

bool cmp_suffix(int x, int y)
{
	return suffix[x] < suffix[y];
}

int main()
{
	freopen("hs.in", "r", stdin);
	freopen("hs.out", "w", stdout);
	
	po[0] = 1;
	FOR(i, 1, 2000000) po[i] = po[i - 1] * BASE;
	
	std::ios::sync_with_stdio(false);  
	cin >> t;
	while (t--) {
		cin >> n;
		FOR(i, 1, n) cin >> str[i], len[i] = str[i].size();

		bool isy = true, isn = true;
		FOR(i, 1, n) {
			gethash(i);
			bool iss = false;
			FOR(j, 0, len[i] - 1) {
				if (str[i][j] == '*') isn = false, iss = true;
			}
			if (!iss) isy = false;
		}

		if (isn) {
			bool flag = false;
			FOR(i, 2, n) if (hs[i] != hs[i - 1]) {
				flag = true;
				break;
			}
			if (!flag) puts("Y");
			else puts("N");
			continue;
		}

		else if (isy) {
			FOR(i, 1, n) {
				FOR(j, 0, len[i] - 1) {
					if (str[i][j] == '*') {
						prefix[i] = j; break;
					}
				}
				DNF(j, len[i] - 1, 0) {
					if (str[i][j] == '*') {
						suffix[i] = len[i] - 1 - j; break;
					}
				}
			}
			bool flag = true;
			FOR(i, 1, n) numb[i] = i;
			std::sort(numb + 1, numb + n + 1, cmp_prefix);
			FOR(i, 2, n) {
				int last = numb[i - 1], now = numb[i];
				FOR(j, 1, prefix[last]) {
					if (str[last][j - 1] != str[now][j - 1]) {
						flag = false; puts("N"); break;
					}
				}
				if (!flag) break;
			}
			if (!flag) continue;
			std::sort(numb + 1, numb + n + 1, cmp_suffix);
			FOR(i, 2, n) {
				int last = numb[i - 1], now = numb[i];
				FOR(j, 1, suffix[last]) {
					if (str[last][len[last] - j] != str[now][len[now] - j]) {
						flag = false; puts("N"); break;
					}
				}
				if (!flag) break;
			}
			if (flag) puts("Y");
			continue;
		}

		else {
			work();
		}
	}
	return 0;
}
