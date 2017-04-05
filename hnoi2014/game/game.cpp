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
 
const int MAXN = 100010;
 
int t, f, sg[MAXN], can[MAXN];
 
int main()
{
	freopen("game.in", "r", stdin);
	freopen("game.out", "w", stdout);
	
    in(t); in(f);
    FOR(i, 0, f - 1) sg[i] = 0;
    FOR(i, f, 100000) {
        for (int j = 2, k; j <= i; j = k + 1) {
            k = i / (i / j);
            int s1 = j, s2 = j + 1;
            int a = ((i % s1) & 1) ? (i / s1 + 1) : 0;
            int b = ((s1 - (i % s1)) & 1) ? (i / s1) : 0;
            can[sg[a] ^ sg[b]] = i;
            a = ((i % s2) & 1) ? (i / s2 + 1) : 0;
            b = ((s2 - (i % s2)) & 1) ? (i / s2) : 0;
            can[sg[a] ^ sg[b]] = i;
        }
        FOR(j, 0, 100000)
            if (can[j] != i) {
                sg[i] = j;
                break;
            }
    }
    FOR(i, 1, t) {
        int n, ret = 0; in(n);
        FOR(j, 1, n) {
            int x; in(x); ret ^= sg[x];
        }
        if (ret) printf("1");
        else printf("0");
        if (i != t) printf(" ");
    }
     
    return 0;
}
