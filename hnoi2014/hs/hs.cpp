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

const int MAXN = 100010;

using std::string;
using std::cin;
using std::cout;

string str[MAXN];
int t, n, len[MAXN];
ULL hs[MAXN];
const ULL BASE = 31;

void gethash(int num)
{
	hs[num] = 0;
	FOR(i, 0, len[num] - 1) {
		hs[num] = hs[num] * BASE + str[num][i];
	}
}

int main()
{
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
//			FOR(j, 0
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
			int prefix = 0x3f3f3f3f, suffix = -1;
			FOR(i, 1, n) {
				FOR(j, 0, len[i] - 1) {
					if (str[i][j] == '*') {
						chkmin(prefix, j - 1); break;
					}
				}
				DNF(j, len[i] - 1, 0) {
					if (str[i][j] == '*') {
						chkmax(suffix, j + 1); break;
					}
				}
			}
			bool flag = false;
			FOR(i, 2, n) {
				FOR(j, 0, prefix) {
					if (str[i][j] != str[i - 1][j]) {
						flag = true; break;
					}
				}
				if (flag) {
					puts("N");
					break;
				}
				DNF(j, len[i] - 1, suffix) {
					if (str[i][j] != str[i - 1][j]) {
						flag = true; break;
					}
				}
				if (flag) {
					puts("N");
					break;
				}
			}
			if (!flag)
				puts("Y");
			continue;
		}
	}
	return 0;
}
